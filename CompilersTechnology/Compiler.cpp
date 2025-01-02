#include "Compiler.h"
#include "CompilersTechnology.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStack>

Compiler::Compiler(Ui::CompilersTechnologyClass* ui)
    : ui(ui)
{
    csInPut5 = ui->csInPut5;
    loadFileButton = ui->loadFile;
    csOutPut5 = ui->csOutPut5;
    csOutPut6 = ui->csOutPut6;
    chargeCs5_2  = ui->chargeCs5_2;
    error5 = ui->error5;

    connect(loadFileButton, &QPushButton::clicked, this, &Compiler::loadFile);
    connect(chargeCs5_2, &QPushButton::clicked, this, &Compiler::on_chargeCs5_2_clicked);
}

Compiler::~Compiler()
{
}
// 符号表
struct identifier2 {   // 保存的标识符信息
    int token;        // 该标识符返回的标记
    int line;
    QString name;     // 存放标识符本身的字符串
    int _class;       // 该标识符的类别，如数字，全局变量或局部变量等。
    int type;         // 标识符的类型，即如果它是个变量，变量是 int 型、char 型还是指针型。
    int value;        // 存放这个标识符的值，如标识符是函数，刚存放函数的地址。
    int Bclass;       // C 语言中标识符可以是全局的也可以是局部的，当局部标识符的名字与全局标识符相同时，用作保存全局标识符的信息。
    int Btype;
    int Bvalue;
};
QVector<identifier2> identifierList;  // 标识符列表
// 错误列表
struct error {
    QString identifier;
    int line;
};
QVector<error> errorList;  // 错误列表

//------------------------加载text文件---------------------//
void Compiler::loadFile() {
    // 使用 QFileDialog 打开文件选择对话框
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Text File"), "", tr("Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        // 打开文件
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            // 使用 QTextStream 读取文件内容
            QTextStream in(&file);
            QString content = in.readAll();

            // 将内容显示在 QTextEdit 中
            csInPut5->setText(content);

            // 关闭文件
            file.close();
        }
        else
        {
            // 如果文件打开失败，显示错误消息
            QMessageBox::warning(this, tr("Error"), tr("Cannot open file: %1").arg(fileName));
        }
    }
}
//------------------------加载text文件---------------------//
// 
//---------------------------词法分析----------------------//
// 词法分析输出
struct LexicalAnalysis_Output {   // 保存的标识符信息
    int token;        // 该标识符返回的标记
    QString name;     // 存放标识符本身的字符串
};
QVector<LexicalAnalysis_Output> lexicalAnalysis_Output;  // 标识符列表

#define KEYWORD         1      // 关键词
#define TYPE            2      // 类型
#define OPERATOR        3      // 运算符
#define IDENTIFIER      4      // 标识符
#define CONSTINTEGRAL   5      // 常量
#define left_Parentheses 6     // 左小括号
#define right_Parentheses 7    // 右小括号
#define left_Bracket 8         // 左中括号
#define right_Bracket 9        // 右中括号
#define left_Brace 10          // 左大括号
#define right_Brace 11         // 右大括号
#define MAIN 12                // main
#define SEMICOLON 13           // 分号
#define COMMA 14               // 逗号
#define IF 15                  // if
#define Else 16                // else
#define BOOL_OPERATOR 17       // bool运算符
#define WHILE 18               // while
#define FOR 19                 // for


#define ELSE            0      // 错误

// 关键词列表，界限符列表、运算符列表
QString type2[] = { "void", "int", "char" , "float", "bool"};
QString keyWord2[] = { "return"};
QString operator02[] = { "+", "-", "*", "/", "%", "=" , "|", "&", "&&", "||"};
QString boolOperator2[] = { ">", "<" , "<=", ">=" , "==" , "!="};

// 检测 char 中字符是否为字母
bool Letter2(QChar c)
{
    if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z')
    {
        return true;
    }
    return false;
}

// 检测单词是否为关键字
bool KeyWord2(QString str)
{
    for (QString p : keyWord2)
    {
        if (p == str)
            return true;
    }
    return false;
}

// 判断单词是否是类型
bool Type2(QString str)
{
    for (QString p : type2)
    {
        if (p == str)
            return true;
    }
    return false;
}
// 判断单词是否是运算符
bool Operator2(QString str)
{
    for (QString p : operator02)
    {
        if (p == str)
            return true;
    }
    return false;
}
// 判断单词是否是布尔运算符
bool BoolOperator2(QString str)
{
    for (QString p : boolOperator2)
    {
        if (p == str)
            return true;
    }
    return false;
}

// 判断字符是否为数字
bool Digit2(QChar c)
{
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    return false;
}
// 由 token 查保留字表，若 token 中字符串为保留字符则返回其类别编码，否则返回值为0
int Reserve2(QString str)
{
    int type = ELSE;

    if (KeyWord2(str))
        type = KEYWORD;

    else if (Type2(str))
        type = TYPE;

    else if (Operator2(str))
        type = OPERATOR;
    else if (BoolOperator2(str))
        type = BOOL_OPERATOR;
    else if (Digit2(str[0]))
        type = CONSTINTEGRAL;
    else if (str == "(")
        type = left_Parentheses;
    else if (str == ")")
        type = right_Parentheses;
    else if (str == "[")
        type = left_Bracket;
    else if (str == "]")
        type = right_Bracket;
    else if (str == "{")
        type = left_Brace;
    else if (str == "}")
        type = right_Brace;
    else if (str == ";")
        type = SEMICOLON;
    else if (str == "main")
        type = MAIN;
    else if (str == ",")
        type = COMMA;
    else if (str == "if")
        type = IF;
    else if (str == "else")
        type = Else;
    else if (str == "while")
        type = WHILE;
    else if (str == "for")
        type = FOR;
    // 标识符
    else if (str[0] == '_' || Letter2(str[0]))
    {
        type = IDENTIFIER;
        {
            for (int i = 1; i < str.length(); i++)
            {
                // 若不符合标识符规则，则type=0
                if (!(str[i] == '_' || Letter2(str[i]) || Digit2(str[i])))
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

void Compiler::lexicalAnalysis()
{
    // 清空
    lexicalAnalysis_Output.clear();

    int line = 1;   // 行号
    // 清空输出框
    csOutPut5->clear();
    csOutPut5->setPlainText("");
    // 读取源代码将空格和回车删除
    QString token = csInPut5->toPlainText();
    token.remove(QChar(' '));
    token.remove(QChar('\t'));
    // str 存入一个单词
    QString str = "";
    int i = 0;

    // 依次遍历源代码中的每一个字符
    while (i < token.length())
    {
        str = "";
        // 数字开头
        if (Digit2(token[i]))
        {
            while (Digit2(token[i])) {
                str = str + token[i++];
                // 保存表格
                LexicalAnalysis_Output temp_lexicalAnalysis_Output;
                temp_lexicalAnalysis_Output.token = Reserve2(str);
                temp_lexicalAnalysis_Output.name = str;
                lexicalAnalysis_Output.append(temp_lexicalAnalysis_Output);
                // 打印分类结果
                csOutPut5->moveCursor(QTextCursor::End);
                csOutPut5->insertPlainText('(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');
            }
        }
        // 下划线或者字母开头的单词，即标识符或者关键字
        else if (token[i] == '_' || Letter2(token[i]))
        {
            while (token[i] == '_' || Letter2(token[i]) || Digit2(token[i]))
            {
                str = str + token[i++];
                // 如果是关键字，立即结束
                if (Reserve2(str) == KEYWORD || Reserve2(str) == TYPE || Reserve2(str) == MAIN || Reserve2(str) == SEMICOLON || Reserve2(str) == COMMA || Reserve2(str) == IF || Reserve2(str) == Else || Reserve2(str) == WHILE || Reserve2(str) == FOR)
                    break;
            }
            LexicalAnalysis_Output temp_lexicalAnalysis_Output;
            temp_lexicalAnalysis_Output.token = Reserve2(str);
            temp_lexicalAnalysis_Output.name = str;
            lexicalAnalysis_Output.append(temp_lexicalAnalysis_Output);
            csOutPut5->moveCursor(QTextCursor::End);
            csOutPut5->insertPlainText('(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');
        }
        else if (token[i] == '\n') {
            line++;
            i++;
        }
        // 运算符
        else {
            str = str + token[i++];

            // 保存表格
            LexicalAnalysis_Output temp_lexicalAnalysis_Output;
            temp_lexicalAnalysis_Output.token = Reserve2(str);
            if (Reserve2(str) == 17) {
                temp_lexicalAnalysis_Output.name = "b";
            }
            else temp_lexicalAnalysis_Output.name = str;
            lexicalAnalysis_Output.append(temp_lexicalAnalysis_Output);
            // 
            csOutPut5->moveCursor(QTextCursor::End);
            csOutPut5->insertPlainText('(' + QString::number(Reserve2(str)) + '"' + temp_lexicalAnalysis_Output.name + '"' + ')' + '\n');
        }
    }
}
//---------------------------词法分析----------------------//

//---------------------------语法分析----------------------//
// LL（1）输入结构体
struct LL1Input2 {
    QString L;
    QString R;
    bool haveEmpty = 0;      // 是否有空产生式
    bool isFix = 0;          // 产生式是否修改过
};
// LL（1）分析表结构体
struct LL1Table2 {
    QString c;
    QMap<QString, QString> table;
};
QVector<LL1Table2> Table_all2;

// LL(1)输出
struct LL1Output2 {
    int id;
    QStack<QString> analysis_stack;
    QStack<QString> input_stack;
    QString use;
};

// 文法输入表
QVector<LL1Input2> Input_all2
{
    //LL1Input2{"Pro", "State_str"},                  // Program ::= <声明串>
    //LL1Input2{"State_str", "State A"},              // <声明串> ::=<声明>{<声明>}
    //LL1Input2{"A", "State_str|$"},
    //LL1Input2{"State", "int ID State_type|void ID Fun_state"}, //<声明> ::=int <声明类型> | void <函数声明>
    //LL1Input2{"State_type", "Var_state|Fun_state"},   // <声明类型>::=<变量声明> | <函数声明>
    //LL1Input2{"Var_state", ";"},                   // <变量声明> ::= ;
    //LL1Input2{"Fun_state", "(Form_par) Block"},     // <函数声明> ::=’ (‘<形参>’) ‘<语句块>
    //LL1Input2{"Form_par", "Par_list|$"}, // <形参>::= <参数列表> | $
    //LL1Input2{"Par_list", "Par B"},     // <参数列表> ::= <参数> {, <参数>}
    //LL1Input2{"B", ",Par B|$"},
    //LL1Input2{"Par", "int ID|char ID|float ID"},  // <参数> ::= int
    //LL1Input2{"Block", "{Iner_state Sent_str}"}, // <语句块> ::= ‘{‘<内部声明> <语句串>’}’
    //LL1Input2{"Iner_state", "Iner_par_sate Iner_state|$"},  // <内部声明> ::= 空 | <内部变量声明>{<内部变量声明>}
    //LL1Input2{"Iner_par_state", "int ID"},    // <内部变量声明>::=int ;
    //LL1Input2{"Sent_str", "Sent C"},    // <语句串> ::= <语句>{ <语句> }
    //LL1Input2{"C", "Sent_str|$"},
    //LL1Input2{"Sent", "Sent_if|Sent_while|Sent_return|Sent_eval"},  // <语句> ::= <if语句> |< while语句> | <return语句> | <赋值语句>
    //LL1Input2{"Sent_eval", "ID = Exp;"},  // <赋值语句> ::= =<表达式>;
    //LL1Input2{"D", "Exp;|;"},  // <return语句> ::= return [ <表达式> ] 
    //LL1Input2{"Sent_while", "while (Exp) Block"}, // <while语句> ::= while ‘( ‘<表达式> ‘)’ <语句块>
    //LL1Input2{"Sent_if", "if(Exp) Block E"}, // <if语句> ::= if ‘(‘<表达式>’)’ <语句块> else <语句块>
    //LL1Input2{"E", "else Block|$"}, 
    //LL1Input2{"Exp", "Exp_add F"},// <表达式>::=<加法表达式>{ relop <加法表达式> } 
    //LL1Input2{"F", "relop Exp_add F|$"},
    //LL1Input2{"Exp_add", "Nape G"}, // <加法表达式> ::= <项> {+ <项> | -<项>}
    //LL1Input2{"G", "+ Nape G|- Nape G|$"},
    //LL1Input2{"Nape", "Div H"}, // <项> ::= <因子> {* <因子> | /<因子>}
    //LL1Input2{"H", "* DivH|/ DivH|$"},
    //LL1Input2{"Div", "num|(Exp)|ID Ftype"}, // <因子> ::=num | ‘(‘<表达式>’)’ | FTYPE
    //LL1Input2{"Ftype", "Call|$"}, 
    //LL1Input2{"Call", "(Act)"}, // <call> ::=’(’<实参> ’) ’
    //LL1Input2{"Act", "Act_list|$"}, // <实参> ::=<实参列表> | 空
    //LL1Input2{"Act_list", "Exp H"}, // <实参列表> ::= <表达式>{, <表达式>}
    //LL1Input2{"H", ",Exp H|$"}, // <实参列表> ::= <表达式>{, <表达式>}
    // 文法输入表
    // 终结符:{
    /* 'm' : keyword:main 12, 't' : 类型TYPE 2, 'i' : 标识符IDENTIFIER 4， 'n' : 常量CONSTINTEGRAL 5, '(' : 左小括号left_Parentheses 6,
     ')' : 右小括号right_Parentheses 7, '[' : 左中括号left_Bracket 8, ']' : 右中括号right_Bracket 9,
     '{' : 左大括号left_Brace 10, '}' : 右大括号right_Brace 11, ';' : 分号SEMICOLON 13, ',' : 逗号COMMA 14
     'r' : keyword:if, 'e' :keyword:else, 'b' : 布尔运算符BOOL, 'w' : keyword:while, 'f' : keyword:for,*/
    LL1Input2{"P", "tm()K"}, // "程序", "void main()语句块"
    LL1Input2{"K", "{C}"}, // "语句块", "{语句串}"
    //LL1Input2{"C", "CJ"}, // "语句串", "语句串 语句|$"
    LL1Input2{"C", "C'"}, // "语句串", "语句串 语句|$"
    LL1Input2{"C'", "JC'"}, // "语句串", "语句串 语句|$"
    LL1Input2{"C'", "$"},
    LL1Input2{"J", "I"},//"语句", "if语句|while语句|return语句|赋值语句|输入语句|输出语句|声明语句|for语句"
    LL1Input2{"J", "W"},// while语句
    LL1Input2{"J", "R"},// return语句
    LL1Input2{"J", "Z"},// 赋值语句
    LL1Input2{"J", "S"},// 输入语句
    LL1Input2{"J", "O"},// 输出语句
    LL1Input2{"J", "V"},// 声明语句
    LL1Input2{"J", "X"},// for语句
    // 声明语句
    LL1Input2{"V", "tiV';"}, // "声明语句", "int ID;"
    LL1Input2{"V'", ",iV'"}, 
    LL1Input2{"V'", "$"},
    // 赋值语句
    LL1Input2{"Z", "i=E;"}, // "赋值语句", "i = E;"
    LL1Input2{"E", "TE'"},  // "表达式", "表达式 运算符 表达式|ID|常量"
    LL1Input2{"E'", "ATE'"},
    LL1Input2{"E'", "$"},
    LL1Input2{"T", "FT'"},
    LL1Input2{"T'", "MFT'"},
    LL1Input2{"T'", "$"},
    LL1Input2{"F", "(E)"},
    LL1Input2{"F", "i"},
    LL1Input2{"F", "n"},
    LL1Input2{"A", "-"},
    LL1Input2{"A", "+"},
    LL1Input2{"M", "/"},
    LL1Input2{"M", "*"},
    // 表达式
    LL1Input2{"Z'", "i=E"},
    // 布尔表达式
    LL1Input2{"B", "EbE"},
    //// 输入输出语句
    //LL1Input2{"输入语句", "cin >> i;"},
    //LL1Input2{"输出语句", "cout << E;"},
    // 条件语句
    // if语句
    LL1Input2{"I", "r(B)KI'"}, // "if语句", "if(E)语句块else语句块"
    LL1Input2{"I'", "eK"},
    LL1Input2{"I'", "$"},
    // while语句
    LL1Input2{"W", "w(B)K"}, // "while语句", "while(E)语句块"
    // for语句
    LL1Input2{"X", "f(tZB';G)K"}, // "for语句", "f(B)K"

    LL1Input2{"B'", "B"},
    LL1Input2{"B'", "$"},
    LL1Input2{"G", "iG'"},
    LL1Input2{"G", "$"},
    LL1Input2{"G'", "++"},
    LL1Input2{"G'", "--"},
    // return语句
    //LL1Input2{"return语句", "return E;"},
};
QChar is_vt(LexicalAnalysis_Output num) {
    if (num.token == 2) return 't';
    else if (num.token == 4) return 'i';
    else if (num.token == 5) return 'n';
    else if (num.token == 12) return 'm';
    else if (num.token == 15) return 'r';
    else if (num.token == 16) return 'e';
    else if (num.token == 17) return 'b';
    else if (num.token == 18) return 'w';
    else if (num.token == 19) return 'f';

    else return num.name[0];
}
//
QString vt = "wfbreimtn+-*/=()[]{};,#";
QString vn = "PKCJIWRZSOVETFAMBLXDG";
QString s = "P";

// 判断是否属于非终结符
bool isVn2(QString Vn, QString s) {
    for (int i = 0; i < Vn.size(); i++) {
        if (Vn[i] == s[0]) return true;
    }
    return false;
}
// 判断s中是否含有字符串t
QVector<int> KMP2(QString s, QString t) {
    QVector<int> temp;
    for (int i = 0; i < s.size(); i++) {
        int j = 0;
        int tmp = i;
        while (j < t.size() && tmp < s.size()) {
            if (t[j] == s[tmp]) {
                tmp++;
                j++;
                if (j == t.size()) {
                    if (t.size() == 2) temp.push_back(i + 1);
                    else if (t.size() == 1 && tmp < s.size() && s[tmp] == "'") continue;
                    else temp.push_back(i);
                }
            }
            else {
                break;
            }
        }

    }
    return temp;
}

QSet<QString> first2;      // first集
QSet<QString> follow2;      // follow集
// 求产生式右侧FIRST集
void getFirst2(QVector<LL1Input2> input, QString sr, QString vn, QString S) {
    QString one = sr[0];  // 假设右侧第一个符号为FIRST集
    if (isVn2(vn, one)) {
        // 右侧第一个符号为非终结符
        if (sr.size() > 1 && sr[1] == "'") one.append(sr[1]); // 右侧第一个符号为去除递归的非终结符

        // 遍历所有产生式
        for (int i = 0;i < input.size();i++) {
            if (input[i].R != "$" && input[i].L == one) {
                QString one = input[i].R[0];  // 假设右侧第一个符号为FIRST集
                if (isVn2(vn, one)) {
                    // 右侧第一个符号为非终结符
                    if (1 < input[i].R.size() && input[i].R[1] == "'") one.append("'"); // 右侧第一个符号为去除递归的非终结符
                    getFirst2(input, one, vn, S);
                }
                else {
                    // 右侧第一个符号为终结符或空集$
                    first2.insert(one);
                }
            }
            else if (input[i].L == one && input[i].R == "$") first2.insert("$");
        }
    }
    else {
        // 右侧第一个符号为终结符
        first2.insert(one);
    }
}
// 求产生式左侧FOLLOW集
void getFollow2(QVector<LL1Input2> input, QString s, QString vn, QString S) {
    // 如果产生式左侧符号为开始符号S，则FOLLOW集首先包含#
    if (s == S) follow2.insert("#");
    // 遍历所有产生式
    for (int i = 0; i < input.size(); i++) {
        QVector<int> temp = KMP2(input[i].R, s);

        // 找到该符号的位置
        for (int j = 0;j < temp.size();j++) {
            // 如果是最后一个符号，则求产生式左侧FOLLOW集
            if (temp[j] == input[i].R.size() - 1) {
                if (input[i].R[temp[j]] != input[i].L[input[i].L.size() - 1]) {
                    qDebug() << "左递归" << input[i].L;
                    getFollow2(input, input[i].L, vn, S);
                }
            }
            else {
                QString next = input[i].R[temp[j] + 1];
                qDebug() << "next0" << next;
                if (isVn2(vn, next)) {
                    if (temp[j] + 2 < input[i].R.size() && input[i].R[temp[j] + 2] == "'") next.append("'"); // 右侧第一个符号为去除递归的非终结符
                    qDebug() << "next1" << next;
                    // 如果是非终结符，则其FIRST集都加入FOLLOW集
                    first2.clear();
                    getFirst2(input, next, vn, S);
                    for (auto f : first2) {
                        follow2.insert(f);
                    }
                    // 如果该非终结符可以推导出空串，那么可以认为该符号在末尾，使用第三条规则
                    if (first2.contains("$")) getFollow2(input, input[i].L, vn, S);
                }
                else {
                    follow2.insert(next);
                }
            }
        }
    }
}
// 构造LL(1)分析表
void getLL1Table2(QString vn, QString s) {
    for (int i = 0;i < Input_all2.size();i++) {
        LL1Table2 tmp_table;
        tmp_table.c = Input_all2[i].L;
        // 求产生式右侧FIRST集
        first2.clear();
        getFirst2(Input_all2, Input_all2[i].R, vn, s);
        for (auto f : first2) {
            QString tmp = Input_all2[i].L + "->" + Input_all2[i].R;
            tmp_table.table.insert(f, tmp);
        }
        if (Input_all2[i].R == "$") {
            // 如果产生式为空串，则求FOLLOW集
            follow2.clear();
            getFollow2(Input_all2, Input_all2[i].L, vn, s);
            for (auto f : follow2) {
                QString tmp = Input_all2[i].L + "->$";
                tmp_table.table.insert(f, tmp);
            }
        }

        // 加入Table_all
        Table_all2.push_back(tmp_table);
    }
    QSet<QString> temp;      // 用于去重
    // 去除重复的产生式，并且合并
    for (int i = 0;i < Table_all2.size();i++) {
        if (!temp.contains(Table_all2[i].c)) temp.insert(Table_all2[i].c);
        else {
            int index = -1;
            // 找到已存在的下标
            for (int j = 0;j < Table_all2.size();j++) {
                if (Table_all2[j].c == Table_all2[i].c) {
                    index = j;
                    break;
                }
            }

            // 合并产生式
            for (auto it = Table_all2[i].table.begin(); it != Table_all2[i].table.end(); ++it) {
                Table_all2[index].table.insert(it.key(), it.value());
            }
            // 去除多余的产生式
            Table_all2.remove(i);
            i--;
        }
    }
    qDebug() << Table_all2.size();
}
// 分析输入串
LL1Output2 analysis(LL1Output2 output, QString inputString, QString S, int now) {
    LL1Output2 temp_output = output;
    temp_output.id = now + 1;
    // 
    if (!output.use.isEmpty()) {
        // 产生式左侧出栈
        qDebug() << QString::number(temp_output.id) + "出栈：" + temp_output.analysis_stack.top();
        temp_output.analysis_stack.pop();

        // 产生式右侧入栈
        int r = KMP2(output.use, ">")[0] + 1;
        QString right = output.use.mid(r, output.use.size() - r);
        qDebug() << "右侧：" << right;
        for (int i = right.size() - 1; i >= 0; i--) {
            // 如果含有’，则先将’和前面的符号一起入栈
            if (i > 0 && right[i] == "'") {
                QString temp = right[i - 1];
                temp.append("'");
                qDebug() << "入栈：" + temp;
                temp_output.analysis_stack.push(temp);
                i--;
            }
            else {
                if (right[i] != "$") temp_output.analysis_stack.push(right[i]);
            }
        }
        qDebug() << temp_output.analysis_stack.top() << temp_output.input_stack.top();
        qDebug() << ((temp_output.analysis_stack.top() == temp_output.input_stack.top()) && (temp_output.analysis_stack.top() == "#"));
        qDebug() << (temp_output.analysis_stack.top() != temp_output.input_stack.top());
        // 如果栈顶不相等则查表得出产生式
        if (temp_output.analysis_stack.top() != temp_output.input_stack.top()) {
            for (int i = 0;i < Table_all2.size();i++) {
                if (Table_all2[i].c == temp_output.analysis_stack.top()) {
                    if (Table_all2[i].table.contains(temp_output.input_stack.top()))
                    {
                        temp_output.use = Table_all2[i].table.value(temp_output.input_stack.top());
                        break;
                    }
                }
                else if (i == Table_all2.size() - 1) {
                    temp_output.use = "error";
                    return temp_output;
                }
            }
        }
        else if (temp_output.analysis_stack.top() == "#") temp_output.use = "acc";   // 若栈顶相等且为#，则接受
        else temp_output.use = ""; // 如果相等且不为#，则产生式为空
    }
    else {
        temp_output.analysis_stack.pop();
        temp_output.input_stack.pop();
        // 如果栈顶不相等则查表得出产生式
        if (temp_output.analysis_stack.top() != temp_output.input_stack.top()) {
            for (int i = 0;i < Table_all2.size();i++) {
                if (Table_all2[i].c == temp_output.analysis_stack.top()) {
                    if (Table_all2[i].table.contains(temp_output.input_stack.top()))
                    {
                        temp_output.use = Table_all2[i].table.value(temp_output.input_stack.top());
                        break;
                    }
                }
                else if (i == Table_all2.size() - 1) {
                    temp_output.use = "error";
                    return temp_output;
                }
            }
        }
        else if (temp_output.analysis_stack.top() == "#") temp_output.use = "acc";   // 若栈顶相等且为#，则接受
        else temp_output.use = ""; // 如果相等且不为#，则产生式为空

    }

    // 分析过程
    return temp_output;
}

void Compiler::syntaxAnalysis() {
    // 清空
    first2.clear();
    follow2.clear();
    Table_all2.clear();

    // 清空输出窗口
    csOutPut5->setPlainText("LL(1)分析表构造中...");
    // 获取输入串
    QString input_String = "";
    for (auto f : lexicalAnalysis_Output) {
        input_String.append(is_vt(f));
    }
    qDebug() << "输入串：" << input_String;

    // 构造LL(1)分析表
    getLL1Table2(vn, s);
    qDebug() << Input_all2.size();
    qDebug() << Table_all2.size();
    // 输出LL(1)分析表Table_all
    csOutPut6->setPlainText("LL(1)分析表：\n");
    for (int i = 0; i < vt.size();i++) {
        csOutPut6->moveCursor(QTextCursor::End);
        csOutPut6->insertPlainText('\t' + vt[i]);
        if (i == vt.size() - 1) csOutPut6->insertPlainText("\n");
    }
    for (int i = 0; i < Table_all2.size();i++) {
        csOutPut6->moveCursor(QTextCursor::End);
        csOutPut6->insertPlainText(Table_all2[i].c + "\t");
        for (auto f : vt) {
            // 如果Table_all中含有该终结符的产生式, 则输出
            if (Table_all2[i].table.contains(f)) {
                csOutPut6->insertPlainText(Table_all2[i].table[f] + "\t");
            }
            else {
                csOutPut6->moveCursor(QTextCursor::End);
                csOutPut6->insertPlainText("\t");
            }
        }
        csOutPut6->insertPlainText("\n");
    }

    // 输出分析过程
    csOutPut5->setPlainText("字符分析过程：\n步骤\t分析栈\t输入串\t产生式\n");
    // 初始化输出表
    int now = 1;
    LL1Output2 output;
    output.id = now;
    output.analysis_stack.push("#");
    output.analysis_stack.push(s);
    output.input_stack.push("#");
    for (int i = input_String.size() - 1; i >= 0; i--) output.input_stack.push(input_String[i]);
    for (int i = 0;i < Table_all2.size();i++) {
        if (Table_all2[i].c == s) output.use = Table_all2[i].table.value(output.input_stack.top());
    }
    // 输出分析过程
    csOutPut5->moveCursor(QTextCursor::End);
    csOutPut5->insertPlainText(QString::number(output.id) + "\t");
    QString temp = "";
    QStack<QString> temp_stack = output.analysis_stack;
    while (temp_stack.size()) {
        temp.prepend(temp_stack.top());
        temp_stack.pop();
    }
    csOutPut5->moveCursor(QTextCursor::End);
    csOutPut5->insertPlainText(temp + "\t");
    qDebug() << "分析栈" << temp;
    temp = "";
    temp_stack = output.input_stack;
    while (temp_stack.size()) {
        temp.append(temp_stack.top());
        temp_stack.pop();
    }
    csOutPut5->moveCursor(QTextCursor::End);
    csOutPut5->insertPlainText(temp + "\t" + output.use + '\n');
    qDebug() << "输入栈" << temp;
    while (output.use != "acc" && output.use != "error") {
        output = analysis(output, input_String, s, now);
        now++;
        // 输出分析过程
        csOutPut5->moveCursor(QTextCursor::End);
        csOutPut5->insertPlainText(QString::number(output.id) + "\t");
        QString temp = "";
        temp_stack = output.analysis_stack;
        while (temp_stack.size()) {
            temp.prepend(temp_stack.top());
            temp_stack.pop();
        }
        csOutPut5->moveCursor(QTextCursor::End);
        csOutPut5->insertPlainText(temp + "\t");
        temp = "";
        temp_stack = output.input_stack;
        while (temp_stack.size()) {
            temp.append(temp_stack.top());
            temp_stack.pop();
        }
        csOutPut5->moveCursor(QTextCursor::End);
        csOutPut5->insertPlainText(temp + "\t" + output.use + '\n');
        
    }
}
//---------------------------语法分析----------------------//

//---------------------------语义分析----------------------//
void Compiler::semanticAnalysis() {

}
//---------------------------语义分析----------------------//

//---------------------------生成代码----------------------//
void Compiler::generateCode() {

}
//---------------------------生成代码----------------------//

// 
void Compiler::on_chargeCs5_2_clicked() {
    Compiler compiler(ui);
    compiler.lexicalAnalysis(); // 调用 lexicalAnalysis 方法
    compiler.syntaxAnalysis(); // 调用 syntaxAnalysis 方法
    //compiler.semanticAnalysis(); // 调用 semanticAnalysis 方法
    //compiler.generateCode(); // 调用 generateCode 方法

}

