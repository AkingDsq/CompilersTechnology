#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "CompilersTechnology.h"

class NFA : public QMainWindow
{
    Q_OBJECT

public:
    explicit NFA(Ui::CompilersTechnologyClass* ui);
    ~NFA();

private:
    Ui::CompilersTechnologyClass *ui;
    // ÊäÈë¿ò
    QGridLayout* CsInput_2Layout;
    QPushButton* addCs2;
    QPushButton* deleteCs2;

    QLineEdit* L02;
    QLineEdit* R02;
    QScrollArea* WfSa2;
    QWidget* WifaWidget2;
    QLineEdit* Z;
    QLineEdit* S;
    // ÅÐ¶ÏÎÄ·¨
    QPushButton* chargeCs2;
    QTextEdit* csOutput2;     // Êä³ö¿ò
    QTextEdit* csOutput3;      // ÊäÈë¿ò

public slots:
    void on_addCs2_clicked();
    void on_deleteCs2_clicked();
    void on_chargeCs2_clicked();
};
//void NFA::setUi(Ui::CompilersTechnologyClass *ui) {
//    this->ui = ui;
//}
