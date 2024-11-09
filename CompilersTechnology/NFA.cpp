#include "NFA.h"
#include <set>
#include "CompilersTechnology.h"
#define LARGE_NUM 1000

struct NFAnode {
    int i;
    std::set<int> nexts[LARGE_NUM];
}nst[LARGE_NUM];

NFA::NFA(Ui::CompilersTechnologyClass *ui)
    : ui(ui)
{

    CsInput_2Layout = ui->CsInput2;

    addCs2 = ui->addCs2;
    deleteCs2 = ui->deleteCs2;
    chargeCs2 = ui->chargeCs2;
    L02 = ui->L02;
    WfSa2 = ui->WfSa2;
    WifaWidget2 = ui->WifaWidget2;
    Z = ui->Z;
    S = ui->S2;

    csOutput2 = ui->csOutput2;

    connect(addCs2, &QPushButton::clicked, this, & NFA::on_addCs2_clicked);
    connect(deleteCs2, &QPushButton::clicked, this, &NFA::on_deleteCs2_clicked);

    //判断文法类型
    connect(chargeCs2, &QPushButton::clicked, this, &NFA::on_chargeCs2_clicked);
}

NFA::~NFA()
{}

void NFA::on_addCs2_clicked() {
    int SumNum = CsInput_2Layout->count()/5;
    
    // 假设您想放在第一行的第0列、第1列和第2列
    QLabel* label1 = new QLabel(QString::number(SumNum + 1) + ".");
    label1->setObjectName("wfNum" + SumNum);    // 设置对象名称

    QLineEdit* lineEdit1 = new QLineEdit("");// 创建 QLineEdit
    lineEdit1->setObjectName("L" + SumNum);    // 设置对象名称

    QLineEdit* lineEdit3 = new QLineEdit("");// 创建 QLineEdit
    lineEdit3->setObjectName("L2" + SumNum);    // 设置对象名称

    QLabel* label2 = new QLabel("::=");
    QFont font = label2->font(); // 获取当前字体
    font.setPointSize(15); // 设置字体大小为15
    label2->setFont(font); // 应用修改后的字体

    QLineEdit* lineEdit2 = new QLineEdit("");
    lineEdit2->setObjectName("R" + SumNum);    // 设置对象名称
    qDebug() << (SumNum + 1) * (L02->height() + 3 + 10) << WifaWidget2->height();
    if ((SumNum + 2) * (L02->height() + 3 + 10) > WfSa2->height()) WifaWidget2->setFixedHeight(WifaWidget2->height() + L02->height() + 3 + 10);       // 添加时自适应滑块
    CsInput_2Layout->addWidget(label1, SumNum, 0);
    CsInput_2Layout->addWidget(lineEdit1, SumNum, 1); // 第一行第一列
    CsInput_2Layout->addWidget(lineEdit3, SumNum, 2); // 第一行第二列
    CsInput_2Layout->addWidget(label2, SumNum, 3);     // 第一行第二列
    CsInput_2Layout->addWidget(lineEdit2, SumNum, 4); // 第一行第三列
}
void NFA::on_deleteCs2_clicked() {
    int SumNum = CsInput_2Layout->count()/5;
    if ((SumNum) * (L02->height() + 3 + 10) >= WfSa2->height() - 10) WifaWidget2->setFixedHeight(WifaWidget2->height() - L02->height() - 3 - 10);// 删除时自适应滑块
    for (int i = 0; i < 5; i++) {
        QWidget* widget = CsInput_2Layout->itemAtPosition((SumNum) - 1, i)->widget(); // 获取控件
        if (SumNum != 1) CsInput_2Layout->removeWidget(widget); // 第一行第一列
    }

}
void NFA::on_chargeCs2_clicked() {
    ui->csOutput->setPlainText("开始解析NFA");
    int SumNum = CsInput_2Layout->count();

    // 读取Vn输入框内容
    QLineEdit* K = ui->K;
    QString K_str = K->text();
    // 读取Vt输入框内容
    QLineEdit* VT = ui->VtInput2;
    QString Vt = VT->text();

    if (K_str.isEmpty()) {
        csOutput2->setPlainText("K不能为空");
        return;
    }
    else if (Vt.isEmpty()) {
        csOutput2->setPlainText("Vt不能为空");
        return;
    }

    QLineEdit* test1 = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(0, 1)->widget());
    QLineEdit* test2 = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(0, 3)->widget());
    if (test1->text().isEmpty() || test2->text().isEmpty()) {
        csOutput2->setPlainText("输入的状态转换不正确");
        return;
    }

    QString four1 = "M(\nK: " + K_str + "\nVt: " + Vt + "\nM:\n";
    csOutput2->setPlainText(four1);

    for (int i = 0; i < SumNum / 4; i++) {
        // 读取左边输入框内容
        QLineEdit* left = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(i, 1)->widget()); // 2表示第三列，索引从0开始
        QLineEdit* right = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(i, 3)->widget()); // 2表示第三列，索引从0开始
        QString leftText = left->text(); // 获取内容
        QString rightText = right->text(); // 获取内容
        csOutput2->append("M(" + leftText + ") = {" + rightText + "}");
    }
    csOutput2->append("\nS: " + S->text() + "\nZ: " + Z->text());


}