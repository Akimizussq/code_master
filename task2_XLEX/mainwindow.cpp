#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QStack>
#include <QMap>
#include <QSet>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    zzbds_str="";
    zzbds_niPolan="";
//    NFATable = new QTableWidget(this);

}
/*
 test:
 a(a|b)*
 */
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InsertCatNode(){
    QString exp = ui->lineEdit_input_zzbds->text();   //得到文本框中的正则表达式  加点代表联合
    zzbds_str = "";
    //情况：ab,  a(a)*a , )( ,  a(   ,  )a   *(
    while (!exp.isEmpty())
    {
        if ( exp[0]>='a'&&exp[0]<='z'&&((exp[1]>='a'&&exp[1]<='z')||exp[1]=='('))
        {
            zzbds_str+=exp[0];
            zzbds_str+='.';
            exp.remove(0,1);
        }

        else if(exp[0]=='*'&&((exp[1]>='a'&&exp[1]<='z')||exp[1]=='('))
        {
            zzbds_str+=exp[0];
            zzbds_str+='.';
            exp.remove(0,1);
        }

        else if(exp[0]==')' && ((exp[1]>='a'&&exp[1]<='z')||exp[1]=='('))
        {
            zzbds_str+=exp[0];
            zzbds_str+='.';
            exp.remove(0,1);
        }
        else
        {
            zzbds_str+=exp[0];
            exp.remove(0,1);
        }
    }

    qDebug()<<zzbds_str;
}

int MainWindow::Precedence(QChar symbol)  //各种符号的优先级  优先数越大优先级越低
{
    int priority;
    if(symbol=='|')priority = 1;
    else if(symbol=='.')priority = 2;
    else if(symbol=='*')priority = 3;
    else priority = 0;
    return priority;
}

void MainWindow::RegExpToPost(){//将正则表达式转换成逆波兰式（后缀形式）
    QString exp = zzbds_str;
    qDebug()<<exp;
//    int i = 0, j = 0;
    QChar ch, cl;
    QString post = "";
    QStack<QChar> ls;

    ch = exp[0];

    while (!exp.isEmpty())
    {
        if (ch == '(')
        {
            ls.push(ch);    //进栈
            exp.remove(0,1);
            ch = exp[0];
        }
        else if (ch == ')')
        {
            QChar tmp = ls.top();
            while (!ls.empty()&& tmp!= '(')//出栈括号里的内容
            {
                post += tmp;
                ls.pop();
                if(!ls.empty()){
                    tmp = ls.top();
                }
                if(ls.empty()) break;

            }
            if(!ls.empty())
            ls.pop();      //把（出栈
            exp.remove(0,1);
            ch = exp[0];
        }
        else if ((ch == '|') || (ch == '*') || (ch == '.'))//ch:*  3
        {
            if(!ls.empty())
            {
                cl = ls.top();//cl:. 2
                while (Precedence(cl) >= Precedence(ch))
                {
                    post += cl;
                    if(!ls.empty())
                    ls.pop();
                    else break;
                    if(!ls.empty())
                    cl = ls.top();

                }
            }
            ls.push(ch);

            exp.remove(0,1);
            ch = exp[0];
        }
        else//英文字母
        {
            post += ch;
            exp.remove(0,1);
            ch = exp[0];
        }
    }


    while (!ls.empty())
    {
        post += ls.pop();
    }



    ls.clear();

    zzbds_niPolan = post;
    qDebug()<<zzbds_niPolan;

}

void MainWindow::GetEdgeNumber()    //从后缀式中得到字母个数  aab|*.   ---->  ab
{
    QString post = zzbds_niPolan;
    Edgenumber = "";
    QSet<QChar> s;
    while (!post.isEmpty())
    {
        if (post[0] == '.' || post[0] == '|' || post[0] == '*')
        {
            post.remove(0,1);
            continue;
        }
        else if(!s.contains(post[0])){
            s.insert(post[0]);
            Edgenumber+=post[0];
            post.remove(0,1);
        }
        else {
            post.remove(0,1);  //直接跳掉
        }
    }
    qDebug()<<Edgenumber;
}
bool unfind(QVector<int> vis,int c){   //没有找到
    for(int i=0;i<vis.size();i++){
        if(vis[i]==c) return 0;
    }
    return 1;
}

//void MainWindow::Move(QChar ch, int v,QVector<int> &vis)
//{
//深度优先搜索

//      QString c = "";
//    c+=ch;
//    qDebug()<<NFAStatesNumber;
//    for(int i=0;i<NFAStatesNumber;i++){
//        if(NFATable->item(v,i)->text()==ch&&unfind(vis,i)){//防回路
//            vis.push_back(i);
//            qDebug()<<vis.size();
//            Move(ch,i,vis);
//        }
//    }
//    return;
//}


void MainWindow::change()    //得到NFA表的字符串二维数组形式。
{
    rowCount = columnCount = NFAStatesNumber;  //11

    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < columnCount; ++j) {
            QTableWidgetItem *item = NFATable->item(i, j);
            if (item) {
                tableData[i][j] = item->text();
            } else {
                tableData[i][j] = ""; // 如果单元格为空，则添加空字符串
            }
        }
    }
    qDebug()<<"rowCount:"<<rowCount;
    qDebug()<<"colCount:"<<columnCount;

}

void MainWindow::Move(QChar ch, int v,QVector<int>&vis)
{
    // 检查当前状态是否已经被访问过
    if (vis.contains(v)) {
        return;
    }

    // 将当前状态添加到已访问列表中
    vis.push_back(v);

    // 遍历所有可能的状态转移
    for (int i = 0; i < NFAStatesNumber; i++) {
        // 如果存在从当前状态到状态i的转移，且该转移的字符为ch，则递归地对状态i调用Move函数
        QChar c =  tableData[v][i][0];  //表元素的第一位
        if ( c == ch) {
//            qDebug()<<c;
//            qDebug()<<i<<" "<<vis.size();
            Move(ch, i,vis);
        }
    }
    return;
}


void MainWindow::tst(){
    QVector<int>visit;
    visit.clear();
    Move('~',0,visit);
    for(int i=0;i<visit.size();i++) qDebug()<<visit[i];

//    for(int i=0;i<rowCount;i++){
//        for(int j=0;j<columnCount;j++){
//            qDebug()<<tableData[i][j]<<" ";
//        }
//    }
}

void MainWindow::SubsetConstruction()  //史山
{
    GetEdgeNumber();//得到压缩的点集

    QSet<QVector<int> >visit_x; //一个hash
//    QVector<QVector<int> >ls;

    //先从0开始只经过~到达的点集
    QVector<int> visit_0;
    Move('~',0,visit_0);
    //对visit表排一下序，行栏目添加一个visit表。
    visit_x.insert(visit_0);//先添加一个visit_0进去

    ls.push_back(visit_0);
    int sz = ls.size();
//    qDebug()<<"ls.size:"<<sz;

    int index = 0;
    DFA_temp0[index] = visit_0;
    DFA_temp1[visit_0]=index++;

    for(int idx=0;idx<sz;idx++){   //行
        for(int i=0;i<Edgenumber.length();i++){  //列   EdgeNumber 是字符串 eg: "ab"
            QVector<int> visit_1;//新建一个容器
            visit_1.clear();


            for(int j=0;j<ls[idx].size();j++){  //对于行表头容器里的每一个元素
                Move(Edgenumber[i],ls[idx][j],visit_1);   //如果第一个是a ,那么得到visit_0中每一个元素直接经过a得到的点集
            }

            //以上操作得到了一个由行表头数组延伸a（例子）得到的数组
            //以下还需要做的操作：如果visit_1(刚刚得到的整个数组)中有母数组出现过的，要删掉，只留下经过a得到的结果。
            //然后第二步，再由这个删除过的数组再跑一遍 ~ ，得到的才是表中的项。


            QVector<int> visit_2=visit_1;

            for (const auto &item : ls[idx]) {  //如此得到visit_1中独有的元素。
                visit_1.removeAll(item);
            }

            if(idx==0&&visit_1.size()==0){//特判第一行
                DFA_Map[ls[idx]][Edgenumber[i]] = visit_1;
                continue;
            }
//            if(idx==1&&i==0) {
//                for(int j=0;j<visit_1.size();j++){  //对于行表头容器里的每一个元素
//                    qDebug()<<visit_1[j];
//                }
//            }

            QVector<int> visit_3=visit_1;
//            if(i==1&&idx==0) qDebug()<<"visit_3:size"<<visit_3.size();
            if(visit_1.size()!=0)visit_2 = visit_1;//把visit_1的最终内容赋值到visit_2;(如果visit1非空)
//            else visit_3.clear();
            visit_1.clear();
            for(int j=0;j<visit_2.size();j++){  //对于行表头容器里的每一个元素
                Move('~',visit_2[j],visit_1);   //如果第一个是a ,那么得到visit_0中每一个元素直接经过a得到的点集
            }
            //visit_1中元素的内容不会重复

            //debug区
//            if(i==1&&idx==0)for(int j=0;j<visit_1.size();j++) qDebug().noquote().nospace()<< visit_1[j];
//            qDebug()<<"第"<<idx<<"行"<<"第"<<i<<"列：visiit_1.size:"<<visit_1.size();



            if(visit_x.contains(visit_1)==0&&visit_3.size()!=0){//用set来查找,空的不要
                DFA_temp0[index] = visit_1;
                DFA_temp1[visit_1] = index++;//对不同的新vector容器进行标号
                visit_x.insert(visit_1);
                ls.push_back(visit_1);
                sz++;
            }

            DFA_Map[ls[idx]][Edgenumber[i]] = visit_1; //得到一个表格值。如果这个值没有出现过，那么就要新开一行。
            DFA_daiti_Map[DFA_temp1[ls[idx]]][Edgenumber[i]] = DFA_temp1[visit_1];

//            if(idx==1)qDebug()<<DFA_daiti_Map[DFA_temp1[ls[1]]][Edgenumber[0]]<<DFA_daiti_Map[DFA_temp1[ls[1]]][Edgenumber[1]];
//            qDebug()<<"sz:"<<sz;
        }
        //说明：sz处于动态变化中，只要有一次idx到达了sz-1且sz不再增加，那么整个外层循环就结束了。说明闭包完成了。
    }
    DFAinit_row_sz = sz;
    DFAinit_col_sz = Edgenumber.length();
}

void MainWindow::devideDFA()
{
    int ending = NFAStatesNumber - 1;  //在例子中是10
    int item_num = DFAinit_row_sz;
    //接下来处理DFA出现的情况数。从0到3（标号）代表的数组中，只要数组中出现了10，就进入终态集，否则进入非终态集。
    for(int i=0;i<item_num;i++){
        if(DFA_temp0[i].contains(ending)){   //代号 代表的数组 中是否含有10

            zhongt.push_back(i);
        }
        else {

            feizhongt.push_back(i);
        }
    }

    //进行子集分类。
    /*
    然后对于终态，需要对终态中的每一个元素，对ab进行移动一步。
    如果移动过后结果属于终态，那么不处理，否则将这个元素从终态集合中分离。
    单独成为一个集合。先a，如果a分不出来就用b。如果分出来就不用了（？）
    非终态也是同理。
    */
    //这里分配没有问题
    QVector<int>t1=zhongt;
    QVector<int>t2=feizhongt;
    qDebug()<<"终态";

//    for(int i=0;i<t1.size();i++) qDebug()<<t1[i];

    //处理终态
    for(int i=0;i<Edgenumber.length();i++){  //a b

        QVector<int>yes;
        QVector<int>no;
        for(int j=0;j<t1.size();j++)  //对于终态数组的每一个元素
        {
            QChar ch = Edgenumber[i];//列表头符号
            int ttmp = DFA_daiti_Map[t1[j]][ch];//get表项
//            qDebug()<<j<<t1[j]<<ttmp<<ch;
            //两两划分
            if(zhongt.contains(ttmp)){  //通过一步到终态集了。
                yes.push_back(t1[j]);
            }
            else {
                int flag=1;
                for(int k=i+1;k<Edgenumber.length();k++){
                    QChar cch = Edgenumber[k];
                    int tttmp = DFA_daiti_Map[t1[j]][cch];
                    if(zhongt.contains(tttmp)){
                        yes.push_back(t1[j]);
                        flag=0;
                        break;
                    }
                }
                if(flag)no.push_back(ttmp);
            }

        }
        //上面完成后，分别得到指向终态和指向非终态的。yes的放入容器中
//用指向非终态的作为操作数组进行递推。
        terminal.push_back(yes);
//        qDebug()<<"ternmal_yes_sz"<<yes.size();
//        for(int i=0;i<zhongt.size();i++) qDebug()<<yes[i];
        t1.clear();
        t1=no;
        if(t1.isEmpty())break;//分类完成
    }

    //处理非终态
    for(int i=0;i<Edgenumber.length();i++){  //a b

        QVector<int>yes;
        QVector<int>no;
        for(int j=0;j<t2.size();j++)
        {
            QChar ch = Edgenumber[i];
            int ttmp = DFA_daiti_Map[t2[j]][ch];

            //两两划分
            if(feizhongt.contains(ttmp)){  //通过一步到非终态集了。
                yes.push_back(t2[j]);
            }
            else
            {
                int flag=1;
                for(int k=i+1;k<Edgenumber.length();k++){
                    QChar cch = Edgenumber[k];
                    int tttmp = DFA_daiti_Map[t2[j]][cch];
                    if(feizhongt.contains(tttmp)){
                        yes.push_back(t2[j]);
                        flag=0;
                        break;
                    }
                }
                if(flag)no.push_back(ttmp);
            }
        }
        //上面完成后，分别得到指向非终态和指向终态的。yes的放入容器中
//用指向非终态的作为操作数组进行递推。
        interminal.push_back(yes);

        t2.clear();
        t2=no;
        if(t2.isEmpty())break;//分类完成
    }

    for(int i=0;i<item_num;i++) DFA_rows.push_back(i);//行表头  等下用来删

//    qDebug()<<"terminalsize"<<terminal.size()<<"interminalsize"<<interminal.size();
//    for(int i=0;i<DFAinit_row_sz;i++){
//       qDebug()<<DFA_daiti_Map[i][Edgenumber[1]];
//    }


    //至此得到了二维容器。下面操作这些容器，合并
//    操作终态的
    for(int i=0;i<terminal.size();i++){
        for(int j=1;j<terminal[i].size();j++){
            //全部变成terminal_0
            DFA_rows.removeAll(terminal[i][j]);//删除被合并的行
        }
    }
    for(int i=0;i<terminal.size();i++){
        for(int j=1;j<terminal[i].size();j++){
            //全部变成terminal_0
            for(int m=0;m<DFA_rows.size();m++){
                for(int n=0;n<DFAinit_col_sz;n++){
                    if(DFA_daiti_Map[DFA_rows[m]][Edgenumber[n]] == terminal[i][j]){
                        DFA_daiti_Map[DFA_rows[m]][Edgenumber[n]] = terminal[i][0];
                    }
                }
            }
        }
    }


//    操作非终态的
    for(int i=0;i<interminal.size();i++){
        for(int j=1;j<interminal[i].size();j++){
            //全部变成terminal_0
            DFA_rows.removeAll(interminal[i][j]);//删除被合并的行
        }
    }
    for(int i=0;i<interminal.size();i++){
        for(int j=1;j<interminal[i].size();j++){
            //全部变成terminal_0
            for(int m=0;m<DFA_rows.size();m++){
                for(int n=0;n<DFAinit_col_sz;n++){
                    if(DFA_daiti_Map[DFA_rows[m]][Edgenumber[n]] == interminal[i][j]){
                        DFA_daiti_Map[DFA_rows[m]][Edgenumber[n]] = interminal[i][0];
                    }
                }
            }
        }
    }


}


void MainWindow::showinitDFA(){


    // 设置表格的行数和列数
    DFAinitTable->setRowCount(DFAinit_row_sz);
    DFAinitTable->setColumnCount(DFAinit_col_sz);


    // 设置行和列的表头
    for (int i=0;i<DFAinit_row_sz;i++) {
        QString row = "";
        for(int j=0;j<ls[i].size();j++){   //设计行表头
            QString num = QString::number(ls[i][j]);  //转换成QString
            row += num;
            row += ',';
        }
        QTableWidgetItem *header = new QTableWidgetItem(row);
        DFAinitTable->setVerticalHeaderItem(i, header);
    }
    for (int i = 0; i < DFAinit_col_sz; i++) {
        QString col = "";
        col += Edgenumber[i];
        QTableWidgetItem *header = new QTableWidgetItem(col);
        DFAinitTable->setHorizontalHeaderItem(i, header);
    }

    // 设置每个单元格的值
    for (int i = 0; i < DFAinit_row_sz; i++) {
        for (int j = 0; j < DFAinit_col_sz; j++) {
            QVector<int> value = DFA_Map[ls[i]][Edgenumber[j]];
            QString val = "";
            for(int k=0;k<value.size();k++){   //设计行表头
                QString num = QString::number(value[k]);
                val += num;
                val += ',';
            }
            QTableWidgetItem *item = new QTableWidgetItem(val);
            DFAinitTable->setItem(i, j, item);
        }
    }

    DFAinitTable->show();
}

void MainWindow::show_terminal_DFA()
{
//    确定行数列数
    int row_sz = DFA_rows.size();
    DFAminTable->setRowCount(row_sz);
    DFAminTable->setColumnCount(DFAinit_col_sz);

    // 设置行和列的表头
    for (int i=0;i<row_sz;i++) {
        QString row = "";

        QString num = QString::number(DFA_rows[i]);  //转换成QString
        row += num;

        QTableWidgetItem *header = new QTableWidgetItem(row);
        DFAminTable->setVerticalHeaderItem(i, header);
    }
    for (int i = 0; i < DFAinit_col_sz; i++) {
        QString col = "";
        col += Edgenumber[i];
        QTableWidgetItem *header = new QTableWidgetItem(col);
        DFAminTable->setHorizontalHeaderItem(i, header);
    }

    // 设置每个单元格的值
    for (int i = 0; i < row_sz; i++) {
        for (int j = 0; j < DFAinit_col_sz; j++) {
            int value = DFA_daiti_Map[DFA_rows[i]][Edgenumber[j]];
            QString val = "";

            QString num = QString::number(value);
            val += num;

            QTableWidgetItem *item = new QTableWidgetItem(val);
            DFAminTable->setItem(i, j, item);
        }
    }

    DFAminTable->show();

}

void MainWindow::on_pushButton_openfile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Text Files (*.txt);;All Files (*)"));

        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;
            }

            QTextStream in(&file);
            ui->lineEdit_input_zzbds->setText(in.readAll());
        }
}

void MainWindow::on_pushButton_savefile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Text Files (*.txt);;All Files (*)"));

        if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);

            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"), file.errorString());
                return;
            }

            QTextStream out(&file);
            out << ui->lineEdit_input_zzbds->text();
        }
}

void MainWindow::ThompsonConstruction()
{
    int i=0;
    QChar ch='\0';
    int s1=0, s2=0;
    int currentRowCount=30,currentColumnCount=30;
    QStack<int> states;
    states.clear();
    QString post = zzbds_niPolan;//后缀化的正则表达式
    if (post.isEmpty())
    {
        QMessageBox::information(this,"worning","没有可供分析的正则表达式！");
        return;
    }
    //添加行和列  这部分是用于显示的。

    NFATable->setRowCount(currentRowCount);
    NFATable->setColumnCount(currentColumnCount);
    QTableWidgetItem *newItem0 = new QTableWidgetItem("0");

//    NFATable->SetValue(0, 0);
    NFATable->setItem(0, 0, newItem0);
    i = 1;
    ch = post[0];
    while (!post.isEmpty())
    {
        if (ch == '.')
        {
            s2 = states.pop();
            int temp1 = states.pop();
            int temp2 = states.pop();
            s1 = states.pop();
//            NFATable->InsertEdgeByValue(temp2, temp1, '~');
            QTableWidgetItem *newItem = new QTableWidgetItem("~");
            NFATable->setItem(temp2, temp1, newItem);              //[temp1,temp2]中插入 ~
            states.push(s1);
            states.push(s2);
        }
        else if (ch == '|')   //并
        {
            s2 = states.pop();
            int temp1 = states.pop();
            int temp2 = states.pop();
            s1 = states.pop();

//            NFATable->InsertVertex(i);
            NFATable->insertRow(i);
            NFATable->insertColumn(i); //增加一行和增加一列来达到这个加点的目的

//            NFATable->InsertVertex(i + 1);
            NFATable->insertRow(i+1);
            NFATable->insertColumn(i+1);

            QTableWidgetItem *newItem1 = new QTableWidgetItem("~");
            NFATable->setItem(i, s1, newItem1);

            QTableWidgetItem *newItem2 = new QTableWidgetItem("~");
            NFATable->setItem(i, temp1, newItem2);

            QTableWidgetItem *newItem3 = new QTableWidgetItem("~");
            NFATable->setItem(temp2, i+1, newItem3);

            QTableWidgetItem *newItem4 = new QTableWidgetItem("~");
            NFATable->setItem(s2, i+1, newItem4);

//            NFATable->InsertEdgeByValue(i, temp1, '~');
//            NFATable->InsertEdgeByValue(temp2, i + 1, '~');
//            NFATable->InsertEdgeByValue(s2, i + 1, '~');
            s1 = i;
            s2 = i + 1;
            states.push(s1);
            states.push(s2);
            i += 2;
        }
        else if (ch == '*')
        {
            s2 = states.pop();
            s1 = states.pop();
//            NFATable->InsertVertex(i);
            NFATable->insertRow(i);
            NFATable->insertColumn(i); //增加一行和增加一列来达到这个加点的目的

//            NFATable->InsertVertex(i + 1);
            NFATable->insertRow(i+1);
            NFATable->insertColumn(i+1);


//            NFATable->InsertEdgeByValue(i, i + 1, '~');
//            NFATable->InsertEdgeByValue(s2, s1, '~');
//            NFATable->InsertEdgeByValue(i, s1, '~');
//            NFATable->InsertEdgeByValue(s2, i + 1, '~');

            QTableWidgetItem *newItem1 = new QTableWidgetItem("~");
            NFATable->setItem(i, i+1, newItem1);

            QTableWidgetItem *newItem2 = new QTableWidgetItem("~");
            NFATable->setItem(s2, s1, newItem2);

            QTableWidgetItem *newItem3 = new QTableWidgetItem("~");
            NFATable->setItem(i, s1, newItem3);

            QTableWidgetItem *newItem4 = new QTableWidgetItem("~");
            NFATable->setItem(s2, i+1, newItem4);

            s1 = i;
            s2 = i + 1;
            states.push(s1);
            states.push(s2);
            i += 2;
        }
        else
        {

//            NFATable->InsertVertex(i);
            NFATable->insertRow(i);
            NFATable->insertColumn(i); //增加一行和增加一列来达到这个加点的目的

//            NFATable->InsertVertex(i + 1);
            NFATable->insertRow(i+1);
            NFATable->insertColumn(i+1);

//            NFATable->InsertEdgeByValue(i, i + 1, ch);参考
            QTableWidgetItem *newItem = new QTableWidgetItem(ch);//创建一个边权
            NFATable->setItem(i, i+1, newItem);
            s1 = i;
            s2 = i + 1;
            states.push(s1);
            states.push(s2);
            i += 2;

        }
        post.remove(0,1);
        ch = post[0];

        //添加行和列  这部分是用于显示的。
//        currentRowCount = NFATable->rowCount();
//        NFATable->insertRow(currentRowCount);

//        currentColumnCount = NFATable->columnCount();
//        NFATable->insertColumn(currentColumnCount);
    }
    if(!states.empty()){
        s2 = states.pop();
        s1 = states.pop();
    }

//    NFATable->InsertEdgeByValue(0, s1, '~');
    QTableWidgetItem *newItem1 = new QTableWidgetItem("~");//创建一个边权 ~
    NFATable->setItem(0, s1, newItem1);

    QTableWidgetItem *newItem2 = new QTableWidgetItem("END");//创建一个边权 ~
    NFATable->setItem(s2, s2, newItem2);
    if (!states.isEmpty())
    {
        QMessageBox::information(this,"worning","出现了一些错误！");
        return;
    }
    NFAStatesNumber = s2 + 1;

    //添加行和列  这部分是用于显示的。
//    currentRowCount = NFATable->rowCount();
//    NFATable->insertRow(currentRowCount);

//    currentColumnCount = NFATable->columnCount();
//    NFATable->insertColumn(currentColumnCount);

     //设置水平表头（列号）
    QStringList horizontalHeaderLabels;
    for (int i = 0; i < NFATable->columnCount(); i++) {
        horizontalHeaderLabels << QString::number(i);
    }
    NFATable->setHorizontalHeaderLabels(horizontalHeaderLabels);

    // 设置垂直表头（行号）
    QStringList verticalHeaderLabels;
    for (int i = 0; i < NFATable->rowCount(); i++) {
        verticalHeaderLabels << QString::number(i);
    }
    NFATable->setVerticalHeaderLabels(verticalHeaderLabels);

    NFATable->show();
    qDebug()<<NFAStatesNumber;
}

void MainWindow::on_pushButton_buildNFA_clicked()
{
    ui->tableWidget_showtable->clear();
    NFATable = ui->tableWidget_showtable;
    InsertCatNode();
    RegExpToPost();
    ThompsonConstruction();
}

void MainWindow::on_pushButton_buildDFA_clicked()
{
    ui->tableWidget_showtable->clear();
    NFATable = ui->tableWidget_showtable;
    InsertCatNode();
    RegExpToPost();
    ThompsonConstruction();
    //以上目的是得到NFAtable
    GetEdgeNumber();//从逆波兰式中得到colsize
    change();
    SubsetConstruction();//DFA构造算法
    ui->tableWidget_showtable->clear();//展示之前清空列表
    DFAinitTable = ui->tableWidget_showtable;//指向dfa表
    showinitDFA();//展示DFA

//    tst();   //test通过。得到0得到的01
}

void MainWindow::on_pushButton_minDFA_clicked()
{
    ui->tableWidget_showtable->clear();
    NFATable = ui->tableWidget_showtable;
    InsertCatNode();
    RegExpToPost();
    ThompsonConstruction();
    //以上目的是得到NFAtable
    GetEdgeNumber();//从逆波兰式中得到colsize
    change();
    SubsetConstruction();//DFA构造算法
    ui->tableWidget_showtable->clear();//展示之前清空列表
    DFAinitTable = ui->tableWidget_showtable;//指向dfa表
    showinitDFA();//展示DFA
    devideDFA();
    ui->tableWidget_showtable->clear();//展示之前清空列表
    DFAminTable = ui->tableWidget_showtable;//指向最小dfa表
    show_terminal_DFA();
}
