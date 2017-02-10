/********************************************************************************
** Form generated from reading UI file 'WorkZone.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WORKZONE_H
#define UI_WORKZONE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include "Vtk2DWnd.h"
#include "Vtk3DWnd.h"

QT_BEGIN_NAMESPACE

class Ui_WorkZone
{
public:
    QGridLayout *WorkZoneGLayout;
    Vtk3DWnd *VR_Wnd;
    Vtk2DWnd *Axial_Wnd;
    Vtk2DWnd *Coronal_Wnd;
    Vtk2DWnd *Sagittal_Wnd;

    void setupUi(QWidget *WorkZone)
    {
        if (WorkZone->objectName().isEmpty())
            WorkZone->setObjectName(QStringLiteral("WorkZone"));
        WorkZone->resize(400, 300);
        WorkZone->setAutoFillBackground(false);
        WorkZone->setStyleSheet(QStringLiteral("background-color: rgb(95, 138, 172);"));
        WorkZoneGLayout = new QGridLayout(WorkZone);
        WorkZoneGLayout->setSpacing(1);
        WorkZoneGLayout->setObjectName(QStringLiteral("WorkZoneGLayout"));
        WorkZoneGLayout->setContentsMargins(0, 0, 0, 0);
        VR_Wnd = new Vtk3DWnd(WorkZone);
        VR_Wnd->setObjectName(QStringLiteral("VR_Wnd"));

        WorkZoneGLayout->addWidget(VR_Wnd, 0, 0, 1, 1);

        Axial_Wnd = new Vtk2DWnd(WorkZone);
        Axial_Wnd->setObjectName(QStringLiteral("Axial_Wnd"));

        WorkZoneGLayout->addWidget(Axial_Wnd, 0, 1, 1, 1);

        Coronal_Wnd = new Vtk2DWnd(WorkZone);
        Coronal_Wnd->setObjectName(QStringLiteral("Coronal_Wnd"));

        WorkZoneGLayout->addWidget(Coronal_Wnd, 1, 0, 1, 1);

        Sagittal_Wnd = new Vtk2DWnd(WorkZone);
        Sagittal_Wnd->setObjectName(QStringLiteral("Sagittal_Wnd"));

        WorkZoneGLayout->addWidget(Sagittal_Wnd, 1, 1, 1, 1);

        WorkZoneGLayout->setRowStretch(0, 1);
        WorkZoneGLayout->setRowStretch(1, 1);
        WorkZoneGLayout->setColumnStretch(0, 1);
        WorkZoneGLayout->setColumnStretch(1, 1);

        retranslateUi(WorkZone);

        QMetaObject::connectSlotsByName(WorkZone);
    } // setupUi

    void retranslateUi(QWidget *WorkZone)
    {
        WorkZone->setWindowTitle(QApplication::translate("WorkZone", "Form", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class WorkZone: public Ui_WorkZone {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WORKZONE_H
