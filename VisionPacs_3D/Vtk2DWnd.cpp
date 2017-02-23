#include "stdafx.h"

#include "Vtk2DWnd.h"
#include "ui_Vtk2DWnd.h"
#include "HsImage.h"

Vtk2DWnd::Vtk2DWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vtk2DWnd)
	, m_nOriImgType(ORIIMG_AXIAL)
	, m_nImageNum(0)
{
    ui->setupUi(this);

	QObject::connect(ui->ImgWnd, SIGNAL(SendImageNum(int)), this, SLOT(RecieveImageNum(int)));
}

Vtk2DWnd::~Vtk2DWnd()
{
    delete ui;
}

void Vtk2DWnd::InitShowImage(int nOriImgType)
{
	QString sWndName = this->objectName();

	m_nOriImgType = nOriImgType;

	ui->ImgWnd->GetImgNumAndWndType(sWndName, nOriImgType);
}

void Vtk2DWnd::ChangeOperate(QString operate)
{
	ui->ImgWnd->setOperate(operate);
}

void Vtk2DWnd::SetImageData(void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, vector<CHsImage*> vOriImg)
{
	ui->ImgWnd->m_p3DImgData = p3Ddata;
	ui->ImgWnd->m_vOriImg = vOriImg;
	ui->ImgWnd->m_p3DArray = pImgArray;
}

void Vtk2DWnd::OnScrollBarMoved(int nValue)
{
	QString sWndName = sender()->parent()->objectName();
	ui->ImgWnd->CalcAndShowNormalImg(sWndName, m_nOriImgType, nValue, 1);
}

void Vtk2DWnd::RecieveImageNum(int nImgNum)
{
	m_nImageNum = nImgNum;
	ui->ScrollBar->setRange(0, m_nImageNum - 1);
	ui->ScrollBar->setValue(m_nImageNum / 2);
}
