#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"

class CompilersTechnology : public QMainWindow
{
    Q_OBJECT

public:
    CompilersTechnology(QWidget *parent = nullptr);
    ~CompilersTechnology();

private:
    Ui::CompilersTechnologyClass ui;

    //�л�ҳ��
    QStackedWidget* pages;
    QPushButton* page1;
    QPushButton* page2;
    QPushButton* page3;
    // �ķ����루��ӻ���ɾ����
    QGridLayout* CsInputLayout;
    QPushButton* addCS;
    QPushButton* deleteCS;
    // �ж��ķ�
    QPushButton* chargeCs;
    QTextEdit* csOutput;     // �����

public slots:
    void on_stackedWidget_currentChanged(int index);
    void on_addCS_clicked();
    void on_deleteCS_clicked();
    void on_chargeCs_clicked();
}; 

    
