#include "LL1.h"
#include "CompilersTechnology.h"
#include "qstack.h"

LL1::LL1(Ui::CompilersTechnologyClass* ui)
    : ui(ui)
{

    CsInput_5Layout = ui->CsInput5;

    addCs5 = ui->addCs5;
    deleteCs5 = ui->deleteCs5;
    chargeCs5 = ui->chargeCs5;
    L05 = ui->L05;
    WfSa5 = ui->WfSa5;
    WifaWidget5 = ui->WifaWidget5;

    csOutput5 = ui->csOutput5;
    csOutput6 = ui->csOutput6;

    inputString = ui->InputString;
    Vn = ui->Vn2;
    Vt = ui->Vt2;
    S = ui->S3;
    connect(addCs5, &QPushButton::clicked, this, &LL1::on_addCs5_clicked);
    connect(deleteCs5, &QPushButton::clicked, this, &LL1::on_deleteCs5_clicked);

    // 判断文法类型
    connect(chargeCs5, &QPushButton::clicked, this, &LL1::on_chargeCs5_clicked);
    //connect(chargeCs5, &QPushButton::clicked, this, &LL1::test);
}

LL1::~LL1()
{
}
void LL1::on_addCs5_clicked() {
    int SumNum = CsInput_5Layout->count() / 4;
    // 假设您想放在第一行的第0列、第1列和第2列
    QLabel* label1 = new QLabel(QString::number(SumNum + 1) + ".");
    label1->setObjectName("wfNum" + SumNum);    // 设置对象名称

    QLineEdit* lineEdit1 = new QLineEdit("");// 创建 QLineEdit
    lineEdit1->setObjectName("L" + SumNum);    // 设置对象名称

    QLabel* label2 = new QLabel("::=");
    QFont font = label2->font(); // 获取当前字体
    font.setPointSize(15); // 设置字体大小为15
    label2->setFont(font); // 应用修改后的字体

    QLineEdit* lineEdit2 = new QLineEdit("");
    lineEdit2->setObjectName("R" + SumNum);    // 设置对象名称
    qDebug() << (SumNum + 1) * (L05->height() + 3 + 10) << WifaWidget5->height();
    if ((SumNum + 2) * (L05->height() + 3 + 10) > WfSa5->height()) WifaWidget5->setFixedHeight(WifaWidget5->height() + L05->height() + 3 + 10);       // 添加时自适应滑块
    CsInput_5Layout->addWidget(label1, SumNum, 0);
    CsInput_5Layout->addWidget(lineEdit1, SumNum, 1); // 第一行第一列
    CsInput_5Layout->addWidget(label2, SumNum, 2);     // 第一行第二列
    CsInput_5Layout->addWidget(lineEdit2, SumNum, 3); // 第一行第三列
}
void LL1::on_deleteCs5_clicked() {
    int SumNum = CsInput_5Layout->count() / 4;
    if ((SumNum) * (L05->height() + 3 + 10) >= WfSa5->height() - 10) WifaWidget5->setFixedHeight(WifaWidget5->height() - L05->height() - 3 - 10);// 删除时自适应滑块
    for (int i = 0; i < 4; i++) {
        QWidget* widget = CsInput_5Layout->itemAtPosition((SumNum)-1, i)->widget(); // 获取控件
        if (SumNum != 1) CsInput_5Layout->removeWidget(widget); // 第一行第一列
    }

}
// LL（1）输入结构体
struct LL1Input {
    QString L;
    QString R;
    bool haveEmpty = 0;      // 是否有空产生式
    bool isFix = 0;          // 产生式是否修改过
};
QVector<LL1Input> Input_all;
// LL（1）分析表结构体
struct LL1Table {
    QString c;
    QMap<QString, QString> table;
};
QVector<LL1Table> Table_all;

struct LL1Output {
    int id;
    QStack<QString> analysis_stack;
    QStack<QString> input_stack;
    QString use;
};

// 判断是否属于非终结符
bool isVn(QString Vn, QString s) {
    for (int i = 0; i < Vn.size(); i++) {
        if (Vn[i] == s[0]) return true;
    }
    return false;
}
// 判断s中是否含有字符串t
QVector<int> KMP(QString s, QString t) {
    QVector<int> temp;
    for (int i = 0; i < s.size(); i++) {
        int j = 0;
        int tmp = i;
        while(j < t.size() && tmp < s.size()) {
            if (t[j] == s[tmp]) {
                tmp++;
                j++;
                if (j == t.size()) {
                    if (t.size() == 2) temp.push_back(i + 1);
                    else if (t.size() == 1 && s[tmp] == "'");
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
// 消除左递归
void xc() {

}
// 消除左公因子
void xh() {

}
QSet<QString> first;      // first集
QSet<QString> follow;      // follow集
// 求产生式右侧FIRST集
void getFirst(QVector<LL1Input> input, QString sr, QString vn, QString S) {
    QString one = sr[0];  // 假设右侧第一个符号为FIRST集
    if (isVn(vn, one)) {
        // 右侧第一个符号为非终结符
        if (sr.size() > 1 && sr[1] == "'") one.append(sr[1]); // 右侧第一个符号为去除递归的非终结符

        // 遍历所有产生式
        for (int i = 0;i < input.size();i++) {
            if (input[i].R != "$" && input[i].L == one) {
                QString one = input[i].R[0];  // 假设右侧第一个符号为FIRST集
                if (isVn(vn, one)) {
                    // 右侧第一个符号为非终结符
                    if (1 < input[i].R.size() && input[i].R[1] == "'") one.append("'"); // 右侧第一个符号为去除递归的非终结符
                    getFirst(input, one, vn, S);
                }
                else {
                    // 右侧第一个符号为终结符或空集$
                    first.insert(one);
                }
            }
            else if (input[i].L == one && input[i].R == "$") first.insert("$");
        }
    }
    else {
        // 右侧第一个符号为终结符
        first.insert(one);
    }
}
// 求产生式左侧FOLLOW集
void getFollow(QVector<LL1Input> input, QString s, QString vn, QString S) {
    // 如果产生式左侧符号为开始符号S，则FOLLOW集首先包含#
    if (s == S) follow.insert("#");
    // 遍历所有产生式
    for (int i = 0; i < input.size(); i++) {
        QVector<int> temp = KMP(input[i].R, s);
        
        // 找到该符号的位置
        for (int j = 0;j < temp.size();j++) {
            // 如果是最后一个符号，则求产生式左侧FOLLOW集
            if (temp[j] == input[i].R.size() - 1) { 
                if (input[i].R[temp[j]] != input[i].L[input[i].L.size() - 1]) { 
                    qDebug() << "左递归" << input[i].L;
                    getFollow(input, input[i].L, vn, S); 
                } 
            }
            else {
                QString next = input[i].R[temp[j] + 1];
                qDebug() << "next0" << next;
                if (isVn(vn, next)) {
                    if (temp[j] + 2 < input[i].R.size() && input[i].R[temp[j] + 2] == "'") next.append("'"); // 右侧第一个符号为去除递归的非终结符
                    qDebug() << "next1" << next;
                    // 如果是非终结符，则其FIRST集都加入FOLLOW集
                    first.clear();
                    getFirst(input, next, vn, S);
                    for (auto f : first) {
                        follow.insert(f);
                    }
                    // 如果该非终结符可以推导出空串，那么可以认为该符号在末尾，使用第三条规则
                    if(first.contains("$")) getFollow(input, input[i].L, vn, S);
                }
                else {
                    follow.insert(next);
                }
            }
        }
    }
}
// 构造LL(1)分析表
void getLL1Table(QString vn, QString s) {
    for (int i = 0;i < Input_all.size();i++) {
        LL1Table tmp_table;
        tmp_table.c = Input_all[i].L;
        // 求产生式右侧FIRST集
        first.clear();
        getFirst(Input_all, Input_all[i].R, vn, s);
        for (auto f : first) {
            QString tmp = Input_all[i].L + "->" + Input_all[i].R;
            tmp_table.table.insert(f, tmp);
        }
        if (Input_all[i].R == "$") {
            // 如果产生式为空串，则求FOLLOW集
            follow.clear();
            getFollow(Input_all, Input_all[i].L, vn, s);
            for (auto f : follow) {
                QString tmp = Input_all[i].L + "->$";
                tmp_table.table.insert(f, tmp);
            }
        }
        
        // 加入Table_all
        Table_all.push_back(tmp_table);
    }
    QSet<QString> temp;      // 用于去重
    // 去除重复的产生式，并且合并
    for (int i = 0;i < Table_all.size();i++) {
        if(!temp.contains(Table_all[i].c)) temp.insert(Table_all[i].c);
        else {
            int index = -1;
            // 找到已存在的下标
            for (int j = 0;j < Table_all.size();j++) {
                if (Table_all[j].c == Table_all[i].c) {
                    index = j;
                    break;
                }
            }

            // 合并产生式
            for (auto it = Table_all[i].table.begin(); it != Table_all[i].table.end(); ++it) {
                Table_all[index].table.insert(it.key(), it.value());
            }
            // 去除多余的产生式
            Table_all.remove(i);
            i--;
        }
    }
}
// 分析输入串
LL1Output analysis(LL1Output output, QString inputString, QString S, int now) {
    LL1Output temp_output = output;
    temp_output.id = now + 1;
    // 
    if (!output.use.isEmpty()) {
        // 产生式左侧出栈
        qDebug() << QString::number(temp_output.id) + "出栈：" + temp_output.analysis_stack.top();
        temp_output.analysis_stack.pop();

        // 产生式右侧入栈
        int r = KMP(output.use, ">")[0] + 1;
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
                if(right[i] != "$") temp_output.analysis_stack.push(right[i]);
            }
        }
        qDebug() << temp_output.analysis_stack.top() << temp_output.input_stack.top();
        qDebug() << ((temp_output.analysis_stack.top() == temp_output.input_stack.top()) && (temp_output.analysis_stack.top() == "#"));
        qDebug() << (temp_output.analysis_stack.top() != temp_output.input_stack.top());
        // 如果栈顶不相等则查表得出产生式
        if (temp_output.analysis_stack.top() != temp_output.input_stack.top()) {
            for (int i = 0;i < Table_all.size();i++) {
                if (Table_all[i].c == temp_output.analysis_stack.top()) {
                    if(Table_all[i].table.contains(temp_output.input_stack.top())) temp_output.use = Table_all[i].table.value(temp_output.input_stack.top());
                    else temp_output.use == "error";
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
            for (int i = 0;i < Table_all.size();i++) {
                if (Table_all[i].c == temp_output.analysis_stack.top()) {
                    if (Table_all[i].table.contains(temp_output.input_stack.top())) temp_output.use = Table_all[i].table.value(temp_output.input_stack.top());
                    else temp_output.use == "error";
                }
            }
        }
        else if (temp_output.analysis_stack.top() == "#") temp_output.use = "acc";   // 若栈顶相等且为#，则接受
        else temp_output.use = ""; // 如果相等且不为#，则产生式为空

    }
    
    // 分析过程
    return temp_output;
}

void LL1::on_chargeCs5_clicked() {
    // 清空
    first.clear();
    follow.clear();
    Input_all.clear();
    Table_all.clear();

    // 清空输出窗口
    csOutput5->setPlainText("LL(1)分析表构造中...");
    QString vn = Vn->text();
    QString vt = Vt->text();
    QString s = S->text();
    QString input_String = inputString->text();
    // 输入文法
    for (int i = 0; i < CsInput_5Layout->count() / 4; i++) {
        // 获取输入
        QLineEdit* left1 = qobject_cast<QLineEdit*>(CsInput_5Layout->itemAtPosition(i, 1)->widget()); // 1表示第二列，索引从0开始
        QLineEdit* right1 = qobject_cast<QLineEdit*>(CsInput_5Layout->itemAtPosition(i, 3)->widget()); // 1表示第二列，索引从0开始
        LL1Input tmp_input;
        tmp_input.L = left1->text();
        // 如果产生式右侧含有|，则分割为多个产生式
        if (right1->text().contains("|")) 
        {
            // 获取|的位置
            QVector<int> tmp_num = KMP(right1->text(), "|");
            // 分割字符串
            for (int j = 0;j < tmp_num.size() + 1;j++) {
                LL1Input tmp_input;
                tmp_input.L = left1->text();
                // 获取分割的左右边界l，r
                int l, r;
                if (j == 0) {
                    l = 0;
                    r = tmp_num[j] - 1;
                }
                else if (j == tmp_num.size()) {
                    l = tmp_num[j - 1] + 1;
                    r = right1->text().size() - 1;
                }
                else {
                    l = tmp_num[j - 1] + 1;
                    r = tmp_num[j] - 1;
                }
                // 分割
                tmp_input.R = right1->text().mid(l, r - l + 1);
                Input_all.push_back(tmp_input);
            }
        }
        else {
            LL1Input tmp_input;
            tmp_input.L = left1->text();
            tmp_input.R = right1->text();
            Input_all.push_back(tmp_input);
        }
    }
    qDebug() << "Input_all:";
    for (int i = 0; i < Input_all.size(); i++) {
        qDebug() << Input_all[i].L << "->" << Input_all[i].R;
    }
    // 消除左公因子
    xh();
    // 消除左递归
    xc();
    
    // 构造LL(1)分析表
    getLL1Table(vn, s);
    
    // 输出LL(1)分析表Table_all
    csOutput6->setPlainText("LL(1)分析表：\n");
    for (int i = 0; i < vt.size();i++) {
        csOutput6->moveCursor(QTextCursor::End);
        csOutput6->insertPlainText('\t' + vt[i]);
        if(i == vt.size() - 1) csOutput6->insertPlainText("\n");
    }
    for (int i = 0; i < Table_all.size();i++) {
        csOutput6->moveCursor(QTextCursor::End);
        csOutput6->insertPlainText(Table_all[i].c + "\t");
        for (auto f : vt) {
            // 如果Table_all中含有该终结符的产生式, 则输出
            if (Table_all[i].table.contains(f)) {
                csOutput6->insertPlainText(Table_all[i].table[f] + "\t");
            }
            else {
                csOutput6->moveCursor(QTextCursor::End);
                csOutput6->insertPlainText("\t");
            }
        }
        csOutput6->insertPlainText("\n");
    }
    // 输出分析过程
    csOutput5->setPlainText("字符分析过程：\n步骤\t分析栈\t输入串\t产生式\n");
    // 初始化输出表
    int now = 1;
    LL1Output output;
    output.id = now;
    output.analysis_stack.push("#");
    output.analysis_stack.push(s);
    output.input_stack.push("#");
    for (int i = input_String.size() - 1; i >= 0; i--) output.input_stack.push(input_String[i]);
    for (int i = 0;i < Table_all.size();i++) {
        if (Table_all[i].c == s) output.use = Table_all[i].table.value(output.input_stack.top());
    }
    // 输出分析过程
    csOutput5->moveCursor(QTextCursor::End);
    csOutput5->insertPlainText(QString::number(output.id) + "\t");
    QString temp = "";
    QStack<QString> temp_stack = output.analysis_stack;
    while (temp_stack.size()) {
        temp.prepend(temp_stack.top());
        temp_stack.pop();
    }
    csOutput5->moveCursor(QTextCursor::End);
    csOutput5->insertPlainText(temp + "\t");
    qDebug() << "分析栈" << temp;
    temp = "";
    temp_stack = output.input_stack;
    while(temp_stack.size()) {
        temp.append(temp_stack.top());
        temp_stack.pop();
    }
    csOutput5->moveCursor(QTextCursor::End);
    csOutput5->insertPlainText(temp + "\t"+ output.use + '\n');
    qDebug() << "输入栈" << temp;
    while (output.use != "acc" && output.use != "error") {
        output = analysis(output, input_String, s, now);
        now++;
        // 输出分析过程
        csOutput5->moveCursor(QTextCursor::End);
        csOutput5->insertPlainText(QString::number(output.id) + "\t");
        QString temp = "";
        temp_stack = output.analysis_stack;
        while (temp_stack.size()) {
            temp.prepend(temp_stack.top());
            temp_stack.pop();
        }
        csOutput5->moveCursor(QTextCursor::End);
        csOutput5->insertPlainText(temp + "\t");
        temp = "";
        temp_stack = output.input_stack;
        while (temp_stack.size()) {
            temp.append(temp_stack.top());
            temp_stack.pop();
        }
        csOutput5->moveCursor(QTextCursor::End);
        csOutput5->insertPlainText(temp + "\t" + output.use + '\n');
    }
}
void LL1::test() {

}