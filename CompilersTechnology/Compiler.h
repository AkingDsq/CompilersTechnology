#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "CompilersTechnology.h"

class Compiler : public QMainWindow
{
    Q_OBJECT

public:
    explicit Compiler(Ui::CompilersTechnologyClass* ui);
    ~Compiler();
    struct identifier {   // ����ı�ʶ����Ϣ
        int token;        // �ñ�ʶ�����صı��
        int hash;
        QString name;     // ��ű�ʶ��������ַ���
        int _class;       // �ñ�ʶ������������֣�ȫ�ֱ�����ֲ������ȡ�
        int type;         // ��ʶ�������ͣ���������Ǹ������������� int �͡�char �ͻ���ָ���͡�
        int value;        // ��������ʶ����ֵ�����ʶ���Ǻ������մ�ź����ĵ�ַ��
        int Bclass;       // C �����б�ʶ��������ȫ�ֵ�Ҳ�����Ǿֲ��ģ����ֲ���ʶ����������ȫ�ֱ�ʶ����ͬʱ����������ȫ�ֱ�ʶ������Ϣ��
        int Btype;
        int Bvalue;
    };
private:
    Ui::CompilersTechnologyClass* ui;
    // �����ļ���ť
    QPushButton* loadFileButton;
    // Դ����
    QTextEdit* csInPut5;
    // �������
    QTextEdit* csOutPut5;
    // error
    QTextEdit* error5;
    //
    QTextEdit* csOutPut6;
    // ���밴ť
    QPushButton* chargeCs5_2;

public slots:
    void loadFile();     // �����ļ�
    void lexicalAnalysis();   // �ʷ�����
    void syntaxAnalysis();    // �﷨����
    void semanticAnalysis();  // �������
    void generateCode();      // ���ɴ���
    void on_chargeCs5_2_clicked();   // ���밴ť
};