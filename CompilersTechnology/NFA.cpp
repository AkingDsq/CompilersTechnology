#include "NFA.h"
#include "CompilersTechnology.h"

struct NFAnode {                     // 节点结构体
    QString state;                      // 状态
    QMap<QChar, QSet<QChar>> nexts;            // 下一个状态集合
    bool isChange = false ;                     // 是否改变
};
QVector<NFAnode> nodes;         // 输入节点数组

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
    csOutput3 = ui->csOutput3;

    connect(addCs2, &QPushButton::clicked, this, & NFA::on_addCs2_clicked);
    connect(deleteCs2, &QPushButton::clicked, this, &NFA::on_deleteCs2_clicked);

    //判断文法类型
    connect(chargeCs2, &QPushButton::clicked, this, &NFA::on_chargeCs2_clicked);
}

NFA::~NFA()
{}
//添加节点
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
    
    if ((SumNum + 2) * (L02->height() + 3 + 10) > WfSa2->height()) WifaWidget2->setFixedHeight(WifaWidget2->height() + L02->height() + 3 + 10);       // 添加时自适应滑块
    CsInput_2Layout->addWidget(label1, SumNum, 0);
    CsInput_2Layout->addWidget(lineEdit1, SumNum, 1); // 第一行第一列
    CsInput_2Layout->addWidget(lineEdit3, SumNum, 2); // 第一行第二列
    CsInput_2Layout->addWidget(label2, SumNum, 3);     // 第一行第二列
    CsInput_2Layout->addWidget(lineEdit2, SumNum, 4); // 第一行第三列
}
// 删除节点
void NFA::on_deleteCs2_clicked() {
    int SumNum = CsInput_2Layout->count()/5;
    if ((SumNum) * (L02->height() + 3 + 10) >= WfSa2->height() - 10) WifaWidget2->setFixedHeight(WifaWidget2->height() - L02->height() - 3 - 10);// 删除时自适应滑块
    for (int i = 0; i < 5; i++) {
        QWidget* widget = CsInput_2Layout->itemAtPosition((SumNum) - 1, i)->widget(); // 获取控件
        if (SumNum != 1) CsInput_2Layout->removeWidget(widget); // 第一行第一列
    }
}
// 把输入的状态转换添加到节点数组
void add_node(QString state, QChar c, QString next) {          
    NFAnode node;
    node.state = state;
    QSet<QChar> temp;                  // 寻找当前节点的输入c的下一状态的集合
    for (const QChar& ch : next) {
        temp.insert(ch);
    }
    node.nexts.insert(c, temp);
    nodes.push_back(node);
}
// 寻找闭包
QString getClosure(QVector<NFAnode> nodes, QString state, QChar c) {         
    QSet<QChar> Closure;
    QString ans = "";
    for (int k = 0; k < state.size(); k++) {                             // 如果有多个状态
        // 遍历所有输入的节点
        for (int i = 0; i < nodes.size(); i++) {                         // 遍历所有输入的节点
            if (nodes[i].state == state[k]) {                               // 寻找指定状态的闭包
                QSet<QChar> temp = nodes[i].nexts.value(c);              // 寻找当前节点的输入c的下一状态的集合
                for (const QChar& ch : temp) {
                    Closure.insert(ch);
                }
            }
        }
    }
    for (auto ch : Closure) {        // 去重并且输入到ans中，但是没有按照顺序排列
        ans.append(ch);
    }
    return ans;
}
// 判断字符x是否属于集合X
bool chargeIsIn(QString X, QChar x) {           
    for (int i = 0; i < X.size(); i++) {
        if (x == X[i]) {
            return true;
        }
    }
    return false;
}
//判断字符串x是否属于集合X
bool chargeIsIn(QString X, QString x) {
    int temp = 0;
    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < X.size(); j++) {
            if (x[i] == X[j]) {
                temp++;
            }
        }
    }
    if(temp == x.size()) return true;
    else return false;
}
// 判断
void NFA::on_chargeCs2_clicked() {
    nodes.clear(); // 清空节点数组
    csOutput2->setPlainText("开始解析NFA");
    int SumNum = CsInput_2Layout->count() / 5;

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
    QLineEdit* test2 = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(0, 2)->widget());
    if (test1->text().isEmpty() || test2->text().isEmpty()) {
        csOutput2->setPlainText("输入的状态转换不正确");
        return;
    }

    QString four1 = "NFA:\nM(\nK: " + K_str + "\nVt: " + Vt + "\nM:\n";
    csOutput2->setPlainText(four1);

    
    for (int i = 0; i < SumNum; i++) {
        // 读取左边输入框内容
        QLineEdit* left1 = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(i, 1)->widget()); // 1表示第二列，索引从0开始
        QLineEdit* left2 = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(i, 2)->widget()); // 2表示第三列，索引从0开始
        QLineEdit* right = qobject_cast<QLineEdit*>(CsInput_2Layout->itemAtPosition(i, 4)->widget()); // 4表示第五列，索引从0开始
        QString leftText1 = "", leftText2 = "", rightText = "";
        if (left1 == nullptr || left2 == nullptr || right == nullptr) return;
        if (!left1->text().isEmpty()) {
            if (!chargeIsIn(K_str, left1->text()[0])) {
                csOutput2->setPlainText("输入的状态不属于K");
                return;
            }
            else leftText1 = left1->text(); // 获取转化前状态
        }
        else{
            csOutput2->setPlainText("输入的状态不能为空");
            return;
        }

        if (!left2->text().isEmpty()) {
            if (!chargeIsIn(Vt, left2->text()[0])) {
                csOutput2->setPlainText("输入的字符不属于Vt");
                return;
            }
            else leftText2 = left2->text(); // 获取输入字符内容

        }
        else{
            csOutput2->setPlainText("输入的字符不能为空");
            return;
        }

        if (!right->text().isEmpty()) {
            if (!chargeIsIn(K_str, right->text())) {
                csOutput2->setPlainText("输入的转化状态不属于K");
                return;
            }
            else rightText = right->text(); // 获取转化后内容
        }
        else{
            csOutput2->setPlainText("输入的转化状态不能为空");
            return;
        }
       

        csOutput2->append("M(" + leftText1 + ", " + leftText2 + ") = {" + rightText + "}");

        // 读取所有的状态转换
        add_node(leftText1, leftText2[0], rightText);
    }
    csOutput2->append("\nS: " + S->text() + "\nZ: " + Z->text());

    QVector<NFAnode> NFA;                   // NFA数组
    QString nowNode = S->text();           // 从起始状态开始
    QSet<QString> isVisited;                // 判断是否已经遍历过的状态
    QVector<QString> sort;                    // 按顺序存储状态
    int index = 0;                            // 记录当前状态的下标
    sort.push_back(nowNode);
    isVisited.insert(nowNode);

    while (1) {                            // 从起始状态开始遍历到没有新的状态
        
        for (int j = 0; j < Vt.size(); j++) {
            QString nowClosure = getClosure(nodes, nowNode, Vt[j]);
            NFAnode temp;
            temp.state = nowNode;
            if (nowClosure.isEmpty())  temp.nexts[Vt[j]].insert('\0');
            else {
                for (int k = 0; k < nowClosure.size(); k++) {
                    temp.nexts[Vt[j]].insert(nowClosure[k]);
                }
                
            }

            NFA.push_back(temp);

            if (!nowClosure.isEmpty() && isVisited.find(nowClosure) == isVisited.end()) {
                sort.push_back(nowClosure);
                
            }
            if (!nowClosure.isEmpty()) isVisited.insert(nowClosure);
        }
        
        if (index >= sort.size() - 1 && isVisited.size() == sort.size()) break;
        nowNode = sort[++index]; // 获取当前状态的下一个状态
    }

    // 输出NFA
    csOutput2->append("\nNFA:\n状态\t");
    for (int i = 0; i < Vt.size(); i++) {
        csOutput2->moveCursor(QTextCursor::End);
        csOutput2->insertPlainText(Vt[i] + '\t');
    }
    csOutput2->append("\n");
    for (int i = 0; i < NFA.size(); i += Vt.size()) {
        NFA[i].state;
        csOutput2->append(NFA[i].state);
        for (int j = 0; j < Vt.size(); j++) {
            QSet<QChar> temp = NFA[i + j].nexts[Vt[j]];

            QString tempStr = "";
            for (auto ch : temp) {
                tempStr.append(ch);
            }
            csOutput2->moveCursor(QTextCursor::End);
            csOutput2->insertPlainText("\t" + tempStr);
        }
        csOutput2->append("\n");
    }

    // 修改转换状态
    for (int i = 0; i < NFA.size()/Vt.size(); i++) {
        QString af = QString::number(i);             // 重新编号后的状态
        for (int j = 0; j < NFA.size(); j++) {
            QString pre = NFA[i * Vt.size()].state;
            
            // 修改转换状态
            QString tempStr = "";
            for (const QChar& ch : NFA[j].nexts[Vt[j % Vt.size()]]) {
                tempStr.append(ch);
            }

            QSet<QChar> tmp;
            for(int i = 0; i < af.size(); i++) tmp.insert(af[i]);
            if (!NFA[j].isChange && tempStr == pre) {
                NFA[j].nexts.insert(Vt[j % Vt.size()], tmp);
                NFA[j].isChange = true;    
            }

        }
        
    }
    // 修改状态
    for (int i = 0; i < NFA.size() / Vt.size(); i++) {
        QString af = QString::number(i);             // 重新编号后的状态
        for (int j = 0; j < Vt.size(); j++) {
            NFA[i * Vt.size() + j].state = af;
        }
    }
    
    // 输出DFA
    csOutput3->setPlainText("\nDFA:\n状态\t");
    for (int i = 0; i < Vt.size(); i++) {
        csOutput3->moveCursor(QTextCursor::End);
        csOutput3->insertPlainText(Vt[i] + '\t');
    }
    csOutput3->append("\n");
    for (int i = 0; i < NFA.size(); i += Vt.size()) {
        NFA[i].state;
        csOutput3->append(NFA[i].state);
        for (int j = 0; j < Vt.size(); j++) {
            QSet<QChar> temp = NFA[i + j].nexts[Vt[j]];

            QString tempStr = "";
            for (auto ch : temp) {
                tempStr.append(ch);
            }
            csOutput3->moveCursor(QTextCursor::End);
            csOutput3->insertPlainText("\t" + tempStr);
        }
        csOutput3->append("\n");
    }
}
