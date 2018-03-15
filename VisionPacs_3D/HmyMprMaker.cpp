#include "stdafx.h"
#include "HmyMprMaker.h"
#include "HsImage.h"
#include "Hmy3dMath.h"

extern void** ArrayNew(unsigned long nRow, unsigned long nCol, unsigned long nSize, unsigned long *nNewRow = NULL, unsigned long* nNewCol = NULL);
extern void ArrayFree(void **pArr, int iflag = 0);

int round_double(double number);
int round_double(double number)
{
	return (number > 0.0) ? (number + 0.5) : (number - 0.5);
}

CHmyMprMaker::CHmyMprMaker()
	:m_nSlabMode(IMAGE_SLAB_MAX)
	,m_nSlabNumberOfSlices(1)
	,m_pCutMatrix(nullptr)
	,m_pCutPlane(nullptr)
{
}

CHmyMprMaker::~CHmyMprMaker()
{
	if (m_pCutMatrix)
	{
		delete m_pCutMatrix;
		m_pCutMatrix = nullptr;
	}

	if (m_pCutPlane)
	{
		delete m_pCutPlane;
		m_pCutPlane = nullptr;
	}
}

void CHmyMprMaker::InitParaAndData(vector<CHsImage*> vImg, void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, int nWndType, HmyImageData3D *pHmyImgData)
{
	m_vOriImage = vImg;
	m_p3DImgArray = pImgArray;
	m_p3DImgData = p3Ddata;
	m_nImgWndType = nWndType;
	m_pHmyImageData = pHmyImgData;
}

void CHmyMprMaker::CalculateCutPlane()
{
	Hmy3DVector vector1 = m_pLine1->GetLinesVector();
	Hmy3DVector vector2 = m_pLine2->GetLinesVector();
	Hmy3DVector normal = vector1.Cross(vector2);
	double cutElements[16] = {
		vector1.x(), vector1.y(),vector1.z(), 0,
		vector2.x(), vector2.y(),vector2.z(), 0,
		normal.x(), normal.y(),normal.z(), 0,
		0, 0, 0, 1 };

	if (m_pCutMatrix == nullptr)
	{
		m_pCutMatrix = new HmyMatrix4x4();
	}
	m_pCutMatrix->DeepCopy(cutElements);
	if (abs(normal.x()) == 1.00)
	{
		double elements[16] = {
			0, -1, 0, 0,
			0, 0, -1, 0,
			1, 0, 0, 0,
			0, 0, 0, 1 };
		HmyMatrix4x4 *temp = new HmyMatrix4x4();
		temp->DeepCopy(elements);
		*m_pCutMatrix = m_pCutMatrix->MultiplyMatrix(*temp);
		delete temp;
	}

	if (m_pCutPlane == nullptr)
	{
		m_pCutPlane = new HmyPlane3D();
	}
	m_pCutPlane->SetFromNormalAndCoplanarPoint(normal, m_pLine1->start());

}

void CHmyMprMaker::CalculateCutImageInfo(CHsImage *pCutImg)
{
	int i, j;
	double inSpacing[3], inOrigin[3];
	int inWholeExt[6];
	double outSpacing[3], outOrigin[3];
	int outWholeExt[6];

	m_p3DImgData->GetExtent(inWholeExt);
	m_p3DImgData->GetSpacing(inSpacing);
	m_p3DImgData->GetOrigin(inOrigin);

	// reslice axes matrix is identity by default
	HmyMatrix4x4 matrix;
	HmyMatrix4x4 imatrix;
	for (i = 0; i < 4; i++)
	{
		matrix.SetElement(i, i, 1);
		imatrix.SetElement(i, i, 1);
	}

	if (m_pCutMatrix)
	{
		matrix = *m_pCutMatrix;
		imatrix = matrix.Invert();
	}

	// pass the center of the volume through the inverse of the
	// 3x3 direction cosines matrix
	double inCenter[3];
	for (i = 0; i < 3; i++)
	{
		inCenter[i] = inOrigin[i] + \
			0.5*(inWholeExt[2 * i] + inWholeExt[2 * i + 1])*inSpacing[i];
	}

	// the default spacing, extent and origin are the input spacing, extent
	// and origin,  transformed by the direction cosines of the ResliceAxes
	// if requested (note that the transformed output spacing will always
	// be positive)
	for (i = 0; i < 3; i++)
	{
		double s = 0;  // default output spacing
		double d = 0;  // default linear dimension
		double e = 0;  // default extent start
		double c = 0;  // transformed center-of-volume

		double r = 0.0;
		for (j = 0; j < 3; j++)
		{
			c += imatrix.GetElement(i,j) * (inCenter[j] - matrix.GetElement(j,3));
			double tmp = matrix.GetElement(j, i)*matrix.GetElement(j, i);
			s += tmp*fabs(inSpacing[j]);
			d += tmp*(inWholeExt[2 * j + 1] - inWholeExt[2 * j])*fabs(inSpacing[j]);
			e += tmp*inWholeExt[2 * j];
			r += tmp;
		}
		s /= r;
		d /= r*sqrt(r);
		e /= r;

		outSpacing[i] = s;

		if (i >= 2)
		{
			outWholeExt[2 * i] = 0;
			outWholeExt[2 * i + 1] = 0;
		}
		else
		{
			outWholeExt[2 * i] = round_double(e);
			outWholeExt[2 * i + 1] = round_double(outWholeExt[2 * i] +
				fabs(d / outSpacing[i]));
		}

		if (i >= 2)
		{
			outOrigin[i] = 0;
		}
		else
		{
			outOrigin[i] = c - \
				0.5*(outWholeExt[2 * i] + outWholeExt[2 * i + 1])*outSpacing[i];
		}
	}
	pCutImg->m_ImgInfo.nOriCols = outWholeExt[1] + 1;
	pCutImg->m_ImgInfo.nOriRows = outWholeExt[3] + 1;
	pCutImg->m_ImgInfo.fPixelSpaceX = outSpacing[0];
	pCutImg->m_ImgInfo.fPixelSpaceY = outSpacing[1];	
}

bool CHmyMprMaker::IsOrthogonalMatrix(HmyMatrix4x4 *matrix)
{
	for (int i = 0; i < 3; i++)
	{
		if (matrix->GetElement(3, i) != 0)
		{
			return false;
		}
	}
	if (matrix->GetElement(3, 3) != 1)
	{
		return false;
	}
	for (int j = 0; j < 3; j++)
	{
		int k = 0;
		for (int i = 0; i < 3; i++)
		{
			if (matrix->GetElement(i, j) != 0)
			{
				k++;
			}
		}
		if (k != 1)
		{
			return false;
		}
	}
	return true;
}

void CHmyMprMaker::GetStartAndEndSlice(int nIndex, int &nSliceNum, int &nStartS, int &nEndS)
{
	int nExtents[6];
	m_p3DImgData->GetExtent(nExtents);

	int nMaxSlice = 0;
	if (m_nImgWndType == ORIIMG_AXIAL)
		nMaxSlice = nExtents[5];
	else if (m_nImgWndType == ORIIMG_CORONAL)
	{
		nMaxSlice = nExtents[3];
		nIndex = nMaxSlice - nIndex;
	}		
	else
		nMaxSlice = nExtents[1];

	if (nSliceNum <= 1)
	{
		nStartS = nEndS = nIndex;
	}
	else
	{
		if (nIndex == 0)
		{
			nStartS = 0;
			nEndS = nSliceNum;

			nEndS = min(nEndS, nMaxSlice);
		}
		else if (nIndex == nMaxSlice)
		{
			nStartS = nMaxSlice - nSliceNum;
			nEndS = nMaxSlice;

			nStartS = max(nStartS, 0);
		}
		else
		{
			if (nSliceNum % 2 == 0)
			{
				nStartS = nIndex - nSliceNum / 2;
				nEndS = nIndex + nSliceNum / 2 - 1;
			}
			else
			{
				nStartS = nIndex - nSliceNum / 2;
				nEndS = nIndex + nSliceNum / 2;
			}
			nStartS = max(nStartS, 0);
			nEndS = min(nEndS, nMaxSlice);
		}
	}

	nSliceNum = nEndS - nStartS + 1;
}

void CHmyMprMaker::GetCutImage(CHsImage *pCutImg, HmyLine3D *pLine1, HmyLine3D *pLine2, int nSliceNum)
{
	m_pLine1 = pLine1;
	m_pLine2 = pLine2;

	CalculateCutPlane();
	CalculateCutImageInfo(pCutImg);

	bool bOrthogonal = IsOrthogonalMatrix(m_pCutMatrix);
	if (bOrthogonal)
	{
		GetOrthogonalImage(pCutImg);
	}
	else
	{

	}


	return;
}


void CHmyMprMaker::GetOrthogonalImage(CHsImage *pCutImage)
{
	if (abs(m_pCutPlane->normal().z()) == 1.000)
	{
		CalculateOrthogonalImage(pCutImage, m_pLine1->start().z(), m_nSlabNumberOfSlices, 0);
	}
	else if (abs(m_pCutPlane->normal().y()) == 1.000)
	{
		CalculateOrthogonalImage(pCutImage, m_pLine1->start().y(), m_nSlabNumberOfSlices, 1);
	}
	else if (abs(m_pCutPlane->normal().x()) == 1.000)
	{
		CalculateOrthogonalImage(pCutImage, m_pLine1->start().x(), m_nSlabNumberOfSlices, 2);
	}
}


void CHmyMprMaker::CalculateOrthogonalImage(CHsImage *pCutImg, int nIndex, int nSliceNum, int nOritation)
{
	int nRows = pCutImg->m_ImgInfo.nOriRows;
	int nCols = pCutImg->m_ImgInfo.nOriCols;

	int nSize = m_vOriImage[0]->m_ImgInfo.nBitsAllocated / 8;

	int nStratS = 0, nEndS = 0;
	GetStartAndEndSlice(nIndex, nSliceNum, nStratS, nEndS);

	if (nOritation == 0)
	{
		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[nSliceNum];
			for (int r = 0; r < nRows; r++)
			{
				int nIndex = 0;
				for (int s = nStratS; s <= nEndS; s++)
				{
					BYTE **pSlice = pData[s];
					pRows[nIndex] = long(pSlice[r]);
					nIndex++;
				}
				BYTE **pAllRows = (BYTE**)pRows;
				CalMprMode(pRetData, pAllRows, r, nCols, nSliceNum);
			}
			delete pRows;
			pCutImg->m_pOriData = (BYTE**)pRetData;
		}
		else if (nSize == 2)
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[nEndS - nStratS + 1];
			for (int r = 0; r < nRows; r++)
			{
				int nIndex = 0;
				for (int s = nStratS; s <= nEndS; s++)
				{
					short **pSlice = pData[s];
					pRows[nIndex] = long(pSlice[r]);
					nIndex++;
				}
				short **pAllRows = (short**)pRows;
				CalMprMode(pRetData, pAllRows, r, nCols, nSliceNum);
			}
			delete pRows;
			pCutImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	else if (nOritation == 1)
	{
		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[nEndS - nStratS + 1];
			for (int r = 0; r < nRows; r++)
			{
				BYTE **pSlice = pData[nRows - 1 - r];
				int nIndex = 0;
				for (int s = nStratS; s <= nEndS; s++)
				{
					pRows[nIndex] = long(pSlice[s]);
					nIndex++;
				}
				BYTE **pAllRows = (BYTE**)pRows;
				CalMprMode(pRetData, pAllRows, r, nCols, nIndex);

			}
			delete []pRows;
			pCutImg->m_pOriData = (BYTE**)pRetData;
		}
		else
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);

			long *pRows = new long[nEndS - nStratS + 1];
			for (int r = 0; r < nRows; r++)
			{
				short **pSlice = pData[nRows - 1 - r];
				int nIndex = 0;
				for (int s = nStratS; s <= nEndS; s++)
				{
					pRows[nIndex] = long(pSlice[s]);
					nIndex++;
				}
				short **pAllRows = (short**)pRows;
				CalMprMode(pRetData, pAllRows, r, nCols, nSliceNum);
			}
			delete pRows;
			pCutImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	else if (nOritation == 2)
	{
		if (nSize == 1)
		{
			BYTE ***pData = (BYTE ***)m_p3DImgArray;
			BYTE **pRetData = (BYTE**)ArrayNew(nRows, nCols, nSize);
			for (int r = 0; r < nRows; r++)
			{
				BYTE **pSlice = pData[r];
				for (int c = 0; c < nCols; c++)
				{
					BYTE *pRows = pSlice[nCols - 1 - c];
					if (m_nSlabMode == IMAGE_SLAB_MAX)
					{
						int nMax = pRows[nStratS];
						for (int s = nStratS + 1; s <= nEndS; s++)
						{
							int nCur = pRows[s];
							nMax = max(nMax, nCur);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nSlabMode == IMAGE_SLAB_MEAN)
					{
						int nTotle = pRows[nStratS];
						for (int s = nStratS + 1; s <= nEndS; s++)
						{
							nTotle += pRows[s];
						}
						pRetData[r][c] = nTotle / (nEndS - nStratS + 1);
					}
				}
			}
			pCutImg->m_pOriData = (BYTE**)pRetData;
		}
		else
		{
			short ***pData = (short ***)m_p3DImgArray;
			short **pRetData = (short**)ArrayNew(nRows, nCols, nSize);

			for (int r = 0; r < nRows; r++)
			{
				short **pSlice = pData[nRows - 1 - r];
				for (int c = 0; c < nCols; c++)
				{
					short *pRows = pSlice[nCols - 1 - c];
					if (m_nSlabMode == IMAGE_SLAB_MAX)
					{
						int nMax = pRows[nStratS];
						for (int s = nStratS + 1; s <= nEndS; s++)
						{
							int nCur = pRows[s];
							nMax = max(nMax, nCur);
						}
						pRetData[r][c] = nMax;
					}
					else if (m_nSlabMode == IMAGE_SLAB_MEAN)
					{
						int nTotle = pRows[nStratS];
						for (int s = nStratS + 1; s <= nEndS; s++)
						{
							nTotle += pRows[s];
						}
						pRetData[r][c] = nTotle / (nEndS - nStratS + 1);
					}
				}
			}
			pCutImg->m_pOriData = (BYTE**)pRetData;
		}
	}
	DoInterpolationToImage(pCutImg, pCutImg->m_ImgInfo.fPixelSpaceX, pCutImg->m_ImgInfo.fPixelSpaceY, pCutImg->m_ImgInfo.nOriRows, pCutImg->m_ImgInfo.nOriCols);
	CalImagePositon(pCutImg);
}

void CHmyMprMaker::CalMprMode(BYTE **pRetData, BYTE **pAllRows, int r, int nCols, int nSliceNum)
{
	for (int c = 0; c < nCols; c++)
	{
		if (m_nSlabMode == IMAGE_SLAB_MAX)
		{
			int nMax = pAllRows[0][c];
			for (int n = 1; n < nSliceNum; n++)
			{
				int nCur = pAllRows[n][c];
				nMax = max(nMax, nCur);
			}
			pRetData[r][c] = nMax;
		}
		else if (m_nSlabMode == IMAGE_SLAB_MEAN)
		{
			int nTotle =  pAllRows[0][c];
			for (int n = 1; n < nSliceNum; n++)
			{
				int nCur = pAllRows[n][c];
				nTotle += nCur;
			}
			pRetData[r][c] = nTotle / nSliceNum;
		}
	}
}

void CHmyMprMaker::CalMprMode(short **pRetData, short **pAllRows, int r, int nCols, int nSliceNum)
{
	for (int c = 0; c < nCols; c++)
	{
		if (m_nSlabMode == IMAGE_SLAB_MAX)
		{
			int nMax = pAllRows[0][c];
			for (int n = 1; n < nSliceNum; n++)
			{
				int nCur = pAllRows[n][c];
				nMax = max(nMax, nCur);
			}
			pRetData[r][c] = nMax;
		}
		else if (m_nSlabMode == IMAGE_SLAB_MEAN)
		{
			int nTotle = pAllRows[0][c];
			for (int n = 1; n < nSliceNum; n++)
			{
				int nCur = pAllRows[n][c];
				nTotle += nCur;
			}
			pRetData[r][c] = nTotle / nSliceNum;
		}
	}
}

void CHmyMprMaker::DoInterpolationToImage(CHsImage *pImg, double dSpacingX, double dSpacingY, int nRows, int nCols)
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

bool CHmyMprMaker::IsInterLineInRect(int zIndex, Hmy3DVector origin, Hmy3DVector direction, HmyLine3D &interSetLine)
{
	int nRows = m_vOriImage[zIndex]->m_ImgInfo.nOriRows;
	int nCols = m_vOriImage[zIndex]->m_ImgInfo.nOriCols;
	double dis = sqrt(nRows * nRows + nCols * nCols);
	HmyLine3D line;
	line.SetValue(0, origin);
	line.SetValue(1, origin.Add(direction.Scale(dis)));

	Hmy3DVector topLeft = Hmy3DVector(0, 0, zIndex);
	Hmy3DVector topRight = Hmy3DVector(nCols - 1, 0, zIndex);
	Hmy3DVector bottomLeft = Hmy3DVector(0, nRows - 1, zIndex);
	Hmy3DVector bottomRight = Hmy3DVector(nCols - 1, nRows - 1, zIndex);

	HmyLine3D rectLines[4];
	rectLines[0] = HmyLine3D(topLeft, topRight);
	rectLines[1] = HmyLine3D(topRight, bottomRight);
	rectLines[2] = HmyLine3D(bottomLeft, bottomRight);
	rectLines[3] = HmyLine3D(topLeft, bottomLeft);

	vector<Hmy3DVector> vIntersects;
	for (int i = 0; i < 4; i++)
	{
		Hmy3DVector interSect;
		bool re = line.IntersectLine(rectLines[i], interSect);
		if (re)
			vIntersects.push_back(interSect);
	}

	if (vIntersects.size() != 2)
		return false;

	interSetLine.SetValue(0, vIntersects[0]);
	interSetLine.SetValue(1, vIntersects[1]);
	return true;
}

void CHmyMprMaker::CalImagePositon(CHsImage *pImg)
{
	ImageInfo infos = pImg->m_ImgInfo;
	Hmy3DVector rowsVector = Hmy3DVector(infos.ImgLocPara.fOriFirstRowCosX, infos.ImgLocPara.fOriFirstRowCosY, infos.ImgLocPara.fOriFirstRowCosZ);
	Hmy3DVector colsVector = Hmy3DVector(infos.ImgLocPara.fOriFirstColCosX, infos.ImgLocPara.fOriFirstColCosY, infos.ImgLocPara.fOriFirstColCosZ);
	Hmy3DVector normal = rowsVector.Cross(colsVector);
	double elements[16] = {
		rowsVector.x(), rowsVector.y(),rowsVector.z(), 0,
		colsVector.x(), colsVector.y(),colsVector.z(), 0,
		normal.x(), normal.y(),normal.z(), 0,
		0, 0, 0, 1 };

	HmyMatrix4x4 imagePoston;
	imagePoston.DeepCopy(elements);
	imagePoston = imagePoston.MultiplyMatrix(*m_pCutMatrix);

	pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosX = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosX = imagePoston.GetElement(0, 0);
	pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosY = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosY = imagePoston.GetElement(0, 1);
	pImg->m_ImgInfo.ImgLocPara.fOriFirstRowCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstRowCosZ = imagePoston.GetElement(0, 2);
	pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosX = pImg->m_ImgInfo.ImgLocPara.fFirstColCosX = imagePoston.GetElement(1, 0);
	pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosY = pImg->m_ImgInfo.ImgLocPara.fFirstColCosY = imagePoston.GetElement(1, 1);
	pImg->m_ImgInfo.ImgLocPara.fOriFirstColCosZ = pImg->m_ImgInfo.ImgLocPara.fFirstColCosZ= imagePoston.GetElement(1, 2);

}
