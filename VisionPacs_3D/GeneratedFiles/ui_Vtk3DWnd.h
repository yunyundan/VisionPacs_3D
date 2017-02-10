/********************************************************************************
** Form generated from reading UI file 'Vtk3DWnd.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VTK3DWND_H
#define UI_VTK3DWND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_Vtk3DWnd
{
public:
    QHBoxLayout *VRHLayout;
    QVTKWidget *VtkWidget;

    void setupUi(QWidget *Vtk3DWnd)
    {
        if (Vtk3DWnd->objectName().isEmpty())
            Vtk3DWnd->setObjectName(QStringLiteral("Vtk3DWnd"));
        Vtk3DWnd->resize(400, 300);
        VRHLayout = new QHBoxLayout(Vtk3DWnd);
        VRHLayout->setSpacing(0);
        VRHLayout->setObjectName(QStringLiteral("VRHLayout"));
        VRHLayout->setContentsMargins(0, 0, 0, 0);
        VtkWidget = new QVTKWidget(Vtk3DWnd);
        VtkWidget->setObjectName(QStringLiteral("VtkWidget"));

        VRHLayout->addWidget(VtkWidget);


        retranslateUi(Vtk3DWnd);

        QMetaObject::connectSlotsByName(Vtk3DWnd);
    } // setupUi

    void retranslateUi(QWidget *Vtk3DWnd)
    {
        Vtk3DWnd->setWindowTitle(QApplication::translate("Vtk3DWnd", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Vtk3DWnd: public Ui_Vtk3DWnd {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VTK3DWND_H
