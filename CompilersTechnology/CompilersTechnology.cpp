#include "CompilersTechnology.h"
#define LARGE_NUM 100000
CompilersTechnology::CompilersTechnology(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 切换页面
    pages = ui.stackedWidget;
    connect(ui.buttonPage1, &QPushButton::released, this, [this]() { on_stackedWidget_currentChanged(0); });
    connect(ui.buttonPage2, &QPushButton::released, this, [this]() { on_stackedWidget_currentChanged(1); });
    connect(ui.buttonPage3, &QPushButton::released, this, [this]() { on_stackedWidget_currentChanged(2); });
    // 文法的输入（添加或者删除产生式）
    CsInputLayout = ui.CsInput;
    connect(ui.AddCs, &QPushButton::released, this, &CompilersTechnology::on_addCS_clicked);
    connect(ui.DeleteCs, &QPushButton::released, this, &CompilersTechnology::on_deleteCS_clicked);

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
void CompilersTechnology::on_addCS_clicked() {
    int SumNum = CsInputLayout->count();
    // 假设您想放在第一行的第0列、第1列和第2列
    QLineEdit* lineEdit1 = new QLineEdit("");// 创建 QLineEdit
    lineEdit1->setObjectName("L"+ SumNum / 3);    // 设置对象名称
    QLabel* label = new QLabel("::=");
    QLineEdit* lineEdit2 = new QLineEdit("");
    lineEdit2->setObjectName("R" + SumNum / 3);    // 设置对象名称

    CsInputLayout->addWidget(lineEdit1, SumNum / 3, 0); // 第一行第一列
    CsInputLayout->addWidget(label, SumNum / 3, 1);     // 第一行第二列
    CsInputLayout->addWidget(lineEdit2, SumNum /3, 2); // 第一行第三列
}
void CompilersTechnology::on_deleteCS_clicked() {
    int SumNum = CsInputLayout->count();
    for (int i = 0; i < 3; i++) {
        QWidget* widget = CsInputLayout->itemAtPosition((SumNum / 3) - 1, i)->widget(); // 获取控件
        if(SumNum / 3 != 1) CsInputLayout->removeWidget(widget); // 第一行第一列
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
    ui.csOutput->setPlainText("开始解析文法"); 
    int SumNum = CsInputLayout->count();
    
    // 读取Vn输入框内容
    QLineEdit* VN = ui.VnInput;
    QString Vn = VN->text();
    // 读取Vt输入框内容
    QLineEdit* VT = ui.VtInput;
    QString Vt = VT->text();

    if(Vn.isEmpty()) {
        ui.csOutput->setPlainText("Vn不能为空");
        return;
    }
    else if(Vt.isEmpty()) {
        ui.csOutput->setPlainText("Vt不能为空");
        return;
    }

    QLineEdit* test1 = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(0, 0)->widget());
    QLineEdit* test2 = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(0, 2)->widget());
    if(test1->text().isEmpty() || test2->text().isEmpty()) {
        ui.csOutput->setPlainText("输入的文法不正确");
        return;
    }

    QTextEdit* output = ui.csOutput;
    QString four1 = "G("+ ui.S->text() + ") :\nVn = " + Vn + "\nVt = " + Vt + "\nP:";
    output->setPlainText(four1);
    for (int i = 0; i < SumNum / 3; i++) {
        // 读取左边输入框内容
        QLineEdit* left = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 0)->widget()); // 2表示第三列，索引从0开始
        QLineEdit* right = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 2)->widget()); // 2表示第三列，索引从0开始
        QString leftText = left->text(); // 获取内容
        QString rightText = right->text(); // 获取内容
        output->append(leftText + " -> " + rightText);
    }
    output->append("S:" + ui.S->text());

    int wf,wf1 = 0, wf2 = 0, wf3 = 0;
    for (int i = 0; i < SumNum / 3; i++) {
        // 读取左边输入框内容
        QLineEdit* left = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 0)->widget()); // 2表示第三列，索引从0开始
        QLineEdit* right = qobject_cast<QLineEdit*>(CsInputLayout->itemAtPosition(i, 2)->widget()); // 2表示第三列，索引从0开始
        QString leftText = left->text(); // 获取内容
        QString rightText = right->text(); // 获取内容

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
    qDebug() << wf3 << wf2 << wf1;
    if(wf3 == SumNum / 3) wf = 3;
    else if(wf2 == SumNum / 3) wf = 2;
    else if(wf1 == SumNum / 3) wf = 1;
    ui.csOutput->append("文法类型为" + QString::number(wf) + "型文法");
}
    