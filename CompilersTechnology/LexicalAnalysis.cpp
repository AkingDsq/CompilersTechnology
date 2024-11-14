#include "LexicalAnalysis.h"
#include "CompilersTechnology.h"

#define KEYWORD         1
#define DELIMITER       2
#define OPERATOR        3
#define IDENTIFIER      4
#define CONSTINTEGRAL   5
#define ELSE            0

// �ؼ����б����޷��б�������б�
QString keyWord[] = { "void", "int", "char", "main", "return", "if", "else" };
QString delimiter[] = { "(", ")", "[", "]", "{", "}", ";", "," };
QString operator0[] = { "+", "-", "*", "/", "%", "=" ,">", "<"};

LexicalAnalysis::LexicalAnalysis(Ui::CompilersTechnologyClass* ui)
    : ui(ui)
{
    csInPut4 = ui->csInPut4;
    csOutput4 = ui->csOutPut4;
    chargeCs4 = ui->chargeCs4;

    connect(chargeCs4, &QPushButton::clicked, this, &LexicalAnalysis::on_chargeCs4_clicked);
}

LexicalAnalysis::~LexicalAnalysis()
{}

// ��� char ���ַ��Ƿ�Ϊ��ĸ
bool Letter(QChar c)
{
    if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
    {
        return true;
    }
    return false;
}

// ��ⵥ���Ƿ�Ϊ�ؼ���
bool KeyWord(QString str)
{
    for (QString p : keyWord)
    {
        if (p == str)
            return true;
    }
    return false;
}

// �жϵ����Ƿ��ǽ��޷�
bool Delimiter(QString str)
{
    for (QString p : delimiter)
    {
        if (p == str)
            return true;
    }
    return false;
}

// �жϵ����Ƿ��������
bool Operator(QString str)
{
    for (QString p : operator0)
    {
        if (p == str)
            return true;
    }
    return false;
}

// �ж��ַ��Ƿ�Ϊ����
bool Digit(QChar c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}


// �� token �鱣���ֱ��� token ���ַ���Ϊ�����ַ��򷵻��������룬���򷵻�ֵΪ0
int Reserve(QString str)
{
    int type = ELSE;

    if (KeyWord(str))
        type = KEYWORD;

    else if (Delimiter(str))
        type = DELIMITER;

    else if (Operator(str))
        type = OPERATOR;

    else if (Digit(str[0]))
        type = CONSTINTEGRAL;

    // ��ʶ��
    else if (str[0] == '_' || Letter(str[0]))
    {
        type = IDENTIFIER;
        {
            for (int i = 1; i < str.length(); i++)
            {
                // �������ϱ�ʶ��������type=0
                if (!(str[i] == '_' || Letter(str[i]) || Digit(str[i])))
                {
                    type = ELSE;
                    break;
                }
            }
        }
    }
    else
        type = ELSE;

    return type;
}

void LexicalAnalysis::on_chargeCs4_clicked()
{
    // ��������
    csOutput4->clear();
    csOutput4->setPlainText("");
    // ��ȡԴ���뽫�ո�ͻس�ɾ��
    QString token = csInPut4->toPlainText();

    // str ����һ������
    QString str = "";
    int i = 0;

    // ���α���Դ�����е�ÿһ���ַ�
    while (i < token.length())
    {
        str = "";
        // ���ֿ�ͷ
        if (Digit(token[i]))
        {
            while (Digit(token[i])) {
                str = str + token[i++];
                // ��ӡ������
                csOutput4->moveCursor(QTextCursor::End);
                csOutput4->insertPlainText('(' + QString::number(Reserve(str)) + '"' + str + '"' + ')' + '\n');
            }
        }
        // �»��߻�����ĸ��ͷ�ĵ��ʣ�����ʶ�����߹ؼ���
        else if (token[i] == '_' || Letter(token[i]))
        {
            while (token[i] == '_' || Letter(token[i]) || Digit(token[i]))
            {
                str = str + token[i++];
                // ����ǹؼ��֣���������
                if (Reserve(str) == KEYWORD)
                    break;
            }
            csOutput4->moveCursor(QTextCursor::End);
            csOutput4->insertPlainText('(' + QString::number(Reserve(str)) + '"' + str + '"' + ')' + '\n');
        }
        // �����
        else {
            str = str + token[i++];
            csOutput4->moveCursor(QTextCursor::End);
            csOutput4->insertPlainText('(' + QString::number(Reserve(str)) + '"' + str + '"' + ')' + '\n');
        }
    }
}