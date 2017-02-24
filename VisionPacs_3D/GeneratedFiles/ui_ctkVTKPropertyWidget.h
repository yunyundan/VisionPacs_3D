/********************************************************************************
** Form generated from reading UI file 'ctkVTKPropertyWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKVTKPROPERTYWIDGET_H
#define UI_CTKVTKPROPERTYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ctkCollapsibleGroupBox.h"
#include "ctkColorPickerButton.h"
#include "ctkSliderWidget.h"
#include "ctkVTKSurfaceMaterialPropertyWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ctkVTKPropertyWidget
{
public:
    QVBoxLayout *verticalLayout;
    ctkCollapsibleGroupBox *RepresentationCollapsibleGroupBox;
    QFormLayout *formLayout_4;
    QLabel *RepresentationLabel;
    QComboBox *RepresentationComboBox;
    QLabel *PointSizeLabel;
    ctkSliderWidget *PointSizeSliderWidget;
    QLabel *LineWidthLabel;
    ctkSliderWidget *LineWidthSliderWidget;
    QLabel *CullingLabel;
    QHBoxLayout *horizontalLayout;
    QCheckBox *FrontfaceCullingCheckBox;
    QCheckBox *BackfaceCullingCheckBox;
    ctkCollapsibleGroupBox *ColorCollapsibleGroupBox;
    QFormLayout *formLayout_3;
    QLabel *ColorLabel;
    ctkColorPickerButton *ColorPickerButton;
    QLabel *OpacityLabel;
    ctkSliderWidget *OpacitySliderWidget;
    QLabel *EdgeVisibilityLabel;
    QCheckBox *EdgeVisibilityCheckBox;
    QLabel *EdgeColorLabel;
    ctkColorPickerButton *EdgeColorPickerButton;
    ctkCollapsibleGroupBox *LightingCollapsibleGroupBox;
    QFormLayout *formLayout_2;
    QLabel *LightingLabel;
    QCheckBox *LightingCheckBox;
    QLabel *InterpolationLabel;
    QComboBox *InterpolationComboBox;
    QLabel *ShadingLabel;
    QCheckBox *ShadingCheckBox;
    ctkCollapsibleGroupBox *MaterialCollapsibleGroupBox;
    QFormLayout *formLayout;
    ctkVTKSurfaceMaterialPropertyWidget *MaterialPropertyWidget;

    void setupUi(QWidget *ctkVTKPropertyWidget)
    {
        if (ctkVTKPropertyWidget->objectName().isEmpty())
            ctkVTKPropertyWidget->setObjectName(QStringLiteral("ctkVTKPropertyWidget"));
        ctkVTKPropertyWidget->resize(406, 284);
        verticalLayout = new QVBoxLayout(ctkVTKPropertyWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        RepresentationCollapsibleGroupBox = new ctkCollapsibleGroupBox(ctkVTKPropertyWidget);
        RepresentationCollapsibleGroupBox->setObjectName(QStringLiteral("RepresentationCollapsibleGroupBox"));
        RepresentationCollapsibleGroupBox->setCollapsed(true);
        formLayout_4 = new QFormLayout(RepresentationCollapsibleGroupBox);
        formLayout_4->setObjectName(QStringLiteral("formLayout_4"));
        formLayout_4->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        RepresentationLabel = new QLabel(RepresentationCollapsibleGroupBox);
        RepresentationLabel->setObjectName(QStringLiteral("RepresentationLabel"));

        formLayout_4->setWidget(0, QFormLayout::LabelRole, RepresentationLabel);

        RepresentationComboBox = new QComboBox(RepresentationCollapsibleGroupBox);
        RepresentationComboBox->setObjectName(QStringLiteral("RepresentationComboBox"));

        formLayout_4->setWidget(0, QFormLayout::FieldRole, RepresentationComboBox);

        PointSizeLabel = new QLabel(RepresentationCollapsibleGroupBox);
        PointSizeLabel->setObjectName(QStringLiteral("PointSizeLabel"));

        formLayout_4->setWidget(1, QFormLayout::LabelRole, PointSizeLabel);

        PointSizeSliderWidget = new ctkSliderWidget(RepresentationCollapsibleGroupBox);
        PointSizeSliderWidget->setObjectName(QStringLiteral("PointSizeSliderWidget"));
        PointSizeSliderWidget->setDecimals(2);
        PointSizeSliderWidget->setMinimum(1);
        PointSizeSliderWidget->setMaximum(100);

        formLayout_4->setWidget(1, QFormLayout::FieldRole, PointSizeSliderWidget);

        LineWidthLabel = new QLabel(RepresentationCollapsibleGroupBox);
        LineWidthLabel->setObjectName(QStringLiteral("LineWidthLabel"));

        formLayout_4->setWidget(2, QFormLayout::LabelRole, LineWidthLabel);

        LineWidthSliderWidget = new ctkSliderWidget(RepresentationCollapsibleGroupBox);
        LineWidthSliderWidget->setObjectName(QStringLiteral("LineWidthSliderWidget"));
        LineWidthSliderWidget->setDecimals(2);
        LineWidthSliderWidget->setMinimum(1);
        LineWidthSliderWidget->setMaximum(100);

        formLayout_4->setWidget(2, QFormLayout::FieldRole, LineWidthSliderWidget);

        CullingLabel = new QLabel(RepresentationCollapsibleGroupBox);
        CullingLabel->setObjectName(QStringLiteral("CullingLabel"));

        formLayout_4->setWidget(3, QFormLayout::LabelRole, CullingLabel);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        FrontfaceCullingCheckBox = new QCheckBox(RepresentationCollapsibleGroupBox);
        FrontfaceCullingCheckBox->setObjectName(QStringLiteral("FrontfaceCullingCheckBox"));

        horizontalLayout->addWidget(FrontfaceCullingCheckBox);

        BackfaceCullingCheckBox = new QCheckBox(RepresentationCollapsibleGroupBox);
        BackfaceCullingCheckBox->setObjectName(QStringLiteral("BackfaceCullingCheckBox"));

        horizontalLayout->addWidget(BackfaceCullingCheckBox);


        formLayout_4->setLayout(3, QFormLayout::FieldRole, horizontalLayout);


        verticalLayout->addWidget(RepresentationCollapsibleGroupBox);

        ColorCollapsibleGroupBox = new ctkCollapsibleGroupBox(ctkVTKPropertyWidget);
        ColorCollapsibleGroupBox->setObjectName(QStringLiteral("ColorCollapsibleGroupBox"));
        formLayout_3 = new QFormLayout(ColorCollapsibleGroupBox);
        formLayout_3->setObjectName(QStringLiteral("formLayout_3"));
        formLayout_3->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        ColorLabel = new QLabel(ColorCollapsibleGroupBox);
        ColorLabel->setObjectName(QStringLiteral("ColorLabel"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, ColorLabel);

        ColorPickerButton = new ctkColorPickerButton(ColorCollapsibleGroupBox);
        ColorPickerButton->setObjectName(QStringLiteral("ColorPickerButton"));
        ColorPickerButton->setDisplayColorName(true);

        formLayout_3->setWidget(0, QFormLayout::FieldRole, ColorPickerButton);

        OpacityLabel = new QLabel(ColorCollapsibleGroupBox);
        OpacityLabel->setObjectName(QStringLiteral("OpacityLabel"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, OpacityLabel);

        OpacitySliderWidget = new ctkSliderWidget(ColorCollapsibleGroupBox);
        OpacitySliderWidget->setObjectName(QStringLiteral("OpacitySliderWidget"));
        OpacitySliderWidget->setSingleStep(0.1);
        OpacitySliderWidget->setPageStep(0.25);
        OpacitySliderWidget->setMaximum(1);

        formLayout_3->setWidget(1, QFormLayout::FieldRole, OpacitySliderWidget);

        EdgeVisibilityLabel = new QLabel(ColorCollapsibleGroupBox);
        EdgeVisibilityLabel->setObjectName(QStringLiteral("EdgeVisibilityLabel"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, EdgeVisibilityLabel);

        EdgeVisibilityCheckBox = new QCheckBox(ColorCollapsibleGroupBox);
        EdgeVisibilityCheckBox->setObjectName(QStringLiteral("EdgeVisibilityCheckBox"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, EdgeVisibilityCheckBox);

        EdgeColorLabel = new QLabel(ColorCollapsibleGroupBox);
        EdgeColorLabel->setObjectName(QStringLiteral("EdgeColorLabel"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, EdgeColorLabel);

        EdgeColorPickerButton = new ctkColorPickerButton(ColorCollapsibleGroupBox);
        EdgeColorPickerButton->setObjectName(QStringLiteral("EdgeColorPickerButton"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, EdgeColorPickerButton);


        verticalLayout->addWidget(ColorCollapsibleGroupBox);

        LightingCollapsibleGroupBox = new ctkCollapsibleGroupBox(ctkVTKPropertyWidget);
        LightingCollapsibleGroupBox->setObjectName(QStringLiteral("LightingCollapsibleGroupBox"));
        LightingCollapsibleGroupBox->setCollapsed(true);
        formLayout_2 = new QFormLayout(LightingCollapsibleGroupBox);
        formLayout_2->setObjectName(QStringLiteral("formLayout_2"));
        LightingLabel = new QLabel(LightingCollapsibleGroupBox);
        LightingLabel->setObjectName(QStringLiteral("LightingLabel"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, LightingLabel);

        LightingCheckBox = new QCheckBox(LightingCollapsibleGroupBox);
        LightingCheckBox->setObjectName(QStringLiteral("LightingCheckBox"));

        formLayout_2->setWidget(0, QFormLayout::FieldRole, LightingCheckBox);

        InterpolationLabel = new QLabel(LightingCollapsibleGroupBox);
        InterpolationLabel->setObjectName(QStringLiteral("InterpolationLabel"));

        formLayout_2->setWidget(1, QFormLayout::LabelRole, InterpolationLabel);

        InterpolationComboBox = new QComboBox(LightingCollapsibleGroupBox);
        InterpolationComboBox->setObjectName(QStringLiteral("InterpolationComboBox"));

        formLayout_2->setWidget(1, QFormLayout::FieldRole, InterpolationComboBox);

        ShadingLabel = new QLabel(LightingCollapsibleGroupBox);
        ShadingLabel->setObjectName(QStringLiteral("ShadingLabel"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, ShadingLabel);

        ShadingCheckBox = new QCheckBox(LightingCollapsibleGroupBox);
        ShadingCheckBox->setObjectName(QStringLiteral("ShadingCheckBox"));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, ShadingCheckBox);


        verticalLayout->addWidget(LightingCollapsibleGroupBox);

        MaterialCollapsibleGroupBox = new ctkCollapsibleGroupBox(ctkVTKPropertyWidget);
        MaterialCollapsibleGroupBox->setObjectName(QStringLiteral("MaterialCollapsibleGroupBox"));
        MaterialCollapsibleGroupBox->setCollapsed(true);
        formLayout = new QFormLayout(MaterialCollapsibleGroupBox);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
        MaterialPropertyWidget = new ctkVTKSurfaceMaterialPropertyWidget(MaterialCollapsibleGroupBox);
        MaterialPropertyWidget->setObjectName(QStringLiteral("MaterialPropertyWidget"));
        MaterialPropertyWidget->setColorVisible(false);
        MaterialPropertyWidget->setOpacityVisible(false);
        MaterialPropertyWidget->setBackfaceCullingVisible(false);

        formLayout->setWidget(0, QFormLayout::SpanningRole, MaterialPropertyWidget);


        verticalLayout->addWidget(MaterialCollapsibleGroupBox);


        retranslateUi(ctkVTKPropertyWidget);

        QMetaObject::connectSlotsByName(ctkVTKPropertyWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkVTKPropertyWidget)
    {
        ctkVTKPropertyWidget->setWindowTitle(QApplication::translate("ctkVTKPropertyWidget", "Property", Q_NULLPTR));
        RepresentationCollapsibleGroupBox->setTitle(QApplication::translate("ctkVTKPropertyWidget", "Representation", Q_NULLPTR));
        RepresentationLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Representation:", Q_NULLPTR));
        RepresentationComboBox->clear();
        RepresentationComboBox->insertItems(0, QStringList()
         << QApplication::translate("ctkVTKPropertyWidget", "Points", Q_NULLPTR)
         << QApplication::translate("ctkVTKPropertyWidget", "Wireframe", Q_NULLPTR)
         << QApplication::translate("ctkVTKPropertyWidget", "Surface", Q_NULLPTR)
        );
        PointSizeLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Point Size:", Q_NULLPTR));
        LineWidthLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Line Width:", Q_NULLPTR));
        CullingLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Culling:", Q_NULLPTR));
        FrontfaceCullingCheckBox->setText(QApplication::translate("ctkVTKPropertyWidget", "Frontface", Q_NULLPTR));
        BackfaceCullingCheckBox->setText(QApplication::translate("ctkVTKPropertyWidget", "Backface", Q_NULLPTR));
        ColorCollapsibleGroupBox->setTitle(QApplication::translate("ctkVTKPropertyWidget", "Color", Q_NULLPTR));
        ColorLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Color:", Q_NULLPTR));
        OpacityLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Opacity:", Q_NULLPTR));
        EdgeVisibilityLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Edge Visibility:", Q_NULLPTR));
        EdgeVisibilityCheckBox->setText(QString());
        EdgeColorLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Edge Color:", Q_NULLPTR));
        LightingCollapsibleGroupBox->setTitle(QApplication::translate("ctkVTKPropertyWidget", "Lighting", Q_NULLPTR));
        LightingLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Lighting:", Q_NULLPTR));
        LightingCheckBox->setText(QString());
        InterpolationLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Interpolation:", Q_NULLPTR));
        InterpolationComboBox->clear();
        InterpolationComboBox->insertItems(0, QStringList()
         << QApplication::translate("ctkVTKPropertyWidget", "Flat", Q_NULLPTR)
         << QApplication::translate("ctkVTKPropertyWidget", "Gouraud", Q_NULLPTR)
         << QApplication::translate("ctkVTKPropertyWidget", "Phong", Q_NULLPTR)
        );
        ShadingLabel->setText(QApplication::translate("ctkVTKPropertyWidget", "Shading:", Q_NULLPTR));
        ShadingCheckBox->setText(QString());
        MaterialCollapsibleGroupBox->setTitle(QApplication::translate("ctkVTKPropertyWidget", "Material", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkVTKPropertyWidget: public Ui_ctkVTKPropertyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKVTKPROPERTYWIDGET_H
