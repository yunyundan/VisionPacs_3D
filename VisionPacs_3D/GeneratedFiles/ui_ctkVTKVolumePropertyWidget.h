/********************************************************************************
** Form generated from reading UI file 'ctkVTKVolumePropertyWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKVTKVOLUMEPROPERTYWIDGET_H
#define UI_CTKVTKVOLUMEPROPERTYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "ctkCollapsibleGroupBox.h"
#include "ctkMaterialPropertyWidget.h"
#include "ctkVTKScalarsToColorsWidget.h"
#include "ctkVTKThresholdWidget.h"

QT_BEGIN_NAMESPACE

class Ui_ctkVTKVolumePropertyWidget
{
public:
    QVBoxLayout *verticalLayout_3;
    ctkCollapsibleGroupBox *ScalarOpacityGroupBox;
    QVBoxLayout *verticalLayout_2;
    ctkVTKThresholdWidget *ScalarOpacityThresholdWidget;
    ctkVTKScalarsToColorsWidget *ScalarOpacityWidget;
    ctkCollapsibleGroupBox *ScalarColorGroupBox;
    QVBoxLayout *verticalLayout_4;
    ctkVTKScalarsToColorsWidget *ScalarColorWidget;
    ctkCollapsibleGroupBox *GradientGroupBox;
    QVBoxLayout *verticalLayout;
    ctkVTKScalarsToColorsWidget *GradientWidget;
    ctkCollapsibleGroupBox *AdvancedGroupBox;
    QGridLayout *gridLayout;
    QLabel *InterpolationLabel;
    QComboBox *InterpolationComboBox;
    QLabel *ShadeLabel;
    QCheckBox *ShadeCheckBox;
    QLabel *MaterialLabel;
    ctkMaterialPropertyWidget *MaterialPropertyWidget;

    void setupUi(QWidget *ctkVTKVolumePropertyWidget)
    {
        if (ctkVTKVolumePropertyWidget->objectName().isEmpty())
            ctkVTKVolumePropertyWidget->setObjectName(QStringLiteral("ctkVTKVolumePropertyWidget"));
        ctkVTKVolumePropertyWidget->resize(391, 699);
        verticalLayout_3 = new QVBoxLayout(ctkVTKVolumePropertyWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        ScalarOpacityGroupBox = new ctkCollapsibleGroupBox(ctkVTKVolumePropertyWidget);
        ScalarOpacityGroupBox->setObjectName(QStringLiteral("ScalarOpacityGroupBox"));
        verticalLayout_2 = new QVBoxLayout(ScalarOpacityGroupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 15, 0, 0);
        ScalarOpacityThresholdWidget = new ctkVTKThresholdWidget(ScalarOpacityGroupBox);
        ScalarOpacityThresholdWidget->setObjectName(QStringLiteral("ScalarOpacityThresholdWidget"));

        verticalLayout_2->addWidget(ScalarOpacityThresholdWidget);

        ScalarOpacityWidget = new ctkVTKScalarsToColorsWidget(ScalarOpacityGroupBox);
        ScalarOpacityWidget->setObjectName(QStringLiteral("ScalarOpacityWidget"));
        ScalarOpacityWidget->setMinimumSize(QSize(0, 120));
        ScalarOpacityWidget->setProperty("editColors", QVariant(false));

        verticalLayout_2->addWidget(ScalarOpacityWidget);


        verticalLayout_3->addWidget(ScalarOpacityGroupBox);

        ScalarColorGroupBox = new ctkCollapsibleGroupBox(ctkVTKVolumePropertyWidget);
        ScalarColorGroupBox->setObjectName(QStringLiteral("ScalarColorGroupBox"));
        verticalLayout_4 = new QVBoxLayout(ScalarColorGroupBox);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 15, 0, 0);
        ScalarColorWidget = new ctkVTKScalarsToColorsWidget(ScalarColorGroupBox);
        ScalarColorWidget->setObjectName(QStringLiteral("ScalarColorWidget"));
        ScalarColorWidget->setMinimumSize(QSize(0, 120));

        verticalLayout_4->addWidget(ScalarColorWidget);


        verticalLayout_3->addWidget(ScalarColorGroupBox);

        GradientGroupBox = new ctkCollapsibleGroupBox(ctkVTKVolumePropertyWidget);
        GradientGroupBox->setObjectName(QStringLiteral("GradientGroupBox"));
        verticalLayout = new QVBoxLayout(GradientGroupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 15, 0, 0);
        GradientWidget = new ctkVTKScalarsToColorsWidget(GradientGroupBox);
        GradientWidget->setObjectName(QStringLiteral("GradientWidget"));
        GradientWidget->setMinimumSize(QSize(0, 120));

        verticalLayout->addWidget(GradientWidget);


        verticalLayout_3->addWidget(GradientGroupBox);

        AdvancedGroupBox = new ctkCollapsibleGroupBox(ctkVTKVolumePropertyWidget);
        AdvancedGroupBox->setObjectName(QStringLiteral("AdvancedGroupBox"));
        gridLayout = new QGridLayout(AdvancedGroupBox);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        InterpolationLabel = new QLabel(AdvancedGroupBox);
        InterpolationLabel->setObjectName(QStringLiteral("InterpolationLabel"));

        gridLayout->addWidget(InterpolationLabel, 0, 0, 1, 1);

        InterpolationComboBox = new QComboBox(AdvancedGroupBox);
        InterpolationComboBox->setObjectName(QStringLiteral("InterpolationComboBox"));

        gridLayout->addWidget(InterpolationComboBox, 0, 1, 1, 1);

        ShadeLabel = new QLabel(AdvancedGroupBox);
        ShadeLabel->setObjectName(QStringLiteral("ShadeLabel"));

        gridLayout->addWidget(ShadeLabel, 1, 0, 1, 1);

        ShadeCheckBox = new QCheckBox(AdvancedGroupBox);
        ShadeCheckBox->setObjectName(QStringLiteral("ShadeCheckBox"));

        gridLayout->addWidget(ShadeCheckBox, 1, 1, 1, 1);

        MaterialLabel = new QLabel(AdvancedGroupBox);
        MaterialLabel->setObjectName(QStringLiteral("MaterialLabel"));

        gridLayout->addWidget(MaterialLabel, 2, 0, 1, 1);

        MaterialPropertyWidget = new ctkMaterialPropertyWidget(AdvancedGroupBox);
        MaterialPropertyWidget->setObjectName(QStringLiteral("MaterialPropertyWidget"));
        MaterialPropertyWidget->setProperty("colorVisible", QVariant(false));
        MaterialPropertyWidget->setProperty("opacityVisible", QVariant(false));
        MaterialPropertyWidget->setProperty("backfaceCullingVisible", QVariant(false));

        gridLayout->addWidget(MaterialPropertyWidget, 3, 0, 1, 2);


        verticalLayout_3->addWidget(AdvancedGroupBox);


        retranslateUi(ctkVTKVolumePropertyWidget);

        QMetaObject::connectSlotsByName(ctkVTKVolumePropertyWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkVTKVolumePropertyWidget)
    {
        ctkVTKVolumePropertyWidget->setWindowTitle(QApplication::translate("ctkVTKVolumePropertyWidget", "Volume property", Q_NULLPTR));
        ScalarOpacityGroupBox->setTitle(QApplication::translate("ctkVTKVolumePropertyWidget", "Scalar Opacity Mapping", Q_NULLPTR));
        ScalarColorGroupBox->setTitle(QApplication::translate("ctkVTKVolumePropertyWidget", "Scalar Color Mapping", Q_NULLPTR));
        GradientGroupBox->setTitle(QApplication::translate("ctkVTKVolumePropertyWidget", "Gradient Opacity", Q_NULLPTR));
        AdvancedGroupBox->setTitle(QApplication::translate("ctkVTKVolumePropertyWidget", "Advanced", Q_NULLPTR));
        InterpolationLabel->setText(QApplication::translate("ctkVTKVolumePropertyWidget", "Interpolation:", Q_NULLPTR));
        InterpolationComboBox->clear();
        InterpolationComboBox->insertItems(0, QStringList()
         << QApplication::translate("ctkVTKVolumePropertyWidget", "Nearest Neighbor", Q_NULLPTR)
         << QApplication::translate("ctkVTKVolumePropertyWidget", "Linear", Q_NULLPTR)
        );
        ShadeLabel->setText(QApplication::translate("ctkVTKVolumePropertyWidget", "Shade:", Q_NULLPTR));
        MaterialLabel->setText(QApplication::translate("ctkVTKVolumePropertyWidget", "Material:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkVTKVolumePropertyWidget: public Ui_ctkVTKVolumePropertyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKVTKVOLUMEPROPERTYWIDGET_H
