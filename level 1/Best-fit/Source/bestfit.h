#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_bestfit.h"

class bestfit : public QMainWindow
{
    Q_OBJECT

public:
    bestfit(QWidget *parent = nullptr);
    ~bestfit();

private:
    Ui::bestfitClass ui;
};
