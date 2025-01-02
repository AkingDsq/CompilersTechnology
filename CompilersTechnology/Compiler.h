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
private:
    Ui::CompilersTechnologyClass* ui;
    // 加载文件按钮
    QPushButton* loadFileButton;
    // 源程序
    QTextEdit* csInPut5;
    // 输出程序
    QTextEdit* csOutPut5;
    // error
    QTextEdit* error5;
    //
    QTextEdit* csOutPut6;
    // 编译按钮
    QPushButton* chargeCs5_2;

public slots:
    void loadFile();     // 加载文件
    void lexicalAnalysis();   // 词法分析
    void syntaxAnalysis();    // 语法分析
    void semanticAnalysis();  // 语义分析
    void generateCode();      // 生成代码
    void on_chargeCs5_2_clicked();   // 编译按钮
};