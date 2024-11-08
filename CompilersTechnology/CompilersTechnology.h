#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"

class CompilersTechnology : public QMainWindow
{
    Q_OBJECT

public:
    CompilersTechnology(QWidget *parent = nullptr);
    ~CompilersTechnology();

    QPixmap setWaterMask();

private:
    Ui::CompilersTechnologyClass ui;

    //切换页面
    QStackedWidget* pages;
    QPushButton* page1;
    QPushButton* page2;
    QPushButton* page3;
    // 
    QScrollArea* wfSa;
    // 文法输入（添加或者删除）
    QGridLayout* CsInputLayout;
    QPushButton* addCS;
    QPushButton* deleteCS;
    // 判断文法
    QPushButton* chargeCs;
    QTextEdit* csOutput;     // 输出框

public slots:
    void on_stackedWidget_currentChanged(int index);
    void on_addCs_clicked();
    void on_deleteCs_clicked();
    void on_chargeCs_clicked();
}; 

    
