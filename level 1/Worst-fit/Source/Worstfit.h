#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Worstfit.h"

class Worstfit : public QMainWindow
{
    Q_OBJECT

public:
    Worstfit(QWidget *parent = nullptr);
    ~Worstfit();

private:
    Ui::WorstfitClass ui;
};
