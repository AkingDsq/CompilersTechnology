#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "NFA.h"       // 非确定性有穷自动机
#include "LexicalAnalysis.h"   // 词法分析器
#include "LL1.h"            // LL(1)分析表
#include "Compiler.h"       // 编译器
//#include "LR1.h"            // LR(1)分析表
//#include "LR0.h"            // LR(0)分析表
class NFA;             // 非确定性有穷自动机
class LexicalAnalysis;   // 词法分析器
class LL1;
class Compiler;

class CompilersTechnology : public QMainWindow
{
    Q_OBJECT

public:
    CompilersTechnology(QWidget *parent = nullptr);
    ~CompilersTechnology(); 

private:
    Ui::CompilersTechnologyClass *ui;
    NFA* nfa;     // 非确定性有穷自动机
    LexicalAnalysis* lexicalAnalysis;   // 词法分析器
    LL1* ll1;     // LL(1)分析表
    Compiler* compiler;

    //切换页面
    QStackedWidget* pages;
    //菜单栏
    QAction* page1;
    QAction* page2;
    QAction* page3;
    QAction* page4;
    QAction* page5;
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

    
