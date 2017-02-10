/********************************************************************************
** Form generated from reading UI file 'Vtk2DWnd.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VTK2DWND_H
#define UI_VTK2DWND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>
#include "ImageWnd.h"

QT_BEGIN_NAMESPACE

class Ui_Vtk2DWnd
{
public:
    QHBoxLayout *Vtk2DWndHLayout;
    ImageWnd *ImgWnd;
    QScrollBar *ScrollBar;

    void setupUi(QWidget *Vtk2DWnd)
    {
        if (Vtk2DWnd->objectName().isEmpty())
            Vtk2DWnd->setObjectName(QStringLiteral("Vtk2DWnd"));
        Vtk2DWnd->resize(400, 300);
        Vtk2DWnd->setStyleSheet(QStringLiteral(""));
        Vtk2DWndHLayout = new QHBoxLayout(Vtk2DWnd);
        Vtk2DWndHLayout->setSpacing(0);
        Vtk2DWndHLayout->setObjectName(QStringLiteral("Vtk2DWndHLayout"));
        Vtk2DWndHLayout->setContentsMargins(0, 0, 0, 0);
        ImgWnd = new ImageWnd(Vtk2DWnd);
        ImgWnd->setObjectName(QStringLiteral("ImgWnd"));
        ImgWnd->setCursor(QCursor(Qt::ArrowCursor));

        Vtk2DWndHLayout->addWidget(ImgWnd);

        ScrollBar = new QScrollBar(Vtk2DWnd);
        ScrollBar->setObjectName(QStringLiteral("ScrollBar"));
        ScrollBar->setOrientation(Qt::Vertical);

        Vtk2DWndHLayout->addWidget(ScrollBar);


        retranslateUi(Vtk2DWnd);

        QMetaObject::connectSlotsByName(Vtk2DWnd);
    } // setupUi

    void retranslateUi(QWidget *Vtk2DWnd)
    {
        Vtk2DWnd->setWindowTitle(QApplication::translate("Vtk2DWnd", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Vtk2DWnd: public Ui_Vtk2DWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VTK2DWND_H
