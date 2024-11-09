#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "NFA.h"       // 非确定性有穷自动机
class NFA;             // 非确定性有穷自动机
class CompilersTechnology : public QMainWindow
{
    Q_OBJECT

public:
    CompilersTechnology(QWidget *parent = nullptr);
    ~CompilersTechnology(); 

private:
    Ui::CompilersTechnologyClass *ui;
    NFA* nfa;     // 非确定性有穷自动机

    //切换页面
    QStackedWidget* pages;
    QPushButton* page1;
    QPushButton* page2;
    QPushButton* page3;
    // 
    QScrollArea* wfSa;
    // 文法输入（添加或者删除）
    QGridLayout* CsInputLayout;
    QPushButton* addCs;
    QPushButton* deleteCs;

    QLineEdit* L0;
    QLineEdit* R0;
    QScrollArea* WfSa;
    QWidget* WifaWidget;

    // 判断文法
    QPushButton* chargeCs;
    QTextEdit* csOutput;     // 输出框

public slots:
    void on_stackedWidget_currentChanged(int index);
    void on_addCs_clicked();
    void on_deleteCs_clicked();
    void on_chargeCs_clicked();
}; 

    
