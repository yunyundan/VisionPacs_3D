#include "stdafx.h"
#include "HsImage.h"
#include "HsFile.h"
#include "AppConfig.h"
#include "ImageWnd.h"
#include "OperateMprLines.h"

extern void** ArrayNew(unsigned long nRow, unsigned long nCol, unsigned long nSize, unsigned long *nNewRow = NULL, unsigned long* nNewCol = NULL);
extern void ArrayFree(void **pArr, int iflag = 0);
extern void** ArrayCopy(void** pSrcArr, unsigned long nSrcRow, unsigned long nSrcCol, unsigned long nSrcSize);

LONG ConvertStrToLong(const char *ch);
LONG ConvertStrToLong(const char *ch)
{
	if (ch == NULL)
		return 0;

	int n = int(strlen(ch));

	if (n == 0)
		return 0;

	if (n == 10)//0x00100010
	{
		if (ch[0] == '0' && (ch[1] == 'x' || ch[1] == 'X'))
		{
			LONG re = 0;
			for (int i = 9; i >= 2; i--)
			{
				if (ch[i] == '0')	re += 0;
				if (ch[i] == '1')	re += 1 * pow(16.00, 9 - i);
				if (ch[i] == '2')	re += 2 * pow(16.00, 9 - i);
				if (ch[i] == '3')	re += 3 * pow(16.00, 9 - i);
				if (ch[i] == '4')	re += 4 * pow(16.00, 9 - i);
				if (ch[i] == '5')	re += 5 * pow(16.00, 9 - i);
				if (ch[i] == '6')	re += 6 * pow(16.00, 9 - i);
				if (ch[i] == '7')	re += 7 * pow(16.00, 9 - i);
				if (ch[i] == '8')	re += 8 * pow(16.00, 9 - i);
				if (ch[i] == '9')	re += 9 * pow(16.00, 9 - i);
				if (ch[i] == 'A' || ch[i] == 'a')	re += 10 * pow(16.00, 9 - i);
				if (ch[i] == 'B' || ch[i] == 'b')	re += 11 * pow(16.00, 9 - i);
				if (ch[i] == 'C' || ch[i] == 'c')	re += 12 * pow(16.00, 9 - i);
				if (ch[i] == 'D' || ch[i] == 'd')	re += 13 * pow(16.00, 9 - i);
				if (ch[i] == 'E' || ch[i] == 'e')	re += 14 * pow(16.00, 9 - i);
				if (ch[i] == 'F' || ch[i] == 'f')	re += 15 * pow(16.00, 9 - i);
			}

			return re;
		}
	}
	else
	{
		return atol(ch);
	}


	return 0;
}

CHsImage::CHsImage()
: m_pDs(NULL)
, m_fOriSilceThick(0.00)
, m_fCurSliceThick(0.00)
, m_pBelongWnd(NULL)
{
	SetRect(&m_WndRc, 0, 0, 0, 0);
}


CHsImage::~CHsImage()
{
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
	
	if (m_pBelongWnd != NULL)
	{
		if (m_pBelongWnd->GetOperateLine())
		{
			m_pBelongWnd->GetOperateLine()->RefreshMprLinesPara(m_WndRc, m_ImgState.nCurOriPixCol, m_ImgState.nCurOriPixRow
				, m_ImgInfo.fPixelSpaceX, m_ImgInfo.fPixelSpaceY);
		}
	}
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
		Hs_Size(NULL, rc.right - rc.left, rc.bottom - rc.top, HSIZE_RESAMPLE, true);
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

void CHsImage::Hs_GetCornerInfo(bool bFrist)
{
	if (m_pDs == NULL)
		return;

	bool b = true;
	if(bFrist == true)
		b = CAppConfig::GetInfoSet(m_ImgInfo.sModality, m_CornorInfo);

	if ( b == true )
	{
		int nCornor = int(m_CornorInfo.coInfoV.size());
		for (int i = 0; i < nCornor; i++)
		{
			int nItem = int(m_CornorInfo.coInfoV[i].infoV.size());
			for (int j = 0; j < nItem; j++)
			{
				if (bFrist)
					InitInfoItem(m_pDs, m_CornorInfo.coInfoV[i].infoV[j]);
				else
					RefreshInfoItem(m_pDs, m_CornorInfo.coInfoV[i].infoV[j]);
			}
		}
	}
}

int CHsImage::InitInfoItem(CHsFile* pDs, INFOITEM &infoItem)
{
	int nRet = 0;
	char cc[256];

	if (infoItem.sTag.indexOf("0x", 0) >= 0)//Dicom Tag
	{
		unsigned long nTag = ConvertStrToLong(infoItem.sTag.toLatin1().data()) ;

		int nType = GetFormatType(infoItem.sFormat);

		pHsElement pEle = NULL;
		if (nType != 0)
			pEle = pDs->Hs_FindFirstEle(NULL, nTag, false);

		if (pEle)
		{
			if (nType == 1)//string
			{
				QString s = m_pDs->Hs_GetConvertValue(pEle, 0, nRet);
				if (infoItem.sFormat.compare("%c ") == 0)
					infoItem.sFormat = "%s ";
				if (infoItem.sFormat.compare("%A") == 0)
				{
					infoItem.sValue = s;
				}
				else
				{
					sprintf(cc, infoItem.sFormat.toLatin1().data(), s.toLatin1().data());
					infoItem.sValue = cc;
				}
			}
			else if (nType == 2)//整型
			{
				long v = 0;
				pDs->Hs_GetLongValue(pEle, v, 0);

				sprintf(cc, infoItem.sFormat.toLatin1().data(), v);
				infoItem.sValue = cc;
			}
			else if (nType == 3)//小数
			{
				double v = 0.00;
				pDs->Hs_GetDoubleValue(pEle, v, 0);

				sprintf(cc, infoItem.sFormat.toLatin1().data(), v);
				infoItem.sValue = cc;
			}
		}
	}

	return 0;
}

int CHsImage::RefreshInfoItem(CHsFile* pDs, INFOITEM &infoItem)
{
	char cc[256] = { 0 };

	if (infoItem.sTag.indexOf("0x") == 0)//已填充(窗宽窗位除外)
	{
		if (infoItem.sTag.compare("0x00281050") == 0)//WC
		{
			int nInfoType = GetFormatType(infoItem.sFormat);
			if (nInfoType == 2)
				sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.CurWc.y);
			else
				sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.CurWc.y*1.00);

			infoItem.sValue = cc;
		}
		if (infoItem.sTag.compare("0x00281051") == 0)//WW
		{
			int nInfoType = GetFormatType(infoItem.sFormat);
			if (nInfoType == 2)
				sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.CurWc.x);
			else
				sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.CurWc.x*1.00);
			infoItem.sValue = cc;
		}
		return 0;
	}

	//常量
	if (infoItem.sTag.indexOf('[', 0) == 0)
	{
		CString tr = infoItem.sTag.toLatin1().data();
		tr.TrimLeft('[');
		tr.TrimRight(']');

		sprintf(cc, infoItem.sFormat.toLatin1().data(), tr);

		infoItem.sValue = cc;
		return 0;
	}

	if (infoItem.sTag.compare("-1") == 0)//层厚
	{
		sprintf(cc, infoItem.sFormat.toLatin1().data(), m_fCurSliceThick);
		infoItem.sValue = cc;
	}
	else if (infoItem.sTag.compare("-3") == 0)//image width
	{
		sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.nCurOriPixCol);
		infoItem.sValue = cc;
	}
	else if (infoItem.sTag.compare("-4") == 0)//image height
	{
		sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.nCurOriPixRow);
		infoItem.sValue = cc;
	}
	else if (infoItem.sTag.compare("-5") == 0)//ZoomFactor
	{
		int nInfoType = GetFormatType(infoItem.sFormat);
		if (nInfoType == 2)
			sprintf(cc, infoItem.sFormat.toLatin1().data(), int(m_ImgState.fZoomX));
		else
			sprintf(cc, infoItem.sFormat.toLatin1().data(), m_ImgState.fZoomX);

		infoItem.sValue = cc;
	}
	return 0;
}

int CHsImage::GetFormatType(QString &sFormat)
{
	if (int(sFormat.indexOf("%s")) >= 0 || int(sFormat.indexOf("%A")) >= 0 || int(sFormat.indexOf("%B")) >= 0 || int(sFormat.indexOf("%c")) >= 0)
	{
		return 1;
	}
	else
	{
		int iPos = int(sFormat.indexOf("%", 0));
		if (iPos >= 0)
		{
			int nLen = int(sFormat.size());
			for (int p = 0; p < nLen; p++)
			{
				if (sFormat[p] == 'f')
					return 3;


				if (sFormat[p] == 'd')
					return 2;

			}
		}
	}

	return 0;
}
