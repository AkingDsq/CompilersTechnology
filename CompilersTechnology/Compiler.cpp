#include "Compiler.h"
#include "CompilersTechnology.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStack>
#include <algorithm>


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
    int line;         // 返回行号
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
    /*csOutPut5->clear();
    csOutPut5->setPlainText("");*/

    error5->clear();
    error5->setPlainText("");

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
                temp_lexicalAnalysis_Output.line = line;
                lexicalAnalysis_Output.append(temp_lexicalAnalysis_Output);
                // 打印分类结果
                /*csOutPut5->moveCursor(QTextCursor::End);
                csOutPut5->insertPlainText(QString::number(temp_lexicalAnalysis_Output.line) + ',' + '(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');*/
                
                error5->moveCursor(QTextCursor::End);
                error5->insertPlainText(QString::number(temp_lexicalAnalysis_Output.line) + ',' + '(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');
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
            temp_lexicalAnalysis_Output.line = line;
            lexicalAnalysis_Output.append(temp_lexicalAnalysis_Output);
            /*csOutPut5->moveCursor(QTextCursor::End);
            csOutPut5->insertPlainText(QString::number(temp_lexicalAnalysis_Output.line) + ',' + '(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');*/
            error5->moveCursor(QTextCursor::End);
            error5->insertPlainText(QString::number(temp_lexicalAnalysis_Output.line) + ',' + '(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');
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
            //if (Reserve2(str) == 17) {
            //    temp_lexicalAnalysis_Output.name = "b";
            //}
            temp_lexicalAnalysis_Output.name = str;
            temp_lexicalAnalysis_Output.line = line;
            lexicalAnalysis_Output.append(temp_lexicalAnalysis_Output);
            // 
            /*csOutPut5->moveCursor(QTextCursor::End);
            csOutPut5->insertPlainText(QString::number(temp_lexicalAnalysis_Output.line) + ',' + '(' + QString::number(Reserve2(str)) + '"' + temp_lexicalAnalysis_Output.name + '"' + ')' + '\n');*/
        
            error5->moveCursor(QTextCursor::End);
            error5->insertPlainText(QString::number(temp_lexicalAnalysis_Output.line) + ',' + '(' + QString::number(Reserve2(str)) + '"' + str + '"' + ')' + '\n');
        }
    }
}
//---------------------------词法分析----------------------//

//---------------------------语法分析----------------------//
// LL（1）输入结构体
struct LL1Input2 {
    QString L;
    QString R;

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
// 
struct Symbol
{
    QString varName;       //变量名
    QString valueStr{ "0" }; //变量的值，字符串形式,初始化为0
    int PLACE{ -1 };        //该变量在符号表中的位置,初始化为-1
};
struct FourYuanFormula
{                  //四元式结构体
    QString op;     //操作符
    int arg1Index; //源操作数1的符号表地址
    int arg2Index; //源操作数2的符号表地址
    Symbol result; //目的操作数
};
QVector<FourYuanFormula> formula; //四元式序列
QVector<Symbol> symbolTable;      //符号表
QMap<QString, int> ENTRY;          //用于查变量的符号表入口地址
int tempVarNum = 0;              //临时变量个数
Symbol newtemp()
{ //生成新的临时变量
    tempVarNum++;
    return Symbol{ "T" + QString::number(tempVarNum) };
}

void GEN(QString op, int arg1, int arg2, Symbol& result)
{ //运算符、参数1在符号表的编号、参数2在符号表的编号，结果符号
    //产生一个四元式，并填入四元式序列表
    QString op_str = "";
    op_str = "(" + op + ",";
    arg1 != -1 ? op_str.append( symbolTable[arg1].varName) : op_str.append("_");
    op_str.append(",");
    arg2 != -1 ? op_str.append(symbolTable[arg2].varName) : op_str.append("_");
    op_str.append("," + result.varName + ")" );
    formula.push_back(FourYuanFormula{ op, arg1, arg2, result }); //插入到四元式序列中
    if (op == "@")
    { //将临时变量result注册进入符号表
        result.varName;
        result.PLACE = symbolTable.size();
        result.valueStr = "-" + symbolTable[arg1].valueStr;
        symbolTable.push_back(result);
        ENTRY[result.varName] = result.PLACE;
    }
    if (op == "+")
    { //将临时变量result注册进入符号表
        result.PLACE = symbolTable.size();
        result.valueStr = QString::number(symbolTable[arg1].valueStr.toInt() + symbolTable[arg2].valueStr.toInt());
        symbolTable.push_back(result);
        ENTRY[result.varName] = result.PLACE;
    }
    if (op == "-")
    { //将临时变量result注册进入符号表
        result.PLACE = symbolTable.size();
        result.valueStr = QString::number(symbolTable[arg1].valueStr.toInt() - symbolTable[arg2].valueStr.toInt());
        symbolTable.push_back(result);
        ENTRY[result.varName] = result.PLACE;
    }
    if (op == "*")
    { //将临时变量result注册进入符号表
        result.PLACE = symbolTable.size();
        result.valueStr = QString::number(symbolTable[arg1].valueStr.toInt() * symbolTable[arg2].valueStr.toInt());
        symbolTable.push_back(result);
        ENTRY[result.varName] = result.PLACE;
    }
    if (op == "/")
    { //将临时变量result注册进入符号表
        result.PLACE = symbolTable.size();
        result.valueStr = QString::number(symbolTable[arg1].valueStr.toInt() / symbolTable[arg2].valueStr.toInt());
        symbolTable.push_back(result);
        ENTRY[result.varName] = result.PLACE;
    }
    if (op == "=") //这个result不是临时变量了，故不用注册进入符号表，只进行绑定
        result.valueStr = symbolTable[arg1].valueStr;
}

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
// 临时变量
struct Temp_num {
    QString name;
    QString arg1;
    QString arg2 = "";
    QString res;
};
QVector<Temp_num> temp_num;    // 保存的临时变量列表

// 四元式
struct four_address_code {
    QString op;
    QString arg1;
    QString arg2 = "";
    QString res;
    int Block;
    QString type;
};
QVector<four_address_code> four_address_code_all;
// 语法块
QVector<QString> blockNumber;  // 保存的语法块列表
// 划分行
void Block() {
    int line = 1;
    QString temp;
    for (int i = 0; i < lexicalAnalysis_Output.size(); i++) {
        if (lexicalAnalysis_Output[i].line == line) {
            temp.append(lexicalAnalysis_Output[i].name);
        }
        else {
            blockNumber.push_back(temp);
            line++;
            temp.clear();
            temp.append(lexicalAnalysis_Output[i].name);
        }
    }
}

//给运算符优先级排序
int prio(QChar op) {                 
    int priority;
    if (op == '*' || op == '/')
        priority = 4;
    if (op == '+' || op == '-')
        priority = 3;
    if (op == '(')
        priority = 2;
    if (op == '=' || op == '>' || op == '<') {
        priority = 1;
    }
    return priority;
}
// 转化后缀表达式
QString Trans(QString str) {   //引用传递
    QString str1;
    QStack<QChar> ts;                   //定义一个char类型的栈s
    int i;
    for (i = 0; i < str.size(); i++) {
        if (str[i] >= '0' && str[i] <= '9' || str[i] >= 'a' && str[i] <= 'z') {    //如果是数字或字母，直接入中缀表达式
            str1 += str[i];
        }
        else {                        //否则不是数字
            if (ts.empty())            //栈空则入站
                ts.push(str[i]);
            else if (str[i] == '(')   //左括号入栈
                ts.push(str[i]);
            else if (str[i] == ')') {  //如果是右括号，只要栈顶不是左括号，就弹出并输出
                while (ts.top() != '(') {
                    str1 += ts.top();
                    ts.pop();
                }
                ts.pop();                 //弹出左括号，但不输出
            }
            else {
                while (prio(str[i]) <= prio(ts.top())) { //栈顶优先级大于等于当前运算符，则输出
                    str1 += ts.top();
                    ts.pop();
                    if (ts.empty())      //栈为空，停止
                        break;
                }
                ts.push(str[i]);   //把当前运算符入栈
            }
        }
    }
    while (!ts.empty()) {      //最后，如果栈不空，则弹出所有元素并输出
        str1 += ts.top();
        ts.pop();
    }
    return str1;
}
int temp_num_count = 0;
// 真链
QVector<int> True1;
QVector<int> True2;
// 假链
QVector<int> Flase;
// 生成四元式
// 块号
void Gen(int start, int end) {
    
    // 四元式生成
    for (int i = start; i <= end; i++) {
        if (blockNumber[i].contains("if")) {
            // 清空真链假链
            True1.clear();
            True2.clear();
            Flase.clear();

            int l = KMP2(blockNumber[i], "(")[0] + 1;
            int r = KMP2(blockNumber[i], ")")[0] - 1;
            QString back = Trans(blockNumber[i].mid(l, r - l + 1));

            QStack<QString> tmp;
            for (int j = 0; j < back.size(); j++) {
                if (back[j] >= 'a' && back[j] <= 'z' || back[j] >= '0' && back[j] <= '9') {
                    tmp.push(back[j]);
                }
                else if (back[j] == '<' || back[j] == '>') {
                    four_address_code code;
                    code.op = 'J' + back[j];
                    code.arg2 = tmp.top();
                    tmp.pop();
                    code.arg1 = tmp.top();
                    tmp.pop();
                    code.res = "";
                    code.type = "if";
                    four_address_code_all.push_back(code);
                    True1.push_back(four_address_code_all.size() - 1); // 真链
                }
                else {
                    four_address_code code;
                    code.op = back[j];
                    code.arg2 = tmp.top();
                    tmp.pop();
                    code.arg1 = tmp.top();
                    tmp.pop();
                    QString temp_name = "T" + QString::number(temp_num_count);
                    code.res = temp_name;
                    temp_num_count++;
                    tmp.push(temp_name);
                    code.type = "if";
                    four_address_code_all.push_back(code);
                }
            }
            four_address_code code;
            code.op = 'J';
            code.arg2 = " ";
            code.arg1 = " ";
            code.res = "";
            four_address_code_all.push_back(code);
            True2.push_back(four_address_code_all.size() - 1); // 真2链
            // 回填真链1
            four_address_code_all[True1[0]].res = QString::number(four_address_code_all.size() + 1);
            // if
            int temp_end = 0;
            for (int j = i + 1;j < blockNumber.size(); j++) {
                if (blockNumber[j].contains('}')) {
                    temp_end = j - 1;
                    break;
                }
            }
            Gen(i + 1, temp_end);
            
            code.op = 'J';
            code.arg2 = " ";
            code.arg1 = " ";
            code.res = "";
            four_address_code_all.push_back(code);
            Flase.push_back(four_address_code_all.size() - 1);
            i = temp_end + 2;
            // else
            if (blockNumber[i].contains("else")) {
                // 回填真链2
                four_address_code_all[True2[0]].res = QString::number(four_address_code_all.size() + 1);
                for (int j = i;j < blockNumber.size(); j++) {
                    if (blockNumber[j].contains('}')) {
                        temp_end = j - 1;
                        break;
                    }
                }

                Gen(i + 1, temp_end);
                code.op = 'J';
                code.arg2 = " ";
                code.arg1 = " ";
                code.res = "";
                four_address_code_all.push_back(code);
                Flase.push_back(four_address_code_all.size() - 1);
            }
            i = temp_end + 1;

            // 回填
            for (int j = 0; j < Flase.size(); j++) {
                four_address_code_all[Flase[j]].res = QString::number(four_address_code_all.size() + 1);
            }
        }
        else if (blockNumber[i].contains("while")) {
            // 清空真链假链
            True1.clear();
            True2.clear();
            Flase.clear();
            True2.push_back(four_address_code_all.size() + 1);
            // 
            int l = KMP2(blockNumber[i], "(")[0] + 1;
            int r = KMP2(blockNumber[i], ")")[0] - 1;
            QString back = Trans(blockNumber[i].mid(l, r - l + 1));

            QStack<QString> tmp;
            for (int j = 0; j < back.size(); j++) {
                if (back[j] >= 'a' && back[j] <= 'z' || back[j] >= '0' && back[j] <= '9') {
                    tmp.push(back[j]);
                }
                else if (back[j] == '<' || back[j] == '>') {
                    four_address_code code;
                    code.op = 'J' + back[j];
                    code.arg2 = tmp.top();
                    tmp.pop();
                    code.arg1 = tmp.top();
                    tmp.pop();
                    code.res = "";
                    code.type = "while";
                    four_address_code_all.push_back(code);
                    True1.push_back(four_address_code_all.size() - 1); // 真链
                }
                else {
                    four_address_code code;
                    code.op = back[j];
                    code.arg2 = tmp.top();
                    tmp.pop();
                    code.arg1 = tmp.top();
                    tmp.pop();
                    QString temp_name = "T" + QString::number(temp_num_count);
                    code.res = temp_name;
                    temp_num_count++;
                    tmp.push(temp_name);
                    code.type = "while";
                    four_address_code_all.push_back(code);
                }
            }
            four_address_code code;
            code.op = 'J';
            code.arg2 = " ";
            code.arg1 = " ";
            code.res = "";
            four_address_code_all.push_back(code);
            Flase.push_back(four_address_code_all.size() - 1); // 假链
            // 回填真链
            four_address_code_all[True1[0]].res = QString::number(four_address_code_all.size() + 1);
            // while
            int temp_end = 0;
            for (int j = i;j < blockNumber.size(); j++) {
                if (blockNumber[j].contains('}')) {
                    temp_end = j - 1;
                    break;
                }
            }
            Gen(i + 1, temp_end);

            code.op = 'J';
            code.arg2 = " ";
            code.arg1 = " ";
            code.res = "";
            four_address_code_all.push_back(code);
            four_address_code_all[four_address_code_all.size() - 1].res = QString::number(True2[0]);
            i = temp_end + 1;
            // 回填
            for (int j = 0; j < Flase.size(); j++) {
                four_address_code_all[Flase[j]].res = QString::number(four_address_code_all.size() + 1);
            }

        }
        else if (blockNumber[i].contains("for")) {
            int temp_end = 0;
            for (int j = i;j < blockNumber.size(); j++) {
                if (blockNumber[j].contains('}')) {
                    temp_end = j - 1;
                    break;
                }
            }
            Gen(i + 1, temp_end);

            four_address_code code;
            code.op = 'J';
            code.arg2 = " ";
            code.arg1 = " ";
            code.res = "";
            four_address_code_all.push_back(code);
        }
        else if (blockNumber[i].contains("=")) {
            qDebug() << blockNumber[i];
            QString back = Trans(blockNumber[i].mid(0, blockNumber[i].size() - 1));
            qDebug() << back;
            QStack<QString> tmp;
            for (int j = 0; j < back.size(); j++) {
                if (back[j] >= 'a' && back[j] <= 'z' || back[j] >= '0' && back[j] <= '9') {
                    tmp.push(back[j]);
                }
                else {
                    four_address_code code;
                    code.op = back[j];
                    if (back[j] != '=') {
                        code.arg2 = tmp.top();
                        tmp.pop();
                    }
                    code.arg1 = tmp.top();
                    tmp.pop();
                    if (back[j] != '=') {
                        QString temp_name = "T" + QString::number(temp_num_count);
                        code.res = temp_name;
                        temp_num_count++;
                        tmp.push(temp_name);
                    }
                    else {
                        code.res = tmp.top();
                        tmp.pop();
                    }
                    code.type = "assign";
                    four_address_code_all.push_back(code);
                }
            }
        }
    }
}
void Compiler::semanticAnalysis() {
    // 初始化
    temp_num_count = 0;
    blockNumber.clear();
    temp_num.clear();
    four_address_code_all.clear();
    //
    Block();
    // 生成四元式
    Gen(0, blockNumber.size() - 1);
    four_address_code code;
    code.op = "return";
    code.arg2 = " ";
    code.arg1 = " ";
    code.res = " ";
    four_address_code_all.push_back(code);
    // 输出四元式
    csOutPut6->setPlainText("四元式：\n");
    for (int i = 0; i < four_address_code_all.size();i++) {
        csOutPut6->moveCursor(QTextCursor::End);
        csOutPut6->insertPlainText(QString::number(i + 1) + " (" + four_address_code_all[i].op + "," + four_address_code_all[i].arg1 + "," + four_address_code_all[i].arg2 + "," + four_address_code_all[i].res + ")\n");
    }
}
//---------------------------语义分析----------------------//

//---------------------------生成代码----------------------//
// 划分块
QVector<QVector<int>> fenKuai() {
    QVector<QVector<int>> temp;
    QVector<int> temp_block;
    QVector<bool> visited(four_address_code_all.size(), false); // 用于标记已访问的指令

    for (int i = 0; i < four_address_code_all.size(); i++) {
        if (!visited[i]) { // 如果该指令还未被访问，则开始一个新的块
            temp_block.clear();
            // 从当前指令开始，添加到当前块中
            int current = i;
            while (current < four_address_code_all.size() && !visited[current]) {
                temp_block.push_back(current);
                visited[current] = true; // 标记为已访问

                // 检查是否有跳转指令，如果有则根据跳转找到下一个指令
                if (four_address_code_all[current].op == "J" || four_address_code_all[current].op == "J<" || four_address_code_all[current].op == "J>") {
                    temp.push_back(temp_block); // 添加当前块到结果中
                    temp_block.clear(); // 清空当前块
                    // 这里假设跳转的目标是指令的索引，可以根据你的数据结构调整
                    current = QString(four_address_code_all[current].res).toInt() - 1; // 设置为跳转的目标
                }
                else {
                    current++; // 否则继续处理下一条指令
                }
            }
            if(temp_block.size() > 0) temp.push_back(temp_block); // 添加当前块到结果中
        }
    }

    return temp;
}

// 寄存器数量
int register_num;
QHash<QString, QString> tempToRegMap; // 临时变量与寄存器的映射
QString getRegister() {
    return "$t" + QString::number(register_num++);
}
// 辅助函数：获取操作数的寄存器
QString getOperandReg(QString operand) {
    if (operand[0] == 'T') {
        if (!tempToRegMap.contains(operand)) {
            QString reg = getRegister();
            tempToRegMap[operand] = reg;
            return reg; // 新分配的寄存器
        }
        else {
            return tempToRegMap[operand]; // 已存在的寄存器
        }
    }
    else {
        // 直接返回操作数（如常数或变量）
        return operand;
    }
}
QVector<QString> GenerateCode(four_address_code qua, QVector<QVector<int>> temp_fenKuai) {
    QVector<QString> code; // 存放生成的代码
    if (qua.op == "+") {
        QString reg1 = getOperandReg(qua.arg1);
        QString reg2 = getOperandReg(qua.arg2);
        QString resultReg = getRegister();
        code.push_back("\tmov " + resultReg + ", " + reg1);
        code.push_back("\tadd " + resultReg + ", " + reg2);
        code.push_back("\tmov " + resultReg + ", " + getOperandReg(qua.res));
    }
    else if (qua.op == "-") {
        QString reg1 = getOperandReg(qua.arg1);
        QString reg2 = getOperandReg(qua.arg2);
        QString resultReg = getRegister();
        code.push_back("\tmov " + resultReg + ", " + reg1);
        code.push_back("\tsub " + resultReg + ", " + reg2);
        code.push_back("\tmov " + resultReg + ", " + getOperandReg(qua.res));
    }
    else if (qua.op == "*") {
        QString reg1 = getOperandReg(qua.arg1);
        QString reg2 = getOperandReg(qua.arg2);
        QString resultReg = getRegister();
        code.push_back("\tmov " + resultReg + ", " + reg1);
        code.push_back("\tmul " + resultReg + ", " + reg2);
        code.push_back("\tmov " + resultReg + ", " + getOperandReg(qua.res));
    }
    else if (qua.op == "/") {
        QString reg1 = getOperandReg(qua.arg1);
        QString reg2 = getOperandReg(qua.arg2);
        code.push_back("\tmov $t" + QString::number(register_num++) + ", " + reg1);
        code.push_back("\tmov $t" + QString::number(register_num++) + ", " + reg2);
        code.push_back("\tdiv $t" + QString::number(register_num - 1)); // 用于除法
        code.push_back("\tmov " + getOperandReg(qua.res) + ", $t" + QString::number(register_num - 2)); // 存储结果
    }
    else if (qua.op == "=") {
        QString reg1 = getOperandReg(qua.arg1);
        code.push_back("\tmov " + getOperandReg(qua.res) + ", " + reg1);
    }
    else if (qua.op == "return") {
        code.push_back("\tEND");
    }
    
    else if (qua.op == "J<") {
        //if (qua.type == "if") {
        //    code.push_back("L" + QString::number(qua.Block) + ":\n");
        //}
        //if (qua.type == "while") {
        //    code.push_back("L" + QString::number(qua.Block) + ":\n");
        //}
        code.push_back("\tCMP " + qua.arg1 + ", " + qua.arg2);
        code.push_back("\tJA L" + QString::number(four_address_code_all[QString(qua.res).toInt() - 1].Block));
    }
    else if (qua.op == "J>") {
        /*if (qua.type == "if") {
            code.push_back("L" + QString::number(qua.Block) + ":\n");
        }
        if (qua.type == "while") {
            code.push_back("L" + QString::number(qua.Block) + ":\n");
        }*/
        code.push_back("\tCMP " + qua.arg1 + ", " + qua.arg2);
        code.push_back("\tJB L" + QString::number(four_address_code_all[QString(qua.res).toInt() - 1].Block));
    }
    else if (qua.op == 'J') {
        code.push_back("\tJMP L" + QString::number(four_address_code_all[QString(qua.res).toInt()- 1].Block));
    }
    return code;
}
void Compiler::generateCode() {
    QVector<QVector<int>> temp_fenKuai = fenKuai();
    std::sort(temp_fenKuai.begin(), temp_fenKuai.end());
    
    for (int i = 1; i <= temp_fenKuai.size(); i++) {
        for (auto it : temp_fenKuai[i - 1]) {
            four_address_code_all[it].Block = i;
        }
    }
    for (auto it : temp_fenKuai) {
            qDebug() << it;
    }
    qDebug() << four_address_code_all[5].op;
    // 创建一个QFile对象
    QFile file("C:/Users/dsq2/Desktop/Output.txt");

    // 打开文件，以写入模式打开
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        // 创建一个QTextStream对象
        QTextStream out(&file);
        out << "main:\n";
        for (int i = 0;i < temp_fenKuai.size();i++) {
            register_num = 0;
            out << "L" << i + 1 << ":\n";
            for (int j = 0;j < temp_fenKuai[i].size();j++) {
                // 写入内容到文件
                for (auto it : GenerateCode(four_address_code_all[temp_fenKuai[i][j]], temp_fenKuai)) {
                    out << it << "\n";
                }
            }

        } 
        // 关闭文件
        file.close();
    }
    else {
        // 如果无法打开文件，打印错误信息
        qDebug() << "无法打开文件:" << file.errorString();
    }
}
//---------------------------生成代码----------------------//

// 
void Compiler::on_chargeCs5_2_clicked() {
    Compiler compiler(ui);
    compiler.lexicalAnalysis(); // 调用 lexicalAnalysis 方法
    compiler.syntaxAnalysis(); // 调用 syntaxAnalysis 方法
    compiler.semanticAnalysis(); // 调用 semanticAnalysis 方法
    compiler.generateCode(); // 调用 generateCode 方法

}

