/********************************************************************************
** Form generated from reading UI file 'ctkVTKTextPropertyWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CTKVTKTEXTPROPERTYWIDGET_H
#define UI_CTKVTKTEXTPROPERTYWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>
#include "ctkColorPickerButton.h"
#include "ctkDoubleSlider.h"

QT_BEGIN_NAMESPACE

class Ui_ctkVTKTextPropertyWidget
{
public:
    QGridLayout *gridLayout;
    QLabel *TextLabel;
    QLineEdit *TextLineEdit;
    QLabel *ColorLabel;
    ctkColorPickerButton *ColorPickerButton;
    QLabel *OpacityLabel;
    ctkDoubleSlider *OpacitySlider;
    QLabel *FontLabel;
    QComboBox *FontComboBox;
    QLabel *StyleLabel;
    QCheckBox *BoldCheckBox;
    QCheckBox *ItalicCheckBox;
    QCheckBox *ShadowCheckBox;
    QLabel *SizeLabel;
    ctkDoubleSlider *SizeSlider;

    void setupUi(QWidget *ctkVTKTextPropertyWidget)
    {
        if (ctkVTKTextPropertyWidget->objectName().isEmpty())
            ctkVTKTextPropertyWidget->setObjectName(QStringLiteral("ctkVTKTextPropertyWidget"));
        ctkVTKTextPropertyWidget->resize(271, 169);
        gridLayout = new QGridLayout(ctkVTKTextPropertyWidget);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        TextLabel = new QLabel(ctkVTKTextPropertyWidget);
        TextLabel->setObjectName(QStringLiteral("TextLabel"));

        gridLayout->addWidget(TextLabel, 0, 0, 1, 1);

        TextLineEdit = new QLineEdit(ctkVTKTextPropertyWidget);
        TextLineEdit->setObjectName(QStringLiteral("TextLineEdit"));

        gridLayout->addWidget(TextLineEdit, 0, 1, 1, 3);

        ColorLabel = new QLabel(ctkVTKTextPropertyWidget);
        ColorLabel->setObjectName(QStringLiteral("ColorLabel"));

        gridLayout->addWidget(ColorLabel, 1, 0, 1, 1);

        ColorPickerButton = new ctkColorPickerButton(ctkVTKTextPropertyWidget);
        ColorPickerButton->setObjectName(QStringLiteral("ColorPickerButton"));

        gridLayout->addWidget(ColorPickerButton, 1, 1, 1, 3);

        OpacityLabel = new QLabel(ctkVTKTextPropertyWidget);
        OpacityLabel->setObjectName(QStringLiteral("OpacityLabel"));

        gridLayout->addWidget(OpacityLabel, 2, 0, 1, 1);

        OpacitySlider = new ctkDoubleSlider(ctkVTKTextPropertyWidget);
        OpacitySlider->setObjectName(QStringLiteral("OpacitySlider"));
        OpacitySlider->setSingleStep(0.1);
        OpacitySlider->setMaximum(1);
        OpacitySlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(OpacitySlider, 2, 1, 1, 3);

        FontLabel = new QLabel(ctkVTKTextPropertyWidget);
        FontLabel->setObjectName(QStringLiteral("FontLabel"));

        gridLayout->addWidget(FontLabel, 3, 0, 1, 1);

        FontComboBox = new QComboBox(ctkVTKTextPropertyWidget);
        FontComboBox->setObjectName(QStringLiteral("FontComboBox"));

        gridLayout->addWidget(FontComboBox, 3, 1, 1, 3);

        StyleLabel = new QLabel(ctkVTKTextPropertyWidget);
        StyleLabel->setObjectName(QStringLiteral("StyleLabel"));

        gridLayout->addWidget(StyleLabel, 4, 0, 1, 1);

        BoldCheckBox = new QCheckBox(ctkVTKTextPropertyWidget);
        BoldCheckBox->setObjectName(QStringLiteral("BoldCheckBox"));

        gridLayout->addWidget(BoldCheckBox, 4, 1, 1, 1);

        ItalicCheckBox = new QCheckBox(ctkVTKTextPropertyWidget);
        ItalicCheckBox->setObjectName(QStringLiteral("ItalicCheckBox"));

        gridLayout->addWidget(ItalicCheckBox, 4, 2, 1, 1);

        ShadowCheckBox = new QCheckBox(ctkVTKTextPropertyWidget);
        ShadowCheckBox->setObjectName(QStringLiteral("ShadowCheckBox"));

        gridLayout->addWidget(ShadowCheckBox, 4, 3, 1, 1);

        SizeLabel = new QLabel(ctkVTKTextPropertyWidget);
        SizeLabel->setObjectName(QStringLiteral("SizeLabel"));

        gridLayout->addWidget(SizeLabel, 5, 0, 1, 1);

        SizeSlider = new ctkDoubleSlider(ctkVTKTextPropertyWidget);
        SizeSlider->setObjectName(QStringLiteral("SizeSlider"));
        SizeSlider->setSingleStep(1);
        SizeSlider->setMaximum(100);
        SizeSlider->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(SizeSlider, 5, 1, 1, 3);


        retranslateUi(ctkVTKTextPropertyWidget);

        QMetaObject::connectSlotsByName(ctkVTKTextPropertyWidget);
    } // setupUi

    void retranslateUi(QWidget *ctkVTKTextPropertyWidget)
    {
        ctkVTKTextPropertyWidget->setWindowTitle(QApplication::translate("ctkVTKTextPropertyWidget", "vtkTextProperty", Q_NULLPTR));
        TextLabel->setText(QApplication::translate("ctkVTKTextPropertyWidget", "Text:", Q_NULLPTR));
        ColorLabel->setText(QApplication::translate("ctkVTKTextPropertyWidget", "Color:", Q_NULLPTR));
        OpacityLabel->setText(QApplication::translate("ctkVTKTextPropertyWidget", "Opacity:", Q_NULLPTR));
        FontLabel->setText(QApplication::translate("ctkVTKTextPropertyWidget", "Font:", Q_NULLPTR));
        FontComboBox->clear();
        FontComboBox->insertItems(0, QStringList()
         << QApplication::translate("ctkVTKTextPropertyWidget", "Arial", Q_NULLPTR)
         << QApplication::translate("ctkVTKTextPropertyWidget", "Courier", Q_NULLPTR)
         << QApplication::translate("ctkVTKTextPropertyWidget", "Times", Q_NULLPTR)
        );
        StyleLabel->setText(QApplication::translate("ctkVTKTextPropertyWidget", "Style:", Q_NULLPTR));
        BoldCheckBox->setText(QApplication::translate("ctkVTKTextPropertyWidget", "bold", Q_NULLPTR));
        ItalicCheckBox->setText(QApplication::translate("ctkVTKTextPropertyWidget", "italic", Q_NULLPTR));
        ShadowCheckBox->setText(QApplication::translate("ctkVTKTextPropertyWidget", "shadow", Q_NULLPTR));
        SizeLabel->setText(QApplication::translate("ctkVTKTextPropertyWidget", "Size:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ctkVTKTextPropertyWidget: public Ui_ctkVTKTextPropertyWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CTKVTKTEXTPROPERTYWIDGET_H
