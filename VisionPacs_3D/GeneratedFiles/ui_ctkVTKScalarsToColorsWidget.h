/********************************************************************************
** Form generated from reading UI file 'ctkVTKScalarsToColorsWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKVTKSCALARSTOCOLORSWIDGET_H
#define UI_CTKVTKSCALARSTOCOLORSWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include "ctkExpandButton.h"
#include "ctkColorPickerButton.h"
#include "ctkDoubleRangeSlider.h"
#include "ctkVTKScalarsToColorsView.h"

QT_BEGIN_NAMESPACE

class Ui_ctkVTKScalarsToColorsWidget
{
public:
    QGridLayout *gridLayout;
    QHBoxLayout *TopLayout;
    QSpacerItem *TopSpacer;
    QLabel *PointIdLabel;
    QSpinBox *PointIdSpinBox;
    ctkColorPickerButton *ColorPickerButton;
    QLabel *XLabel;
    QDoubleSpinBox *XSpinBox;
    QLabel *OpacityLabel;
    QDoubleSpinBox *OpacitySpinBox;
    QLabel *MidPointLabel;
    QDoubleSpinBox *MidPointSpinBox;
    QLabel *SharpnessLabel;
    QDoubleSpinBox *SharpnessSpinBox;
    ctkExpandButton *ExpandButton;
    ctkVTKScalarsToColorsView *View;
    ctkDoubleRangeSlider *XRangeSlider;
    ctkDoubleRangeSlider *YRangeSlider;

    void setupUi(QWidget *ctkVTKScalarsToColorsWidget)
    {
        if (ctkVTKScalarsToColorsWidget->objectName().isEmpty())
            ctkVTKScalarsToColorsWidget->setObjectName(QStringLiteral("ctkVTKScalarsToColorsWidget"));
        ctkVTKScalarsToColorsWidget->resize(508, 144);
        gridLayout = new QGridLayout(ctkVTKScalarsToColorsWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        TopLayout = new QHBoxLayout();
        TopLayout->setObjectName(QStringLiteral("TopLayout"));
        TopSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        TopLayout->addItem(TopSpacer);

        PointIdLabel = new QLabel(ctkVTKScalarsToColorsWidget);
        PointIdLabel->setObjectName(QStringLiteral("PointIdLabel"));

        TopLayout->addWidget(PointIdLabel);

        PointIdSpinBox = new QSpinBox(ctkVTKScalarsToColorsWidget);
        PointIdSpinBox->setObjectName(QStringLiteral("PointIdSpinBox"));
        PointIdSpinBox->setEnabled(false);
        PointIdSpinBox->setMinimum(-1);
        PointIdSpinBox->setMaximum(-1);

        TopLayout->addWidget(PointIdSpinBox);

        ColorPickerButton = new ctkColorPickerButton(ctkVTKScalarsToColorsWidget);
        ColorPickerButton->setObjectName(QStringLiteral("ColorPickerButton"));
        ColorPickerButton->setEnabled(false);
        ColorPickerButton->setDisplayColorName(false);
        ColorPickerButton->setDialogOptions(ctkColorPickerButton::UseCTKColorDialog);

        TopLayout->addWidget(ColorPickerButton);

        XLabel = new QLabel(ctkVTKScalarsToColorsWidget);
        XLabel->setObjectName(QStringLiteral("XLabel"));

        TopLayout->addWidget(XLabel);

        XSpinBox = new QDoubleSpinBox(ctkVTKScalarsToColorsWidget);
        XSpinBox->setObjectName(QStringLiteral("XSpinBox"));
        XSpinBox->setEnabled(false);

        TopLayout->addWidget(XSpinBox);

        OpacityLabel = new QLabel(ctkVTKScalarsToColorsWidget);
        OpacityLabel->setObjectName(QStringLiteral("OpacityLabel"));

        TopLayout->addWidget(OpacityLabel);

        OpacitySpinBox = new QDoubleSpinBox(ctkVTKScalarsToColorsWidget);
        OpacitySpinBox->setObjectName(QStringLiteral("OpacitySpinBox"));
        OpacitySpinBox->setEnabled(false);
        OpacitySpinBox->setMaximum(1);
        OpacitySpinBox->setSingleStep(0.1);

        TopLayout->addWidget(OpacitySpinBox);

        MidPointLabel = new QLabel(ctkVTKScalarsToColorsWidget);
        MidPointLabel->setObjectName(QStringLiteral("MidPointLabel"));

        TopLayout->addWidget(MidPointLabel);

        MidPointSpinBox = new QDoubleSpinBox(ctkVTKScalarsToColorsWidget);
        MidPointSpinBox->setObjectName(QStringLiteral("MidPointSpinBox"));
        MidPointSpinBox->setEnabled(false);
        MidPointSpinBox->setMaximum(1);
        MidPointSpinBox->setSingleStep(0.1);

        TopLayout->addWidget(MidPointSpinBox);

        SharpnessLabel = new QLabel(ctkVTKScalarsToColorsWidget);
        SharpnessLabel->setObjectName(QStringLiteral("SharpnessLabel"));

        TopLayout->addWidget(SharpnessLabel);

        SharpnessSpinBox = new QDoubleSpinBox(ctkVTKScalarsToColorsWidget);
        SharpnessSpinBox->setObjectName(QStringLiteral("SharpnessSpinBox"));
        SharpnessSpinBox->setEnabled(false);
        SharpnessSpinBox->setMaximum(1);
        SharpnessSpinBox->setSingleStep(0.1);

        TopLayout->addWidget(SharpnessSpinBox);

        ExpandButton = new ctkExpandButton(ctkVTKScalarsToColorsWidget);
        ExpandButton->setObjectName(QStringLiteral("ExpandButton"));
        ExpandButton->setContextMenuPolicy(Qt::DefaultContextMenu);

        TopLayout->addWidget(ExpandButton);


        gridLayout->addLayout(TopLayout, 0, 0, 1, 2);

        View = new ctkVTKScalarsToColorsView(ctkVTKScalarsToColorsWidget);
        View->setObjectName(QStringLiteral("View"));

        gridLayout->addWidget(View, 1, 0, 1, 1);

        XRangeSlider = new ctkDoubleRangeSlider(ctkVTKScalarsToColorsWidget);
        XRangeSlider->setObjectName(QStringLiteral("XRangeSlider"));
        XRangeSlider->setMaximum(1);
        XRangeSlider->setSingleStep(0.01);
        XRangeSlider->setMaximumValue(1);
        XRangeSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(XRangeSlider, 2, 0, 1, 1);

        YRangeSlider = new ctkDoubleRangeSlider(ctkVTKScalarsToColorsWidget);
        YRangeSlider->setObjectName(QStringLiteral("YRangeSlider"));
        YRangeSlider->setMaximum(1);
        YRangeSlider->setSingleStep(0.01);
        YRangeSlider->setOrientation(Qt::Vertical);

        gridLayout->addWidget(YRangeSlider, 1, 1, 1, 1);

        gridLayout->setColumnStretch(0, 1);

        retranslateUi(ctkVTKScalarsToColorsWidget);

        QMetaObject::connectSlotsByName(ctkVTKScalarsToColorsWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkVTKScalarsToColorsWidget)
    {
        ctkVTKScalarsToColorsWidget->setWindowTitle(QApplication::translate("ctkVTKScalarsToColorsWidget", "ScalarsToColorsWidget", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PointIdLabel->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Point ID", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        PointIdLabel->setText(QApplication::translate("ctkVTKScalarsToColorsWidget", "Point:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        PointIdSpinBox->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Point ID", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        ColorPickerButton->setText(QString());
#ifndef QT_NO_TOOLTIP
        XLabel->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "X coordinate of the current point", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        XLabel->setText(QApplication::translate("ctkVTKScalarsToColorsWidget", "X:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        XSpinBox->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "X coordinate of the current point", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        OpacityLabel->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Opacity of the current point", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        OpacityLabel->setText(QApplication::translate("ctkVTKScalarsToColorsWidget", "O:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        OpacitySpinBox->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Opacity of the current point", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        MidPointLabel->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Position of the midpoint", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        MidPointLabel->setText(QApplication::translate("ctkVTKScalarsToColorsWidget", "M:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        MidPointSpinBox->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Position of the midpoint", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
#ifndef QT_NO_TOOLTIP
        SharpnessLabel->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Sharpness of the midpoint", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        SharpnessLabel->setText(QApplication::translate("ctkVTKScalarsToColorsWidget", "S:", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        SharpnessSpinBox->setToolTip(QApplication::translate("ctkVTKScalarsToColorsWidget", "Sharpness of the midpoint", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
    } // retranslateUi

};

namespace Ui {
    class ctkVTKScalarsToColorsWidget: public Ui_ctkVTKScalarsToColorsWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKVTKSCALARSTOCOLORSWIDGET_H
