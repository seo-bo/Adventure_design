/********************************************************************************
** Form generated from reading UI file 'Worstfit.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORSTFIT_H
#define UI_WORSTFIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WorstfitClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WorstfitClass)
    {
        if (WorstfitClass->objectName().isEmpty())
            WorstfitClass->setObjectName("WorstfitClass");
        WorstfitClass->resize(600, 400);
        menuBar = new QMenuBar(WorstfitClass);
        menuBar->setObjectName("menuBar");
        WorstfitClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(WorstfitClass);
        mainToolBar->setObjectName("mainToolBar");
        WorstfitClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(WorstfitClass);
        centralWidget->setObjectName("centralWidget");
        WorstfitClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(WorstfitClass);
        statusBar->setObjectName("statusBar");
        WorstfitClass->setStatusBar(statusBar);

        retranslateUi(WorstfitClass);

        QMetaObject::connectSlotsByName(WorstfitClass);
    } // setupUi

    void retranslateUi(QMainWindow *WorstfitClass)
    {
        WorstfitClass->setWindowTitle(QCoreApplication::translate("WorstfitClass", "Worstfit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WorstfitClass: public Ui_WorstfitClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORSTFIT_H
