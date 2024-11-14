#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "CompilersTechnology.h"

class LexicalAnalysis : public QMainWindow
{
    Q_OBJECT

public:
    explicit LexicalAnalysis(Ui::CompilersTechnologyClass* ui);
    ~LexicalAnalysis();

private:
    Ui::CompilersTechnologyClass* ui;
    
    QTextEdit* csOutput4;     // �����
    QTextEdit* csInPut4;      // �����
    QPushButton* chargeCs4;

public slots:
    void on_chargeCs4_clicked();
};


