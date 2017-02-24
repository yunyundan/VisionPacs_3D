/********************************************************************************
** Form generated from reading UI file 'ctkVTKScalarBarWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKVTKSCALARBARWIDGET_H
#define UI_CTKVTKSCALARBARWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "ctkVTKTextPropertyWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ctkVTKScalarBarWidget
{
public:
    QGridLayout *gridLayout_3;
    QCheckBox *DisplayScalarBarCheckBox;
    QGroupBox *TitleGroupBox;
    QGridLayout *gridLayout;
    ctkVTKTextPropertyWidget *TitleTextPropertyWidget;
    QGroupBox *LabelsGroupBox;
    QGridLayout *gridLayout_2;
    ctkVTKTextPropertyWidget *LabelsTextPropertyWidget;
    QLabel *MaxNumberOfColorsLabel;
    QSpinBox *MaxNumberOfColorsSpinBox;
    QLabel *NumberOfLabelsLabel;
    QSpinBox *NumberOfLabelsSpinBox;

    void setupUi(QWidget *ctkVTKScalarBarWidget)
    {
        if (ctkVTKScalarBarWidget->objectName().isEmpty())
            ctkVTKScalarBarWidget->setObjectName(QStringLiteral("ctkVTKScalarBarWidget"));
        ctkVTKScalarBarWidget->resize(311, 535);
        gridLayout_3 = new QGridLayout(ctkVTKScalarBarWidget);
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        DisplayScalarBarCheckBox = new QCheckBox(ctkVTKScalarBarWidget);
        DisplayScalarBarCheckBox->setObjectName(QStringLiteral("DisplayScalarBarCheckBox"));

        gridLayout_3->addWidget(DisplayScalarBarCheckBox, 0, 0, 1, 2);

        TitleGroupBox = new QGroupBox(ctkVTKScalarBarWidget);
        TitleGroupBox->setObjectName(QStringLiteral("TitleGroupBox"));
        TitleGroupBox->setFlat(true);
        gridLayout = new QGridLayout(TitleGroupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        TitleTextPropertyWidget = new ctkVTKTextPropertyWidget(TitleGroupBox);
        TitleTextPropertyWidget->setObjectName(QStringLiteral("TitleTextPropertyWidget"));

        gridLayout->addWidget(TitleTextPropertyWidget, 0, 0, 1, 1);


        gridLayout_3->addWidget(TitleGroupBox, 1, 0, 1, 2);

        LabelsGroupBox = new QGroupBox(ctkVTKScalarBarWidget);
        LabelsGroupBox->setObjectName(QStringLiteral("LabelsGroupBox"));
        LabelsGroupBox->setFlat(true);
        gridLayout_2 = new QGridLayout(LabelsGroupBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        LabelsTextPropertyWidget = new ctkVTKTextPropertyWidget(LabelsGroupBox);
        LabelsTextPropertyWidget->setObjectName(QStringLiteral("LabelsTextPropertyWidget"));

        gridLayout_2->addWidget(LabelsTextPropertyWidget, 0, 0, 1, 1);


        gridLayout_3->addWidget(LabelsGroupBox, 2, 0, 1, 2);

        MaxNumberOfColorsLabel = new QLabel(ctkVTKScalarBarWidget);
        MaxNumberOfColorsLabel->setObjectName(QStringLiteral("MaxNumberOfColorsLabel"));

        gridLayout_3->addWidget(MaxNumberOfColorsLabel, 3, 0, 1, 1);

        MaxNumberOfColorsSpinBox = new QSpinBox(ctkVTKScalarBarWidget);
        MaxNumberOfColorsSpinBox->setObjectName(QStringLiteral("MaxNumberOfColorsSpinBox"));
        MaxNumberOfColorsSpinBox->setMinimum(2);
        MaxNumberOfColorsSpinBox->setMaximum(9999);

        gridLayout_3->addWidget(MaxNumberOfColorsSpinBox, 3, 1, 1, 1);

        NumberOfLabelsLabel = new QLabel(ctkVTKScalarBarWidget);
        NumberOfLabelsLabel->setObjectName(QStringLiteral("NumberOfLabelsLabel"));

        gridLayout_3->addWidget(NumberOfLabelsLabel, 4, 0, 1, 1);

        NumberOfLabelsSpinBox = new QSpinBox(ctkVTKScalarBarWidget);
        NumberOfLabelsSpinBox->setObjectName(QStringLiteral("NumberOfLabelsSpinBox"));
        NumberOfLabelsSpinBox->setMaximum(64);

        gridLayout_3->addWidget(NumberOfLabelsSpinBox, 4, 1, 1, 1);


        retranslateUi(ctkVTKScalarBarWidget);

        QMetaObject::connectSlotsByName(ctkVTKScalarBarWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkVTKScalarBarWidget)
    {
        ctkVTKScalarBarWidget->setWindowTitle(QApplication::translate("ctkVTKScalarBarWidget", "vtkScalarBarWidget", Q_NULLPTR));
        DisplayScalarBarCheckBox->setText(QApplication::translate("ctkVTKScalarBarWidget", "Display scalar bar", Q_NULLPTR));
        TitleGroupBox->setTitle(QApplication::translate("ctkVTKScalarBarWidget", "Title properties:", Q_NULLPTR));
        LabelsGroupBox->setTitle(QApplication::translate("ctkVTKScalarBarWidget", "Labels properties:", Q_NULLPTR));
        LabelsTextPropertyWidget->setTextLabel(QApplication::translate("ctkVTKScalarBarWidget", "Format:", Q_NULLPTR));
        MaxNumberOfColorsLabel->setText(QApplication::translate("ctkVTKScalarBarWidget", "Maximum number of colors:", Q_NULLPTR));
        NumberOfLabelsLabel->setText(QApplication::translate("ctkVTKScalarBarWidget", "Number of labels:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkVTKScalarBarWidget: public Ui_ctkVTKScalarBarWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKVTKSCALARBARWIDGET_H
