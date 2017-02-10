#include "stdafx.h"

#include "Vtk2DWnd.h"
#include "ui_Vtk2DWnd.h"
#include "HsImage.h"

Vtk2DWnd::Vtk2DWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vtk2DWnd)
	, m_nOriImgType(ORIIMG_AXIAL)
	, m_nWndType(ORIIMG_AXIAL)
	, m_nImageNum(0)
{
    ui->setupUi(this);
}

Vtk2DWnd::~Vtk2DWnd()
{
    delete ui;
}

void Vtk2DWnd::showImage(CHsImage *pImg)
{
	ui->ImgWnd->SetImage(pImg);
}

void Vtk2DWnd::InitShowImage(int nOriImgType)
{
	QString sWndName = this->objectName();
	if (sWndName.compare("Axial_Wnd") == 0)
		m_nWndType = ORIIMG_AXIAL;
	else if (sWndName.compare("Coronal_Wnd") == 0)
		m_nWndType = ORIIMG_CORONAL;
	else if (sWndName.compare("Sagittal_Wnd") == 0)
		m_nWndType = ORIIMG_SAGGITAL;

	m_nOriImgType = nOriImgType;

	if (m_nWndType == m_nOriImgType)
	{
		m_nImageNum = m_vImg.size();
		ui->ImgWnd->SetImage(m_vImg[0]);
	}

	
}

void Vtk2DWnd::ChangeOperate(QString operate)
{
	ui->ImgWnd->setOperate(operate);
}
