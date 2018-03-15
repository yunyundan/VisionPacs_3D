#include "stdafx.h"
#include "WorkZone.h"
#include "ui_WorkZone.h"
#include "HsFile.h"
#include "HsImage.h"
#include "Hmy3dMath.h"
#include "ui_Vtk2DWnd.h"
#include "LoadImageThread.h"
#include "ResliceControl.h"

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
	,m_pImageArray(NULL)
	,m_nDataX(0)
	,m_nDataY(0)
	,m_nDataZ(0)
	,m_bOblique(false)
	,m_pHmyImageData3D(nullptr)
	,m_nLoadImageNum(0)
	,m_pResliceControl(nullptr)
{
    ui->setupUi(this);

	m_v2DWnd.push_back(ui->Axial_Wnd);
	m_v2DWnd.push_back(ui->Coronal_Wnd);
	m_v2DWnd.push_back(ui->Sagittal_Wnd);

	InitSignalsAndSlots();
}

WorkZone::~WorkZone()
{
    delete ui;

	if (m_pImageArray)
	{
		for (int z = 0; z < m_nDataZ; z++)
		{
			BYTE *pSlice = (BYTE *)m_pImageArray[z];
			delete[]pSlice;
		}
		BYTE *pImgArray = (BYTE *)m_pImageArray;
		delete[]pImgArray;
		m_pImageArray = NULL;
	}

	if (m_pHmyImageData3D)
	{
		delete m_pHmyImageData3D;
		m_pHmyImageData3D = nullptr;
	}

	if (m_pResliceControl)
	{
		delete m_pResliceControl;
		m_pResliceControl = nullptr;
	}
}

void WorkZone::ClearImg(bool bIncludeVR)
{

}

void WorkZone::LoadImageData()
{
	if (m_vImage.size() == 1)
	{
		QMessageBox::about(this, "提示", "不能输入1张图像");
		return;
	}

	//先确定本序列属于什么断位
	Hmy3DVector vecRow = Hmy3DVector(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
	Hmy3DVector vecCol = Hmy3DVector(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
	Hmy3DVector vecImageNormal = vecRow.Cross(vecCol);
	double maxOrientation = max(vecImageNormal.x(), max(vecImageNormal.y(), vecImageNormal.z()));
	int nImageNum = m_vImage.size();
	double dMaxSpacingDiff = 0.0;
	if (abs(maxOrientation - vecImageNormal.x()) < 0.00001)
	{
		m_nOriImageType = ORIIMG_SAGITTAL;
		dMaxSpacingDiff = abs(m_vImage[0]->m_ImgInfo.fImagePosition[0] - m_vImage[nImageNum - 1]->m_ImgInfo.fImagePosition[0]);
	}
	else if(abs(maxOrientation - vecImageNormal.y()) < 0.00001)
	{
		m_nOriImageType = ORIIMG_CORONAL;
		dMaxSpacingDiff = abs(m_vImage[0]->m_ImgInfo.fImagePosition[1] - m_vImage[nImageNum - 1]->m_ImgInfo.fImagePosition[1]);
	}
	else if (abs(maxOrientation - vecImageNormal.z()) < 0.00001)
	{
		m_nOriImageType = ORIIMG_AXIAL;
		dMaxSpacingDiff = abs(m_vImage[0]->m_ImgInfo.fImagePosition[2] - m_vImage[nImageNum - 1]->m_ImgInfo.fImagePosition[2]);
	}

	if (dMaxSpacingDiff <= 0)
	{
		QMessageBox::about(this, "提示", "图像层厚不合理");
		return;
	}

	//按照断位给图像排序
	QString sManufactor = "";
	CHsFile *pFile = m_vImage[0]->GetDs();
	pHsElement pManufactor = pFile->Hs_FindFirstEle(m_vImage[0]->m_ImgInfo.pEle, TAG_MANUFACTURER, true);
	if(pManufactor)
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

	double dSpacingX,dSpacingY, dSpacingZ;

	if (m_pImageData == NULL)
		m_pImageData = vtkSmartPointer<vtkImageData>::New();
	else
		m_pImageData->Initialize();

	if (m_nOriImageType == ORIIMG_AXIAL)
	{
		m_nDataX = m_vImage[0]->m_ImgInfo.nCols;
		m_nDataY = m_vImage[0]->m_ImgInfo.nRows;
		m_nDataZ = m_vImage.size();

		dSpacingX = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
		dSpacingY = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
		dSpacingZ = dMaxSpacingDiff / nImageNum;
	}
	else if (m_nOriImageType == ORIIMG_CORONAL)
	{
		m_nDataZ = m_vImage[0]->m_ImgInfo.nRows;
		m_nDataX = m_vImage[0]->m_ImgInfo.nCols;
		m_nDataY = m_vImage.size();

		dSpacingX = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
		dSpacingZ = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
		dSpacingY = dMaxSpacingDiff / nImageNum;

	}
	else if (m_nOriImageType == ORIIMG_SAGITTAL)
	{
		m_nDataZ = m_vImage[0]->m_ImgInfo.nRows;
		m_nDataY = m_vImage[0]->m_ImgInfo.nCols;
		m_nDataX = m_vImage.size();

		dSpacingY = m_vImage[0]->m_ImgInfo.fPixelSpaceX;
		dSpacingZ = m_vImage[0]->m_ImgInfo.fPixelSpaceY;
		dSpacingX = dMaxSpacingDiff / nImageNum;
	}

	m_pImageData->SetDimensions(m_nDataX, m_nDataY, m_nDataZ);
	m_pImageData->SetSpacing(dSpacingX, dSpacingY, dSpacingZ);
	
	double fRescaleIntercept = m_vImage[0]->m_ImgInfo.fRescaleIntercept;
	double fRescaleSlope = 1;
	if (m_vImage[0]->m_ImgInfo.fRescaleSlope != 0.00)
		fRescaleSlope = m_vImage[0]->m_ImgInfo.fRescaleSlope;
	
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

					if (m_nOriImageType == ORIIMG_AXIAL)
					{
						*ptr++ = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_CORONAL)
					{
						*(ptr + (m_nDataX*m_nDataY)*r + (m_nDataX*z) + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_SAGITTAL)
					{
						*(ptr + (m_nDataX*m_nDataY)*r + (m_nDataX*c) +(m_nDataX - z - 1)) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
				}
			}

			if (pImg->m_pOriData)
				ArrayFree((void **)pImg->m_pOriData);
			pImg->m_pOriData = NULL;

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

					if (m_nOriImageType == ORIIMG_AXIAL)
					{
						*ptr++ = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_CORONAL)
					{
						*(ptr + (m_nDataX*m_nDataY)*r + (m_nDataX*z) + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
					else if (m_nOriImageType == ORIIMG_SAGITTAL)
					{
						*(ptr + (m_nDataX*m_nDataY)*(m_nDataZ - 1 - r) + (m_nDataX*(m_nDataY - 1 -c)) + (m_nDataX - 1 - z )) = fRescaleSlope * nPixelValue + fRescaleIntercept;
					}
				}
			}

			if (pImg->m_pOriData)
				ArrayFree((void **)pImg->m_pOriData);
			pImg->m_pOriData = NULL;

			emit SetWaitProgress(35 + z*iStep);
		}		
	}

	//转成三维数组，如果存在先删除
	if (m_pImageArray)
	{
		for (int z=0; z<m_nDataZ;z++)
		{
			for (int y=0; y<m_nDataY; y++)
			{
				long *pRows = (long *)m_pImageArray[z][y];
				delete[]pRows;
			}
			long *pSlice = (long *)m_pImageArray[z];
			delete[]pSlice;
		}
		m_pImageArray = NULL;
	}

	if (m_pImageArray == NULL)
	{
		BYTE *ptr = (BYTE *)m_pImageData->GetScalarPointer();
		long *pArray = new long[m_nDataZ];
		for (int z =0; z<m_nDataZ; z++ )
		{
			int nStartPoint = m_nDataX*m_nDataY*nSize*z;
			long *pRows = new long[m_nDataY];
			for (int y=0; y<m_nDataY; y++)
			{
				pRows[y] = long(&(ptr[nStartPoint + m_nDataX*nSize*y]));
			}
			pArray[z] = long(pRows);
		}
		m_pImageArray = (void***)pArray;
	}

	Hmy3DVector origin(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriLeftTopPixX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriLeftTopPixY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriLeftTopPixZ);
	Hmy3DVector xCosin, yCosin, zCosin;
	if (m_nOriImageType == ORIIMG_AXIAL)
	{
		xCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
		yCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
		zCosin = xCosin.Cross(yCosin);
	}
	else if (m_nOriImageType == ORIIMG_CORONAL)
	{
		xCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
		zCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
		yCosin = xCosin.Cross(zCosin);
	}
	else if (m_nOriImageType == ORIIMG_SAGITTAL)
	{
		yCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
		zCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
		xCosin = yCosin.Cross(zCosin);
	}
	m_pHmyImageData3D = new HmyImageData3D(origin, xCosin, yCosin, zCosin);
	m_pHmyImageData3D->SetSpacing(dSpacingX, dSpacingY, dSpacingZ);
	m_pHmyImageData3D->SetExtent(m_nDataX, m_nDataY, m_nDataZ);
	InitDisplayWnd();
}

void WorkZone::ProcessImageData()
{
	int nSize = m_vImage[0]->m_ImgInfo.nBitsAllocated / 8;
	double dSpacing[3];
	m_pImageData->GetSpacing(dSpacing);
	//转成三维数组，如果存在先删除
	if (m_pImageArray)
	{
		for (int z = 0; z < m_nDataZ; z++)
		{
			for (int y = 0; y < m_nDataY; y++)
			{
				long *pRows = (long *)m_pImageArray[z][y];
				delete[]pRows;
			}
			long *pSlice = (long *)m_pImageArray[z];
			delete[]pSlice;
		}
		m_pImageArray = NULL;
	}

	if (m_pImageArray == NULL)
	{
		BYTE *ptr = (BYTE *)m_pImageData->GetScalarPointer();
		long *pArray = new long[m_nDataZ];
		for (int z = 0; z < m_nDataZ; z++)
		{
			int nStartPoint = m_nDataX*m_nDataY*nSize*z;
			long *pRows = new long[m_nDataY];
			for (int y = 0; y < m_nDataY; y++)
			{
				pRows[y] = long(&(ptr[nStartPoint + m_nDataX*nSize*y]));
			}
			pArray[z] = long(pRows);
		}
		m_pImageArray = (void***)pArray;
	}

	Hmy3DVector origin(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriLeftTopPixX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriLeftTopPixY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriLeftTopPixZ);
	Hmy3DVector xCosin, yCosin, zCosin;
	if (m_nOriImageType == ORIIMG_AXIAL)
	{
		xCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
		yCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
		zCosin = xCosin.Cross(yCosin);
	}
	else if (m_nOriImageType == ORIIMG_CORONAL)
	{
		xCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
		zCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
		yCosin = xCosin.Cross(zCosin);
	}
	else if (m_nOriImageType == ORIIMG_SAGITTAL)
	{
		yCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ);
		zCosin.SetValue(m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosX, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosY, m_vImage[0]->m_ImgInfo.ImgLocPara.fOriFirstColCosZ);
		xCosin = yCosin.Cross(zCosin);
	}
	m_pHmyImageData3D = new HmyImageData3D(origin, xCosin, yCosin, zCosin);
	InitDisplayWnd();
}

void WorkZone::InitDisplayWnd()
{	
	m_pResliceControl = new CResliceControl();
	m_pResliceControl->SetImageData(m_pHmyImageData3D);
	for (int i = 0; i < m_v2DWnd.size(); i++)
	{
		m_v2DWnd[i]->SetImageData(m_pImageArray,m_pImageData,m_vImage,m_pHmyImageData3D, m_pResliceControl);
		m_v2DWnd[i]->InitShowImage();
	}
	ui->VR_Wnd->SetSourceDs(m_vImage[0]->GetDs());
	ui->VR_Wnd->SetImageDateAndShow(m_pImageData);
	emit SetWaitProgress(100);
}

void WorkZone::InitSignalsAndSlots()
{
	for (int i=0; i<m_v2DWnd.size();i++)
	{
		connect(this, SIGNAL(ActiveMprOblique(bool)), m_v2DWnd[i], SLOT(OnActiveMprOblique(bool)));
		connect(m_v2DWnd[i]->GetImageWnd(), SIGNAL(ImageThickChange(double)), this, SLOT(OnEditSTChange(double)));
		connect(m_v2DWnd[i], SIGNAL(ScrollBarChange(QString, int)), this, SLOT(OnScrollBarChange(QString, int)));
	}
}

void WorkZone::Btn_VrOrientationClick()
{
	QString sOreientationName = sender()->objectName();
	ui->VR_Wnd->SetupCamera(sOreientationName);
	ReRenderVrWnd();
}

void WorkZone::CB_VrModeChanged(QString sModeName)
{
	ui->VR_Wnd->Set3DVRmode(sModeName);
	ReRenderVrWnd();
}

void WorkZone::Btn_SetMprState(int nBtnID)
{
	if ( nBtnID == 0)
	{
		m_bOblique = false;
		emit ActiveMprOblique(false);
	}
	else if (nBtnID == 1)
	{
		m_bOblique = true;
		emit ActiveMprOblique(true);
	}
}

void WorkZone::OnMprLinesInfoChange(MprLinesInfo info)
{
	if (info.nWndType == ORIIMG_AXIAL)
	{
		if (info.moveObject == CENTERPOINT)
		{
			ui->Coronal_Wnd->CenterPointChanged();
			ui->Coronal_Wnd->ReCalculateImage();
			ui->Sagittal_Wnd->CenterPointChanged();
			ui->Sagittal_Wnd->ReCalculateImage();
		}
	}
	else if(info.nWndType == ORIIMG_CORONAL)
	{
		if (info.moveObject == CENTERPOINT)
		{
			ui->Axial_Wnd->CenterPointChanged();
			ui->Sagittal_Wnd->CenterPointChanged();
		}
	}
	else if(info.nWndType == ORIIMG_SAGITTAL)
	{
		if (info.moveObject == CENTERPOINT)
		{
			ui->Axial_Wnd->CenterPointChanged();
			ui->Coronal_Wnd->CenterPointChanged();
		}
	}
	/*if (m_bOblique == false)
	{
		if (info.nWndType == ORIIMG_AXIAL)
		{
			ui->Coronal_Wnd->SetSliceLinePos(dSliceThick);
			ui->Coronal_Wnd->SetNormalMainLine(A2LINE, info.ptV1, info.ptV2);
			ui->Sagittal_Wnd->SetSliceLinePos(dSliceThick);
			ui->Sagittal_Wnd->SetNormalMainLine(A2LINE, info.ptH1, info.ptH2);
			ui->Axial_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			if (m_nOriImageType == ORIIMG_CORONAL)
			{				
				y = (y*m_vImage.size()) / nOriRow;
			}			
			else if (m_nOriImageType == ORIIMG_SAGITTAL)
			{
				x = (x*m_vImage.size()) / nOriCols;
			}
			ui->Axial_Wnd->SetImageSlice(dSliceThick);
			ui->Coronal_Wnd->SetImageSlice(dSliceThick);
			ui->Coronal_Wnd->SetImageIndex(y);
			ui->Sagittal_Wnd->SetImageSlice(dSliceThick);
			ui->Sagittal_Wnd->SetImageIndex(x);
		}
		else if (info.nWndType == ORIIMG_CORONAL)
		{
			ui->Axial_Wnd->SetSliceLinePos(dSliceThick);
			ui->Axial_Wnd->SetNormalMainLine(A2LINE, x);
			ui->Sagittal_Wnd->SetSliceLinePos(dSliceThick);
			ui->Sagittal_Wnd->SetNormalMainLine(A1LINE, y);
			ui->Coronal_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			if (m_nOriImageType == ORIIMG_AXIAL)
			{				
				y = (y*m_vImage.size()) / nOriRow;
			}
			else if (m_nOriImageType == ORIIMG_SAGITTAL)
			{
				x = (x*m_vImage.size()) / nOriCols;
			}
			ui->Axial_Wnd->SetImageSlice(dSliceThick);
			ui->Axial_Wnd->SetImageIndex(y);
			ui->Coronal_Wnd->SetImageSlice(dSliceThick);
			ui->Sagittal_Wnd->SetImageSlice(dSliceThick);
			ui->Sagittal_Wnd->SetImageIndex(x);

		}
		else if (info.nWndType == ORIIMG_SAGITTAL)
		{
			ui->Axial_Wnd->SetSliceLinePos(dSliceThick);
			ui->Axial_Wnd->SetNormalMainLine(A1LINE, x);
			ui->Coronal_Wnd->SetSliceLinePos(dSliceThick);
			ui->Coronal_Wnd->SetNormalMainLine(A1LINE, y);
			ui->Sagittal_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			if (m_nOriImageType == ORIIMG_AXIAL)
			{				
				y = (y*m_vImage.size()) / nOriRow;
			}
			else if (m_nOriImageType == ORIIMG_CORONAL)
			{
				x = (x*m_vImage.size()) / nOriCols;
			}
			ui->Axial_Wnd->SetImageSlice(dSliceThick);
			ui->Axial_Wnd->SetImageIndex(y);
			ui->Coronal_Wnd->SetImageSlice(dSliceThick);
			ui->Coronal_Wnd->SetImageIndex(x);
			ui->Sagittal_Wnd->SetImageSlice(dSliceThick);
		}

		for (int i=0; i<m_v2DWnd.size(); i++)
		{
			m_v2DWnd[i]->ReDrawImage();
		}
	}
	else
	{

	}*/

}

void WorkZone::CB_MprModeChanged(QString sModeName)
{
	for (int i = 0; i<m_v2DWnd.size();i++)
	{
		m_v2DWnd[i]->GetImageWnd()->SetMprMode(sModeName);
		m_v2DWnd[i]->ReDrawImage();
	}
}

void WorkZone::OnEditSTChange(double dSliceThick)
{
	for (int i = 0; i < m_v2DWnd.size(); i++)
	{
		m_v2DWnd[i]->SetSliceLinePos(dSliceThick);
		m_v2DWnd[i]->SetImageSlice(dSliceThick);
		m_v2DWnd[i]->ReDrawImage();
	}
}

void WorkZone::OnScrollBarChange(QString sWndName, int nValue)
{
	for (int i=0; i<m_v2DWnd.size();i++)
	{
		if (m_v2DWnd[i]->GetImageWnd()->GetImage() == NULL)
			return;
	}

	int nOriRow, nOriCols;
	int nNewValue = nValue;
	
	if (sWndName.compare("Axial_Wnd")==0)
	{
		if (m_nOriImageType == ORIIMG_AXIAL)
		{
			ui->Coronal_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			nNewValue = (nValue*nOriRow) / (m_vImage.size()-1);
			ui->Coronal_Wnd->SetNormalMainLine(A1LINE, nNewValue);
			ui->Sagittal_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			nNewValue = (nValue*nOriRow) / (m_vImage.size()-1);
			ui->Sagittal_Wnd->SetNormalMainLine(A1LINE, nNewValue);
		}
		else
		{
			ui->Coronal_Wnd->SetNormalMainLine(A1LINE, nValue);
			ui->Sagittal_Wnd->SetNormalMainLine(A1LINE, nValue);
		}
	}
	else if (sWndName.compare("Coronal_Wnd") == 0)
	{
		if (m_nOriImageType == ORIIMG_CORONAL)
		{
			ui->Axial_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			nNewValue = (nValue*nOriRow) / (m_vImage.size() - 1);
			ui->Axial_Wnd->SetNormalMainLine(A1LINE, nNewValue);
			ui->Sagittal_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			nNewValue = (nValue*nOriCols) / (m_vImage.size() - 1);
			ui->Sagittal_Wnd->SetNormalMainLine(A2LINE, nNewValue);
		}
		else
		{
			ui->Axial_Wnd->SetNormalMainLine(A1LINE, nValue);
			ui->Sagittal_Wnd->SetNormalMainLine(A2LINE, nValue);
		}
	}
	else if (sWndName.compare("Sagittal_Wnd") == 0)
	{
		if (m_nOriImageType == ORIIMG_SAGITTAL)
		{
			ui->Axial_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			nNewValue = (nValue*nOriCols) / (m_vImage.size() - 1);
			ui->Axial_Wnd->SetNormalMainLine(A2LINE, nNewValue);
			ui->Coronal_Wnd->GetImageRowAndCol(nOriRow, nOriCols);
			nNewValue = (nValue*nOriCols) / (m_vImage.size() - 1);
			ui->Coronal_Wnd->SetNormalMainLine(A2LINE, nNewValue);
		}
		else
		{
			ui->Axial_Wnd->SetNormalMainLine(A2LINE, nValue);
			ui->Coronal_Wnd->SetNormalMainLine(A2LINE, nValue);
		}
	}
	
}

void WorkZone::OnLoadImageFinished()
{
	int nImageNum = m_vImage.size();
	double iStep = 60.00 / nImageNum;
	m_nLoadImageNum++;
	emit SetWaitProgress(35 + m_nLoadImageNum*iStep);
	if (m_nLoadImageNum == nImageNum)
		ProcessImageData();
}

void WorkZone::ReRenderVrWnd()
{
	ui->VR_Wnd->ReRender();
}

void WorkZone::VrOperteChange(QString sOperateName)
{
	ui->VR_Wnd->ChangeOperate(sOperateName);
}

void WorkZone::ImgOperteChange(QString sOperateName)
{
	for (int i = 0; i < m_v2DWnd.size(); i++)
	{
		m_v2DWnd[i]->ChangeOperate(sOperateName);
	}
}

void WorkZone::InitVolumePropertyWidget(ctkVTKVolumePropertyWidget *pCtkVPwidget)
{
	ui->VR_Wnd->ConnectVolumeToWidget(pCtkVPwidget);
}

