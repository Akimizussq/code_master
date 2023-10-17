/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *lineEdit_input_zzbds;
    QPushButton *pushButton_openfile;
    QPushButton *pushButton_savefile;
    QPushButton *pushButton_buildNFA;
    QPushButton *pushButton_buildDFA;
    QPushButton *pushButton_minDFA;
    QPushButton *pushButton_build_sourcefile;
    QTableWidget *tableWidget_showtable;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 50, 121, 16));
        lineEdit_input_zzbds = new QLineEdit(centralwidget);
        lineEdit_input_zzbds->setObjectName(QString::fromUtf8("lineEdit_input_zzbds"));
        lineEdit_input_zzbds->setGeometry(QRect(40, 80, 691, 31));
        pushButton_openfile = new QPushButton(centralwidget);
        pushButton_openfile->setObjectName(QString::fromUtf8("pushButton_openfile"));
        pushButton_openfile->setGeometry(QRect(530, 30, 93, 28));
        pushButton_savefile = new QPushButton(centralwidget);
        pushButton_savefile->setObjectName(QString::fromUtf8("pushButton_savefile"));
        pushButton_savefile->setGeometry(QRect(640, 30, 93, 28));
        pushButton_buildNFA = new QPushButton(centralwidget);
        pushButton_buildNFA->setObjectName(QString::fromUtf8("pushButton_buildNFA"));
        pushButton_buildNFA->setGeometry(QRect(60, 150, 111, 41));
        pushButton_buildDFA = new QPushButton(centralwidget);
        pushButton_buildDFA->setObjectName(QString::fromUtf8("pushButton_buildDFA"));
        pushButton_buildDFA->setGeometry(QRect(240, 150, 111, 41));
        pushButton_minDFA = new QPushButton(centralwidget);
        pushButton_minDFA->setObjectName(QString::fromUtf8("pushButton_minDFA"));
        pushButton_minDFA->setGeometry(QRect(410, 150, 111, 41));
        pushButton_build_sourcefile = new QPushButton(centralwidget);
        pushButton_build_sourcefile->setObjectName(QString::fromUtf8("pushButton_build_sourcefile"));
        pushButton_build_sourcefile->setGeometry(QRect(590, 150, 131, 41));
        tableWidget_showtable = new QTableWidget(centralwidget);
        tableWidget_showtable->setObjectName(QString::fromUtf8("tableWidget_showtable"));
        tableWidget_showtable->setGeometry(QRect(50, 211, 681, 351));
        MainWindow->setCentralWidget(centralwidget);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QApplication::translate("MainWindow", "\350\257\267\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217", nullptr));
        pushButton_openfile->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200", nullptr));
        pushButton_savefile->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        pushButton_buildNFA->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220NFA", nullptr));
        pushButton_buildDFA->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220DFA", nullptr));
        pushButton_minDFA->setText(QApplication::translate("MainWindow", "\346\234\200\345\260\217\345\214\226DFA", nullptr));
        pushButton_build_sourcefile->setText(QApplication::translate("MainWindow", "\347\224\237\346\210\220\350\257\215\346\263\225\345\210\206\346\236\220\347\250\213\345\272\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
