#include "stdafx.h"
#include "HsNormalMprMaker.h"
#include "HsImage.h"

extern void** ArrayNew(unsigned long nRow, unsigned long nCol, unsigned long nSize, unsigned long *nNewRow = NULL, unsigned long* nNewCol = NULL);
extern void ArrayFree(void **pArr, int iflag = 0);

CHsNormalMprMaker::CHsNormalMprMaker()
	:m_nMprMode(0)
{
}


CHsNormalMprMaker::~CHsNormalMprMaker()
{
}

void CHsNormalMprMaker::InitParaAndData(vector<CHsImage*> vImg, void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, int nWndType, int nOriType)
{
	m_vOriImage = vImg;
	m_p3DImgArray = pImgArray;
	m_p3DImgData = p3Ddata;
	m_nImgWndType = nWndType;
	m_nOriImgType = nOriType;
}

void CHsNormalMprMaker::GetStartAndEndSlice(int iIndex, int &iSliceNum, int &iStartS, int &iEndS)
{
	int nExtents[6];
	m_p3DImgData->GetExtent(nExtents);

	int nMaxSlice = 0;
	if (m_nImgWndType == ORIIMG_AXIAL)
		nMaxSlice = nExtents[5];
	else if (m_nImgWndType == ORIIMG_CORONAL)
		nMaxSlice = nExtents[1];
	else
		nMaxSlice = nExtents[3];

	if (iSliceNum <= 1)
	{
		iStartS = iEndS = iIndex;
	}
	else
	{
		if (iIndex == 0)
		{
			iStartS = 0;
			iEndS = iSliceNum;

			if (iEndS > nMaxSlice - 1)
				iEndS = nMaxSlice;
		}
		else if (iIndex == nMaxSlice)
		{
			iStartS = nMaxSlice - iSliceNum;
			iEndS = iEndS = nMaxSlice;

			if (iStartS < 0)
				iStartS = 0;
		}
		else
		{
			if (iSliceNum % 2 == 0)
			{
				iStartS = iIndex - iSliceNum / 2;
				iEndS = iIndex + iSliceNum / 2 - 1;
			}
			else
			{
				iStartS = iIndex - iSliceNum / 2;
				iEndS = iIndex + iSliceNum / 2;
			}
			if (iStartS < 0)
				iStartS = 0;
			if (iEndS > nMaxSlice)
				iEndS = nMaxSlice;
		}
	}

	iSliceNum = iEndS - iStartS + 1;
}

void CHsNormalMprMaker::GetShowImage(CHsImage *pShowImg, int iIndex, int iSliceNum)
{
	int nExtents[6];
	double dSpacing[3];
	m_p3DImgData->GetExtent(nExtents);
	m_p3DImgData->GetSpacing(dSpacing);

	int nOriRows = nExtents[3] + 1, nOriCols = nExtents[1] + 1, nOriSlice = nExtents[5] + 1;
	int nSize = m_vOriImage[0]->m_ImgInfo.nBitsAllocated / 8;

	int iStratS = 0, iEndS = 0, nRows = 0, nCols = 0;

	GetStartAndEndSlice(iIndex,iSliceNum,iStratS, iEndS);
	
	double dSpacingX = 0.00, dSpacingY = 0.00;

	if (m_nImgWndType == ORIIMG_AXIAL)
	{
		nRows = nOriRows;
		nCols = nOriCols;

		dSpacingX = dSpacing[0];
		dSpacingY = dSpacing[1];

		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);

			for (int s=iStratS; s<=iEndS; s++)
			{
				BYTE **pSlice = pData[s];
				for (int r=0; r<nRows; r++)
				{
					BYTE *pRows = pSlice[r];
					for (int c=0; c<nCols; c++)
					{
						int iValue = pRows[c];
						pRetData[r][c] = iValue;
					}
				}
			}

			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
		else if (nSize == 2)
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);			

			for (int s = iStratS; s <= iEndS; s++)
			{
				short **pSlice = pData[s];
				for (int r = 0; r < nRows; r++)
				{
					short *pRows = pSlice[r];
					for (int c = 0; c < nCols; c++)
					{
						int iValue = pRows[c];
						pRetData[r][c] = iValue;
					}
				}
			}

			//for (int r = 0; r < nRows; r++)
			//{
			//	for (int c = 0; c < nCols; c++)
			//	{
			//		vector<int> vPixelValues;
			//		for (int s = iStratS; s <= iEndS; s++)
			//		{
			//			int iValue = pData[s][r][c];
			//			//int iValue = pData[nOriRows*nOriCols*s + nOriCols *r + c];
			//			vPixelValues.push_back(iValue);
			//		}
			//		if (iSliceNum > 1)
			//		{
			//			if (m_nMprMode == 0)
			//			{
			//				vector<int>::iterator maxValue = max_element(begin(vPixelValues), end(vPixelValues));
			//				pRetData[r][c] = *maxValue;
			//			}
			//			else if (m_nMprMode == 1)
			//			{
			//				double sum = accumulate(begin(vPixelValues), end(vPixelValues), 0.0);
			//				int mean = sum / vPixelValues.size();
			//				pRetData[r][c] = mean;
			//			}
			//		}
			//		else
			//			pRetData[r][c] = vPixelValues[0];
			//	}
			//}
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	else if (m_nImgWndType == ORIIMG_CORONAL)
	{
		nRows = nOriSlice;
		nCols = nOriCols;

		dSpacingX = dSpacing[0];
		dSpacingY = dSpacing[2];

		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);

			for (int r = 0; r < nRows; r++)
			{
				BYTE **pSlice = pData[nRows - 1 - r];
				for (int s = iStratS; s <= iEndS; s++)
				{
					BYTE *pRows = pSlice[s];
					for (int c = 0; c < nCols; c++)
					{
						int iValue = pRows[c];
						pRetData[r][c] = iValue;
					}
				}
			}

			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
		else
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);

			for (int r = 0; r < nRows; r++)
			{
				short **pSlice = pData[nRows - 1 - r];
				for (int s = iStratS; s <= iEndS; s++)
				{
					short *pRows = pSlice[s];
					for (int c = 0; c < nCols; c++)
					{
						int iValue = pRows[c];
						pRetData[r][c] = iValue;
					}
				}
			}

			//for (int r = 0; r < nRows; r++)
			//{
			//	for (int c = 0; c < nCols; c++)
			//	{
			//		vector<int> vPixelValues;
			//		for (int s = iStratS; s <= iEndS; s++)
			//		{
			//			int iValue = pData[nRows - 1 - r][s][c];
			//			//int iValue = pData[nOriRows*nOriCols*(nRows - 1 - r) + nOriCols*s + c];
			//			vPixelValues.push_back(iValue);
			//		}
			//		if (m_nMprMode == 0)
			//		{
			//			vector<int>::iterator maxValue = max_element(begin(vPixelValues), end(vPixelValues));
			//			pRetData[r][c] = *maxValue;
			//		}
			//		else if (m_nMprMode == 1)
			//		{
			//			double sum = accumulate(begin(vPixelValues), end(vPixelValues), 0.0);
			//			int mean = sum / vPixelValues.size();
			//			pRetData[r][c] = mean;
			//		}
			//	}
			//}
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	else
	{
		nRows = nOriSlice;
		nCols = nOriRows;

		dSpacingX = dSpacing[1];
		dSpacingY = dSpacing[2];

		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);
			for (int r = 0; r < nRows; r++)
			{
				BYTE **pSlice = pData[nRows - 1 - r];
				for (int c = 0; c < nCols; c++)
				{
					BYTE *pRows = pSlice[nCols-1-c];
					for (int s = iStratS; s <= iEndS; s++)
					{
						int iValue = pRows[s];
						pRetData[r][c] = iValue;
					}
				}
			}
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
		else
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);

			for (int r = 0; r < nRows; r++)
			{
				short **pSlice = pData[nRows-1-r];
				for (int c = 0; c < nCols; c++)
				{
					short *pRows = pSlice[nCols-1-c];
					for (int s = iStratS; s <= iEndS; s++)
					{
						int iValue = pRows[s];
						pRetData[r][c] = iValue;
					}
				}
			}

			//for (int r = 0; r < nRows; r++)
			//{
			//	for (int c = 0; c < nCols; c++)
			//	{
			//		vector<int> vPixelValues;
			//		for (int s = iStratS; s <= iEndS; s++)
			//		{
			//			int iValue = pData[nRows - 1 - r][nCols - 1 - c][s];
			//			//int iValue = pData[nOriRows*nOriCols*(nRows - 1 - r) + nOriCols*(nCols - 1 -c) + s];
			//			vPixelValues.push_back(iValue);
			//		}
			//		if (m_nMprMode == 0)
			//		{
			//			vector<int>::iterator maxValue = max_element(begin(vPixelValues), end(vPixelValues));
			//			pRetData[r][c] = *maxValue;
			//		}
			//		else if (m_nMprMode == 1)
			//		{
			//			double sum = accumulate(begin(vPixelValues), end(vPixelValues), 0.0);
			//			int mean = sum / vPixelValues.size();
			//			pRetData[r][c] = mean;
			//		}
			//	}
			//}
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	
	pShowImg->m_ImgInfo.nPixelRepresentation = 1;
	DoInterpolationToImage(pShowImg, dSpacingX, dSpacingY, nRows,nCols);	
	return;
}



void CHsNormalMprMaker::DoInterpolationToImage(CHsImage *pImg, double dSpacingX, double dSpacingY, int nRows, int nCols)
{
	double dNewSpacingY = (dSpacingX + dSpacingY) / 2;


	long nCurW, nCurC;
	nCurW = pImg->m_ImgState.CurWc.x;
	nCurC = pImg->m_ImgState.CurWc.y;

	unsigned long nHeight = 0;
	unsigned long nWidth = 0;
	if (abs(dNewSpacingY - dSpacingY) > 0.000001)
	{
		int nNewRow = int((nRows * dSpacingY) / dNewSpacingY);

		int nSize = m_vOriImage[0]->m_ImgInfo.nBitsAllocated / 8;

		unsigned long tRow, tCol;
		BYTE** pDataSized = (BYTE**)ArrayNew(nNewRow, nCols, nSize, &tRow, &tCol);//开辟空间，准备接收插值后的像素

		unsigned long x0 = 0;
		unsigned long y0 = 0;
		unsigned long x1 = nCols;
		unsigned long y1 = nRows;

		nHeight = tRow;
		nWidth = tCol;
		
		if (nSize == 1)
		{
			ImgSizeOneSampleResample((BYTE**)pDataSized, nHeight, nWidth, (BYTE**)pImg->m_pOriData, nRows, nCols, x0, y0, x1, y1);
		}
		else if (nSize == 2)
		{
			ImgSizeOneSampleResample((short **)pDataSized, nHeight, nWidth, (short**)pImg->m_pOriData, nRows, nCols, x0, y0, x1, y1);
		}

		ArrayFree((void**)pImg->m_pOriData);//最初的MPR数据没用了
		pImg->m_pOriData = (BYTE**)pDataSized;
	}
	else
	{
		nHeight = nRows;
		nWidth = nCols;
	}
	pImg->m_ImgInfo = m_vOriImage[0]->m_ImgInfo;//先复制源图基本信息

	//修改其中的具体信息：
	pImg->m_ImgInfo.nRows = nHeight;
	pImg->m_ImgInfo.nCols = nWidth;

	//PixSpacing
	pImg->m_ImgInfo.fPixelSpaceY = dNewSpacingY;
	pImg->m_ImgInfo.ImgLocPara.fPixSpacingY = dNewSpacingY;

	pImg->Hs_InitImgState();

	pImg->m_ImgState.nDispalyRow = nHeight;
	pImg->m_ImgState.nDispalyCol = nWidth;

	pImg->m_ImgState.CurWc.x = nCurW;
	pImg->m_ImgState.CurWc.y = nCurC;

	
}
