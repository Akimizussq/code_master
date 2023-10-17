#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>  //与打开文件有关
#include <QFile>
#include <QTextStream>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMap>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void InsertCatNode();
    int Precedence(QChar symbol);
    void RegExpToPost();
    void ThompsonConstruction();//用tps构造法构造NFA
    void SubsetConstruction();//子集构造法构造DFA
    void Move(QChar ch,int v,QVector<int>&);
    void GetEdgeNumber();
    void showinitDFA();//展示初始DFA
    void tst();
    void change();
    void devideDFA();//分类DFA集合。
    void show_terminal_DFA();

private slots:
    void on_pushButton_openfile_clicked();

    void on_pushButton_savefile_clicked();

    void on_pushButton_buildNFA_clicked();

    void on_pushButton_buildDFA_clicked();

    void on_pushButton_minDFA_clicked();

private:
    Ui::MainWindow *ui;
    QString zzbds_str;  //正则表达式原式
    QString zzbds_niPolan;//正则表达式后缀形式
    QTableWidget *NFATable;  //表格NFA
    QTableWidget *DFAinitTable;  //表格DFA
    QTableWidget *DFAminTable;  //表格最小化DFA
    int NFAStatesNumber=0;  //
    QMap<QVector<int> ,QMap<QChar,QVector<int> > >DFA_Map;   //DFA表
    QMap<QVector<int>,int> DFA_temp1;
    QMap<int,QVector<int>> DFA_temp0;
    QMap<int,QMap<QChar,int> >DFA_daiti_Map;//等价DFA
    QString Edgenumber;  //正则表达式后缀形式中字母种类set
    QString tableData[100][100];
    int rowCount;
    int columnCount;
    int DFAinit_row_sz=0;
    int DFAinit_col_sz=0;
    QVector<QVector<int> >ls;  //用来装DFA的行的名字
    QVector<QVector<int> > terminal;//终态集
    QVector<int>zhongt;//终态集 内不含子集
    QVector<int>feizhongt;//非终态集，内不含子集
    QVector<QVector<int> > interminal;//非终态集
    QVector<int>DFA_rows;//最小化DFA中所用到的
};
#endif // MAINWINDOW_H
