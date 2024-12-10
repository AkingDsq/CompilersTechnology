#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "CompilersTechnology.h"

class LL1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit LL1(Ui::CompilersTechnologyClass* ui);
    ~LL1();

private:
    Ui::CompilersTechnologyClass* ui;
    // 输入框
    QGridLayout* CsInput_5Layout;
    QPushButton* addCs5;
    QPushButton* deleteCs5;

    QLineEdit* Vn;
    QLineEdit* Vt;
    QLineEdit* S;
    QLineEdit* L05;
    QLineEdit* R05;
    QScrollArea* WfSa5;
    QWidget* WifaWidget5;
    // 判断文法
    QPushButton* chargeCs5;
    QTextEdit* csOutput5;     // 输出框
    QTextEdit* csOutput6;      // 输入框
    // 输入分析字符串
    QLineEdit* inputString;

public slots:
    void on_addCs5_clicked();
    void on_deleteCs5_clicked();
    void on_chargeCs5_clicked();
    void test();
};