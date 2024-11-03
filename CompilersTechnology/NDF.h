#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CompilersTechnology.h"
#include "CompilersTechnology.h"

class NDF : public QMainWindow
{
    Q_OBJECT

public:
    NDF(QWidget* parent = nullptr);
    ~NDF();

private:
    Ui::CompilersTechnologyClass ui;

};
