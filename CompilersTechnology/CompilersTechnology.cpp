﻿#include "CompilersTechnology.h"
#define LARGE_NUM 100000
CompilersTechnology::CompilersTechnology(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CompilersTechnologyClass)      // 非确定性有穷自动机)
{  
    ui->setupUi(this);
    nfa = new NFA(ui);         // 非确定性有穷自动机
    lexicalAnalysis = new LexicalAnalysis(ui);
    ll1 = new LL1(ui);         // LL(1)分析表
    compiler = new Compiler(ui);
    // 切换页面
    pages = ui->stackedWidget;
    page1 = ui->page1;
    page2 = ui->page2;
    page3 = ui->page3;
    page4 = ui->page4;
    page5 = ui->Compiler;
    connect(page1, &QAction::triggered, this, [this]() { on_stackedWidget_currentChanged(0); });
    connect(page2, &QAction::triggered, this, [this]() { on_stackedWidget_currentChanged(1); });
    connect(page3, &QAction::triggered, this, [this]() { on_stackedWidget_currentChanged(2); });
    connect(page4, &QAction::triggered, this, [this]() { on_stackedWidget_currentChanged(3); });
    connect(page5, &QAction::triggered, this, [this]() { on_stackedWidget_currentChanged(4); });
    // 文法的输入（添加或者删除产生式）
    CsInputLayout = ui->CsInput;
    //connect(ui.addCs, &QPushButton::released, this, &CompilersTechnology::on_addCs_clicked);
    //connect(ui.deleteCs, &QPushButton::released, this, &CompilersTechnology::on_deleteCs_clicked);

    // 判断文法类型
    //connect(ui.chargeCs, &QPushButton::released, this, &CompilersTechnology::on_chargeCs_clicked);
}

CompilersTechnology::~CompilersTechnology()
{}

void CompilersTechnology::on_stackedWidget_currentChanged(int index) {
    if (pages != nullptr) {
        if (index < pages->count()) pages->setCurrentIndex(index);    //切换页面
        else qDebug() << "页面" << index + 1 << "不存在";
    }
    else qDebug() << "页面为空";
}
void CompilersTechnology::on_addCs_clicked() {
    int SumNum = CsInputLayout->count()/4;
    // 假设您想放在第一行的第0列、第1列和第2列
    QLabel* label1 = new QLabel(QString::number(SumNum + 1) + ".");
    label1->setObjectName("wfNum" + SumNum);    // 设置对象名称

    QLineEdit* lineEdit1 = new QLineEdit("");// 创建 QLineEdit
    lineEdit1->setObjectName("L"+ SumNum);    // 设置对象名称

    QLabel* label2 = new QLabel("::=");
    QFont font = label2->font(); // 获取当前字体
    font.setPointSize(15); // 设置字体大小为15
    label2->setFont(font); // 应用修改后的字体

    QLineEdit* lineEdit2 = new QLineEdit("");
    lineEdit2->setObjectName("R" + SumNum);    // 设置对象名称
    qDebug() << (SumNum + 1) * (ui->L0->height() + 3 + 10) << ui->WifaWidget->height();
    if ((SumNum + 2) * (ui->L0->height() + 3 + 10) > ui->WfSa->height()) ui->WifaWidget->setFixedHeight(ui->WifaWidget->height() + ui->L0->height() + 3 + 10) ;       // 添加时自适应滑块
    CsInputLayout->addWidget(label1, SumNum, 0);
    CsInputLayout->addWidget(lineEdit1, SumNum, 1); // 第一行第一列
    CsInputLayout->addWidget(label2, SumNum, 2);     // 第一行第二列
    CsInputLayout->addWidget(lineEdit2, SumNum, 3); // 第一行第三列
}
void CompilersTechnology::on_deleteCs_clicked() {
    int SumNum = CsInputLayout->count()/4;
    if ((SumNum) * (ui->L0->height() + 3 + 10) >= ui->WfSa->height() - 10) ui->WifaWidget->setFixedHeight(ui->WifaWidget->height() - ui->L0->height()  - 3 - 10);// 删除时自适应滑块
    for (int i = 0; i < 4; i++) {
        QWidget* widget = CsInputLayout->itemAtPosition((SumNum) - 1, i)->widget(); // 获取控件
        if(SumNum != 1) CsInputLayout->removeWidget(widget); // 第一行第一列
    }
    
}
bool WF3(QString Vn, QString Vt, QString leftText, QString rightText) {
    int isVn = 0, isVt = 0;
    if (leftText.length() == 1 && rightText.length() <= 2) {
        // 左边只有一个字符，且必须为非终结符
        for (int i = 0; i < Vn.length(); i++) {
            if (leftText[0] == Vn[i]) isVn = 1;
        }
        if (isVn != 1) return false;
        // 右边只有一个字符，且必须为终结符，或右边只有两个字符，且1个为终结符1个为非终结符
        if (rightText.length() == 1) {
            for (int i = 0; i < Vt.length(); i++) {
                if (rightText[0] == Vt[i]) isVt = 1;
            }
            if (isVt != 1) return false;
        }
        else if (rightText.length() == 2) {
            for (int i = 0; i < Vt.length(); i++) {
                if (rightText[0] == Vt[i]) isVt++;
                if (rightText[1] == Vt[i]) isVt++;
            }
            if (isVt != 1) return false;
        }
        return true;
    }
    else return false;
}
bool WF2(QString Vn, QString Vt, QString leftText, QString rightText) {
    if (leftText.length() == 1) {
        // 左边只有一个字符，且必须为非终结符
        for (int i = 0; i < Vn.length(); i++) {
            if (leftText[0] == Vn[i]) return true;
        }
        return false;
    }
    else return false;
}
bool WF1(QString Vn, QString Vt, QString leftText, QString rightText) {
    if (leftText.length() <= rightText.length()) {
        for (int i = 0; i < leftText.length(); i++){
            for (int j = 0; j < Vn.length(); j++) {
                if (leftText[i] == Vn[j]) return true;
            }
        }
        return false;
    }
    else return false;
}
void CompilersTechnology::on_chargeCs_clicked() {
    ui->csOutput->setPlainText("开始解析文法");
    int SumNum = CsInputLayout->count();
    
    // 读取Vn输入框内容
    QLineEdit* VN = ui->VnInput;
    QString Vn = VN->text();
    // 读取Vt输入框内容
    QLineEdit* VT = ui->VtInput;
    QString Vt = VT->text();

    if(Vn.isEmpty()) {
        ui->csOutput->setPlainText("Vn不能为空");
        return;
    }
    else if(Vt.isEmpty()) {
        ui->csOutput->setPlainText("Vt不能为空");
        return;
    }

    QLineEdit* test1 = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(0, 1)->widget());
    QLineEdit* test2 = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(0, 3)->widget());
    if(test1->text().isEmpty() || test2->text().isEmpty()) {
        ui->csOutput->setPlainText("输入的文法不正确");
        return;
    }

    QTextEdit* output = ui->csOutput;
    QString four1 = "G("+ ui->S->text() + ") :\nVn = " + Vn + "\nVt = " + Vt + "\nP:";
    output->setPlainText(four1);
    for (int i = 0; i < SumNum / 4; i++) {
        // 读取左边输入框内容
        QLineEdit* left = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 1)->widget()); // 2表示第三列，索引从0开始
        QLineEdit* right = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 3)->widget()); // 2表示第三列，索引从0开始
        QString leftText = left->text(); // 获取内容
        QString rightText = right->text(); // 获取内容
        output->append(leftText + " -> " + rightText);
    }
    output->append("S:" + ui->S->text());

    int wf,wf1 = 0, wf2 = 0, wf3 = 0;
    bool isEmpty;
    for (int i = 0; i < SumNum / 4; i++) {
        // 读取左边输入框内容
        QLineEdit* left = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 1)->widget()); // 2表示第三列，索引从0开始
        QLineEdit* right = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 3)->widget()); // 2表示第三列，索引从0开始
        
        QString leftText = left->text(); // 获取内容
        QString rightText = right->text(); // 获取内容
        // 如果有含空输入的文法
        isEmpty = false;
        if (leftText.isEmpty() || rightText.isEmpty()) {
            isEmpty = true;
            output->append("第" + QString::number(i + 1) + "行:\t" + leftText + "->" + rightText + "含有空的输入");
            continue;
        }

        if (WF3(Vn, Vt, leftText, rightText)) {               // 判断3型文法
            wf3++;
        }
        if (WF2(Vn, Vt, leftText, rightText)) {         // 判断2型文法
            wf2++;
        }
        if (WF1(Vn, Vt, leftText, rightText)) {              // 判断1型文法
            wf1++;
        }
    }
    if(wf3 == SumNum / 4) wf = 3;
    else if(wf2 == SumNum / 4) wf = 2;
    else if(wf1 == SumNum / 4) wf = 1;
    else wf = 0;
    if(isEmpty) ui->csOutput->append("去除空输入后");
    ui->csOutput->append("文法类型为" + QString::number(wf) + "型文法");
}
    