#pragma once
#include <QtWidgets/QMainWindow>
#include "ui_TSP_project.h"

class TSP_project : public QMainWindow
{
    Q_OBJECT

public:
    TSP_project(QWidget *parent = nullptr);
    ~TSP_project();

private:
    Ui::TSP_projectClass ui;
};
