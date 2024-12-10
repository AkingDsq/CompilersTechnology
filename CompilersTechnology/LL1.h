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
    // �����
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
    // �ж��ķ�
    QPushButton* chargeCs5;
    QTextEdit* csOutput5;     // �����
    QTextEdit* csOutput6;      // �����
    // ��������ַ���
    QLineEdit* inputString;

public slots:
    void on_addCs5_clicked();
    void on_deleteCs5_clicked();
    void on_chargeCs5_clicked();
    void test();
};