/********************************************************************************
** Form generated from reading UI file 'draughtsoft.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DRAUGHTSOFT_H
#define UI_DRAUGHTSOFT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DraughtSoftClass
{
public:
    QWidget *centralWidget;

    void setupUi(QMainWindow *DraughtSoftClass)
    {
        if (DraughtSoftClass->objectName().isEmpty())
            DraughtSoftClass->setObjectName(QString::fromUtf8("DraughtSoftClass"));
        DraughtSoftClass->resize(600, 400);
        centralWidget = new QWidget(DraughtSoftClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        DraughtSoftClass->setCentralWidget(centralWidget);

        retranslateUi(DraughtSoftClass);

        QMetaObject::connectSlotsByName(DraughtSoftClass);
    } // setupUi

    void retranslateUi(QMainWindow *DraughtSoftClass)
    {
        DraughtSoftClass->setWindowTitle(QCoreApplication::translate("DraughtSoftClass", "DraughtSoft", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DraughtSoftClass: public Ui_DraughtSoftClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DRAUGHTSOFT_H
