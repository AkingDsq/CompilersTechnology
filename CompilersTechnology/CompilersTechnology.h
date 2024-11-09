#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "NFA.h"       // ��ȷ���������Զ���
class NFA;             // ��ȷ���������Զ���
class CompilersTechnology : public QMainWindow
{
    Q_OBJECT

public:
    CompilersTechnology(QWidget *parent = nullptr);
    ~CompilersTechnology(); 

private:
    Ui::CompilersTechnologyClass *ui;
    NFA* nfa;     // ��ȷ���������Զ���

    //�л�ҳ��
    QStackedWidget* pages;
    QPushButton* page1;
    QPushButton* page2;
    QPushButton* page3;
    // 
    QScrollArea* wfSa;
    // �ķ����루��ӻ���ɾ����
    QGridLayout* CsInputLayout;
    QPushButton* addCs;
    QPushButton* deleteCs;

    QLineEdit* L0;
    QLineEdit* R0;
    QScrollArea* WfSa;
    QWidget* WifaWidget;

    // �ж��ķ�
    QPushButton* chargeCs;
    QTextEdit* csOutput;     // �����

public slots:
    void on_stackedWidget_currentChanged(int index);
    void on_addCs_clicked();
    void on_deleteCs_clicked();
    void on_chargeCs_clicked();
}; 

    
