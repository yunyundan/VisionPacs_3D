/********************************************************************************
** Form generated from reading UI file 'ctkVTKThresholdWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKVTKTHRESHOLDWIDGET_H
#define UI_CTKVTKTHRESHOLDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include "ctkRangeWidget.h"
#include "ctkSliderWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ctkVTKThresholdWidget
{
public:
    QFormLayout *formLayout;
    QLabel *ThresholdLabel;
    ctkRangeWidget *ThresholdSliderWidget;
    QLabel *OpacityLabel;
    ctkSliderWidget *OpacitySliderWidget;

    void setupUi(QWidget *ctkVTKThresholdWidget)
    {
        if (ctkVTKThresholdWidget->objectName().isEmpty())
            ctkVTKThresholdWidget->setObjectName(QStringLiteral("ctkVTKThresholdWidget"));
        ctkVTKThresholdWidget->resize(251, 46);
        formLayout = new QFormLayout(ctkVTKThresholdWidget);
        formLayout->setContentsMargins(0, 0, 0, 0);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        ThresholdLabel = new QLabel(ctkVTKThresholdWidget);
        ThresholdLabel->setObjectName(QStringLiteral("ThresholdLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, ThresholdLabel);

        ThresholdSliderWidget = new ctkRangeWidget(ctkVTKThresholdWidget);
        ThresholdSliderWidget->setObjectName(QStringLiteral("ThresholdSliderWidget"));

        formLayout->setWidget(0, QFormLayout::FieldRole, ThresholdSliderWidget);

        OpacityLabel = new QLabel(ctkVTKThresholdWidget);
        OpacityLabel->setObjectName(QStringLiteral("OpacityLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, OpacityLabel);

        OpacitySliderWidget = new ctkSliderWidget(ctkVTKThresholdWidget);
        OpacitySliderWidget->setObjectName(QStringLiteral("OpacitySliderWidget"));
        OpacitySliderWidget->setSingleStep(0.01);
        OpacitySliderWidget->setPageStep(0.1);
        OpacitySliderWidget->setMaximum(1);
        OpacitySliderWidget->setValue(1);

        formLayout->setWidget(1, QFormLayout::FieldRole, OpacitySliderWidget);


        retranslateUi(ctkVTKThresholdWidget);

        QMetaObject::connectSlotsByName(ctkVTKThresholdWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkVTKThresholdWidget)
    {
        ctkVTKThresholdWidget->setWindowTitle(QApplication::translate("ctkVTKThresholdWidget", "Threshold", Q_NULLPTR));
        ThresholdLabel->setText(QApplication::translate("ctkVTKThresholdWidget", "Threshold:", Q_NULLPTR));
        OpacityLabel->setText(QApplication::translate("ctkVTKThresholdWidget", "Opacity:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkVTKThresholdWidget: public Ui_ctkVTKThresholdWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKVTKTHRESHOLDWIDGET_H
