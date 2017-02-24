/********************************************************************************
** Form generated from reading UI file 'ctkMaterialPropertyWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKMATERIALPROPERTYWIDGET_H
#define UI_CTKMATERIALPROPERTYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "ctkColorPickerButton.h"
#include "ctkMaterialPropertyPreviewLabel.h"
#include "ctkSliderWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ctkMaterialPropertyWidget
{
public:
    QFormLayout *formLayout;
    QLabel *ColorLabel;
    ctkColorPickerButton *ColorPickerButton;
    QLabel *OpacityLabel;
    ctkSliderWidget *OpacitySliderSpinBox;
    QLabel *AmbientLabel;
    ctkSliderWidget *AmbientSliderSpinBox;
    QLabel *DiffuseLabel;
    ctkSliderWidget *DiffuseSliderSpinBox;
    QLabel *SpecularLabel;
    ctkSliderWidget *SpecularSliderSpinBox;
    QLabel *SpecularPowerLabel;
    ctkSliderWidget *SpecularPowerSliderSpinBox;
    QLabel *BackfaceCullingLabel;
    QCheckBox *BackfaceCullingCheckBox;
    QLabel *PreviewLabel;
    QHBoxLayout *horizontalLayout_2;
    ctkMaterialPropertyPreviewLabel *MaterialPropertyPreviewLabel;
    QSpacerItem *horizontalSpacer;
    QLabel *PresetsLabel;
    QListWidget *PresetsListWidget;

    void setupUi(QWidget *ctkMaterialPropertyWidget)
    {
        if (ctkMaterialPropertyWidget->objectName().isEmpty())
            ctkMaterialPropertyWidget->setObjectName(QStringLiteral("ctkMaterialPropertyWidget"));
        ctkMaterialPropertyWidget->resize(248, 212);
        formLayout = new QFormLayout(ctkMaterialPropertyWidget);
        formLayout->setContentsMargins(0, 0, 0, 0);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
        ColorLabel = new QLabel(ctkMaterialPropertyWidget);
        ColorLabel->setObjectName(QStringLiteral("ColorLabel"));

        formLayout->setWidget(0, QFormLayout::LabelRole, ColorLabel);

        ColorPickerButton = new ctkColorPickerButton(ctkMaterialPropertyWidget);
        ColorPickerButton->setObjectName(QStringLiteral("ColorPickerButton"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ColorPickerButton->sizePolicy().hasHeightForWidth());
        ColorPickerButton->setSizePolicy(sizePolicy);
        ColorPickerButton->setColor(QColor(255, 255, 255));
        ColorPickerButton->setDisplayColorName(false);
        ColorPickerButton->setDialogOptions(ctkColorPickerButton::UseCTKColorDialog);

        formLayout->setWidget(0, QFormLayout::FieldRole, ColorPickerButton);

        OpacityLabel = new QLabel(ctkMaterialPropertyWidget);
        OpacityLabel->setObjectName(QStringLiteral("OpacityLabel"));

        formLayout->setWidget(1, QFormLayout::LabelRole, OpacityLabel);

        OpacitySliderSpinBox = new ctkSliderWidget(ctkMaterialPropertyWidget);
        OpacitySliderSpinBox->setObjectName(QStringLiteral("OpacitySliderSpinBox"));
        OpacitySliderSpinBox->setSingleStep(0.01);
        OpacitySliderSpinBox->setMaximum(1);
        OpacitySliderSpinBox->setValue(1);

        formLayout->setWidget(1, QFormLayout::FieldRole, OpacitySliderSpinBox);

        AmbientLabel = new QLabel(ctkMaterialPropertyWidget);
        AmbientLabel->setObjectName(QStringLiteral("AmbientLabel"));

        formLayout->setWidget(2, QFormLayout::LabelRole, AmbientLabel);

        AmbientSliderSpinBox = new ctkSliderWidget(ctkMaterialPropertyWidget);
        AmbientSliderSpinBox->setObjectName(QStringLiteral("AmbientSliderSpinBox"));
        AmbientSliderSpinBox->setSingleStep(0.01);
        AmbientSliderSpinBox->setMaximum(1);

        formLayout->setWidget(2, QFormLayout::FieldRole, AmbientSliderSpinBox);

        DiffuseLabel = new QLabel(ctkMaterialPropertyWidget);
        DiffuseLabel->setObjectName(QStringLiteral("DiffuseLabel"));

        formLayout->setWidget(3, QFormLayout::LabelRole, DiffuseLabel);

        DiffuseSliderSpinBox = new ctkSliderWidget(ctkMaterialPropertyWidget);
        DiffuseSliderSpinBox->setObjectName(QStringLiteral("DiffuseSliderSpinBox"));
        DiffuseSliderSpinBox->setSingleStep(0.01);
        DiffuseSliderSpinBox->setMaximum(1);
        DiffuseSliderSpinBox->setValue(1);

        formLayout->setWidget(3, QFormLayout::FieldRole, DiffuseSliderSpinBox);

        SpecularLabel = new QLabel(ctkMaterialPropertyWidget);
        SpecularLabel->setObjectName(QStringLiteral("SpecularLabel"));

        formLayout->setWidget(4, QFormLayout::LabelRole, SpecularLabel);

        SpecularSliderSpinBox = new ctkSliderWidget(ctkMaterialPropertyWidget);
        SpecularSliderSpinBox->setObjectName(QStringLiteral("SpecularSliderSpinBox"));
        SpecularSliderSpinBox->setSingleStep(0.01);
        SpecularSliderSpinBox->setMaximum(1);

        formLayout->setWidget(4, QFormLayout::FieldRole, SpecularSliderSpinBox);

        SpecularPowerLabel = new QLabel(ctkMaterialPropertyWidget);
        SpecularPowerLabel->setObjectName(QStringLiteral("SpecularPowerLabel"));

        formLayout->setWidget(5, QFormLayout::LabelRole, SpecularPowerLabel);

        SpecularPowerSliderSpinBox = new ctkSliderWidget(ctkMaterialPropertyWidget);
        SpecularPowerSliderSpinBox->setObjectName(QStringLiteral("SpecularPowerSliderSpinBox"));
        SpecularPowerSliderSpinBox->setDecimals(1);
        SpecularPowerSliderSpinBox->setSingleStep(0.5);
        SpecularPowerSliderSpinBox->setMinimum(1);
        SpecularPowerSliderSpinBox->setMaximum(50);

        formLayout->setWidget(5, QFormLayout::FieldRole, SpecularPowerSliderSpinBox);

        BackfaceCullingLabel = new QLabel(ctkMaterialPropertyWidget);
        BackfaceCullingLabel->setObjectName(QStringLiteral("BackfaceCullingLabel"));

        formLayout->setWidget(6, QFormLayout::LabelRole, BackfaceCullingLabel);

        BackfaceCullingCheckBox = new QCheckBox(ctkMaterialPropertyWidget);
        BackfaceCullingCheckBox->setObjectName(QStringLiteral("BackfaceCullingCheckBox"));

        formLayout->setWidget(6, QFormLayout::FieldRole, BackfaceCullingCheckBox);

        PreviewLabel = new QLabel(ctkMaterialPropertyWidget);
        PreviewLabel->setObjectName(QStringLiteral("PreviewLabel"));

        formLayout->setWidget(7, QFormLayout::LabelRole, PreviewLabel);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        MaterialPropertyPreviewLabel = new ctkMaterialPropertyPreviewLabel(ctkMaterialPropertyWidget);
        MaterialPropertyPreviewLabel->setObjectName(QStringLiteral("MaterialPropertyPreviewLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(MaterialPropertyPreviewLabel->sizePolicy().hasHeightForWidth());
        MaterialPropertyPreviewLabel->setSizePolicy(sizePolicy1);

        horizontalLayout_2->addWidget(MaterialPropertyPreviewLabel);

        horizontalSpacer = new QSpacerItem(0, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        PresetsLabel = new QLabel(ctkMaterialPropertyWidget);
        PresetsLabel->setObjectName(QStringLiteral("PresetsLabel"));

        horizontalLayout_2->addWidget(PresetsLabel);

        PresetsListWidget = new QListWidget(ctkMaterialPropertyWidget);
        PresetsListWidget->setObjectName(QStringLiteral("PresetsListWidget"));
        sizePolicy1.setHeightForWidth(PresetsListWidget->sizePolicy().hasHeightForWidth());
        PresetsListWidget->setSizePolicy(sizePolicy1);
        PresetsListWidget->setMinimumSize(QSize(123, 34));
        PresetsListWidget->setMaximumSize(QSize(123, 34));
        PresetsListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        PresetsListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        PresetsListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        PresetsListWidget->setSelectionMode(QAbstractItemView::NoSelection);
        PresetsListWidget->setFlow(QListView::LeftToRight);
        PresetsListWidget->setUniformItemSizes(true);

        horizontalLayout_2->addWidget(PresetsListWidget);


        formLayout->setLayout(7, QFormLayout::FieldRole, horizontalLayout_2);


        retranslateUi(ctkMaterialPropertyWidget);
        QObject::connect(AmbientSliderSpinBox, SIGNAL(valueChanged(double)), MaterialPropertyPreviewLabel, SLOT(setAmbient(double)));
        QObject::connect(DiffuseSliderSpinBox, SIGNAL(valueChanged(double)), MaterialPropertyPreviewLabel, SLOT(setDiffuse(double)));
        QObject::connect(SpecularSliderSpinBox, SIGNAL(valueChanged(double)), MaterialPropertyPreviewLabel, SLOT(setSpecular(double)));
        QObject::connect(SpecularPowerSliderSpinBox, SIGNAL(valueChanged(double)), MaterialPropertyPreviewLabel, SLOT(setSpecularPower(double)));
        QObject::connect(ColorPickerButton, SIGNAL(colorChanged(QColor)), MaterialPropertyPreviewLabel, SLOT(setColor(QColor)));
        QObject::connect(OpacitySliderSpinBox, SIGNAL(valueChanged(double)), MaterialPropertyPreviewLabel, SLOT(setOpacity(double)));

        QMetaObject::connectSlotsByName(ctkMaterialPropertyWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkMaterialPropertyWidget)
    {
        ctkMaterialPropertyWidget->setWindowTitle(QApplication::translate("ctkMaterialPropertyWidget", "Material Property", Q_NULLPTR));
        ColorLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Color:", Q_NULLPTR));
        OpacityLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Opacity:", Q_NULLPTR));
        AmbientLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Ambient:", Q_NULLPTR));
        DiffuseLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Diffuse:", Q_NULLPTR));
        SpecularLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Specular:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        SpecularPowerLabel->setToolTip(QApplication::translate("ctkMaterialPropertyWidget", "Specular power", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        SpecularPowerLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Power:", Q_NULLPTR));
        BackfaceCullingLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Backface Culling:", Q_NULLPTR));
        BackfaceCullingCheckBox->setText(QString());
        PreviewLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Preview:", Q_NULLPTR));
        PresetsLabel->setText(QApplication::translate("ctkMaterialPropertyWidget", "Presets:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkMaterialPropertyWidget: public Ui_ctkMaterialPropertyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKMATERIALPROPERTYWIDGET_H
