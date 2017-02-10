/********************************************************************************
** Form generated from reading UI file 'ImageWnd.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMAGEWND_H
#define UI_IMAGEWND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ImageWnd
{
public:

    void setupUi(QWidget *ImageWnd)
    {
        if (ImageWnd->objectName().isEmpty())
            ImageWnd->setObjectName(QStringLiteral("ImageWnd"));
        ImageWnd->resize(400, 300);

        retranslateUi(ImageWnd);

        QMetaObject::connectSlotsByName(ImageWnd);
    } // setupUi

    void retranslateUi(QWidget *ImageWnd)
    {
        ImageWnd->setWindowTitle(QApplication::translate("ImageWnd", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ImageWnd: public Ui_ImageWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMAGEWND_H
