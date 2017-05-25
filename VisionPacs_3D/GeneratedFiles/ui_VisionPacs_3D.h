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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
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
    QGridLayout *gridLayout;
    WorkZone *Workzone;
    QWidget *MainBar;
    QTabWidget *OperateTab;
    QWidget *Viewer;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *VR_location;
    QPushButton *VR_rotate;
    QFrame *VR_line;
    QPushButton *VR_zoom;
    QPushButton *VR_pan;
    QHBoxLayout *VROrientionHLayout;
    QPushButton *Orientation_S;
    QPushButton *Orientation_I;
    QPushButton *Orientation_A;
    QPushButton *Orientation_P;
    QPushButton *Orientation_L;
    QPushButton *Orientation_R;
    QHBoxLayout *VRModeHLayout;
    QLabel *VRMode;
    QComboBox *VRModeCBox;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *ImgOperateHLayout;
    QPushButton *Img_location;
    QPushButton *Img_browser;
    QFrame *Img_line;
    QPushButton *Img_wl;
    QPushButton *Img_zoom;
    QPushButton *Img_pan;
    QVBoxLayout *verticalLayout_2;
    QPushButton *Btn_MprLinesShow;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QComboBox *comboBox_2;
    QWidget *RenderWidget;
    QHBoxLayout *RenderWHLayout;
    QVBoxLayout *RenderVLayout;
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
        gridLayout = new QGridLayout(MainWidget);
        gridLayout->setSpacing(0);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        Workzone = new WorkZone(MainWidget);
        Workzone->setObjectName(QStringLiteral("Workzone"));
        Workzone->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(Workzone, 0, 1, 2, 1);

        MainBar = new QWidget(MainWidget);
        MainBar->setObjectName(QStringLiteral("MainBar"));
        MainBar->setMinimumSize(QSize(0, 40));
        MainBar->setMaximumSize(QSize(16777215, 16777215));
        MainBar->setContextMenuPolicy(Qt::DefaultContextMenu);
        MainBar->setStyleSheet(QStringLiteral(""));

        gridLayout->addWidget(MainBar, 0, 0, 1, 1);

        OperateTab = new QTabWidget(MainWidget);
        OperateTab->setObjectName(QStringLiteral("OperateTab"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(OperateTab->sizePolicy().hasHeightForWidth());
        OperateTab->setSizePolicy(sizePolicy);
        OperateTab->setMinimumSize(QSize(290, 0));
        OperateTab->setMaximumSize(QSize(290, 16777215));
        OperateTab->setAutoFillBackground(false);
        OperateTab->setStyleSheet(QStringLiteral(""));
        OperateTab->setTabShape(QTabWidget::Triangular);
        OperateTab->setIconSize(QSize(16, 16));
        OperateTab->setElideMode(Qt::ElideNone);
        OperateTab->setDocumentMode(false);
        OperateTab->setTabsClosable(false);
        OperateTab->setMovable(false);
        OperateTab->setTabBarAutoHide(false);
        Viewer = new QWidget();
        Viewer->setObjectName(QStringLiteral("Viewer"));
        Viewer->setStyleSheet(QStringLiteral(""));
        verticalLayout_4 = new QVBoxLayout(Viewer);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(10);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 10, -1, -1);
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
        icon.addFile(QStringLiteral("Image/appbar.cursor.default.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_location->setIcon(icon);
        VR_location->setIconSize(QSize(60, 60));
        VR_location->setCheckable(true);
        VR_location->setChecked(true);
        VR_location->setAutoExclusive(false);
        VR_location->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_location);

        VR_rotate = new QPushButton(Viewer);
        VR_rotate->setObjectName(QStringLiteral("VR_rotate"));
        sizePolicy1.setHeightForWidth(VR_rotate->sizePolicy().hasHeightForWidth());
        VR_rotate->setSizePolicy(sizePolicy1);
        VR_rotate->setMinimumSize(QSize(50, 50));
        VR_rotate->setMaximumSize(QSize(50, 50));
        QIcon icon1;
        icon1.addFile(QStringLiteral("Image/appbar.transform.rotate.clockwise.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_rotate->setIcon(icon1);
        VR_rotate->setIconSize(QSize(60, 60));
        VR_rotate->setCheckable(true);
        VR_rotate->setAutoExclusive(false);
        VR_rotate->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_rotate);

        VR_line = new QFrame(Viewer);
        VR_line->setObjectName(QStringLiteral("VR_line"));
        VR_line->setStyleSheet(QStringLiteral("background-color: rgb(177, 177, 177);"));
        VR_line->setFrameShadow(QFrame::Sunken);
        VR_line->setFrameShape(QFrame::VLine);

        horizontalLayout->addWidget(VR_line);

        VR_zoom = new QPushButton(Viewer);
        VR_zoom->setObjectName(QStringLiteral("VR_zoom"));
        sizePolicy1.setHeightForWidth(VR_zoom->sizePolicy().hasHeightForWidth());
        VR_zoom->setSizePolicy(sizePolicy1);
        VR_zoom->setMinimumSize(QSize(50, 50));
        VR_zoom->setMaximumSize(QSize(50, 50));
        QIcon icon2;
        icon2.addFile(QStringLiteral("Image/appbar.magnify.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_zoom->setIcon(icon2);
        VR_zoom->setIconSize(QSize(60, 60));
        VR_zoom->setCheckable(true);
        VR_zoom->setChecked(true);
        VR_zoom->setAutoExclusive(false);
        VR_zoom->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_zoom);

        VR_pan = new QPushButton(Viewer);
        VR_pan->setObjectName(QStringLiteral("VR_pan"));
        sizePolicy1.setHeightForWidth(VR_pan->sizePolicy().hasHeightForWidth());
        VR_pan->setSizePolicy(sizePolicy1);
        VR_pan->setMinimumSize(QSize(50, 50));
        VR_pan->setMaximumSize(QSize(50, 50));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Image/appbar.cursor.move.png"), QSize(), QIcon::Normal, QIcon::Off);
        VR_pan->setIcon(icon3);
        VR_pan->setIconSize(QSize(60, 60));
        VR_pan->setCheckable(true);
        VR_pan->setAutoExclusive(false);
        VR_pan->setProperty("ToolButton", QVariant(true));

        horizontalLayout->addWidget(VR_pan);


        verticalLayout->addLayout(horizontalLayout);

        VROrientionHLayout = new QHBoxLayout();
        VROrientionHLayout->setSpacing(6);
        VROrientionHLayout->setObjectName(QStringLiteral("VROrientionHLayout"));
        Orientation_S = new QPushButton(Viewer);
        Orientation_S->setObjectName(QStringLiteral("Orientation_S"));
        Orientation_S->setMinimumSize(QSize(42, 42));
        Orientation_S->setMaximumSize(QSize(42, 42));
        QIcon icon4;
        icon4.addFile(QStringLiteral("Image/100.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_S->setIcon(icon4);
        Orientation_S->setIconSize(QSize(40, 40));
        Orientation_S->setProperty("ToolButton", QVariant(true));

        VROrientionHLayout->addWidget(Orientation_S);

        Orientation_I = new QPushButton(Viewer);
        Orientation_I->setObjectName(QStringLiteral("Orientation_I"));
        Orientation_I->setMinimumSize(QSize(42, 42));
        Orientation_I->setMaximumSize(QSize(42, 42));
        QIcon icon5;
        icon5.addFile(QStringLiteral("Image/101.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_I->setIcon(icon5);
        Orientation_I->setIconSize(QSize(40, 40));
        Orientation_I->setProperty("ToolButton", QVariant(true));

        VROrientionHLayout->addWidget(Orientation_I);

        Orientation_A = new QPushButton(Viewer);
        Orientation_A->setObjectName(QStringLiteral("Orientation_A"));
        Orientation_A->setMinimumSize(QSize(42, 42));
        Orientation_A->setMaximumSize(QSize(42, 42));
        QIcon icon6;
        icon6.addFile(QStringLiteral("Image/102.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_A->setIcon(icon6);
        Orientation_A->setIconSize(QSize(60, 60));
        Orientation_A->setProperty("ToolButton", QVariant(true));

        VROrientionHLayout->addWidget(Orientation_A);

        Orientation_P = new QPushButton(Viewer);
        Orientation_P->setObjectName(QStringLiteral("Orientation_P"));
        Orientation_P->setMinimumSize(QSize(42, 42));
        Orientation_P->setMaximumSize(QSize(42, 42));
        QIcon icon7;
        icon7.addFile(QStringLiteral("Image/103.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_P->setIcon(icon7);
        Orientation_P->setIconSize(QSize(40, 40));
        Orientation_P->setProperty("ToolButton", QVariant(true));

        VROrientionHLayout->addWidget(Orientation_P);

        Orientation_L = new QPushButton(Viewer);
        Orientation_L->setObjectName(QStringLiteral("Orientation_L"));
        Orientation_L->setMinimumSize(QSize(42, 42));
        Orientation_L->setMaximumSize(QSize(42, 42));
        QIcon icon8;
        icon8.addFile(QStringLiteral("Image/104.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_L->setIcon(icon8);
        Orientation_L->setIconSize(QSize(40, 40));
        Orientation_L->setProperty("ToolButton", QVariant(true));

        VROrientionHLayout->addWidget(Orientation_L);

        Orientation_R = new QPushButton(Viewer);
        Orientation_R->setObjectName(QStringLiteral("Orientation_R"));
        Orientation_R->setMinimumSize(QSize(42, 42));
        Orientation_R->setMaximumSize(QSize(42, 42));
        QIcon icon9;
        icon9.addFile(QStringLiteral("Image/105.png"), QSize(), QIcon::Normal, QIcon::Off);
        Orientation_R->setIcon(icon9);
        Orientation_R->setIconSize(QSize(40, 40));
        Orientation_R->setProperty("ToolButton", QVariant(true));

        VROrientionHLayout->addWidget(Orientation_R);


        verticalLayout->addLayout(VROrientionHLayout);

        VRModeHLayout = new QHBoxLayout();
        VRModeHLayout->setSpacing(6);
        VRModeHLayout->setObjectName(QStringLiteral("VRModeHLayout"));
        VRModeHLayout->setContentsMargins(3, -1, 3, -1);
        VRMode = new QLabel(Viewer);
        VRMode->setObjectName(QStringLiteral("VRMode"));
        VRMode->setStyleSheet(QStringLiteral("color: rgb(177, 177, 177);"));
        VRMode->setProperty("uiLable", QVariant(true));

        VRModeHLayout->addWidget(VRMode);

        VRModeCBox = new QComboBox(Viewer);
        VRModeCBox->setObjectName(QStringLiteral("VRModeCBox"));

        VRModeHLayout->addWidget(VRModeCBox);

        VRModeHLayout->setStretch(0, 2);
        VRModeHLayout->setStretch(1, 8);

        verticalLayout->addLayout(VRModeHLayout);


        verticalLayout_4->addLayout(verticalLayout);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        ImgOperateHLayout = new QHBoxLayout();
        ImgOperateHLayout->setSpacing(6);
        ImgOperateHLayout->setObjectName(QStringLiteral("ImgOperateHLayout"));
        Img_location = new QPushButton(Viewer);
        Img_location->setObjectName(QStringLiteral("Img_location"));
        sizePolicy1.setHeightForWidth(Img_location->sizePolicy().hasHeightForWidth());
        Img_location->setSizePolicy(sizePolicy1);
        Img_location->setMinimumSize(QSize(50, 50));
        Img_location->setMaximumSize(QSize(50, 50));
        Img_location->setIcon(icon);
        Img_location->setIconSize(QSize(60, 60));
        Img_location->setCheckable(true);
        Img_location->setChecked(true);
        Img_location->setProperty("ToolButton", QVariant(true));

        ImgOperateHLayout->addWidget(Img_location);

        Img_browser = new QPushButton(Viewer);
        Img_browser->setObjectName(QStringLiteral("Img_browser"));
        sizePolicy1.setHeightForWidth(Img_browser->sizePolicy().hasHeightForWidth());
        Img_browser->setSizePolicy(sizePolicy1);
        Img_browser->setMinimumSize(QSize(50, 50));
        Img_browser->setMaximumSize(QSize(50, 50));
        QIcon icon10;
        icon10.addFile(QStringLiteral("Image/appbar.layer.png"), QSize(), QIcon::Normal, QIcon::Off);
        Img_browser->setIcon(icon10);
        Img_browser->setIconSize(QSize(60, 60));
        Img_browser->setCheckable(true);
        Img_browser->setProperty("ToolButton", QVariant(true));

        ImgOperateHLayout->addWidget(Img_browser);

        Img_line = new QFrame(Viewer);
        Img_line->setObjectName(QStringLiteral("Img_line"));
        Img_line->setStyleSheet(QStringLiteral("background-color: rgb(177, 177, 177);"));
        Img_line->setFrameShadow(QFrame::Sunken);
        Img_line->setFrameShape(QFrame::VLine);

        ImgOperateHLayout->addWidget(Img_line);

        Img_wl = new QPushButton(Viewer);
        Img_wl->setObjectName(QStringLiteral("Img_wl"));
        sizePolicy1.setHeightForWidth(Img_wl->sizePolicy().hasHeightForWidth());
        Img_wl->setSizePolicy(sizePolicy1);
        Img_wl->setMinimumSize(QSize(50, 50));
        Img_wl->setMaximumSize(QSize(50, 50));
        QIcon icon11;
        icon11.addFile(QStringLiteral("Image/appbar.contrast.png"), QSize(), QIcon::Normal, QIcon::Off);
        Img_wl->setIcon(icon11);
        Img_wl->setIconSize(QSize(60, 60));
        Img_wl->setCheckable(true);
        Img_wl->setChecked(true);
        Img_wl->setProperty("ToolButton", QVariant(true));

        ImgOperateHLayout->addWidget(Img_wl);

        Img_zoom = new QPushButton(Viewer);
        Img_zoom->setObjectName(QStringLiteral("Img_zoom"));
        sizePolicy1.setHeightForWidth(Img_zoom->sizePolicy().hasHeightForWidth());
        Img_zoom->setSizePolicy(sizePolicy1);
        Img_zoom->setMinimumSize(QSize(50, 50));
        Img_zoom->setMaximumSize(QSize(50, 50));
        Img_zoom->setIcon(icon2);
        Img_zoom->setIconSize(QSize(60, 60));
        Img_zoom->setCheckable(true);
        Img_zoom->setProperty("ToolButton", QVariant(true));

        ImgOperateHLayout->addWidget(Img_zoom);

        Img_pan = new QPushButton(Viewer);
        Img_pan->setObjectName(QStringLiteral("Img_pan"));
        sizePolicy1.setHeightForWidth(Img_pan->sizePolicy().hasHeightForWidth());
        Img_pan->setSizePolicy(sizePolicy1);
        Img_pan->setMinimumSize(QSize(50, 50));
        Img_pan->setMaximumSize(QSize(50, 50));
        Img_pan->setIcon(icon3);
        Img_pan->setIconSize(QSize(60, 60));
        Img_pan->setCheckable(true);
        Img_pan->setProperty("ToolButton", QVariant(true));

        ImgOperateHLayout->addWidget(Img_pan);


        verticalLayout_3->addLayout(ImgOperateHLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        Btn_MprLinesShow = new QPushButton(Viewer);
        Btn_MprLinesShow->setObjectName(QStringLiteral("Btn_MprLinesShow"));
        sizePolicy1.setHeightForWidth(Btn_MprLinesShow->sizePolicy().hasHeightForWidth());
        Btn_MprLinesShow->setSizePolicy(sizePolicy1);
        Btn_MprLinesShow->setMinimumSize(QSize(50, 50));
        Btn_MprLinesShow->setMaximumSize(QSize(50, 50));
        QIcon icon12;
        icon12.addFile(QStringLiteral("Image/appbar.resource.png"), QSize(), QIcon::Normal, QIcon::Off);
        Btn_MprLinesShow->setIcon(icon12);
        Btn_MprLinesShow->setIconSize(QSize(60, 60));
        Btn_MprLinesShow->setCheckable(true);
        Btn_MprLinesShow->setChecked(true);
        Btn_MprLinesShow->setProperty("ToolButton", QVariant(true));

        verticalLayout_2->addWidget(Btn_MprLinesShow);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(Viewer);
        label->setObjectName(QStringLiteral("label"));
        label->setProperty("uiLable", QVariant(true));

        horizontalLayout_2->addWidget(label);

        comboBox = new QComboBox(Viewer);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setMinimumSize(QSize(0, 20));

        horizontalLayout_2->addWidget(comboBox);

        horizontalLayout_2->setStretch(0, 2);
        horizontalLayout_2->setStretch(1, 8);

        horizontalLayout_4->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(Viewer);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setProperty("uiLable", QVariant(true));

        horizontalLayout_3->addWidget(label_2);

        comboBox_2 = new QComboBox(Viewer);
        comboBox_2->setObjectName(QStringLiteral("comboBox_2"));
        comboBox_2->setMinimumSize(QSize(0, 20));

        horizontalLayout_3->addWidget(comboBox_2);

        horizontalLayout_3->setStretch(0, 2);
        horizontalLayout_3->setStretch(1, 8);

        horizontalLayout_4->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout_3->addLayout(verticalLayout_2);


        verticalLayout_4->addLayout(verticalLayout_3);

        OperateTab->addTab(Viewer, QString());
        RenderWidget = new QWidget();
        RenderWidget->setObjectName(QStringLiteral("RenderWidget"));
        RenderWHLayout = new QHBoxLayout(RenderWidget);
        RenderWHLayout->setSpacing(6);
        RenderWHLayout->setContentsMargins(11, 11, 11, 11);
        RenderWHLayout->setObjectName(QStringLiteral("RenderWHLayout"));
        RenderVLayout = new QVBoxLayout();
        RenderVLayout->setSpacing(6);
        RenderVLayout->setObjectName(QStringLiteral("RenderVLayout"));

        RenderWHLayout->addLayout(RenderVLayout);

        OperateTab->addTab(RenderWidget, QString());

        gridLayout->addWidget(OperateTab, 1, 0, 1, 1);

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
        QObject::connect(Orientation_S, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_I, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_A, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_P, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_L, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(Orientation_R, SIGNAL(clicked()), Workzone, SLOT(Btn_VrOrientationClick()));
        QObject::connect(VRModeCBox, SIGNAL(currentIndexChanged(QString)), Workzone, SLOT(CB_VrModeChanged(QString)));
        QObject::connect(Btn_MprLinesShow, SIGNAL(clicked()), Workzone, SLOT(Btn_IsMprLineShow()));

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
        VRMode->setText(QApplication::translate("VisionPacs_3D", "\346\270\262\346\237\223\346\226\271\346\241\210\357\274\232", Q_NULLPTR));
        VRModeCBox->setProperty("Type", QVariant(QApplication::translate("VisionPacs_3D", "VRmode", Q_NULLPTR)));
        Img_location->setText(QString());
        Img_browser->setText(QString());
        Img_wl->setText(QString());
        Img_zoom->setText(QString());
        Img_pan->setText(QString());
        Btn_MprLinesShow->setText(QString());
        label->setText(QApplication::translate("VisionPacs_3D", "\345\261\202\345\216\232\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("VisionPacs_3D", "\346\250\241\345\274\217\357\274\232", Q_NULLPTR));
        OperateTab->setTabText(OperateTab->indexOf(Viewer), QApplication::translate("VisionPacs_3D", "\345\270\270\350\247\204", Q_NULLPTR));
        OperateTab->setTabText(OperateTab->indexOf(RenderWidget), QApplication::translate("VisionPacs_3D", "\346\270\262\346\237\223\346\226\271\346\241\210", Q_NULLPTR));
        menuDICOM->setTitle(QApplication::translate("VisionPacs_3D", "DICOM\346\226\207\344\273\266", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class VisionPacs_3D: public Ui_VisionPacs_3D {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VISIONPACS_3D_H
