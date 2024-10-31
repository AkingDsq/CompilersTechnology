#include "CompilersTechnology.h"

CompilersTechnology::CompilersTechnology(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 切换页面
    pages = ui.stackedWidget;
    connect(ui.buttonPage1, &QPushButton::clicked, this, [this]() { on_stackedWidget_currentChanged(0); });
    connect(ui.buttonPage2, &QPushButton::clicked, this, [this]() { on_stackedWidget_currentChanged(1); });
    connect(ui.buttonPage3, &QPushButton::clicked, this, [this]() { on_stackedWidget_currentChanged(2); });
    // 文法的输入（添加或者删除产生式）
    CsInputLayout = ui.CsInput;
    connect(ui.AddCs, &QPushButton::clicked, this, &CompilersTechnology::on_addCS_clicked);
    connect(ui.DeleteCs, &QPushButton::clicked, this, &CompilersTechnology::on_deleteCS_clicked);

    // 判断文法类型
    connect(ui.chargeCs, &QPushButton::clicked, this, &CompilersTechnology::on_chargeCs_clicked);
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
void CompilersTechnology::on_chargeCs_clicked() {
    qDebug() << "开始解析文法";
    ui.csOutput->setPlainText("开始解析文法");
    int SumNum = CsInputLayout->count();
    for (int i = 0; i < SumNum / 3; i++) {
        // 读取输入框内容
        //QString input = ui.CsInput->toPlainText();
        // 解析输入内容
        for (int j = 0; j < 3; i++) {
            ui.csOutput->append(".");
        }
        for (int j = 0; j < 3; i++) {
            // 获取当前文本
            QString currentText = ui.csOutput->toPlainText();
            currentText.remove(currentText.length() - 1, 1);
            ui.csOutput->setPlainText(currentText);
        }
    }
    ui.csOutput->setPlainText("文法类型为");
}
    