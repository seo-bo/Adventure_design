/********************************************************************************
** Form generated from reading UI file 'bestfit.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BESTFIT_H
#define UI_BESTFIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_bestfitClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *bestfitClass)
    {
        if (bestfitClass->objectName().isEmpty())
            bestfitClass->setObjectName("bestfitClass");
        bestfitClass->resize(600, 400);
        menuBar = new QMenuBar(bestfitClass);
        menuBar->setObjectName("menuBar");
        bestfitClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(bestfitClass);
        mainToolBar->setObjectName("mainToolBar");
        bestfitClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(bestfitClass);
        centralWidget->setObjectName("centralWidget");
        bestfitClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(bestfitClass);
        statusBar->setObjectName("statusBar");
        bestfitClass->setStatusBar(statusBar);

        retranslateUi(bestfitClass);

        QMetaObject::connectSlotsByName(bestfitClass);
    } // setupUi

    void retranslateUi(QMainWindow *bestfitClass)
    {
        bestfitClass->setWindowTitle(QCoreApplication::translate("bestfitClass", "bestfit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class bestfitClass: public Ui_bestfitClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BESTFIT_H
