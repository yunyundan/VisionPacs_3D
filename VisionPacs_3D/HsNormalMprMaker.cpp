#include "stdafx.h"
#include "HsNormalMprMaker.h"
#include "HsImage.h"
#include "vector3d.h"

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
	{
		nMaxSlice = nExtents[3];
		iIndex = nMaxSlice - iIndex;
	}		
	else
		nMaxSlice = nExtents[1];

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
		pShowImg->m_fOriSilceThick = dSpacing[2];
		pShowImg->m_fCurSliceThick = iSliceNum * dSpacing[2];

		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[iEndS - iStratS + 1];
			for (int r = 0; r < nRows; r++)
			{
				int nIndex = 0;
				for (int s = iStratS; s <= iEndS; s++)
				{
					BYTE **pSlice = pData[s];
					pRows[nIndex] = long(pSlice[r]);
					nIndex++;
				}
				BYTE **pAllRows = (BYTE**)pRows;
				for (int c = 0; c < nCols; c++)
				{
					if (m_nMprMode == 0)
					{
						int nMax = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nMax = max(nMax, pAllRows[n][c]);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nMprMode == 1)
					{
						int nTotle = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nTotle += pAllRows[n][c];
						}
						pRetData[r][c] = nTotle / nIndex;
					}

				}
			}
			delete pRows;
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
		else if (nSize == 2)
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);			

			long *pRows = new long[iEndS - iStratS + 1];
			for (int r = 0; r < nRows; r++)
			{				
				int nIndex = 0;
				for (int s = iStratS; s <= iEndS; s++)
				{
					short **pSlice = pData[s];
					pRows[nIndex] = long(pSlice[r]);
					nIndex++;
				}
				short **pAllRows = (short**)pRows;
				for (int c = 0; c < nCols; c++)
				{
					if (m_nMprMode == 0)
					{
						int nMax = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nMax = max(nMax, pAllRows[n][c]);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nMprMode == 1)
					{
						int nTotle = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nTotle += pAllRows[n][c];
						}
						pRetData[r][c] = nTotle / nIndex;
					}

				}				
			}
			delete pRows;
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	else if (m_nImgWndType == ORIIMG_CORONAL)
	{
		nRows = nOriSlice;
		nCols = nOriCols;

		dSpacingX = dSpacing[0];
		dSpacingY = dSpacing[2];
		pShowImg->m_fOriSilceThick = dSpacing[1];
		pShowImg->m_fCurSliceThick = iSliceNum * dSpacing[1];

		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[iEndS - iStratS + 1];
			for (int r = 0; r < nRows; r++)
			{
				BYTE **pSlice = pData[nRows - 1 - r];
				int nIndex = 0;
				for (int s = iStratS; s <= iEndS; s++)
				{
					pRows[nIndex] = long(pSlice[s]);
					nIndex++;
				}
				BYTE **pAllRows = (BYTE**)pRows;
				for (int c = 0; c < nCols; c++)
				{
					if (m_nMprMode == 0)
					{
						int nMax = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nMax = max(nMax, pAllRows[n][c]);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nMprMode == 1)
					{
						int nTotle = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nTotle += pAllRows[n][c];
						}
						pRetData[r][c] = nTotle / nIndex;
					}
				}
			}
			delete pRows;
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
		else
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[iEndS - iStratS + 1];
			for (int r = 0; r < nRows; r++)
			{
				short **pSlice = pData[nRows - 1 - r];
				int nIndex = 0;
				for (int s = iStratS; s <= iEndS; s++)
				{
					pRows[nIndex] = long(pSlice[s]);
					nIndex++;
				}
				short **pAllRows = (short**)pRows;
				for (int c = 0; c < nCols; c++)
				{
					if (m_nMprMode == 0)
					{
						int nMax = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nMax = max(nMax, pAllRows[n][c]);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nMprMode == 1)
					{
						int nTotle = pAllRows[0][c];
						for (int n = 1; n < nIndex; n++)
						{
							nTotle += pAllRows[n][c];
						}
						pRetData[r][c] = nTotle / nIndex;
					}
				}
			}
			delete pRows;
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	else
	{
		nRows = nOriSlice;
		nCols = nOriRows;

		dSpacingX = dSpacing[1];
		dSpacingY = dSpacing[2];
		pShowImg->m_fOriSilceThick = dSpacing[0];
		pShowImg->m_fCurSliceThick = iSliceNum * dSpacing[0];

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

					if (m_nMprMode == 0)
					{
						int nMax = pRows[iStratS];
						for (int s = iStratS+1; s <= iEndS; s++)
						{
							nMax = max(nMax, pRows[s]);							
						}
						pRetData[r][c] = nMax;
					}
					else if(m_nMprMode == 1)
					{
						int nTotle = pRows[iStratS];
						for (int s = iStratS + 1; s <= iEndS; s++)
						{
							nTotle += pRows[s];
						}
						pRetData[r][c] = nTotle/(iEndS-iStratS+1);
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

					if (m_nMprMode == 0)
					{
						int nMax = pRows[iStratS];
						for (int s = iStratS + 1; s <= iEndS; s++)
						{
							nMax = max(nMax, pRows[s]);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nMprMode == 1)
					{
						int nTotle = pRows[iStratS];
						for (int s = iStratS + 1; s <= iEndS; s++)
						{
							nTotle += pRows[s];
						}
						pRetData[r][c] = nTotle / (iEndS - iStratS + 1);
					}
				}
			}
			pShowImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	DoInterpolationToImage(pShowImg, dSpacingX, dSpacingY, nRows,nCols);	
	CalImagePositon(pShowImg,iIndex);
	return;
}



void CHsNormalMprMaker::DoInterpolationToImage(CHsImage *pImg, double dSpacingX, double dSpacingY, int nRows, int nCols)
{
	double dNewSpacingY = dSpacingX;

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

	if (pImg->m_ImgInfo.nPixelRepresentation == 0)
	{
		pImg->m_ImgInfo.nPixelRepresentation = 1;
		pImg->m_bOriPixelRepresentation = true;
	}
	

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

void CHsNormalMprMaker::CalImagePositon(CHsImage *pImg,int nIndex)
{
	if (m_nImgWndType == m_nOriImgType)
	{
		pImg->m_ImgInfo.ImgLocPara = m_vOriImage[nIndex]->m_ImgInfo.ImgLocPara;
		return;
	}

	ImageInfo ImgInfoOri = m_vOriImage[0]->m_ImgInfo;
	v3D::Vector3D oriLeftTop(ImgInfoOri.ImgLocPara.fOriLeftTopPixX, ImgInfoOri.ImgLocPara.fOriLeftTopPixY, ImgInfoOri.ImgLocPara.fOriLeftTopPixZ);
	v3D::Vector3D colvector(ImgInfoOri.ImgLocPara.fOriFirstColCosX, ImgInfoOri.ImgLocPara.fOriFirstColCosY, ImgInfoOri.ImgLocPara.fOriFirstColCosZ);
	v3D::Vector3D rowvector(ImgInfoOri.ImgLocPara.fOriFirstRowCosX, ImgInfoOri.ImgLocPara.fOriFirstRowCosY, ImgInfoOri.ImgLocPara.fOriFirstRowCosZ);
	v3D::Vector3D zDirVector = v3D::exProd(colvector, rowvector);


	if ((m_nImgWndType == ORIIMG_AXIAL&&m_nOriImgType == ORIIMG_CORONAL) || (m_nImgWndType == ORIIMG_CORONAL&&m_nOriImgType == ORIIMG_AXIAL))
	{
		double len = nIndex  * ImgInfoOri.fPixelSpaceY;
		v3D::Vector3D newLeftTop = v3D::Get3DPoint(oriLeftTop, colvector, len);

		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixX = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixX = newLeftTop.x();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixY = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixY = newLeftTop.y();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixZ = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixZ = newLeftTop.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosX = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosX = rowvector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosY = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosY = rowvector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosZ = rowvector.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosX = pImg->m_ImgInfo.ImgLocPara.fFirstColCosX = zDirVector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosY = pImg->m_ImgInfo.ImgLocPara.fFirstColCosY = zDirVector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstColCosZ = zDirVector.z();
		return;
	}

	if (m_nImgWndType == ORIIMG_AXIAL&&m_nOriImgType == ORIIMG_SAGITTAL)
	{
		double len = nIndex * ImgInfoOri.fPixelSpaceY;
		v3D::Vector3D newLeftTop = v3D::Get3DPoint(oriLeftTop, colvector, len);

		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixX = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixX = newLeftTop.x();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixY = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixY = newLeftTop.y();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixZ = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixZ = newLeftTop.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosX = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosX = zDirVector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosY = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosY = zDirVector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosZ = zDirVector.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosX = pImg->m_ImgInfo.ImgLocPara.fFirstColCosX = rowvector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosY = pImg->m_ImgInfo.ImgLocPara.fFirstColCosY = rowvector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstColCosZ = rowvector.z();
		return;
	}

	if ((m_nImgWndType == ORIIMG_CORONAL&&m_nOriImgType == ORIIMG_SAGITTAL) || (m_nImgWndType == ORIIMG_SAGITTAL&&m_nOriImgType == ORIIMG_CORONAL))
	{
		double len = nIndex  * ImgInfoOri.fPixelSpaceX;
		v3D::Vector3D newLeftTop = v3D::Get3DPoint(oriLeftTop, rowvector, len);

		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixX = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixX = newLeftTop.x();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixY = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixY = newLeftTop.y();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixZ = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixZ = newLeftTop.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosX = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosX = zDirVector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosY = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosY = zDirVector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosZ = zDirVector.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosX = pImg->m_ImgInfo.ImgLocPara.fFirstColCosX = colvector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosY = pImg->m_ImgInfo.ImgLocPara.fFirstColCosY = colvector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstColCosZ = colvector.z();
		return;
	}

	if (m_nImgWndType == ORIIMG_SAGITTAL && m_nOriImgType == ORIIMG_AXIAL)
	{
		double len = nIndex  * ImgInfoOri.fPixelSpaceX;
		v3D::Vector3D newLeftTop = v3D::Get3DPoint(oriLeftTop, rowvector, len);

		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixX = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixX = newLeftTop.x();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixY = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixY = newLeftTop.y();
		pImg->m_ImgInfo.ImgLocPara.fOriLeftTopPixZ = pImg->m_ImgInfo.ImgLocPara.fLeftTopPixZ = newLeftTop.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosX = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosX = colvector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosY = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosY = colvector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosZ = colvector.z();

		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosX = pImg->m_ImgInfo.ImgLocPara.fFirstColCosX = zDirVector.x();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosY = pImg->m_ImgInfo.ImgLocPara.fFirstColCosY = zDirVector.y();
		pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstColCosZ = zDirVector.z();
		return;
	}
}
