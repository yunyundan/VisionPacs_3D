/********************************************************************************
** Form generated from reading UI file 'ctkSliderWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKSLIDERWIDGET_H
#define UI_CTKSLIDERWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include "ctkDoubleSlider.h"
#include "ctkDoubleSpinBox.h"

QT_BEGIN_NAMESPACE

class Ui_ctkSliderWidget
{
public:
    QHBoxLayout *hboxLayout;
    ctkDoubleSlider *Slider;
    ctkDoubleSpinBox *SpinBox;

    void setupUi(QWidget *ctkSliderWidget)
    {
        if (ctkSliderWidget->objectName().isEmpty())
            ctkSliderWidget->setObjectName(QStringLiteral("ctkSliderWidget"));
        ctkSliderWidget->resize(183, 27);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ctkSliderWidget->sizePolicy().hasHeightForWidth());
        ctkSliderWidget->setSizePolicy(sizePolicy);
        hboxLayout = new QHBoxLayout(ctkSliderWidget);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        Slider = new ctkDoubleSlider(ctkSliderWidget);
        Slider->setObjectName(QStringLiteral("Slider"));
        Slider->setOrientation(Qt::Horizontal);

        hboxLayout->addWidget(Slider);

        SpinBox = new ctkDoubleSpinBox(ctkSliderWidget);
        SpinBox->setObjectName(QStringLiteral("SpinBox"));

        hboxLayout->addWidget(SpinBox);


        retranslateUi(ctkSliderWidget);

        QMetaObject::connectSlotsByName(ctkSliderWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkSliderWidget)
    {
        ctkSliderWidget->setWindowTitle(QApplication::translate("ctkSliderWidget", "ctkSliderWidget", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkSliderWidget: public Ui_ctkSliderWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKSLIDERWIDGET_H
