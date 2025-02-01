/********************************************************************************
** Form generated from reading UI file 'firstfit.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FIRSTFIT_H
#define UI_FIRSTFIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_firstfitClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *firstfitClass)
    {
        if (firstfitClass->objectName().isEmpty())
            firstfitClass->setObjectName("firstfitClass");
        firstfitClass->resize(600, 400);
        menuBar = new QMenuBar(firstfitClass);
        menuBar->setObjectName("menuBar");
        firstfitClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(firstfitClass);
        mainToolBar->setObjectName("mainToolBar");
        firstfitClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(firstfitClass);
        centralWidget->setObjectName("centralWidget");
        firstfitClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(firstfitClass);
        statusBar->setObjectName("statusBar");
        firstfitClass->setStatusBar(statusBar);

        retranslateUi(firstfitClass);

        QMetaObject::connectSlotsByName(firstfitClass);
    } // setupUi

    void retranslateUi(QMainWindow *firstfitClass)
    {
        firstfitClass->setWindowTitle(QCoreApplication::translate("firstfitClass", "firstfit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class firstfitClass: public Ui_firstfitClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FIRSTFIT_H
