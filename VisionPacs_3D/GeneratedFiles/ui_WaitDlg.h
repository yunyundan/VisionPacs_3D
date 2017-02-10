/********************************************************************************
** Form generated from reading UI file 'WaitDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITDLG_H
#define UI_WAITDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WaitDlg
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QProgressBar *LoadProgressBar;

    void setupUi(QDialog *WaitDlg)
    {
        if (WaitDlg->objectName().isEmpty())
            WaitDlg->setObjectName(QStringLiteral("WaitDlg"));
        WaitDlg->resize(400, 56);
        verticalLayout = new QVBoxLayout(WaitDlg);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(WaitDlg);
        label->setObjectName(QStringLiteral("label"));
        label->setStyleSheet(QStringLiteral("color: rgb(255, 255, 255);"));

        verticalLayout->addWidget(label);

        LoadProgressBar = new QProgressBar(WaitDlg);
        LoadProgressBar->setObjectName(QStringLiteral("LoadProgressBar"));
        LoadProgressBar->setValue(24);
        LoadProgressBar->setTextVisible(true);
        LoadProgressBar->setTextDirection(QProgressBar::TopToBottom);

        verticalLayout->addWidget(LoadProgressBar);


        retranslateUi(WaitDlg);

        QMetaObject::connectSlotsByName(WaitDlg);
    } // setupUi

    void retranslateUi(QDialog *WaitDlg)
    {
        WaitDlg->setWindowTitle(QApplication::translate("WaitDlg", "WaitDlg", Q_NULLPTR));
        label->setText(QApplication::translate("WaitDlg", "\346\255\243\345\234\250\350\275\275\345\205\245\344\270\255\342\200\246\342\200\246", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WaitDlg: public Ui_WaitDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITDLG_H
