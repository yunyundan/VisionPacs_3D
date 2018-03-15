#include "stdafx.h"
#include "LoadImageThread.h"
#include "HsImage.h"
#include "HsFile.h"

extern void ArrayFree(void **pArr, int iflag = 0);

LoadImageThread::LoadImageThread(QObject *parent)
	: QThread(parent)
{
}

LoadImageThread::~LoadImageThread()
{
}

void LoadImageThread::SetThreadPara(CHsImage *pImage, int nIndex, vtkSmartPointer<vtkImageData> m_pImageData, int nOriImageType)
{
	m_pLoadImage = pImage;
	m_nImageIndex = nIndex;
	m_pVtkData = m_pImageData;
	m_nOriImageType = nOriImageType;
}

void LoadImageThread::run()
{
	int nSize = m_pLoadImage->m_ImgInfo.nBitsAllocated / 8;
	int nRows = m_pLoadImage->m_ImgInfo.nRows;
	int nCols = m_pLoadImage->m_ImgInfo.nCols;

	double fRescaleIntercept = m_pLoadImage->m_ImgInfo.fRescaleIntercept;
	double fRescaleSlope = m_pLoadImage->m_ImgInfo.fRescaleSlope; 

	int Ext[6];
	m_pVtkData->GetExtent(Ext);

	if (m_pLoadImage->Hs_IsEmpty())
	{
		CHsFile *pDs = m_pLoadImage->GetDs();
		pDs->Hs_GetImage(m_pLoadImage->m_ImgInfo.pEle, *m_pLoadImage, m_pLoadImage->m_ImgInfo.iFrame);
		//pImg->Hs_FilpOridata();
	}

	if (nSize == 1)
	{
		char *ptr = (char *)m_pVtkData->GetScalarPointer();
		int nPixelValue = 0;
		char **pOriData = (char**)(m_pLoadImage->m_pOriData);

		for (int r = 0; r < nRows; r++)
		{
			for (int c = 0; c < nCols; c++)
			{
				nPixelValue = pOriData[r][c];

				if (m_nOriImageType == ORIIMG_AXIAL)
				{
					*(ptr + (Ext[1] * Ext[3])*m_nImageIndex + Ext[1]*r + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
				}
				else if (m_nOriImageType == ORIIMG_CORONAL)
				{
					*(ptr + (Ext[1]*Ext[3])*r + (Ext[1]*m_nImageIndex) + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
				}
				else if (m_nOriImageType == ORIIMG_SAGITTAL)
				{
					*(ptr + (Ext[1] * Ext[3])*r + (Ext[1]*c) + (Ext[1] - m_nImageIndex - 1)) = fRescaleSlope * nPixelValue + fRescaleIntercept;
				}
			}
		}

		if (m_pLoadImage->m_pOriData)
			ArrayFree((void **)m_pLoadImage->m_pOriData);
		m_pLoadImage->m_pOriData = NULL;
	}
	else if (nSize == 2)
	{
		int nPixelRepresentation = m_pLoadImage->m_ImgInfo.nPixelRepresentation;
		short *ptr = (short *)m_pVtkData->GetScalarPointer();
		if (m_pLoadImage->Hs_IsEmpty())
		{
			CHsFile *pDs = m_pLoadImage->GetDs();
			pDs->Hs_GetImage(m_pLoadImage->m_ImgInfo.pEle, *m_pLoadImage, m_pLoadImage->m_ImgInfo.iFrame);
			//pImg->Hs_FilpOridata();
		}

		short **pSOriData = NULL; unsigned short **pUSOriData = NULL;
		int nPixelValue = 0;
		if (nPixelRepresentation == 1)
			pSOriData = (short**)(m_pLoadImage->m_pOriData);
		else
			pUSOriData = (unsigned short**)(m_pLoadImage->m_pOriData);

		for (int r = 0; r < nRows; r++)
		{
			for (int c = 0; c < nCols; c++)
			{
				if (nPixelRepresentation == 1)
					nPixelValue = pSOriData[r][c];
				else
					nPixelValue = pUSOriData[r][c];

				if (m_nOriImageType == ORIIMG_AXIAL)
				{
					*(ptr + (Ext[1] * Ext[3])*m_nImageIndex + Ext[1] * r + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
				}
				else if (m_nOriImageType == ORIIMG_CORONAL)
				{
					*(ptr + (Ext[1] * Ext[3])*r + (Ext[1] * m_nImageIndex) + c) = fRescaleSlope * nPixelValue + fRescaleIntercept;
				}
				else if (m_nOriImageType == ORIIMG_SAGITTAL)
				{
					*(ptr + (Ext[1] * Ext[3])*r + (Ext[1] * c) + (Ext[1] - m_nImageIndex - 1)) = fRescaleSlope * nPixelValue + fRescaleIntercept;
				}

			}
		}

		if (m_pLoadImage->m_pOriData)
			ArrayFree((void **)m_pLoadImage->m_pOriData);
		m_pLoadImage->m_pOriData = NULL;
	}
}
