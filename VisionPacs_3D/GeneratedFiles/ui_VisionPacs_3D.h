/********************************************************************************
** Form generated from reading UI file 'VisionPacs_3D.ui'
**
** Created by: Qt User Interface Compiler version 5.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VISIONPACS_3D_H
#define UI_VISIONPACS_3D_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "WorkZone.h"

QT_BEGIN_NAMESPACE

class Ui_VisionPacs_3D
{
public:
    QAction *actionOpen_File;
    QAction *actionTest;
    QWidget *MainWidget;
    QGridLayout *MainGLayout;
    QWidget *MainBar;
    QTabWidget *OperateTab;
    QWidget *Viewer;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *VR_location;
    QPushButton *VR_rotate;
    QPushButton *VR_zoom;
    QPushButton *VR_pan;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *Orientation_S;
    QPushButton *Orientation_I;
    QPushButton *Orientation_A;
    QPushButton *Orientation_P;
    QPushButton *Orientation_L;
    QPushButton *Orientation_R;
    QHBoxLayout *ImgOperateLayout;
    QPushButton *Img_location;
    QPushButton *Img_browser;
    QPushButton *Img_wl;
    QPushButton *Img_zoom;
    QPushButton *Img_pan;
    QWidget *tab_2;
    WorkZone *Workzone;
    QMenuBar *menuBar;
    QMenu *menuDICOM;

    void setupUi(QMainWindow *VisionPacs_3D)
    {
        if (VisionPacs_3D->objectName().isEmpty())
            VisionPacs_3D->setObjectName(QStringLiteral("VisionPacs_3D"));
        VisionPacs_3D->resize(743, 516);
        VisionPacs_3D->setStyleSheet(QStringLiteral(""));
        actionOpen_File = new QAction(VisionPacs_3D);
        actionOpen_File->setObjectName(QStringLiteral("actionOpen_File"));
        actionTest = new QAction(VisionPacs_3D);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        MainWidget = new QWidget(VisionPacs_3D);
        MainWidget->setObjectName(QStringLiteral("MainWidget"));
        MainWidget->setLayoutDirection(Qt::LeftToRight);
        MainWidget->setStyleSheet(QStringLiteral(""));
        MainGLayout = new QGridLayout(MainWidget);
        MainGLayout->setSpacing(0);
        MainGLayout->setContentsMargins(11, 11, 11, 11);
        MainGLayout->setObjectName(QStringLiteral("MainGLayout"));
        MainGLayout->setContentsMargins(0, 0, 0, 0);
        MainBar = new QWidget(MainWidget);
        MainBar->setObjectName(QStringLiteral("MainBar"));
        MainBar->setMinimumSize(QSize(0, 20));

        MainGLayout->addWidget(MainBar, 0, 0, 1, 2);

        OperateTab = new QTabWidget(MainWidget);
        OperateTab->setObjectName(QStringLiteral("OperateTab"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OperateTab->sizePolicy().hasHeightForWidth());
        OperateTab->setSizePolicy(sizePolicy);
        OperateTab->setMinimumSize(QSize(280, 0));
        OperateTab->setAutoFillBackground(false);
        OperateTab->setStyleSheet(QStringLiteral(""));
        OperateTab->setTabShape(QTabWidget::Triangular);
        OperateTab->setElideMode(Qt::ElideNone);
        OperateTab->setDocumentMode(false);
        OperateTab->setTabsClosable(false);
        OperateTab->setMovable(false);
        OperateTab->setTabBarAutoHide(false);
        Viewer = new QWidget();
        Viewer->setObjectName(QStringLiteral("Viewer"));
        Viewer->setStyleSheet(QStringLiteral(""));
        verticalLayout_2 = new QVBoxLayout(Viewer);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 5, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        VR_location = new QPushButton(Viewer);
        VR_location->setObjectName(QStringLiteral("VR_location"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(VR_location->sizePolicy().hasHeightForWidth());
        VR_location->setSizePolicy(sizePolicy1);
        VR_location->setMinimumSize(QSize(50, 50));
        VR_location->setMaximumSize(QSize(50, 50));
        QIcon icon;
        icon.addFile(QStringLiteral("Image/1.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_location->setIcon(icon);
        VR_location->setIconSize(QSize(40, 40));
        VR_location->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_location);

        VR_rotate = new QPushButton(Viewer);
        VR_rotate->setObjectName(QStringLiteral("VR_rotate"));
        sizePolicy1.setHeightForWidth(VR_rotate->sizePolicy().hasHeightForWidth());
        VR_rotate->setSizePolicy(sizePolicy1);
        VR_rotate->setMinimumSize(QSize(50, 50));
        VR_rotate->setMaximumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Image/2.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_rotate->setIcon(icon1);
        VR_rotate->setIconSize(QSize(40, 40));
        VR_rotate->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_rotate);

        VR_zoom = new QPushButton(Viewer);
        VR_zoom->setObjectName(QStringLiteral("VR_zoom"));
        sizePolicy1.setHeightForWidth(VR_zoom->sizePolicy().hasHeightForWidth());
        VR_zoom->setSizePolicy(sizePolicy1);
        VR_zoom->setMinimumSize(QSize(50, 50));
        VR_zoom->setMaximumSize(QSize(50, 50));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Image/3.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_zoom->setIcon(icon2);
        VR_zoom->setIconSize(QSize(40, 40));
        VR_zoom->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_zoom);

        VR_pan = new QPushButton(Viewer);
        VR_pan->setObjectName(QStringLiteral("VR_pan"));
        sizePolicy1.setHeightForWidth(VR_pan->sizePolicy().hasHeightForWidth());
        VR_pan->setSizePolicy(sizePolicy1);
        VR_pan->setMinimumSize(QSize(50, 50));
        VR_pan->setMaximumSize(QSize(50, 50));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Image/4.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_pan->setIcon(icon3);
        VR_pan->setIconSize(QSize(40, 40));
        VR_pan->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_pan);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        Orientation_S = new QPushButton(Viewer);
        Orientation_S->setObjectName(QStringLiteral("Orientation_S"));
        Orientation_S->setMinimumSize(QSize(42, 42));
        Orientation_S->setMaximumSize(QSize(42, 42));
        QIcon icon4;
        icon4.addFile(QStringLiteral("Image/5.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_S->setIcon(icon4);
        Orientation_S->setIconSize(QSize(40, 40));
        Orientation_S->setProperty("ToolButton", QVariant(true));

        horizontalLayout_2->addWidget(Orientation_S);

        Orientation_I = new QPushButton(Viewer);
        Orientation_I->setObjectName(QStringLiteral("Orientation_I"));
        Orientation_I->setMinimumSize(QSize(42, 42));
        Orientation_I->setMaximumSize(QSize(42, 42));
        QIcon icon5;
        icon5.addFile(QStringLiteral("Image/6.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_I->setIcon(icon5);
        Orientation_I->setIconSize(QSize(40, 40));
        Orientation_I->setProperty("ToolButton", QVariant(true));

        horizontalLayout_2->addWidget(Orientation_I);

        Orientation_A = new QPushButton(Viewer);
        Orientation_A->setObjectName(QStringLiteral("Orientation_A"));
        Orientation_A->setMinimumSize(QSize(42, 42));
        Orientation_A->setMaximumSize(QSize(42, 42));
        QIcon icon6;
        icon6.addFile(QStringLiteral("Image/10.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_A->setIcon(icon6);
        Orientation_A->setIconSize(QSize(40, 40));
        Orientation_A->setProperty("ToolButton", QVariant(true));

        horizontalLayout_2->addWidget(Orientation_A);

        Orientation_P = new QPushButton(Viewer);
        Orientation_P->setObjectName(QStringLiteral("Orientation_P"));
        Orientation_P->setMinimumSize(QSize(42, 42));
        Orientation_P->setMaximumSize(QSize(42, 42));
        QIcon icon7;
        icon7.addFile(QStringLiteral("Image/8.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_P->setIcon(icon7);
        Orientation_P->setIconSize(QSize(40, 40));
        Orientation_P->setProperty("ToolButton", QVariant(true));

        horizontalLayout_2->addWidget(Orientation_P);

        Orientation_L = new QPushButton(Viewer);
        Orientation_L->setObjectName(QStringLiteral("Orientation_L"));
        Orientation_L->setMinimumSize(QSize(42, 42));
        Orientation_L->setMaximumSize(QSize(42, 42));
        QIcon icon8;
        icon8.addFile(QStringLiteral("Image/9.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_L->setIcon(icon8);
        Orientation_L->setIconSize(QSize(40, 40));
        Orientation_L->setProperty("ToolButton", QVariant(true));

        horizontalLayout_2->addWidget(Orientation_L);

        Orientation_R = new QPushButton(Viewer);
        Orientation_R->setObjectName(QStringLiteral("Orientation_R"));
        Orientation_R->setMinimumSize(QSize(42, 42));
        Orientation_R->setMaximumSize(QSize(42, 42));
        QIcon icon9;
        icon9.addFile(QStringLiteral("Image/7.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_R->setIcon(icon9);
        Orientation_R->setIconSize(QSize(40, 40));
        Orientation_R->setProperty("ToolButton", QVariant(true));

        horizontalLayout_2->addWidget(Orientation_R);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addLayout(verticalLayout);

        ImgOperateLayout = new QHBoxLayout();
        ImgOperateLayout->setSpacing(6);
        ImgOperateLayout->setObjectName(QStringLiteral("ImgOperateLayout"));
        Img_location = new QPushButton(Viewer);
        Img_location->setObjectName(QStringLiteral("Img_location"));
        sizePolicy1.setHeightForWidth(Img_location->sizePolicy().hasHeightForWidth());
        Img_location->setSizePolicy(sizePolicy1);
        Img_location->setMinimumSize(QSize(50, 50));
        Img_location->setMaximumSize(QSize(50, 50));
        Img_location->setProperty("ToolButton", QVariant(true));

        ImgOperateLayout->addWidget(Img_location);

        Img_browser = new QPushButton(Viewer);
        Img_browser->setObjectName(QStringLiteral("Img_browser"));
        sizePolicy1.setHeightForWidth(Img_browser->sizePolicy().hasHeightForWidth());
        Img_browser->setSizePolicy(sizePolicy1);
        Img_browser->setMinimumSize(QSize(50, 50));
        Img_browser->setMaximumSize(QSize(50, 50));
        Img_browser->setProperty("ToolButton", QVariant(true));

        ImgOperateLayout->addWidget(Img_browser);

        Img_wl = new QPushButton(Viewer);
        Img_wl->setObjectName(QStringLiteral("Img_wl"));
        sizePolicy1.setHeightForWidth(Img_wl->sizePolicy().hasHeightForWidth());
        Img_wl->setSizePolicy(sizePolicy1);
        Img_wl->setMinimumSize(QSize(50, 50));
        Img_wl->setMaximumSize(QSize(50, 50));
        Img_wl->setProperty("ToolButton", QVariant(true));

        ImgOperateLayout->addWidget(Img_wl);

        Img_zoom = new QPushButton(Viewer);
        Img_zoom->setObjectName(QStringLiteral("Img_zoom"));
        sizePolicy1.setHeightForWidth(Img_zoom->sizePolicy().hasHeightForWidth());
        Img_zoom->setSizePolicy(sizePolicy1);
        Img_zoom->setMinimumSize(QSize(50, 50));
        Img_zoom->setMaximumSize(QSize(50, 50));
        Img_zoom->setProperty("ToolButton", QVariant(true));

        ImgOperateLayout->addWidget(Img_zoom);

        Img_pan = new QPushButton(Viewer);
        Img_pan->setObjectName(QStringLiteral("Img_pan"));
        sizePolicy1.setHeightForWidth(Img_pan->sizePolicy().hasHeightForWidth());
        Img_pan->setSizePolicy(sizePolicy1);
        Img_pan->setMinimumSize(QSize(50, 50));
        Img_pan->setMaximumSize(QSize(50, 50));
        Img_pan->setProperty("ToolButton", QVariant(true));

        ImgOperateLayout->addWidget(Img_pan);


        verticalLayout_2->addLayout(ImgOperateLayout);

        OperateTab->addTab(Viewer, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        OperateTab->addTab(tab_2, QString());

        MainGLayout->addWidget(OperateTab, 1, 0, 1, 1);

        Workzone = new WorkZone(MainWidget);
        Workzone->setObjectName(QStringLiteral("Workzone"));
        Workzone->setStyleSheet(QStringLiteral(""));

        MainGLayout->addWidget(Workzone, 1, 1, 1, 1);

        VisionPacs_3D->setCentralWidget(MainWidget);
        menuBar = new QMenuBar(VisionPacs_3D);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 743, 23));
        menuDICOM = new QMenu(menuBar);
        menuDICOM->setObjectName(QStringLiteral("menuDICOM"));
        VisionPacs_3D->setMenuBar(menuBar);

        menuBar->addAction(menuDICOM->menuAction());
        menuDICOM->addAction(actionOpen_File);
        menuDICOM->addSeparator();
        menuDICOM->addAction(actionTest);

        retranslateUi(VisionPacs_3D);
        QObject::connect(Img_location, SIGNAL(clicked()), Workzone, SLOT(Btn_ImgOperateClick()));
        QObject::connect(Img_browser, SIGNAL(clicked()), Workzone, SLOT(Btn_ImgOperateClick()));
        QObject::connect(Img_wl, SIGNAL(clicked()), Workzone, SLOT(Btn_ImgOperateClick()));
        QObject::connect(Img_zoom, SIGNAL(clicked()), Workzone, SLOT(Btn_ImgOperateClick()));
        QObject::connect(Img_pan, SIGNAL(clicked()), Workzone, SLOT(Btn_ImgOperateClick()));
        QObject::connect(VR_location, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOperateClick()));
        QObject::connect(VR_rotate, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOperateClick()));
        QObject::connect(VR_zoom, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOperateClick()));
        QObject::connect(VR_pan, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOperateClick()));
        QObject::connect(Orientation_S, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_I, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_A, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_A, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_P, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_L, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_R, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));

        OperateTab->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(VisionPacs_3D);
    } // setupUi

    void retranslateUi(QMainWindow *VisionPacs_3D)
    {
        VisionPacs_3D->setWindowTitle(QApplication::translate("VisionPacs_3D", "VisionPacs_3D", Q_NULLPTR));
        actionOpen_File->setText(QApplication::translate("VisionPacs_3D", "Open File", Q_NULLPTR));
        actionTest->setText(QApplication::translate("VisionPacs_3D", "\346\265\213\350\257\225", Q_NULLPTR));
#ifndef QT_NO_TOOLTIP
        VR_location->setToolTip(QApplication::translate("VisionPacs_3D", "\345\256\232\344\275\215", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        VR_location->setText(QString());
#ifndef QT_NO_TOOLTIP
        VR_rotate->setToolTip(QApplication::translate("VisionPacs_3D", "\346\227\213\350\275\254", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        VR_rotate->setText(QString());
#ifndef QT_NO_TOOLTIP
        VR_zoom->setToolTip(QApplication::translate("VisionPacs_3D", "\346\224\276\345\244\247", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        VR_zoom->setText(QString());
#ifndef QT_NO_TOOLTIP
        VR_pan->setToolTip(QApplication::translate("VisionPacs_3D", "\347\247\273\345\212\250", Q_NULLPTR));
#endif // QT_NO_TOOLTIP
        VR_pan->setText(QString());
        Orientation_S->setText(QString());
        Orientation_I->setText(QString());
        Orientation_A->setText(QString());
        Orientation_P->setText(QString());
        Orientation_L->setText(QString());
        Orientation_R->setText(QString());
        Img_location->setText(QString());
        Img_browser->setText(QString());
        Img_wl->setText(QString());
        Img_zoom->setText(QString());
        Img_pan->setText(QString());
        OperateTab->setTabText(OperateTab->indexOf(Viewer), QApplication::translate("VisionPacs_3D", "Viewer", Q_NULLPTR));
        OperateTab->setTabText(OperateTab->indexOf(tab_2), QApplication::translate("VisionPacs_3D", "Tab 2", Q_NULLPTR));
        menuDICOM->setTitle(QApplication::translate("VisionPacs_3D", "DICOM\346\226\207\344\273\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class VisionPacs_3D: public Ui_VisionPacs_3D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISIONPACS_3D_H
