/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QWidget *centralwidget_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnAddPC;
    QPushButton *btnAddRouter;
    QPushButton *btnAddSwitch;
    QPushButton *btnConnect;
    QPushButton *btnSend;
    QGraphicsView *graphicsView;
    QStatusBar *statusbar_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        centralwidget_2 = new QWidget(centralwidget);
        centralwidget_2->setObjectName("centralwidget_2");
        centralwidget_2->setGeometry(QRect(-30, 0, 902, 534));
        verticalLayout = new QVBoxLayout(centralwidget_2);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnAddPC = new QPushButton(centralwidget_2);
        btnAddPC->setObjectName("btnAddPC");

        horizontalLayout->addWidget(btnAddPC);

        btnAddRouter = new QPushButton(centralwidget_2);
        btnAddRouter->setObjectName("btnAddRouter");

        horizontalLayout->addWidget(btnAddRouter);

        btnAddSwitch = new QPushButton(centralwidget_2);
        btnAddSwitch->setObjectName("btnAddSwitch");

        horizontalLayout->addWidget(btnAddSwitch);

        btnConnect = new QPushButton(centralwidget_2);
        btnConnect->setObjectName("btnConnect");

        horizontalLayout->addWidget(btnConnect);

        btnSend = new QPushButton(centralwidget_2);
        btnSend->setObjectName("btnSend");

        horizontalLayout->addWidget(btnSend);


        verticalLayout->addLayout(horizontalLayout);

        graphicsView = new QGraphicsView(centralwidget_2);
        graphicsView->setObjectName("graphicsView");
        graphicsView->setMinimumSize(QSize(900, 500));

        verticalLayout->addWidget(graphicsView);

        statusbar_2 = new QStatusBar(centralwidget);
        statusbar_2->setObjectName("statusbar_2");
        statusbar_2->setGeometry(QRect(-30, 0, 3, 22));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnAddPC->setText(QCoreApplication::translate("MainWindow", "Computador", nullptr));
        btnAddRouter->setText(QCoreApplication::translate("MainWindow", "Roteador", nullptr));
        btnAddSwitch->setText(QCoreApplication::translate("MainWindow", "Switch", nullptr));
        btnConnect->setText(QCoreApplication::translate("MainWindow", "Conectar", nullptr));
        btnSend->setText(QCoreApplication::translate("MainWindow", "Enviar Pacote", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
