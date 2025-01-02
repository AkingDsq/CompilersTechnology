#include "LexicalAnalysis.h"
#include "CompilersTechnology.h"

#define KEYWORD         1
#define DELIMITER       2
#define OPERATOR        3
#define IDENTIFIER      4
#define CONSTINTEGRAL   5
#define ELSE            0

struct identifier {   // 保存的标识符信息
    int token;        // 该标识符返回的标记
    int hash;
    QString name;     // 存放标识符本身的字符串
    int _class;       // 该标识符的类别，如数字，全局变量或局部变量等。
    int type;         // 标识符的类型，即如果它是个变量，变量是 int 型、char 型还是指针型。
    int value;        // 存放这个标识符的值，如标识符是函数，刚存放函数的地址。
    int Bclass;       // C 语言中标识符可以是全局的也可以是局部的，当局部标识符的名字与全局标识符相同时，用作保存全局标识符的信息。
    int Btype;
    int Bvalue;
};
QVector<identifier> identifierList;  // 标识符列表

// 关键词列表，界限符列表、运算符列表
QString keyWord[] = { "void", "int", "char", "main", "return", "if", "else" , "float", "string", "for", "while"};
QString delimiter[] = { "(", ")", "[", "]", "{", "}", ";", "," ,"?", ":"};
QString operator0[] = { "+", "-", "*", "/", "%", "=" ,">", "<", "|", "&", "&&", "||", "=="};

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

// 检测 char 中字符是否为字母
bool Letter(QChar c)
{
    if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
    {
        return true;
    }
    return false;
}

// 检测单词是否为关键字
bool KeyWord(QString str)
{
    for (QString p : keyWord)
    {
        if (p == str)
            return true;
    }
    return false;
}

// 判断单词是否是界限符
bool Delimiter(QString str)
{
    for (QString p : delimiter)
    {
        if (p == str)
            return true;
    }
    return false;
}

// 判断单词是否是运算符
bool Operator(QString str)
{
    for (QString p : operator0)
    {
        if (p == str)
            return true;
    }
    return false;
}

// 判断字符是否为数字
bool Digit(QChar c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}


// 由 token 查保留字表，若 token 中字符串为保留字符则返回其类别编码，否则返回值为0
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

    // 标识符
    else if (str[0] == '_' || Letter(str[0]))
    {
        type = IDENTIFIER;
        {
            for (int i = 1; i < str.length(); i++)
            {
                // 若不符合标识符规则，则type=0
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
    // 清空输出框
    csOutput4->clear();
    csOutput4->setPlainText("");
    // 读取源代码将空格和回车删除
    QString token = csInPut4->toPlainText();

    // str 存入一个单词
    QString str = "";
    int i = 0;

    // 依次遍历源代码中的每一个字符
    while (i < token.length())
    {
        str = "";
        // 数字开头
        if (Digit(token[i]))
        {
            while (Digit(token[i])) {
                str = str + token[i++];
                // 打印分类结果
                csOutput4->moveCursor(QTextCursor::End);
                csOutput4->insertPlainText('(' + QString::number(Reserve(str)) + '"' + str + '"' + ')' + '\n');
            }
        }
        // 下划线或者字母开头的单词，即标识符或者关键字
        else if (token[i] == '_' || Letter(token[i]))
        {
            while (token[i] == '_' || Letter(token[i]) || Digit(token[i]))
            {
                str = str + token[i++];
                // 如果是关键字，立即结束
                if (Reserve(str) == KEYWORD)
                    break;
            }
            csOutput4->moveCursor(QTextCursor::End);
            csOutput4->insertPlainText('(' + QString::number(Reserve(str)) + '"' + str + '"' + ')' + '\n');
        }
        // 运算符
        else {
            str = str + token[i++];
            csOutput4->moveCursor(QTextCursor::End);
            csOutput4->insertPlainText('(' + QString::number(Reserve(str)) + '"' + str + '"' + ')' + '\n');
        }
    }
}