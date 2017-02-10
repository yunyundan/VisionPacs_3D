#include "stdafx.h"
#include "WorkZone.h"
#include "ui_WorkZone.h"
#include "HsFile.h"
#include "HsImage.h"
#include "ui_Vtk2DWnd.h"

extern void ArrayFree(void **pArr, int iflag = 0);

bool SortImgByImgPos(const CHsImage *p1, const CHsImage *p2)
{//在比较函数中，相等的要返回false(从右到左为X正向,从前到后为Y正向,从下向上为Z正向)
	if (p1 == NULL || p2 == NULL)
		return true;

	if (abs(p1->m_ImgInfo.fImagePosition[0] - p2->m_ImgInfo.fImagePosition[0]) < 0.000001 && abs(p1->m_ImgInfo.fImagePosition[1] - p2->m_ImgInfo.fImagePosition[1]) < 0.000001)//X与Y相同时
	{//Z(从脚到头为正向)大的放前面
		if (p1->m_ImgInfo.fImagePosition[2] > p2->m_ImgInfo.fImagePosition[2])
			return true;
		else if (abs(p1->m_ImgInfo.fImagePosition[2] - p2->m_ImgInfo.fImagePosition[2]) < 0.000001)
			return false;
		else
			return false;
	}
	else if (abs(p1->m_ImgInfo.fImagePosition[0] - p2->m_ImgInfo.fImagePosition[0]) < 0.000001 && abs(p1->m_ImgInfo.fImagePosition[2] - p2->m_ImgInfo.fImagePosition[2]) < 0.000001)//X与Z相同时
	{//Y从前到后--小的放前面
		if (p1->m_ImgInfo.fImagePosition[1] < p2->m_ImgInfo.fImagePosition[1])
			return true;
		else
			return false;
	}
	else if (abs(p1->m_ImgInfo.fImagePosition[1] - p2->m_ImgInfo.fImagePosition[1]) < 0.000001 && abs(p1->m_ImgInfo.fImagePosition[2] - p2->m_ImgInfo.fImagePosition[2]) < 0.000001)//X与Z相同时
	{//X从右到左为正向--无所谓-小的放前面吧
		if (p1->m_ImgInfo.fImagePosition[0] < p2->m_ImgInfo.fImagePosition[0])
			return true;
		else
			return false;
	}
	else//加起来比大小
	{
		double f1 = p1->m_ImgInfo.fImagePosition[0] + p1->m_ImgInfo.fImagePosition[1] + p1->m_ImgInfo.fImagePosition[2];
		double f2 = p2->m_ImgInfo.fImagePosition[0] + p2->m_ImgInfo.fImagePosition[1] + p2->m_ImgInfo.fImagePosition[2];

		if (f1 < f2)
			return true;

		return false;
	}
}

bool SortImgByImgSliceLoc(const CHsImage *p1, const CHsImage *p2)
{//根据TAG:00201041排列，在读入时已经放入ImgLoc.fSliceLoc里面
	if (p1 == NULL || p2 == NULL)
		return true;

	if (p1->m_ImgInfo.ImgLocPara.fSliceLoction < p2->m_ImgInfo.ImgLocPara.fSliceLoction)
		return false;
	else
		return true;
}

bool SortPhilipsImgByImgPos(const CHsImage *p1, const CHsImage *p2)
{//在比较函数中，相等的要返回false(从右到左为X正向,从前到后为Y正向,从下向上为Z正向)
	if (p1 == NULL || p2 == NULL)
		return true;

	if (abs(p1->m_ImgInfo.fImagePosition[0] - p2->m_ImgInfo.fImagePosition[0]) < 0.000001 && abs(p1->m_ImgInfo.fImagePosition[1] - p2->m_ImgInfo.fImagePosition[1]) < 0.000001)//X与Y相同时
	{//Z(从脚到头为正向)大的放前面
		if (p1->m_ImgInfo.fImagePosition[2] > p2->m_ImgInfo.fImagePosition[2])
			return false;
		else if (abs(p1->m_ImgInfo.fImagePosition[2] - p2->m_ImgInfo.fImagePosition[2]) < 0.000001)
			return true;
		else
			return true;
	}
	else if (abs(p1->m_ImgInfo.fImagePosition[0] - p2->m_ImgInfo.fImagePosition[0]) < 0.000001 && abs(p1->m_ImgInfo.fImagePosition[2] - p2->m_ImgInfo.fImagePosition[2]) < 0.000001)//X与Z相同时
	{//Y从前到后--小的放前面
		if (p1->m_ImgInfo.fImagePosition[1] < p2->m_ImgInfo.fImagePosition[1])
			return false;
		else
			return true;
	}
	else if (abs(p1->m_ImgInfo.fImagePosition[1] - p2->m_ImgInfo.fImagePosition[1]) < 0.000001 && abs(p1->m_ImgInfo.fImagePosition[2] - p2->m_ImgInfo.fImagePosition[2]) < 0.000001)//X与Z相同时
	{//X从右到左为正向--无所谓-小的放前面吧
		if (p1->m_ImgInfo.fImagePosition[0] < p2->m_ImgInfo.fImagePosition[0])
			return false;
		else
			return true;
	}
	else//加起来比大小
	{
		double f1 = p1->m_ImgInfo.fImagePosition[0] + p1->m_ImgInfo.fImagePosition[1] + p1->m_ImgInfo.fImagePosition[2];
		double f2 = p2->m_ImgInfo.fImagePosition[0] + p2->m_ImgInfo.fImagePosition[1] + p2->m_ImgInfo.fImagePosition[2];

		if (f1 < f2)
			return false;

		return true;
	}
}

WorkZone::WorkZone(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkZone)
	, m_pImageData(NULL)
{
    ui->setupUi(this);

	m_v2DWnd.push_back(ui->Axial_Wnd);
	m_v2DWnd.push_back(ui->Coronal_Wnd);
	m_v2DWnd.push_back(ui->Sagittal_Wnd);
}

WorkZone::~WorkZone()
{
    delete ui;
}

void WorkZone::showImg(CHsImage *pImg)
{
	ui->Axial_Wnd->showImage(pImg);
}

void WorkZone::ClearImg(bool bIncludeVR)
{

}

void WorkZone::ProcessImageData()
{
	if (m_vImage.size() == 1)
	{
		QMessageBox::about(this, "提示", "不能输入1张图像");
		return;
	}
	//先确定本序列属于什么断位
	int nImageNum = m_vImage.size();
	double dTotalSpacingX = abs(m_vImage[0]->m_ImgInfo.fImagePosition[0] - m_vImage[nImageNum-1]->m_ImgInfo.fImagePosition[0]);
	double dTotalSpacingY = abs(m_vImage[0]->m_ImgInfo.fImagePosition[1] - m_vImage[nImageNum-1]->m_ImgInfo.fImagePosition[1]);
	double dTotalSpacingZ = abs(m_vImage[0]->m_ImgInfo.fImagePosition[2] - m_vImage[nImageNum-1]->m_ImgInfo.fImagePosition[2]);
	double dMaxSpacingDiff = max(dTotalSpacingX,max(dTotalSpacingY,dTotalSpacingZ));

	if (dMaxSpacingDiff <= 0)
	{
		QMessageBox::about(this, "提示", "图像层厚不合理");
		return;
	}

	if (abs(dMaxSpacingDiff - dTotalSpacingX) < 0.00001)
		m_nOriImageType = ORIIMG_SAGGITAL;
	else if (abs(dMaxSpacingDiff - dTotalSpacingY) < 0.00001)
		m_nOriImageType = ORIIMG_CORONAL;
	else
		m_nOriImageType = ORIIMG_AXIAL;

	//按照断位给图像排序
	QString sManufactor = "";
	CHsFile *pFile = m_vImage[0]->GetDs();
	pHsElement pManufactor = pFile->Hs_FindFirstEle(m_vImage[0]->m_ImgInfo.pEle, TAG_MANUFACTURER, true);
	pFile->Hs_GetStringValue(pManufactor, sManufactor, 0);

	if (sManufactor.indexOf("Philips") != -1 && m_vImage[0]->m_ImgInfo.sModality.compare("MR") == 0)
	{
		std::sort(m_vImage.begin(), m_vImage.end(), SortPhilipsImgByImgPos);
	}
	else if (m_vImage[0]->m_ImgInfo.sModality.compare("MR") == 0)
	{
		if (m_vImage[0]->m_ImgInfo.ImgLocPara.fSliceLoction != 0.00)
			std::sort(m_vImage.begin(), m_vImage.end(), SortImgByImgSliceLoc);
		else
			std::sort(m_vImage.begin(), m_vImage.end(), SortImgByImgPos);
	}
	else
		std::sort(m_vImage.begin(), m_vImage.end(), SortImgByImgPos);
	
	int nDataX,nDataY,nDataZ;
	double dSpacingX,dSpacingY, dSpacingZ;

	nDataX = m_vImage[0]->m_ImgInfo.nCols;
	nDataY = m_vImage[0]->m_ImgInfo.nRows;
	nDataZ = m_vImage.size();

	dSpacingX = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
	dSpacingY = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
	dSpacingZ = dMaxSpacingDiff/nImageNum;

	/*if (m_nOriImageType == ORIIMG_AXIAL)
	{
		nDataX = m_vImage[0]->m_ImgInfo.nCols;
		nDataY = m_vImage[0]->m_ImgInfo.nRows; 
		nDataZ = m_vImage.size();

		dSpacingX = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
		dSpacingY = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
		dSpacingZ = dMaxSpacingDiff;
	}
	else if (m_nOriImageType == ORIIMG_CORONAL)
	{
		nDataZ = m_vImage[0]->m_ImgInfo.nRows;
		nDataX = m_vImage[0]->m_ImgInfo.nCols;
		nDataY = m_vImage.size();

		dSpacingX = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
		dSpacingZ = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
		dSpacingY = dMaxSpacingDiff;
	}
	else if (m_nOriImageType == ORIIMG_SAGGITAL)
	{
		nDataZ = m_vImage[0]->m_ImgInfo.nRows;
		nDataY = m_vImage[0]->m_ImgInfo.nCols;
		nDataX = m_vImage.size();

		dSpacingY = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
		dSpacingZ = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
		dSpacingX = dMaxSpacingDiff;
	}*/

	if (m_pImageData == NULL)
		m_pImageData = vtkSmartPointer<vtkImageData>::New();
	else
		m_pImageData->Initialize();

	double fRescaleIntercept = m_vImage[0]->m_ImgInfo.fRescaleIntercept;
	double fRescaleSlope = m_vImage[0]->m_ImgInfo.fRescaleSlope;

	m_pImageData->SetDimensions(nDataX,nDataY,nDataZ);
	m_pImageData->SetSpacing(dSpacingX,dSpacingY,dSpacingZ);

	int nSize = m_vImage[0]->m_ImgInfo.nBitsAllocated/8;
	int nRows = m_vImage[0]->m_ImgInfo.nRows;
	int nCols = m_vImage[0]->m_ImgInfo.nCols;

	double iStep = 60.00 / nImageNum;
	if (nSize == 1)
	{
		m_pImageData->AllocateScalars(VTK_CHAR,1);

		char *ptr = (char *)m_pImageData->GetScalarPointer();
		for (int z = 0; z<nImageNum; z++)
		{
			CHsImage *pImg = m_vImage[z];
			if (pImg->Hs_IsEmpty())
			{
				CHsFile *pDs = pImg->GetDs();
				pDs->Hs_GetImage(pImg->m_ImgInfo.pEle, *pImg, pImg->m_ImgInfo.iFrame);
				pImg->Hs_FilpOridata();
			}

			int nPixelValue = 0;
			char **pOriData = (char**)(pImg->m_pOriData);

			for (int r = 0; r<nRows; r++)
			{
				for (int c = 0; c<nCols; c++)
				{
					nPixelValue = pOriData[r][c];

					*ptr++ = fRescaleSlope * nPixelValue + fRescaleIntercept;

					/*if (m_nOriImageType == ORIIMG_AXIAL)
					{
					*ptr++ = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_CORONAL)
					{
					*(ptr + (nDataX*nDataY)*r + (nDataX*z) + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_SAGGITAL)
					{
					*(ptr + (nDataX*nDataY)*r + (nDataX*c) + z) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}*/
				}
			}

			if (pImg->m_pOriData)
				ArrayFree((void **)pImg->m_pOriData);
			pImg->m_pOriData = NULL;

			BYTE *pData = (BYTE *)m_pImageData->GetScalarPointer();
			LONG *pAdd = new LONG[nRows];

			for (int i = 0; i < nRows; i++)
				pAdd[i] = long(&(pData[nRows*nCols*z*nSize + i*nCols*nSize]));

			pImg->m_pOriData = (BYTE**)pAdd;
			pImg->m_ImgInfo.nPixelRepresentation = 1;

			emit SetWaitProgress(35 + z*iStep);
		}
	}
	else if (nSize == 2)
	{
		int nPixelRepresentation = m_vImage[0]->m_ImgInfo.nPixelRepresentation;
		m_pImageData->AllocateScalars(VTK_SHORT,1);

		short *ptr = (short *)m_pImageData->GetScalarPointer();

		for (int z=0; z<nImageNum; z++)
		{
			CHsImage *pImg = m_vImage[z];
			if (pImg->Hs_IsEmpty())
			{
				CHsFile *pDs = pImg->GetDs();
				pDs->Hs_GetImage(pImg->m_ImgInfo.pEle, *pImg, pImg->m_ImgInfo.iFrame);
				pImg->Hs_FilpOridata();
			}

			short **pSOriData = NULL; unsigned short **pUSOriData = NULL;
			int nPixelValue = 0;
			if (nPixelRepresentation == 1)
				pSOriData = (short**)(pImg->m_pOriData);
			else
				pUSOriData = (unsigned short**)(pImg->m_pOriData);

			for (int r=0; r<nRows; r++)
			{
				for (int c=0; c<nCols;c++)
				{
					if (nPixelRepresentation == 1)
						nPixelValue = pSOriData[r][c];
					else
						nPixelValue = pUSOriData[r][c];

					*ptr++ = fRescaleSlope * nPixelValue + fRescaleIntercept;

					/*if (m_nOriImageType == ORIIMG_AXIAL)
					{
						*ptr++ = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_CORONAL)
					{
						*(ptr + (nDataX*nDataY)*r + (nDataX*z) + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_SAGGITAL)
					{
						*(ptr + (nDataX*nDataY)*r + (nDataX*c) + z) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}*/
				}
			}

			if (pImg->m_pOriData)
				ArrayFree((void **)pImg->m_pOriData);
			pImg->m_pOriData = NULL;

			BYTE *pData = (BYTE *)m_pImageData->GetScalarPointer();
			LONG *pAdd = new LONG[nRows];

			for (int i = 0; i < nRows; i++)
				pAdd[i] = long(&(pData[nRows*nCols*z*nSize + i*nCols*nSize]));

			pImg->m_pOriData = (BYTE**)pAdd;
			pImg->m_ImgInfo.nPixelRepresentation = 1;

			emit SetWaitProgress(35 + z*iStep);
		}		
	}
	InitDisplayWnd();
}

void WorkZone::InitDisplayWnd()
{
	ui->VR_Wnd->SetImageDate(m_pImageData);
	for (int i=0; i<m_v2DWnd.size();i++)
	{
		m_v2DWnd[i]->m_vImg = m_vImage;
		m_v2DWnd[i]->InitShowImage(m_nOriImageType);
	}
	emit SetWaitProgress(100);
}

void WorkZone::Btn_ImgOperateClick()
{
	QString sOperateName = sender()->objectName();
	for (int i = 0; i < m_v2DWnd.size(); i++)
	{
		m_v2DWnd[i]->ChangeOperate(sOperateName);
	}
}

void WorkZone::Btn_VrOperateClick()
{
	QString sOperateName = sender()->objectName();
	ui->VR_Wnd->ChangeOperate(sOperateName);
}

void WorkZone::Btn_VrOrientationClick()
{
	QString sOreientationName = sender()->objectName();
	ui->VR_Wnd->SetupCamera(sOreientationName);
	ReRenderVrWnd();
}


void WorkZone::ReRenderVrWnd()
{
	ui->VR_Wnd->ReRender();
}

