#include "stdafx.h"
#include "HsImage.h"
#include "HsFile.h"

extern void** ArrayNew(unsigned long nRow, unsigned long nCol, unsigned long nSize, unsigned long *nNewRow = NULL, unsigned long* nNewCol = NULL);
extern void ArrayFree(void **pArr, int iflag = 0);
extern void** ArrayCopy(void** pSrcArr, unsigned long nSrcRow, unsigned long nSrcCol, unsigned long nSrcSize);

CHsImage::CHsImage()
: m_pDs(NULL)
{
	SetRect(&m_WndRc, 0, 0, 0, 0);
}


CHsImage::~CHsImage()
{
	Hs_FreeMem();
}

void CHsImage::SetDs(CHsFile *pDS)
{
	m_pDs = pDS;
}

CHsFile * CHsImage::GetDs()
{
	return m_pDs;
}

int CHsImage::Hs_Reload(int islicenum, bool bApplyCurImgState/*=true*/)
{
	int nRet = Ret_Success;


	CHsFile *pDs = (CHsFile*)(m_pDs);

	nRet = pDs->Hs_GetImage(m_ImgInfo.pEle, *this, m_ImgInfo.iFrame);

	////应用一下各种效果
	if (bApplyCurImgState)
		Hs_ApplyCurImgState();
	

	return nRet;
}

int CHsImage::Hs_ApplyCurImgState()
{
	if (Hs_IsEmpty() == true)
		return Ret_EmptyImage;

	int nRet = Ret_Success;
	bool bWcUsed = false;//有的函数内部调用了wc
	//检查wc
	//if (m_ImgState.CurWc.x != int(m_ImgInfo.fWinWidth) || m_ImgState.CurWc.y != int(m_ImgInfo.fWinCenter) )
	{
		if (bWcUsed == false)
		{
			nRet = Hs_WinLevel(m_ImgState.CurWc.x, m_ImgState.CurWc.y);
			if (nRet != Ret_Success)
				return nRet;
		}

	}

	return nRet;
}

void CHsImage::SetWndRc(RECT rc)
{
	m_WndRc = rc;

	//显示区域发生变化,要及时通知AnnoManager们所含的注释体
	SIZE ImgSize = Hs_GetImgSize();

}

RECT CHsImage::GetWndRc()
{
	return m_WndRc;
}


SIZE CHsImage::Hs_GetImgSize(bool bDisplaySize/*=false*/)
{
	SIZE sz = { m_ImgState.nCurOriPixCol, m_ImgState.nCurOriPixRow };
	if (bDisplaySize == true)
	{
		sz.cx = m_ImgState.nCurBitsCol;
		sz.cy = m_ImgState.nCurBitsRow;
	}
	return sz;
}

int CHsImage::Hs_Restore()
{
	m_ImgState.Reset();
	Hs_InitImgState();

	return Ret_Success;
}

int CHsImage::Hs_WinLevel(long w, long c, bool bChangeValue/*=false*/, long *pRetW/*=NULL*/, long *pRetC/*=NULL*/)
{
	int nRet = CHsBaseImg::Hs_WinLevel(w, c, bChangeValue, pRetW, pRetC);

	return nRet;
}

int CHsImage::Hs_FreeMem()
{
	if (m_pOriData)
	{
		long *pOriData = (long*)m_pOriData;
		delete[]pOriData;
		m_pOriData = NULL;
	}

	int	nRet = CHsBaseImg::Hs_FreeMem();
	return nRet;
}

POINT CHsImage::ConvertWndToImg(RECT ImgRcOnWnd, long nImgW, long nImgH, POINT &pt)
{
	POINT ImgPt = { 0, 0 };
	if (ImgRcOnWnd.left == ImgRcOnWnd.right || ImgRcOnWnd.top == ImgRcOnWnd.bottom)
		return ImgPt;

	double fx = nImgW*1.00 / (ImgRcOnWnd.right - ImgRcOnWnd.left);
	ImgPt.x = fx * (pt.x - ImgRcOnWnd.left);

	double fy = nImgH*1.00 / (ImgRcOnWnd.bottom - ImgRcOnWnd.top);
	ImgPt.y = fy * (pt.y - ImgRcOnWnd.top);

	return ImgPt;
}

int CHsImage::Hs_QtDrawImg(QImage &qImage,RECT &rc)
{
	if (m_pDisplayData == NULL)
	{
		Hs_Size(NULL, rc.right - rc.left, rc.bottom - rc.top, HSIZE_RESAMPLE,true);
	}

	if (m_ImgInfo.bGrayImg)
	{
		long tCol = ((m_ImgState.nDispalyCol * 8) + 31) / 32 * 4;
		long nPix = m_ImgState.nDispalyRow*tCol;

		BYTE bt[1064];
		memset(bt, 0, 1064);

		QSize szImg(tCol, m_ImgState.nDispalyRow);
		qImage = QImage((uchar *)m_pBits, szImg.width(), szImg.height(), QImage::Format_Indexed8);

		QVector<QRgb> grayClrTable;
		for (int i = 0; i < 256; i++)
		{
			QRgb a = qRgb(i,i,i);
			grayClrTable.append(a);
		}

		qImage.setColorTable(grayClrTable);		
	}
	else
	{
		long tCol = ((m_ImgState.nDispalyCol * 8) + 31) / 32 * 4;
		long nPix = m_ImgState.nDispalyRow*tCol;

		BYTE bt[1064];
		memset(bt, 0, 1064);

		QSize szImg(tCol, m_ImgState.nDispalyRow);
		qImage = QImage((uchar *)m_pBits, szImg.width(), szImg.height(), QImage::Format_RGB32);
	}
	return 0;
}

int CHsImage::Hs_FilpOridata()
{
	int nRet = TransformImg(3);
	return nRet;
}
