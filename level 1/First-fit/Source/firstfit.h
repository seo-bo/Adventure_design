#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_firstfit.h"

class firstfit : public QMainWindow
{
    Q_OBJECT

public:
    firstfit(QWidget *parent = nullptr);
    ~firstfit();

private:
    Ui::firstfitClass ui;
};
