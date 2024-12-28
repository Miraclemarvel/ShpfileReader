/********************************************************************************
** Form generated from reading UI file 'ShpReader.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHPREADER_H
#define UI_SHPREADER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShpReaderClass
{
public:
    QAction *actionImport;
    QAction *actionAbout;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    QGraphicsView *graphicsView;
    QMenuBar *menuBar;
    QMenu *menufile;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ShpReaderClass)
    {
        if (ShpReaderClass->objectName().isEmpty())
            ShpReaderClass->setObjectName(QString::fromUtf8("ShpReaderClass"));
        ShpReaderClass->resize(600, 400);
        actionImport = new QAction(ShpReaderClass);
        actionImport->setObjectName(QString::fromUtf8("actionImport"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/ShpReader/res/import.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionImport->setIcon(icon);
        actionAbout = new QAction(ShpReaderClass);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/ShpReader/res/about.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionAbout->setIcon(icon1);
        centralWidget = new QWidget(ShpReaderClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        graphicsView = new QGraphicsView(centralWidget);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));

        horizontalLayout->addWidget(graphicsView);

        ShpReaderClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ShpReaderClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        menufile = new QMenu(menuBar);
        menufile->setObjectName(QString::fromUtf8("menufile"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QString::fromUtf8("menu"));
        ShpReaderClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ShpReaderClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        ShpReaderClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ShpReaderClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        ShpReaderClass->setStatusBar(statusBar);

        menuBar->addAction(menufile->menuAction());
        menuBar->addAction(menu->menuAction());
        menufile->addAction(actionImport);
        menu->addAction(actionAbout);
        mainToolBar->addAction(actionImport);
        mainToolBar->addAction(actionAbout);

        retranslateUi(ShpReaderClass);

        QMetaObject::connectSlotsByName(ShpReaderClass);
    } // setupUi

    void retranslateUi(QMainWindow *ShpReaderClass)
    {
        ShpReaderClass->setWindowTitle(QCoreApplication::translate("ShpReaderClass", "ShpReader", nullptr));
        actionImport->setText(QCoreApplication::translate("ShpReaderClass", "\345\257\274\345\205\245\347\237\242\351\207\217\346\225\260\346\215\256", nullptr));
        actionAbout->setText(QCoreApplication::translate("ShpReaderClass", "\345\205\263\344\272\216", nullptr));
        menufile->setTitle(QCoreApplication::translate("ShpReaderClass", "\346\226\207\344\273\266", nullptr));
        menu->setTitle(QCoreApplication::translate("ShpReaderClass", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShpReaderClass: public Ui_ShpReaderClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHPREADER_H
