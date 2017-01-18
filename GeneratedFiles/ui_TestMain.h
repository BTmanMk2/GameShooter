/********************************************************************************
** Form generated from reading UI file 'TestMain.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TESTMAIN_H
#define UI_TESTMAIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TestMain
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TestMain)
    {
        if (TestMain->objectName().isEmpty())
            TestMain->setObjectName(QStringLiteral("TestMain"));
        TestMain->resize(400, 300);
        menuBar = new QMenuBar(TestMain);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        TestMain->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TestMain);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TestMain->addToolBar(mainToolBar);
        centralWidget = new QWidget(TestMain);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TestMain->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TestMain);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TestMain->setStatusBar(statusBar);

        retranslateUi(TestMain);

        QMetaObject::connectSlotsByName(TestMain);
    } // setupUi

    void retranslateUi(QMainWindow *TestMain)
    {
        TestMain->setWindowTitle(QApplication::translate("TestMain", "TestMain", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TestMain: public Ui_TestMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TESTMAIN_H
