#include "stdafx.h"

#include "Vtk2DWnd.h"
#include "ui_Vtk2DWnd.h"
#include "HsImage.h"
#include "OperateMprLines.h"
#include "Hmy3dMath.h"

Vtk2DWnd::Vtk2DWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vtk2DWnd)
	,m_nImageNum(0)
	,m_nSliceNum(1)
{
    ui->setupUi(this);

	connect(ui->ImgWnd, SIGNAL(SendImageNum(int)), this, SLOT(RecieveImageNum(int)));
	connect(ui->ImgWnd, SIGNAL(ImageIndexChange(int)), this, SLOT(RecieveImageIndexChange(int)));
}

Vtk2DWnd::~Vtk2DWnd()
{
    delete ui;
}

void Vtk2DWnd::InitShowImage()
{
	QString sWndName = this->objectName();
	ui->ImgWnd->SetImageWndType(sWndName);
	ui->ImgWnd->FirstCalAndShowImage();
}

void Vtk2DWnd::ChangeOperate(QString operate)
{
	ui->ImgWnd->SetOperate(operate);
}

void Vtk2DWnd::SetImageData(void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, vector<CHsImage*> vOriImg, HmyImageData3D *pHmyImgData,CResliceControl *pRControl)
{
	ui->ImgWnd->InitImageDataPara(pImgArray, p3Ddata, vOriImg,pHmyImgData);
	ui->ImgWnd->SetResliceControl(pRControl);
}

void Vtk2DWnd::SetImageIndex(int nIndex)
{
	ui->ScrollBar->setValue(nIndex);
}

void Vtk2DWnd::SetImageSlice(double dSlicePos)
{
	CHsImage *pImg = ui->ImgWnd->GetImage();
	m_nSliceNum = qRound(dSlicePos / pImg->m_fOriSilceThick);
}

void Vtk2DWnd::GetImageRowAndCol(int &nRow, int &nCol)
{
	CHsImage *pImg = ui->ImgWnd->GetImage();
	if (pImg)
	{
		nRow = pImg->m_ImgInfo.nRows;
		nCol = pImg->m_ImgInfo.nCols;
	}
}

void Vtk2DWnd::SetSliceLinePos(double dSliceThick)
{
	//ui->ImgWnd->GetOperateLine()->SetSliceLinePos(dSliceThick);
}

void Vtk2DWnd::SetNormalMainLine(MoveObject object, int nIndex)
{
	int nRow, nCol;
	GetImageRowAndCol(nRow, nCol);
	MprLine *tLine = new MprLine;
	if (object == A1LINE)
	{
		tLine->pt1.ImgPt.x = 0;
		tLine->pt1.ImgPt.y = nIndex;
		tLine->pt2.ImgPt.x = nCol;
		tLine->pt2.ImgPt.y = nIndex;
	}

	if (object == A2LINE)
	{
		tLine->pt1.ImgPt.x = nIndex;
		tLine->pt1.ImgPt.y = 0;
		tLine->pt2.ImgPt.x = nIndex;
		tLine->pt2.ImgPt.y = nRow;
	}
	ui->ImgWnd->GetOperateLine()->SetManiLinePos(object, tLine);
	update();
	delete tLine;
}

void Vtk2DWnd::CenterPointChanged()
{
	ui->ImgWnd->GetOperateLine()->OnCenterPointChanged();


	update();
}

void Vtk2DWnd::ReDrawImage()
{
	QString sWndName = objectName();
	int nValue = ui->ScrollBar->value();
	ui->ImgWnd->CalcAndShowImg(sWndName, m_nSliceNum);
}

void Vtk2DWnd::ReCalculateImage()
{
	ui->ImgWnd->CalcAndShowImg();
}

void Vtk2DWnd::OnScrollBarMoved(int nValue)
{
	QString sWndName = sender()->parent()->objectName();
	ui->ImgWnd->SetOrthogonalIndex(nValue);
	ui->ImgWnd->CalcAndShowImg(sWndName, m_nSliceNum);	
	emit ScrollBarChange(sWndName, nValue);
}

void Vtk2DWnd::RecieveImageNum(int nImgNum)
{
	m_nImageNum = nImgNum;
	ui->ScrollBar->setRange(0, m_nImageNum);
	ui->ScrollBar->setValue(m_nImageNum / 2);
}

void Vtk2DWnd::RecieveImageIndexChange(int nChange)
{
	int nCurIndex = ui->ScrollBar->value() + nChange;
	ui->ScrollBar->setValue(nCurIndex);
	ui->ImgWnd->SetOrthogonalIndex(nCurIndex);
}

void Vtk2DWnd::OnActiveMprOblique(bool isActive)
{
	QRect rc = ui->ImgWnd->geometry();
	if (ui->ImgWnd->GetOperateLine() == nullptr)
		return;

	if (isActive)
	{
		ui->ImgWnd->GetOperateLine()->ActiveOblique();
		ui->ScrollBar->setVisible(false);
		ui->ImgWnd->setGeometry(rect());
	}
	else
	{
		ui->ImgWnd->GetOperateLine()->DisactiveOblique();
		ui->ScrollBar->setVisible(true);
		QRect rc = ui->ScrollBar->rect();
		QRect rcImg = QRect(rect().left(), rect().top(), rect().width() - rc.width(), rect().height());
		ui->ImgWnd->setGeometry(rcImg);
	}
}
