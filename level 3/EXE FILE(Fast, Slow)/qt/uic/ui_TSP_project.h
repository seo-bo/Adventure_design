/********************************************************************************
** Form generated from reading UI file 'TSP_project.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TSP_PROJECT_H
#define UI_TSP_PROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TSP_projectClass
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TSP_projectClass)
    {
        if (TSP_projectClass->objectName().isEmpty())
            TSP_projectClass->setObjectName("TSP_projectClass");
        TSP_projectClass->resize(600, 400);
        centralWidget = new QWidget(TSP_projectClass);
        centralWidget->setObjectName("centralWidget");
        TSP_projectClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TSP_projectClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        TSP_projectClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TSP_projectClass);
        mainToolBar->setObjectName("mainToolBar");
        TSP_projectClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TSP_projectClass);
        statusBar->setObjectName("statusBar");
        TSP_projectClass->setStatusBar(statusBar);

        retranslateUi(TSP_projectClass);

        QMetaObject::connectSlotsByName(TSP_projectClass);
    } // setupUi

    void retranslateUi(QMainWindow *TSP_projectClass)
    {
        TSP_projectClass->setWindowTitle(QCoreApplication::translate("TSP_projectClass", "firstfit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class TSP_projectClass: public Ui_TSP_projectClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TSP_PROJECT_H
