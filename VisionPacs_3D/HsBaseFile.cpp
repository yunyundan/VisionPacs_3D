#include "stdafx.h"

#include "HsBaseFile.h"
#include "io.h"

bool g_bDebug = false;

void** ArrayNew(unsigned long nRow, unsigned long nCol, unsigned long nSize,unsigned long *nNewRow=NULL, unsigned long* nNewCol=NULL);
void ArrayFree(void **pArr,int iflag=0);
void** ArrayCopy(void** pSrcArr,unsigned long nSrcRow, unsigned long nSrcCol, unsigned long nSrcSize);

void** ArrayNew(unsigned long nRow, unsigned long nCol, unsigned long nSize,unsigned long *nNewRow, unsigned long* nNewCol)
{
	if (nRow==0 || nCol==0 || nSize==0)
		return NULL;

	nCol = ((nCol*8)+31)/32*4;

	unsigned long nDataLen = nRow*nCol*nSize;
	BYTE* pData = new BYTE[nDataLen];
	memset(pData,0,nDataLen);

	long *pAdd = new long[nRow];
    for(unsigned long i=0;i<nRow;i++)
		pAdd[i] = long(&(pData[i*nCol*nSize]));

	if (nNewRow) *nNewRow = nRow;
	if (nNewCol) *nNewCol = nCol;

	return (void**)pAdd;
}

void ArrayFree(void **pArr,int iflag)
{
	if(pArr==NULL)
		return;

	BYTE *pData = (BYTE*)(pArr[0]);
	delete[]pData;

	long *pAdd = (long*)pArr;
	delete []pAdd;
}

void** ArrayCopy(void** pSrcArr,unsigned long nSrcRow, unsigned long nSrcCol, unsigned long nSrcSize)
{
	if (nSrcRow==0 || nSrcCol==0 || nSrcSize==0 || pSrcArr==NULL)
		return NULL;

	nSrcCol = ((nSrcCol*8)+31)/32*4;

	unsigned long nDataLen = nSrcRow*nSrcCol*nSrcSize;
	BYTE* pDstData = new BYTE[nDataLen];
	memcpy(pDstData,pSrcArr[0],nDataLen);

	long *pAdd = new long[nSrcRow];
    for(unsigned long i=0;i<nSrcRow;i++)
		pAdd[i] = long(&(pDstData[i*nSrcCol*nSrcSize]));

	return (void**)pAdd;
} 


RECT GetShowRcByImgSize(RECT rc, long ImgWidth, long ImgHeight);
RECT GetShowRcByImgSize(RECT rc, double ImgWidth, double ImgHeight)
{
    RECT reRC;
    reRC.bottom = reRC.left = reRC.right =reRC.top = 0;
    if (rc.right == rc.left)
        return reRC;


    if ((rc.bottom-rc.top)*ImgWidth >= ImgHeight*(rc.right-rc.left))//<=(cy/cx)>=(ImgHeight/ImgWidth)
    {//横向充满
        double fNewH=0;
        reRC.left = rc.left;
        reRC.right = rc.right;
        fNewH = ( ImgHeight * (rc.right-rc.left) )/ImgWidth;
        reRC.top = int((rc.bottom+rc.top-fNewH)/2);
        reRC.bottom = reRC.top+(long)(fNewH+0.5);
    }
    else
    {//纵向充满
        double fNewW=0;
        reRC.top = rc.top;
        reRC.bottom = rc.bottom;
        fNewW = ( (rc.bottom-rc.top)*ImgWidth)/ImgHeight;
        reRC.left = int((rc.right+rc.left-fNewW)/2);
        reRC.right = reRC.left+(long)(fNewW+0.5);
    }
    return reRC;
}

CHsBaseFile::CHsBaseFile(void)
:m_nDefaultReadSize(40960)
,m_fp(NULL)
,m_buf(NULL)
,m_pMainEle(NULL)
,m_pPreEle(NULL)
,m_nBufLen(0)
,m_bXRay(false)
,m_sModality("")
{
	m_BaseFileInfo.nReadLen = m_nDefaultReadSize;

	//判断CPU大端小端,会影响我的GetDouble函数
	union ut
	{
		short s;
		char c[2];
	}u;

	u.s = 0x0102;
	if(u.c[0] == 1 && u.c[1] == 2)
		m_BaseFileInfo.bCpuBigEndia = true;
	else
		m_BaseFileInfo.bCpuBigEndia = false;

}

CHsBaseFile::~CHsBaseFile(void)
{
	delete m_pMainEle;
	FreeBuffer();
	Hs_CloseFile();
}
int CHsBaseFile::Hs_CloseFile()
{
	if (m_fp)
		fclose(m_fp);
	m_fp = NULL;

	return Ret_Success;
}

int CHsBaseFile::FreeBuffer(void)
{
	if(m_buf)
		delete []m_buf;
	m_buf = NULL;

	m_nBufLen = 0;
	return Ret_Success;
}

BYTE* CHsBaseFile::Hs_GetByteValue(pHsElement pEle,unsigned long &nValueLen,int &nRet)
{
	nRet = 0;
	if(pEle == NULL)
	{
		nRet = Ret_InvalidPara;
		return NULL;
	}

	if(pEle->nLen==0 || pEle->pValue==NULL)
	{
		nRet = Ret_NoValue;
		return NULL;
	}

	nValueLen = pEle->nLen;
	BYTE* pRet = new BYTE[nValueLen];
	memcpy(pRet,pEle->pValue,nValueLen);
	return pRet;
}

int CHsBaseFile::ShowElement(pHsElement p,int nLevel)
{
	if(p == NULL)
		return 0;

	QString s = "\r\n";
	for(int i=0;i<nLevel;i++)
		s += "	";

	if(p->nVR==VR_SS)//UL SL SS US
	{
		qDebug("\r\n0x%08x	%d	%s", p->nTag, p->nLen, CDcmTag::Find(p->nTag)->pszName);

		unsigned long st = p->nTagPos + p->nOffset;

		for (unsigned long i=0;i<p->nLen;i++)
		{
			qDebug("\r\n%02x,%d,%c", m_buf[st + i], m_buf[st + i], m_buf[st + i]);
		}

		qDebug("\r\n");

		int nRet = 0;
		int n = Hs_GetValueCount(p,nRet);
		
		long v = 0;


		if(n!=0)
			int r = Hs_GetLongValue(p,v,n-1);

		int stp = 0;
	}

	int n = p->pChildEleV.size();
	for (int i = 0; i<n; i++)
	{
		ShowElement(p->pChildEleV[i],nLevel+1);
	}
	return 0;
}

int CHsBaseFile::Hs_GetDoubleValue(pHsElement pEle,	double &fVal,int nValueIndex)
{
	if(pEle==NULL || nValueIndex<0)
		return Ret_InvalidPara;

	if (pEle->nLen==0 || pEle->pValue==NULL)
		return Ret_NoValue;

	BYTE *p = pEle->pValue;

	//unsigned long nPos = pEle->nTagPos + pEle->nOffset;
	unsigned long nPos = 0;
	if (pEle->nVR == VR_DS || pEle->nVR == VR_OF)
	{
		char *pString = new char[pEle->nLen+1];
		memset(pString,0,pEle->nLen+1);
		memcpy(pString,p,pEle->nLen);

		QString sVal;
		int nRet = DivString(pString,"\\",nValueIndex,sVal);
		if(nRet == Ret_Success)
			fVal = sVal.toDouble();

		delete []pString;

		return nRet;
	}

	if (pEle->nVR == VR_FD)
	{
		if(pEle->nLen%8!=0)//不是8的倍数,就有问题
			return Ret_InvalidBuf;

		unsigned long nValueCount = pEle->nLen/8;
		if(nValueIndex>=nValueCount)
			return Ret_OutOfValueCount;

		
        for(unsigned long i=0;i<nValueCount;i++)
		{
			if (i==nValueIndex)
			{
				//unsigned char uc[8] = {m_buf[nPos],m_buf[nPos+1],m_buf[nPos+2],m_buf[nPos+3],m_buf[nPos+4],m_buf[nPos+5],m_buf[nPos+6],m_buf[nPos+7]};
				unsigned char uc[8] = {p[nPos],p[nPos+1],p[nPos+2],p[nPos+3],p[nPos+4],p[nPos+5],p[nPos+6],p[nPos+7]};
				if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
				{
					//uc[0] = m_buf[nPos+7];uc[1] = m_buf[nPos+6];uc[2] = m_buf[nPos+5];uc[3] = m_buf[nPos+4];
					//uc[4] = m_buf[nPos+3];uc[5] = m_buf[nPos+2];uc[6] = m_buf[nPos+1];uc[7] = m_buf[nPos];

					uc[0] = p[nPos+7];uc[1] = p[nPos+6];uc[2] = p[nPos+5];uc[3] = p[nPos+4];
					uc[4] = p[nPos+3];uc[5] = p[nPos+2];uc[6] = p[nPos+1];uc[7] = p[nPos];
				}
				double *f = (double*)uc;
				fVal = *f;

				return Ret_Success;
			}

			nPos += 8;
		}

		return Ret_GetValueFailed;
	}
	if (pEle->nVR == VR_FL)
	{
		if(pEle->nLen%4!=0)//不是4的倍数,就有问题
			return Ret_InvalidBuf;

		unsigned long nValueCount = pEle->nLen/4;
		if(nValueIndex>=nValueCount)
			return Ret_OutOfValueCount;

		for(int i=0;i<nValueCount;i++)
		{
			if (i==nValueIndex)
			{
				//unsigned char uc[4] = {m_buf[nPos],m_buf[nPos+1],m_buf[nPos+2],m_buf[nPos+3]};
				unsigned char uc[4] = {p[nPos],p[nPos+1],p[nPos+2],p[nPos+3]};
				if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
				{
					//uc[0] = m_buf[nPos+3];	uc[1] = m_buf[nPos+2];	uc[2] = m_buf[nPos+1];	uc[3] = m_buf[nPos];
					uc[0] = p[nPos+3];	uc[1] = p[nPos+2];	uc[2] = p[nPos+1];	uc[3] = p[nPos];
				}

				float *f = (float*)uc;
				fVal = *f;

				return Ret_Success;
			}
			nPos += 4;
		}

		return Ret_GetValueFailed;
	}

	return Ret_InvalidDicomVR;

}

int CHsBaseFile::Hs_GetStringValue(pHsElement pEle, QString &sRet,	int nIndex)
{
	sRet = "";

	if(pEle==NULL)
		return Ret_InvalidPara;

	if(pEle->pValue == NULL && m_buf == NULL)
		return Ret_InvalidPara;

	char *pString = new char[pEle->nLen+1];
	memset(pString,0,pEle->nLen+1);

	if(m_buf)//这种情况是在程序最开始时，获取传输语法调用的，这时候是有m_buf并且没有pEle->pValue的
		memcpy(pString,   &(m_buf[pEle->nTagPos+pEle->nOffset]), pEle->nLen);
	else 
		memcpy(pString,   pEle->pValue, pEle->nLen);


	QString sValue = QString(QLatin1String(pString));
	QStringList sections = sValue.split("\\");
	sRet = sections.at(nIndex).trimmed();

	delete pString;

	return Ret_Success;
}

int CHsBaseFile::Hs_GetAgeValue(pHsElement pEle,int &nAge,char &cAgeType)
{
	if(pEle == NULL)
		return Ret_InvalidPara;

	if(pEle->nLen==0 || pEle->pValue == NULL)
		return Ret_NoValue;
	

	char *cNumber = new char[pEle->nLen+1];
	memset(cNumber,0,pEle->nLen+1);


	for (unsigned long i=0;i<pEle->nLen;i++)
	{
		char c = pEle->pValue[i];
		if(c>='0' && c<='9')
		{
			cNumber[strlen(cNumber)] = c;
		}
		else
		{
			cAgeType = c;
			break;
		}
	}

	QString stemp = QString(QLatin1String(cNumber));
	nAge = stemp.toInt();
	delete []cNumber;

	return Ret_Success;
}
int CHsBaseFile::Hs_GetDateValue(pHsElement pEle, HsDateTime &DateValue, int nValueIndex)
{
	//D:\WorkRoom\DicomServer\2009\vss0\加速终结版\Debug\temp_WorkList_Come.dic
	//E:\1图片\Pet-CT\CBK_NECK\34969671
	memset(&DateValue,0,sizeof(HsDateTime));

	

	if(pEle==NULL || nValueIndex<0)
		return Ret_InvalidPara;

	if(pEle->nLen==0 || pEle->pValue==NULL)
		return Ret_NoValue;

	//整理一下Buf
	char *pString = new char[pEle->nLen+1];
	memset(pString,0,pEle->nLen+1);

	int k = 0;
	for(unsigned long i=0;i<pEle->nLen;i++)
	{			
		//char c = m_buf[pEle->nTagPos + pEle->nOffset + i];
		char c = pEle->pValue[i];

		if(c>='0' && c<='9')
			pString[k++] = c;
	}

	QString sValue = QString(QLatin1String(pString)); 
	delete []pString;

	if ((nValueIndex+1)*8 > int( sValue.size() )  )
		return Ret_OutOfValueCount;

	QString sDate = sValue.mid(nValueIndex*8,8);
	DateValue.nYear = sDate.mid(0, 4).toInt();
	DateValue.nMonth = sDate.mid(4, 2).toInt();
	DateValue.nDay = sDate.mid(6,2).toInt();

	return Ret_Success;
}
int CHsBaseFile::Hs_GetTimeValue(pHsElement pEle,	HsDateTime &TimeValue,	int nValueIndex)
{
	memset(&TimeValue,0,sizeof(HsDateTime));

	if(pEle == NULL)
		return Ret_InvalidPara;
	if(pEle->nLen==0 || pEle->pValue == NULL)
		return Ret_NoValue;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;

	char *pString = new char[pEle->nLen+1];
	memset(pString,0,pEle->nLen+1);

	int k = 0;
	for(unsigned long i=0;i<pEle->nLen;i++)
	{
		//char c = m_buf[pEle->nTagPos + pEle->nOffset + i];
		char c = pEle->pValue[i];
		if( (c>='0' && c<='9') || c=='.' )
			pString[k++] = c;
		else
		{
			if(c!=0 && c!=' ')
				pString[k++] = '\\';//就怕有些变态Dcm文件写成'-',所以才这么麻烦,真的有这么写的,我见过
		}
	}

	QString sValue = QString(QLatin1String(pString));
	QStringList sections = sValue.split("\\");
	sValue = sections.at(nValueIndex).trimmed();
	delete []pString;

	int nDot = sValue.indexOf('.',0);
	if(nDot==6)
	{
		TimeValue.nHours = sValue.mid(0,2).toInt();
		TimeValue.nMinutes = sValue.mid(2,2).toInt();
		TimeValue.nSeconds = sValue.mid(4,2).toInt();

		QString sMillisecond = sValue.mid(7,sValue.size() - 7);
		TimeValue.nFractions = sMillisecond.toLong();

		return Ret_Success;
	}
	else if (nDot==-1)//1010也可以表示为10:10分
	{
		int nMemID = 0;

		int nPos = 0;
		while(1)
		{
			if(nPos >= sValue.size()-1)
				break;

			QString s = sValue.mid(nPos,2);
			nPos+=2;

			if (nMemID==0)//时
				TimeValue.nHours = s.toInt();
			else if(nMemID==1)//分
				TimeValue.nMinutes = s.toInt();
			else if(nMemID==2)//秒
				TimeValue.nSeconds = s.toInt();
			else
				break;

			nMemID++;
		}

		return Ret_Success;
	}
	else
	{
		return Ret_InvalidBuf;
	}

	return Ret_Success;
}
int CHsBaseFile::Hs_GetDateTimeValue(pHsElement pEle,	HsDateTime &DateTimeValue,	int nValueIndex)
{
	memset(&DateTimeValue,0,sizeof(HsDateTime));

	//防护
	if(pEle == NULL)
		return Ret_InvalidPara;
	if(pEle->nLen==0 || pEle->pValue == NULL)
		return Ret_NoValue;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;

	//过滤字符
	char *pString = new char[pEle->nLen+1];
	memset(pString,0,pEle->nLen+1);

	int k = 0;
	for(unsigned long i=0;i<pEle->nLen;i++)
	{
		//char c = m_buf[pEle->nTagPos + pEle->nOffset + i];
		char c = pEle->pValue[i];
		if( (c>='0' && c<='9') || c=='.' || c=='+' || c=='-')
			pString[k++] = c;
		else
		{
			if(c!=0 && c!=' ')
				pString[k++] = '\\';//就怕有些变态Dcm文件写成'-',所以才这么麻烦
		}
	}

	//得到相应ID的字符串
	QString sValue = QString(QLatin1String(pString));
	QStringList sections = sValue.split("\\");
	sValue = sections.at(nValueIndex).trimmed();
	delete []pString;


	//分析得到的字符串
	int nMemID = 0;

	int nPos = 0;
	while(1)
	{
		if(nPos >= sValue.size()-1)
			break;

		QString s = "";

		if (nMemID==0)//年
		{
			s = sValue.mid(nPos,4);
			nPos += 4;
			DateTimeValue.nYear = s.toInt();
		}
		else if(nMemID==1)//月
		{
			s = sValue.mid(nPos,2);
			nPos += 2;
			DateTimeValue.nMonth = s.toInt();
		}
		else if(nMemID==2)//日
		{
			s = sValue.mid(nPos,2);
			nPos += 2;
			DateTimeValue.nDay = s.toInt();
		}
		else if (nMemID==3)//时
		{
			s = sValue.mid(nPos,2);
			nPos += 2;
			DateTimeValue.nHours = s.toInt();
		}
		else if (nMemID==4)//分
		{
			s = sValue.mid(nPos,2);
			nPos += 2;
			DateTimeValue.nMinutes = s.toInt();
		}
		else if (nMemID==5)//秒
		{
			s = sValue.mid(nPos,2);
			nPos += 2;
			DateTimeValue.nSeconds = s.toInt();
		}
		else if (nMemID==6)//"." 或 "+" "-"
		{
			if(sValue[nPos]!='.')//没有毫秒部分则结束.至于偏移问题-----不会,不想搞
				break;

			//有点符号的情况
			int tPos = max( (int)sValue.indexOf('+'), (int)sValue.indexOf('-') );
			int nEnd = 0;
			if(tPos>nPos)//找到了
				nEnd = tPos;
			else
				nEnd = int(sValue.size()) - 1;
			
			s = "0";
			s += sValue.mid(nPos,nEnd-nPos);

			DateTimeValue.nFractions = s.toFloat() * 1000000;

			break;

		}

		nMemID++;
	}


	return Ret_Success;
}

int CHsBaseFile::Hs_GetLongValue(pHsElement pEle, long &nValue, int nValueIndex)
{
	nValue = 0;

	if(pEle==NULL)
		return Ret_InvalidPara;
	if (pEle->nLen==0 || pEle->pValue==NULL)
		return Ret_NoValue;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;

	BYTE* p = pEle->pValue;
	//unsigned long nPos = pEle->nTagPos + pEle->nOffset;
	unsigned long nPos = 0;

	if (pEle->nVR == VR_UL)
	{
		if(nValueIndex*4>=pEle->nLen)
			return Ret_OutOfValueCount;
		nPos += nValueIndex*4;

		//unsigned char uc[4] = {m_buf[nPos],m_buf[nPos+1],m_buf[nPos+2],m_buf[nPos+3]};
		unsigned char uc[4] = {p[nPos],p[nPos+1],p[nPos+2],p[nPos+3]};
		if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			uc[0] = p[nPos+3];	uc[1] = p[nPos+2];	uc[2] = p[nPos+1];	uc[3] = p[nPos];
		}

		unsigned long *r = (unsigned long*)uc;
		nValue = long(*r);
	}
	if (pEle->nVR == VR_SL)
	{
		if(nValueIndex*4>=pEle->nLen)
			return Ret_OutOfValueCount;
		nPos += nValueIndex*4;

		//unsigned char uc[4] = {m_buf[nPos],m_buf[nPos+1],m_buf[nPos+2],m_buf[nPos+3]};
		unsigned char uc[4] = {p[nPos],p[nPos+1],p[nPos+2],p[nPos+3]};
		if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			//uc[0] = m_buf[nPos+3];	uc[1] = m_buf[nPos+2];	uc[2] = m_buf[nPos+1];	uc[3] = m_buf[nPos];
			uc[0] = p[nPos+3];	uc[1] = p[nPos+2];	uc[2] = p[nPos+1];	uc[3] = p[nPos];
		}

		long *r = (long*)uc;
		nValue = long(*r);
	}
	if (pEle->nVR == VR_SS)
	{
		if(nValueIndex*2>=pEle->nLen)
			return Ret_OutOfValueCount;
		nPos += nValueIndex*2;

		//unsigned char uc[2] = {m_buf[nPos],m_buf[nPos+1]};
		unsigned char uc[2] = {p[nPos],p[nPos+1]};
		if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			//uc[0] = m_buf[nPos+1];	
			//uc[1] = m_buf[nPos];
			uc[0] = p[nPos+1];	
			uc[1] = p[nPos];
		}

		short *r = (short *)uc;
		nValue = long(*r);
	}
	if (pEle->nVR == VR_US)
	{
		if(nValueIndex*2>=pEle->nLen)
			return Ret_OutOfValueCount;
		nPos += nValueIndex*2;

		//unsigned char uc[2] = {m_buf[nPos],m_buf[nPos+1]};
		unsigned char uc[2] = {p[nPos],p[nPos+1]};
		if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			//uc[0] = m_buf[nPos+1];	uc[1] = m_buf[nPos];
			uc[0] = p[nPos+1];	uc[1] = p[nPos];
		}

		unsigned short *r = (unsigned short *)uc;
		nValue = long(*r);
	}

	if (pEle->nVR == VR_SH || pEle->nVR == VR_IS)
	{
		int nRet = Ret_OutOfValueCount;

		char *pString = new char[pEle->nLen+1];
		memset(pString,0,pEle->nLen+1);

		//memcpy(pString,&(m_buf[pEle->nTagPos+pEle->nOffset]),pEle->nLen);
		memcpy(pString,p,pEle->nLen);

		QString sValue = QString(QLatin1String(pString));
		QStringList sections = sValue.split("\\");
		nValue = sections.at(nValueIndex).trimmed().toInt();
		delete []pString;

		return nRet;

	}

	if (pEle->nVR==VR_OB)
	{
		if(nValueIndex>pEle->nLen)
			return Ret_OutOfValueCount;

		//nValue = long(m_buf[pEle->nTagPos+pEle->nOffset+nValueIndex]);
		nValue = long(p[nValueIndex]);
	}

	if (pEle->nVR==VR_OW)
	{
		if(nValueIndex>pEle->nLen/2)
			return Ret_OutOfValueCount;

		//unsigned long nPos = pEle->nTagPos + pEle->nOffset + 2*nValueIndex;
		nPos = 2*nValueIndex;

		//unsigned char uc[2] = {m_buf[nPos],m_buf[nPos+1]};
		unsigned char uc[2] = {p[nPos], p[nPos+1]};
		if(pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			//uc[0] = m_buf[nPos+1];	
			//uc[1] = m_buf[nPos];
			uc[0] = p[nPos+1];	
			uc[1] = p[nPos];
		}

		unsigned short *r = (unsigned short*)uc;
		nValue = long(*r);
	}

	return Ret_Success;
}

#pragma endregion 后期


//Dcm文件数据起始位置(第一个Tag的位置)
bool CHsBaseFile::GetFirstTagPos(void)
{
	if (m_buf==NULL)
		return false;

	if(m_BaseFileInfo.nReadLen<4)
		return false;

	long iNotZeroPos = -1;
	for (long i=0;i<m_BaseFileInfo.nReadLen-3;i++)
	{
		if(m_buf[i]!=0)//第一个非0字节
		{
			if(iNotZeroPos==-1)
				iNotZeroPos = i;
		}
		if(m_buf[i]=='D' && m_buf[i+1]=='I' && m_buf[i+2]=='C' && m_buf[i+3]=='M')
		{
			m_BaseFileInfo.nCurPos = i+4;
			return true;
		}
	}
	
	m_BaseFileInfo.nCurPos = iNotZeroPos;

	return true;

}

int CHsBaseFile::Hs_LoadFile(const char *cFileName,bool bFullLoad)
{//载入文件,成功返回TRUE,Recode=0;失败返回FALSE,Recode=错误代码
	Hs_CloseFile();
	FreeBuffer();
	delete m_pMainEle;
	m_pMainEle = NULL;

	if(cFileName==NULL)//为空不行
		return Ret_InvalidPara;

	//if (_access(cFileName,0)!=0)//文件不存在不行---不判断了，网络路径的话浪费时间，反正后面err会报错
	//	return Ret_InvalidPara;

	errno_t err = fopen_s( &m_fp, cFileName, "rb" );
	if( err !=0 )
		return Ret_LoadFileError;


	fseek(m_fp,0L,SEEK_SET); 
	fseek(m_fp,0L,SEEK_END);

	long nFullSize = ftell(m_fp);  //文件长度
	if ( nFullSize <= 4 )//文件太小不行
	{
		if(m_fp)
		{	
			fclose(m_fp);
			m_fp = NULL;
		}

		return Ret_LoadFileError;
	}

	m_BaseFileInfo.sFileName = cFileName;
	m_BaseFileInfo.nFullSize = nFullSize;


	//下面一直读,直到遇到像素(缩略图像素当然不算),或者读完整个文件.以便将所有Tag读出来并做好树形结构
	long nToRead = m_nDefaultReadSize;

	while(1)
	{
		FreeBuffer();
		m_pPreEle = NULL;

		if (bFullLoad)
			nToRead = nFullSize;
		else
			nToRead = min(nFullSize , nToRead);//为了提高速度,不全部读取.

		m_buf = new unsigned char[nToRead]; //字节存放,分配内存空间
		m_nBufLen = nToRead;

		fseek(m_fp,0L,SEEK_SET);
		size_t nReadCount = fread(m_buf, nToRead, 1,m_fp);

		if(nReadCount != 1)
		{
			FreeBuffer();

			if(m_fp)
			{	
				fclose(m_fp);
				m_fp = NULL;
			}

			return Ret_LoadFileError;
		}


		m_BaseFileInfo.nReadLen = (unsigned long)nToRead;

		if (GetFirstTagPos()==false)
		{
			Hs_CloseFile();
			FreeBuffer();
			return Ret_InvalidDicomFile;
		}

		//先读取传输语法
		if (GetTsType()!=Ret_Success)
		{
			FreeBuffer();
			Hs_CloseFile();

			return Ret_InvalidDicomFile;
		}


		if(m_pMainEle)
			delete m_pMainEle;

		m_pMainEle = new HsElement;
		m_pMainEle->nTag = 0;
		m_pMainEle->nTagPos = m_BaseFileInfo.nCurPos;

		int nRet = GetChildElement(m_BaseFileInfo.nCurPos,m_pMainEle);
		if (nRet>0)
			break;

		if (nRet==Ret_ReachPixelData)
			break;

		if (nRet==Ret_ReachFileEnd)
			break;

		if (nRet==Ret_ReachBufferEnd)
		{
			if (m_BaseFileInfo.nReadLen >= m_BaseFileInfo.nFullSize)
			{
				break;
			}
			else
			{
				nToRead = 2*nToRead;
				continue;
			}
		}
	}

	//整理出list形式
	BuildListRelation(m_pMainEle);

	//把m_buf分发给各个Tag
	int nd = 0;
	pHsElement pEle = m_pMainEle;
	while(pEle)
	{
		if(pEle->pValue)
			delete []pEle->pValue;
		pEle->pValue = NULL;
		
		if (pEle->nVR == VR_SQ || pEle->nTag == TAG_ITEM)
		{
			pEle = pEle->pNextEle;
			continue;
		}
		if (pEle->nTag == TAG_PIXEL_DATA)
		{
			pEle = pEle->pNextEle;
			continue;
		}
		if (pEle->nTag == 0x56000020UL)
		{
			pEle = pEle->pNextEle;
			continue;
		}

		if (pEle->pChildEleV.empty() == false)//含有子tag
		{
			pEle = pEle->pNextEle;
			continue;
		}

		if (pEle->nLen > 0xFFFFFFFF-1)
		{
			pEle = pEle->pNextEle;
			continue;
		}

	
		//私有Tag不读了
		unsigned long uGp = pEle->nTag>>16;
		if (uGp%2!=0)
		{//1,731,896
			pEle = pEle->pNextEle;
			continue;
		}

		int nRet = 0;
		unsigned long nByte = 0;
		BYTE* pByte = GetByteFromBuffer(pEle,nByte,nRet);
		pEle->pValue = pByte;

		//if(pEle->nTag == TAG_SOP_INSTANCE_UID)
		//{
		//	for (int i=0;i<pEle->nLen;i++)
		//	{
		//		if(i==55)
		//			int g = 0;
		//		AtlTrace("%02d: --------  [%c] -------  [%d] --------  [%x]\r\n",i,pByte[i],pByte[i],pByte[i]);
		//	}

		//}

		pEle = pEle->pNextEle;
	}

	delete []m_buf;
	m_buf = NULL;

	if(m_fp)//需要关闭，因为不能太多FILE*？HMY发现的
	{	
		fclose(m_fp);
		m_fp = NULL;
	}


	int nRet = Hs_GetStringValueA(TAG_MODALITY,m_sModality);
	if( nRet == 0)
		IsXRay();//这里确定一下是不是普放图像

	return 0;

}

int CHsBaseFile::GetTsType()
{
	int nRet = Ret_InvalidDicomFile;
	QString sTransferSnaxType = "";

	unsigned long  nTag = TAG_TRANSFER_SYNTAX_UID;

	//for (unsigned long nPos=0;nPos<m_BaseFileInfo.nReadLen-3;nPos++)
	unsigned long nPos = 0;
	while(nPos < m_BaseFileInfo.nReadLen)
	{//00 02 00 10 TAG_TRANSFER_SYNTAX_UID

		if (m_buf[nPos]==0x02 && m_buf[nPos+1]==0x00 && m_buf[nPos+2]==0x10 && m_buf[nPos+3]==0x00)//这个Tag总是以小端形式存放
		{
			bool bShowVR;
			unsigned long nVR = GetTagVR(nPos+4,nTag,bShowVR,nRet);
			pHsElement pEle = GetNormalElement(nPos,nTag,nVR,false,bShowVR,nRet);

			if(pEle==NULL)
				break;
			if(nRet!=0)
				break;

			nRet = Hs_GetStringValue(pEle,sTransferSnaxType,0);

			delete pEle;
			break;
		}
		else
		{
			nPos++;
		}

	}

	if(sTransferSnaxType.compare("1.2.840.10008.1.2")==0)			m_BaseFileInfo.nTsType = TS_IMPLICIT_VR_LITTLE_ENDIAN;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.1")==0)	m_BaseFileInfo.nTsType = TS_EXPLICIT_VR_LITTLE_ENDIAN;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.2")==0)	m_BaseFileInfo.nTsType = TS_EXPLICIT_VR_BIG_ENDIAN;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.5")==0)	m_BaseFileInfo.nTsType = TS_RLE_LOSSLESS;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.4.50")==0)	m_BaseFileInfo.nTsType = TS_JPEG_BASELINE_1;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.4.51")==0)	m_BaseFileInfo.nTsType = TS_JPEG_EXTENDED_2_4;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.4.57")==0)	m_BaseFileInfo.nTsType = TS_JPEG_LOSSLESS_NONHIER_14;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.4.70")==0)	m_BaseFileInfo.nTsType = TS_JPEG_LOSSLESS_NONHIER_14B;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.4.90")==0)	m_BaseFileInfo.nTsType = TS_JPEG2000_LOSSLESS_ONLY;
	else if(sTransferSnaxType.compare("1.2.840.10008.1.2.4.91")==0)	m_BaseFileInfo.nTsType = TS_JPEG2000;
	else
	{
		m_BaseFileInfo.nTsType = 0;
		//nRet = Ret_NoTransferSyntaxes;
        qDebug("\r\n没有找到传输语法默认为0");
		return Ret_Success;
	}
	//AtlTrace("\r\nTransferSnaxType:%d",m_BaseFileInfo.nTsType);
	return nRet;
}
int CHsBaseFile::GetTagNumber(unsigned long nStartPos,unsigned long &nTag,bool &bBigEndia)
{
	nTag = 0;
	if(nStartPos + 3 >= m_BaseFileInfo.nReadLen )
		return Ret_ReachBufferEnd;
	
	bBigEndia = false;
	nTag = m_buf[nStartPos+1]*16777216 + m_buf[nStartPos]*65536 + m_buf[nStartPos+3]*256 + m_buf[nStartPos+2];//小端读法

	if ( m_BaseFileInfo.nTsType==TS_EXPLICIT_VR_BIG_ENDIAN )
	{
		if (nTag==0x00020000UL || nTag==0x00020102UL || nTag==0x00020001UL || nTag==0x00020002UL || nTag==0x00020003UL || 
			nTag==0x00020010UL || nTag==0x00020012UL || nTag==0x00020013UL || nTag==0x00020016UL || nTag==0x00020100UL )
		{
			
			return Ret_Success;
		}
		else
		{
			nTag = m_buf[nStartPos+2]*256 + m_buf[nStartPos+3] + m_buf[nStartPos]*16777216 + m_buf[nStartPos+1]*65536;//大端读法
			bBigEndia = true;

			return Ret_Success;
		}	
	}
	else
	{
		return Ret_Success;
	}

}

pHsElement CHsBaseFile::GetNormalElement(unsigned long &nStartPos,unsigned long nTag,unsigned long nVR,bool bBigEndia,bool bVrFieldShow,int &nRet)
{//获取非Item类型的元素
	//if(g_bDebug)
	//	AtlTrace("\r\nNormal:%d",nStartPos);

	//if (nStartPos==112554)
	//{
	//	for (int i=nStartPos-10;i<nStartPos+1000;i++)
	//	{
	//		AtlTrace("\r\n%d:	%d	0x%02x	%c",i,m_buf[i],m_buf[i],m_buf[i]);
	//	}
	//}

	unsigned long nPos = nStartPos;

	pHsElement pRetEle = new HsElement;
	pRetEle->nTag = nTag;
	pRetEle->nTagPos = nStartPos;
	pRetEle->nVR = nVR;
	pRetEle->bBigEndian = bBigEndia;

	//========================Tag Number域:4 Byte=======================================================
	if(nPos+3>m_BaseFileInfo.nReadLen)
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachBufferEnd;
		return NULL;
	}
	nPos += 4;//越过Tag num域


	//========================  VR域: 2 Byte===========================================================
	//Do nothing!
	pDcmVR pVR = CDcmVR::Find(nVR);

	//========================Value Length域: 由数据类型决定占多少字节=======================================================
	if (bVrFieldShow)
		pRetEle->nLenDesc = pVR->nLenOfLenDes;
	else
		pRetEle->nLenDesc = 4;

	if(bVrFieldShow)
		nPos += pVR->nLenOfVrDes;//越过类型域,来到值长度描述域的第一个字节

	//当前nPos=长度域的第一个字节
	if(nPos+pVR->nLenOfLenDes > m_BaseFileInfo.nReadLen)
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachBufferEnd;
		return NULL;
	}


	unsigned long nDataSize = 0;
	if (bBigEndia==false)
	{
		if(pVR->nLenOfLenDes == 2)
			nDataSize = m_buf[nPos+1]*256 + m_buf[nPos];
		else if (pVR->nLenOfLenDes == 4)
			nDataSize = m_buf[nPos+3]*16777216 + m_buf[nPos+2]*65536 + m_buf[nPos+1]*256 + m_buf[nPos];//nDataSize = m_buf[nPos+1]*16777216 + m_buf[nPos]*65536 + m_buf[nPos+3]*256 + m_buf[nPos+2];
		else
		{
			pRetEle = DestroyEle(pRetEle) ;
			nRet = Ret_InvalidDicomVR;
			return NULL;
		}
	}
	else
	{
		if(pVR->nLenOfLenDes == 2)
			nDataSize = m_buf[nPos+1] + m_buf[nPos]*256;
		else if (pVR->nLenOfLenDes == 4)
			nDataSize = m_buf[nPos]*16777216 + m_buf[nPos+1]*65536 + m_buf[nPos+2]*256 + m_buf[nPos+3];//nDataSize = m_buf[nPos+1]*65536 + m_buf[nPos]*16777216 + m_buf[nPos+3] + m_buf[nPos+2]*256;
		else
		{
			pRetEle = DestroyEle(pRetEle) ;
			nRet = Ret_InvalidDicomVR;
			return NULL;
		}
	}

	if (bVrFieldShow==false)
		pRetEle->nOffset = 4 + pRetEle->nLenDesc;
	else
		pRetEle->nOffset = 4 + pVR->nLenOfVrDes + pRetEle->nLenDesc;

	pRetEle->nLen = nDataSize;

	if (pRetEle->nTagPos + pRetEle->nLen > m_BaseFileInfo.nFullSize  && nTag!=TAG_PIXEL_DATA)//除了Pixel Data外其余的若是
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachFileEnd;
		return NULL;
	}

	//========================Data Value值域:(&m_filebuf[nCurPos]),nDataSize=======================================================
	nPos = nStartPos + pRetEle->nOffset;//来到值域的第一个字节

	nRet = Ret_Success;
	if(nDataSize==0xFFFFFFFF && nTag==TAG_PIXEL_DATA)	//有的像素有可能长度未指定 0xFFFFFFFF.E:\1图片\大文件\HugeDcm.dic
		nRet = Ret_ReachFileEnd;
	else
		nPos += nDataSize;//越过DataValue域,进入后续数据元素的第一个字节

	nStartPos = nPos;
	if(nStartPos>=m_BaseFileInfo.nFullSize)
		nRet = Ret_ReachFileEnd;

	return pRetEle;
}

pHsElement CHsBaseFile::GetSequenceElement(unsigned long &nStartPos,unsigned long nTag,bool bBigEndia,int &nRet)
{
	if(g_bDebug)
		qDebug("\r\nSQ:%d",nStartPos);

	unsigned long nPos = nStartPos;

	pHsElement pRetEle = new HsElement;
	pRetEle->nTag = nTag;
	pRetEle->nTagPos = nStartPos;
	pRetEle->nVR = VR_SQ;
	pRetEle->bBigEndian = bBigEndia;

	//========================第一个域:Tag Number域:4 Byte=======================================================
	//Do nothing!


	nPos += 4;//越过TagNm域,来到第二个域的第一个字节
	//========================第二个域:(可能是类型域4B 或者 是长度域4B)================================================

	if(nPos+3>=m_BaseFileInfo.nReadLen)//判断防护
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachBufferEnd;
		return NULL;
	}

	if (m_buf[nPos] == 0x53 && m_buf[nPos+1] == 0x51 && m_buf[nPos+2] == 0x00 && m_buf[nPos+3] == 0x00)	//见第五章 Table 7.5-2
	{
		//========================这种情况含有类型域(4B)================================================
		
		nPos += 4;//越过类型域,来到长度描述域的第一个字节
		if(nPos+3>m_BaseFileInfo.nReadLen)//判断防护
		{
			pRetEle = DestroyEle(pRetEle) ;
			nRet = Ret_ReachBufferEnd;
			return NULL;
		}

		if (m_buf[nPos] == 0xFF && m_buf[nPos+1] == 0xFF && m_buf[nPos+2] == 0xFF && m_buf[nPos+3] == 0xFF)
			pRetEle->nLen = 0xFFFFFFFF;
		else
		{
			if (bBigEndia)
				pRetEle->nLen = m_buf[nPos]*16777216 + m_buf[nPos+1]*65536 + m_buf[nPos+2]*256 + m_buf[nPos+3];
			else
				pRetEle->nLen = m_buf[nPos] + m_buf[nPos+1]*256 + m_buf[nPos+2]*65536 + m_buf[nPos+3]*16777216;
		}
		
		pRetEle->nOffset = 12;
	}
	else if (m_buf[nPos] == 0xFF && m_buf[nPos+1] == 0xFF && m_buf[nPos+2] == 0xFF && m_buf[nPos+3] == 0xFF)//见第五章 Table 7.5-3
	{//第二个域为未定义长度的长度域:
		pRetEle->nLen = 0xFFFFFFFF;
		pRetEle->nOffset = 8;
	}
	else//那就是明确长度的长度域://见第五章 Table 7.5-1
	{

		if (bBigEndia)
			pRetEle->nLen = m_buf[nPos]*16777216 + m_buf[nPos+1]*65536 + m_buf[nPos+2]*256 + m_buf[nPos+3];//m_buf[nPos+1]*65536 + m_buf[nPos]*16777216 + m_buf[nPos+3] + m_buf[nPos+2]*256;
		else
			pRetEle->nLen = m_buf[nPos+3]*16777216 + m_buf[nPos+2]*65536 + m_buf[nPos+1]*256 + m_buf[nPos];//m_buf[nPos+1]*16777216 + m_buf[nPos]*65536 + m_buf[nPos+3]*256 + m_buf[nPos+2];

		pRetEle->nOffset = 8;
	}

	//========================值域======================================================================================
	nPos = nStartPos + pRetEle->nOffset;//来到值域的第一个字节(即所含第一个条目的第一个字节)
	if(nPos>=m_BaseFileInfo.nReadLen)
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachBufferEnd;
		return NULL;
	}

	if (pRetEle->nLen!=0xFFFFFFFF)//明确长度的
	{
		unsigned long tPos = nPos;


		while(tPos < nPos + pRetEle->nLen)
		{
			pHsElement pChildEle = GetItemElement(tPos,TAG_ITEM,bBigEndia,nRet);

			if (nRet==Ret_ReachFileEnd)
			{
				BuildTreeRelation(pChildEle,pRetEle,m_pPreEle);
				break;
			}
			else if (nRet==Ret_ReachBufferEnd)//当前buffer不够用
			{				
				pRetEle = DestroyEle(pRetEle);
				pChildEle = DestroyEle(pChildEle);
				return NULL;
			}
			else if (nRet==Ret_ReachPixelData)
			{
				BuildTreeRelation(pChildEle,pRetEle,m_pPreEle);
				break;
			}
			else if (nRet!=Ret_Success)//出错了
			{
				pRetEle = DestroyEle(pRetEle);
				pChildEle = DestroyEle(pChildEle);
				return NULL;
			}

			//确定好父子关系
			BuildTreeRelation(pChildEle,pRetEle,m_pPreEle);

			m_pPreEle = pChildEle;			
		}

		nPos = tPos;

	}
	else//未明确长度的
	{
		unsigned long tPos = nPos;

		while(1)
		{
			if(tPos+8>m_BaseFileInfo.nReadLen)//当前buffer不够用
			{
				pRetEle = DestroyEle(pRetEle) ;
				nRet = Ret_ReachBufferEnd;
				return NULL;
			}

			//说不定上来就是结束符.故先找队列结束符--不受语法影响--一直是小端
			if( (m_buf[tPos] == 0xFF && m_buf[tPos+1] == 0xFE && m_buf[tPos+2] == 0xE0 && m_buf[tPos+3] == 0xDD &&	m_buf[tPos+4] == 0x00 && m_buf[tPos+5] == 0x00 && m_buf[tPos+6] == 0x00 && m_buf[tPos+7] == 0x00) ||
				(m_buf[tPos] == 0xFE && m_buf[tPos+1] == 0xFF && m_buf[tPos+2] == 0xDD && m_buf[tPos+3] == 0xE0 &&	m_buf[tPos+4] == 0x00 && m_buf[tPos+5] == 0x00 && m_buf[tPos+6] == 0x00 && m_buf[tPos+7] == 0x00) )
			{
				tPos += 8;//越过队列结束符
				nRet = Ret_Success;
				break;
			}

			pHsElement pChildEle = GetItemElement(tPos,TAG_ITEM,bBigEndia,nRet);

			if (nRet==Ret_ReachBufferEnd)//当前buffer不够用
			{
				pRetEle = DestroyEle(pRetEle) ;
				pChildEle = DestroyEle(pChildEle) ;
				return NULL;
			}
			else if (nRet==Ret_ReachPixelData)
			{
				BuildTreeRelation(pChildEle,pRetEle,m_pPreEle);
				break;
			}
			else if (nRet==Ret_ReachFileEnd)
			{
				//确定好父子关系
				BuildTreeRelation(pChildEle,pRetEle,m_pPreEle);

				m_pPreEle = pChildEle;
				break;
			}
			else if (nRet!=Ret_Success)//出错了
			{
				pRetEle = DestroyEle(pRetEle) ;
				pChildEle = DestroyEle(pChildEle) ;
				return NULL;
			}


			//确定好父子关系
			BuildTreeRelation(pChildEle,pRetEle,m_pPreEle);

			m_pPreEle = pChildEle;
		}

		pRetEle->nLen = tPos - nPos;
		nPos = tPos;

	}

	nStartPos = nPos;

	return pRetEle;
}
pHsElement CHsBaseFile::GetItemElement(unsigned long &nStartPos,unsigned long nTag,bool bBigEndia,int &nRet)
{
	if(g_bDebug)
		qDebug("\r\nItem:%d",nStartPos);

	HsElement *pRetEle = new HsElement;
	pRetEle->nTag = TAG_ITEM;
	pRetEle->nTagPos = nStartPos;
	pRetEle->bBigEndian = bBigEndia;

	//========================第一个域:Tag Number域:4 Byte=======================================================
	//Do nothing!
	unsigned long nPos = nStartPos;//ITEM的第一个字节

	unsigned long xTag = 0;
	unsigned long xPos = nStartPos;
	bool xBigEdian = false;
	GetTagNumber(xPos,xTag,xBigEdian);
	pHsElement pVirtualItem = NULL;
	if(xTag!=TAG_ITEM)//真就你妈的服了,竟然上来就是普通元素,没用Item包含,那只好直接来到值域了
	{
		//那么此处就相当于直接越过一个虚拟Item的TagNumber域及长度域,直接来到虚拟Item的值域了
		pRetEle->nLen = 0xFFFFFFFF;
		pRetEle->nOffset = 0;

		pRetEle->bVirtualItem = true;//标记一下,这个Item是虚拟的.其实并不存在,以便在GetChildElement内判断它的结束符应该是序列的结束符.

		nRet = GetChildElement(nPos,pRetEle);
		nStartPos = nPos;
		return pRetEle;
	}

	//========================第二个域:(长度域4B)================================================================
	if(nPos+7>=m_BaseFileInfo.nReadLen)//至少得有8位
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachBufferEnd;
		return NULL;
	}
	nPos += 4;//越过TagNum域,进入长度域的第一个字节

	if (m_buf[nPos]==0xFF && m_buf[nPos+1]==0xFF && m_buf[nPos+2]==0xFF && m_buf[nPos+3]==0xFF)//未定义长度
	{
		pRetEle->nLen = 0xFFFFFFFF;
		pRetEle->nOffset = 8;
	}
	else//定义了长度
	{
		if (bBigEndia)
			pRetEle->nLen = m_buf[nPos]*16777216 + m_buf[nPos+1]*65536 + m_buf[nPos+2]*256 + m_buf[nPos+3];//m_buf[nPos+1]*65536 + m_buf[nPos]*16777216 + m_buf[nPos+3] + m_buf[nPos+2]*256;
		else
			pRetEle->nLen = m_buf[nPos+3]*16777216 + m_buf[nPos+2]*65536 + m_buf[nPos+1]*256 + m_buf[nPos];

		pRetEle->nOffset = 8;

		if (pRetEle->nTagPos + pRetEle->nLen>m_BaseFileInfo.nFullSize)
		{
			pRetEle = DestroyEle(pRetEle) ;
			nRet = Ret_ReachFileEnd;
			return NULL;
		}
	}
	//========================第三个域:(值域或Item结束符)=============================================================
	nPos += 4;//越过长度域,进入第三个域的第一个字节
	if(nPos>=m_BaseFileInfo.nReadLen)
	{
		pRetEle = DestroyEle(pRetEle) ;
		nRet = Ret_ReachBufferEnd;
		return NULL;
	}

	nRet = GetChildElement(nPos,pRetEle);

	nStartPos = nPos;

	return pRetEle;
}
unsigned long CHsBaseFile::GetTagVR(unsigned long nPos,unsigned long nTag,bool &bShowField,int &nRet)
{
	nRet = Ret_Success;
	bShowField = false;

	//======先去文件中直接读,有的Tag不止对应一种类型,所以文件中指定的类型优先采用==================================
	if(nPos+1 > m_BaseFileInfo.nReadLen)//对下面保护
	{
		nRet = Ret_ReachBufferEnd;
		return 0;
	}

	unsigned long nVR = m_buf[nPos]*256 + m_buf[nPos+1];//读出了这么个东东

	//======读到的未必就真的是VR,因为有的Tag没有VR域,我们读到的也可能是长度域的前2个字节.
	pDcmVR pVR = CDcmVR::Find(nVR);//得用VR字典判断一下
	if (pVR->nCode!=0)//字典里有定义,那么这是个合法的VR
	{
		bShowField = true;//表示此元素含有VR域
		return nVR;
	}
	else if (m_pPreEle!=NULL)
	{
		if (m_pPreEle->nTag != TAG_ITEM && m_pPreEle->nVR!=VR_SQ && m_pPreEle->nTag!=TAG_PIXEL_DATA && m_pPreEle->nLen!=0xFFFFFFFF && m_pPreEle->nLen%2!=0)//如果上一个元素不是特殊元素并且长度是奇数(一般不会为奇数,若为奇数上一个元素和可能最后补0了)
		{
			//尝试向后挪一字节再试试.
			if(nPos+2 > m_BaseFileInfo.nReadLen)//对下面保护
			{
				nRet = Ret_ReachBufferEnd;
				return 0;
			}

			nVR = m_buf[nPos+1]*256 + m_buf[nPos+2];//读出了这么个东东
			pDcmVR pVR1 = CDcmVR::Find(nVR);//得用VR字典判断一下
			if (pVR1->nCode!=0)//字典里有定义,那么这是个合法的VR
			{			
				bShowField = true;//表示此元素含有VR域
				nRet = Ret_FindOddLen;
				return nVR;
			}
		}
	}
	
	//======若读到的VR非法,那认为是没有VR域,再去Tag字典中查找.采用Tag对应的类型
	nVR = CDcmTag::Find(nTag)->nVR;

	if(nVR!=0)//找到了
		return nVR;//直接用了.

	//======Tag字典中没找到这个Tag,那说明我们的字典还是不够哇,算了,就作为没有VR域的,类型未知来处理(这一般是私有Tag,长度域一般都是4.VR字典中VR_00已定义了4的长度)
	nVR = VR_00;
	//标准 Table 7.5-3 中说，如果一个Tag后面连跟4个FF，那么这是个SQ
	if(m_buf[nPos] == 0xFF && m_buf[nPos+1] == 0xFF && m_buf[nPos+2] == 0xFF && m_buf[nPos+3] == 0xFF)
	{
		nVR = VR_SQ;
		bShowField = false;
	}

	return nVR;
}


// 获取pParentEle所含的所有Element(父元素值域的第一个字节,父元素)
int CHsBaseFile::GetChildElement(unsigned long &nPos,pHsElement pParentEle)
{//本函数是获取某父元素的所有下一级子元素.情况1：整个文件可视为一个大的父元素.情况2：某Item可以作为父元素.至于SQ不适用,因为SQ只含有较为特殊的ITEM
	if(g_bDebug)
		qDebug("\r\nChild:%d",nPos);

	if (pParentEle==NULL)
		return Ret_InvalidPara;

	if (pParentEle->nTag == TAG_ITEM && pParentEle->nLen==0)
		return Ret_Success;


	int nRet = Ret_Success;
	unsigned long nTag = 0;
	unsigned long nCurPos = nPos;
	bool bBigEndia = false;

	while(true)
	{
		if (pParentEle->nTag==TAG_ITEM)//如果现在是在处理Item下的第一级元素
		{
			if (pParentEle->bVirtualItem)//是否是虚拟的Item.
			{
				if (nCurPos+7>=m_BaseFileInfo.nReadLen)
				{
					nRet = Ret_ReachBufferEnd;
					break;
				}

				//虚拟的Item停止的位置不是Item结束符,而是其父SQ的序列结束符
				if ((m_buf[nCurPos]==0xFF && m_buf[nCurPos+1]==0xFE && m_buf[nCurPos+2]==0xE0 && m_buf[nCurPos+3]==0xDD &&	m_buf[nCurPos+4]==0x00 && m_buf[nCurPos+5]==0x00 && m_buf[nCurPos+6]==0x00 && m_buf[nCurPos+7]==0x00) || 
					(m_buf[nCurPos]==0xFE && m_buf[nCurPos+1]==0xFF && m_buf[nCurPos+2]==0xDD && m_buf[nCurPos+3]==0xE0 &&	m_buf[nCurPos+4]==0x00 && m_buf[nCurPos+5]==0x00 && m_buf[nCurPos+6]==0x00 && m_buf[nCurPos+7]==0x00) )//SQ的序列分界符结束
				{					
					break;//到此结束即可,不必跨域.这相当于跨越了这个虚拟的Item结束符.
				}
			}
			else//这些是正宗的Item了:
			{
				if (pParentEle->nLen==0xFFFFFFFF)//若没指定长度
				{
					if (nCurPos+7>=m_BaseFileInfo.nReadLen)
					{
						nRet = Ret_ReachBufferEnd;
						break;
					}
					if (	(m_buf[nCurPos]==0xFF && m_buf[nCurPos+1]==0xFE && m_buf[nCurPos+2]==0xE0 && m_buf[nCurPos+3]==0x0D &&	m_buf[nCurPos+4]==0x00 && m_buf[nCurPos+5]==0x00 && m_buf[nCurPos+6]==0x00 && m_buf[nCurPos+7]==0x00) || 
							(m_buf[nCurPos]==0xFE && m_buf[nCurPos+1]==0xFF && m_buf[nCurPos+2]==0x0D && m_buf[nCurPos+3]==0xE0 &&	m_buf[nCurPos+4]==0x00 && m_buf[nCurPos+5]==0x00 && m_buf[nCurPos+6]==0x00 && m_buf[nCurPos+7]==0x00) )//以Item分界符结束
					{
						nCurPos += 8;//越过Item的结束符
						break;
					}

				}
				else//若指定了长度
				{
					if (nCurPos>=m_BaseFileInfo.nReadLen)
					{
						nRet = Ret_ReachBufferEnd;
						break;
					}

					if (nCurPos >= pParentEle->nTagPos + pParentEle->nOffset + pParentEle->nLen)
					{
						nRet = Ret_Success;
						break;
					}

				}
			}

		}
		else
		{
			if (nCurPos>=m_BaseFileInfo.nReadLen)
			{
				nRet = Ret_ReachBufferEnd;
				break;
			}			
		}


 		nRet = GetTagNumber(nCurPos,nTag,bBigEndia);

		if (nTag == 2145386512)
		{
			int a = 0;
		}

		if(nTag == 0x00190019)
		int dt = 0;

		if( nRet== Ret_ReachBufferEnd)//读完当前Buffer了
		{
			qDebug("\r\nnRet== Ret_ReachBufferEnd");
			break;
		}
		else if (nRet!=Ret_Success)//报错了
		{
			qDebug("\r\nnRet== %d",nRet);
			break;
		}
		else//成功了,继续
		{
			//AtlTrace("\r\n0x%08x:%s,",nTag,CDcmTag::Find(nTag)->pszName);
		}

		pHsElement pEle = NULL;

		//为了获取VR做的防护
		if (nCurPos+5 > m_BaseFileInfo.nReadLen)
		{
			nRet = Ret_ReachBufferEnd;
			break;
		}

		bool bShowVR;
		unsigned long nVR = GetTagVR(nCurPos+4,nTag,bShowVR,nRet);
//=============有些Tag是UNKNOWN型的，却含有子Tag：Item的，就要这样处理=====
		if (nVR == VR_UN)
		{
			if (nTag == 0x0019108A || nTag == 0x0040100A || nTag == 0x00081250 || nTag == 0x0040100A || nTag == 0x7E011010)
			{
				m_buf[nCurPos+4] = 'S';
				m_buf[nCurPos+5] = 'Q';
				m_buf[nCurPos+6] = 0;
				m_buf[nCurPos+7] = 0;
				nVR = VR_SQ;
			}
		}

//==========================================================================
		if (nVR==0)
			break;

		if (nRet ==	Ret_FindOddLen)//发现值长度为奇数导致的问题,并尝试修正读取成功
		{	
			//修正上一个Ele
			m_pPreEle->nLen+=1;
			nCurPos+=1;
			nRet = GetTagNumber(nCurPos,nTag,bBigEndia);
		}

		if ( nVR == VR_SQ)
		{
			pEle = GetSequenceElement(nCurPos,nTag,bBigEndia,nRet);
			if( nRet== Ret_ReachBufferEnd || nRet== Ret_ReachFileEnd)//读完当前Buffer了
			{
				if(pEle)
					BuildTreeRelation(pEle,pParentEle,m_pPreEle);
				break;
			}
			else if (nRet== Ret_ReachPixelData)
			{
			}
			else if (nRet!=Ret_Success)//报错了
			{
				pEle = DestroyEle(pEle) ;

				pEle = NULL;
				break;
			}
			else//成功了,继续
			{

			}
		}
		else if (nTag == TAG_ITEM)//少见
		{
			pEle = GetItemElement(nCurPos,nTag,bBigEndia,nRet);
			if( nRet== Ret_ReachBufferEnd || nRet== Ret_ReachFileEnd)//读完当前Buffer了
			{
				if(pEle)
					BuildTreeRelation(pEle,pParentEle,m_pPreEle);
				break;
			}
			else if (nRet!=Ret_Success)//报错了
			{
				pEle = DestroyEle(pEle) ;
				break;
			}
			else//成功了,继续
			{

			}
		}
		else
		{
			pEle = GetNormalElement(nCurPos,nTag,nVR,bBigEndia,bShowVR,nRet);

			if( nRet== Ret_ReachBufferEnd || nRet== Ret_ReachFileEnd)//读完当前Buffer了
			{
				if(pEle)
					BuildTreeRelation(pEle,pParentEle,m_pPreEle);
				break;
			}
			else if (nRet!=Ret_Success)//报错了
			{
				pEle = DestroyEle(pEle);

				break;
			}
			else//成功了,继续
			{
				if (nTag == TAG_PIXEL_DATA)//遇到像素时
				{
					//if (pParentEle->nTag != TAG_ITEM)//如果父元素不是Item,那么要考究一下是否到达文件末尾
					{
						if (pEle->nTagPos + pEle->nOffset + pEle->nLen > m_BaseFileInfo.nFullSize - Len_AfterPixelData)
						{
							//也要存起来,插到树中
							BuildTreeRelation(pEle,pParentEle,m_pPreEle);

							nRet = Ret_ReachPixelData;
							break;
						}
					}
				}

			}
		}

		BuildTreeRelation(pEle,pParentEle,m_pPreEle);
		m_pPreEle = pEle;
	}

	nPos = nCurPos;
	if(nPos==m_BaseFileInfo.nFullSize)
		nRet = Ret_ReachFileEnd;

	return nRet;
}


void CHsBaseFile::BuildTreeRelation(pHsElement pCurEle,pHsElement pParentEle,pHsElement pPreEle)
{
	//if (pPreEle)
	//	pPreEle->pNextEle = pCurEle;

	//pCurEle->pPreEle = pPreEle;

	pParentEle->pChildEleV.push_back(pCurEle);

	pCurEle->pParentEle = pParentEle;

}

pHsElement CHsBaseFile::Hs_FindFirstEle(pHsElement pSiblingEle,unsigned long nTag,bool bAsTree)
{
	if(m_pMainEle==NULL)
		return NULL;

	if (m_pMainEle->pChildEleV.empty()==true)
		return NULL;
	
	if (bAsTree==true)//树形查找，表示在pSiblingEle同级查找
	{
		if (pSiblingEle==NULL)//若为空，表示在第一级查找
			pSiblingEle = m_pMainEle->pChildEleV[0];

		pHsElement pParent = pSiblingEle->pParentEle;
		int n = pParent->pChildEleV.size();

		for (int i=0;i<n;i++)
		{
			if (pParent->pChildEleV[i]->nTag == nTag)
				return pParent->pChildEleV[i];
		}

		return NULL;
	}
	else
	{
		pHsElement pEle = m_pMainEle->pChildEleV[0];

		while(1)
		{
			if (pEle->nTag == nTag)
				return pEle;
			else
				pEle = pEle->pNextEle;

			if(pEle==NULL)
				return NULL;
		}
	}

	return NULL;
}

pHsElement CHsBaseFile::Hs_FindNextEle(pHsElement pEle,bool bTree)
{//自pEle开始向下,查找与pElen的Tag相等的元素.bTree==true:只在同级查找,否则作为list查找整个文件
	if (pEle==NULL)
		return NULL;

	if(m_pMainEle==NULL)
		return NULL;

	if(m_pMainEle->pChildEleV.empty()==true)
		return NULL;

	if (bTree)
	{
		pHsElement pParentEle = pEle->pParentEle;

		int  n = pParentEle->pChildEleV.size();

		for (int i=0;i<n;i++)
		{
			if (pParentEle->pChildEleV[i]->nTag == pEle->nTag && pParentEle->pChildEleV[i]->nTagPos > pEle->nTagPos)
				return pParentEle->pChildEleV[i];
		}

	}
	else
	{
		pHsElement p = m_pMainEle->pChildEleV[0];

		while(1)
		{
			if (p->nTag == pEle->nTag && p->nTagPos > pEle->nTagPos)
				return p;
			else
				p = p->pNextEle;

			if(p==NULL)
				return NULL;
		}
	}

	return NULL;
}

pHsElement CHsBaseFile::Hs_GetChildEle(pHsElement pEle,unsigned long nTag,int nIndex)
{//返回pEle的直接下级元素中,第nIndex个,或tag号=nTag的元素
	if (pEle==NULL)
		pEle = m_pMainEle;

	if(pEle == NULL)
		return NULL;

	int n = int(pEle->pChildEleV.size());
	if (nTag>0)//认为是在pEle直接子元素中查找tag号=nTag的元素
	{
		for (int i=0;i<n;i++)
		{
			if(pEle->pChildEleV[i]->nTag==nTag)
				return pEle->pChildEleV[i];
		}
	}
	else//认为是要获得nIndex号元素,不关心其Tag号
	{
		if (nIndex>=n || nIndex<0)
			return NULL;

		return pEle->pChildEleV[nIndex];
	}


	return NULL;

}
pHsElement CHsBaseFile::Hs_FindPreEle(pHsElement pEle,bool bTree)
{//自pEle向上查找与pELe的nTag相同的元素.bTree==true:只在pEle同级中查找,否则以list形式查找整个文件
	if (pEle==NULL)
		return NULL;

	if(m_pMainEle==NULL)
		return NULL;

	if(m_pMainEle->pChildEleV.empty()==true)
		return NULL;

	if (bTree)
	{
		pHsElement pParentEle = pEle->pParentEle;

		int n = pParentEle->pChildEleV.size();

		for (int i=0;i<n;i++)
		{
			if (pParentEle->pChildEleV[i]->nTag == pEle->nTag && pParentEle->pChildEleV[i]->nTagPos < pEle->nTagPos)
				return pParentEle->pChildEleV[i];
		}
	}
	else
	{
		pHsElement p = m_pMainEle->pChildEleV[0];

		while(1)
		{
			if (p->nTag == pEle->nTag && p->nTagPos < pEle->nTagPos)
				return p;
			else
				p = p->pPreEle;

			if(p==NULL)
				return NULL;
		}
	}

	return NULL;
}
int CHsBaseFile::DivString(char* pString,const char* pDelChar,int nID, QString &sValue)
{
    sValue = QString(QLatin1String(pString));
	QStringList sections = sValue.split(pDelChar);
	if (nID >= sections.size())
		return Ret_OutOfValueCount;

	sValue = sections.at(nID).trimmed();
	return Ret_Success;
}

int CHsBaseFile::Hs_GetValueCount(pHsElement pEle,int &nRet)
{
	if (pEle == NULL)
	{
		nRet = Ret_InvalidPara;
		return 0;
	}

	nRet = Ret_Success;
	if (pEle->nVR == VR_SQ || pEle->nTag==TAG_ITEM)
		return int(pEle->pChildEleV.size());

	if (pEle->nLen==0 || pEle->pValue == NULL)
		return 0;

	if(pEle->nVR == VR_LT || pEle->nVR == VR_ST ||pEle->nVR == VR_UN || pEle->nVR == VR_UT )
		return 1;



	//定长类型
	if (pEle->nVR == VR_AT || pEle->nVR == VR_AS ||
		pEle->nVR == VR_FL || pEle->nVR == VR_FD || 
		pEle->nVR == VR_SL || pEle->nVR == VR_SS || 
		pEle->nVR == VR_UL || pEle->nVR == VR_US)
	{
		int nSingleLen = CDcmVR::Find(pEle->nVR)->nValueLeng;

		return pEle->nLen/nSingleLen;
	}
	
	//字符串类型
	if (pEle->nVR == VR_DS || pEle->nVR == VR_PN ||
		pEle->nVR == VR_AE || pEle->nVR == VR_CS ||
		pEle->nVR == VR_IS || pEle->nVR == VR_LO ||
		pEle->nVR == VR_SH || pEle->nVR == VR_UI ||		pEle->nVR == VR_OF)
	{
		int nLen = pEle->nLen+1;
		char *pString = new char[nLen];
 		memset(pString,0,nLen);

		//memcpy(pString,&(m_buf[pEle->nTagPos+pEle->nOffset]),pEle->nLen);
		memcpy(pString,pEle->pValue,pEle->nLen);

		QString sValue = QString(QLatin1String(pString));
		QStringList sections = sValue.split("\\");
		int nValueCount = sections.size();

		delete []pString;
		return nValueCount;
	}
	//日期
	if (pEle->nVR == VR_DA)
	{
		int k = 0;
		for(unsigned long i=0;i<pEle->nLen;i++)
		{			
			//char c = m_buf[pEle->nTagPos + pEle->nOffset + i];
			char c = pEle->pValue[i];

			if(c>='0' && c<='9')
				k++;
		}

		return k/8;
	}
	//时间
	if (pEle->nVR == VR_TM)
	{
		char *pString = new char[pEle->nLen+1];
		memset(pString,0,pEle->nLen+1);

		int k = 0;
		for(unsigned long i=0;i<pEle->nLen;i++)
		{
			//char c = m_buf[pEle->nTagPos + pEle->nOffset + i];
			char c = pEle->pValue[i];
			if( (c>='0' && c<='9') || c=='.' )
				pString[k++] = c;
			else
			{
				if(c!=0 && c!=' ')
					pString[k++] = '\\';//就怕有些变态Dcm文件写成'-',所以才这么麻烦
			}
		}

		QString sValue = QString(QLatin1String(pString));
		QStringList sections = sValue.split("\\");
		int nValueCount = sections.size();

		delete []pString;
		return nValueCount;
	}
	//日期时间
	if (pEle->nVR == VR_DT)
	{
		char *pString = new char[pEle->nLen+1];
		memset(pString,0,pEle->nLen+1);

		int k = 0;
		for(unsigned long i=0;i<pEle->nLen;i++)
		{
			//char c = m_buf[pEle->nTagPos + pEle->nOffset + i];
			char c = pEle->pValue[i];
			if( (c>='0' && c<='9') || c=='.' || c=='+'|| c=='-')//这些才是合法字符
				pString[k++] = c;
			else//其余的字符
			{
				if(c!=0 && c!=' ' )//0和空格不收
					pString[k++] = '\\';//要收就转成\\.----就怕有些变态Dcm文件写成'-',所以才这么麻烦
			}
		}

		QString sValue = QString(QLatin1String(pString));
		QStringList sections = sValue.split("\\");
		int nValueCount = sections.size();

		delete []pString;
		return nValueCount;
	}

	if (pEle->nVR==VR_OB && pEle->nTag!=TAG_PIXEL_DATA)
	{
		return pEle->nLen;
	}
	if (pEle->nVR==VR_OW && pEle->nTag!=TAG_PIXEL_DATA)
	{
		return pEle->nLen/2;
	}

	//其他类型 OB OW及一些不曾见过的类型
	if(pEle->nLen==0)
		return 0;
	else
		return 1;

	return 0;
}

QString CHsBaseFile::Hs_GetConvertValue(pHsElement pEle, int nValueIndex ,int& nRet)
{
	if (pEle==NULL)
	{
		nRet = Ret_InvalidPara;
		return "";
	}
	
	if (pEle->nLen==0 || pEle->nTag==TAG_ITEM || pEle->nVR==VR_SQ || pEle->nTag==TAG_PIXEL_DATA)
	{
		nRet = Ret_InvalidPara;
		return "";
	}

	if(pEle->nVR==VR_UL || pEle->nVR==VR_SL || pEle->nVR==VR_SS || pEle->nVR==VR_US)
	{
		long nValue = 0;
		nRet = Hs_GetLongValue(pEle,nValue, nValueIndex);

		if(nRet==Ret_Success)
		{
			char cc[100];
			sprintf(cc,"%d",nValue);

			return cc;
		}
		else
		{
			return "?";
		}
	}

	if (pEle->nVR == VR_AS)
	{
		int nAge = 0;
		char cType = 0;
		nRet = Hs_GetAgeValue(pEle,nAge,cType);
		if (nRet==Ret_Success)
		{
			char cc[100];
			sprintf(cc,"%d%c",nAge,cType);

			return cc;
		}
		else
		{
			return "?";
		}
	}

	if (pEle->nVR==VR_DS || pEle->nVR==VR_FD || pEle->nVR==VR_FL)
	{
		double fValue = 0.00;
		nRet = Hs_GetDoubleValue(pEle,fValue,nValueIndex);
		if (nRet==Ret_Success)
		{
			char cc[100];
			sprintf(cc,"%f",fValue);

			return cc;
		}
		else
		{
			return "?";
		}
	}

	if (pEle->nVR==VR_OF)
		return "发现OF,待处理";

	if (pEle->nVR==VR_OW)
	{
		//return "VR_OW";
		long v;
		Hs_GetLongValue(pEle,v,nValueIndex);

		char cc[100];
		sprintf(cc,"%d",v);

		return cc;
	}

	if (pEle->nVR==VR_OB)
	{			
		long v;
		Hs_GetLongValue(pEle,v,nValueIndex);
		char cc[100];
		sprintf(cc,"%02X",v);

		return cc;
	}


	if (pEle->nVR==VR_DA)
	{
		HsDateTime t;
		nRet = Hs_GetDateValue(pEle,t,nValueIndex);
			
		if (nRet==Ret_Success)
		{
			char cc[200];
			sprintf(cc,"%04d-%02d-%02d",	t.nYear,	t.nMonth,	t.nDay);

			return cc;
		}
		else
		{
			return "?";
		}

	}

	if (pEle->nVR==VR_DT)
	{
		HsDateTime t;
		nRet = Hs_GetDateTimeValue(pEle,t,nValueIndex);

		if (nRet==Ret_Success)
		{
			char cc[200];
			sprintf(cc,"%04d-%02d-%02d %02d:%02d:%02d",	t.nYear,	t.nMonth,	t.nDay,	t.nHours,	t.nMinutes,  t.nSeconds);//.%03d, t.nFractions

			return cc;
		}
		else
		{
			return "?";
		}
	}

	if (pEle->nVR==VR_TM)
	{
		HsDateTime t;
		nRet = Hs_GetTimeValue(pEle,t,nValueIndex);

		if (nRet==Ret_Success)
		{
			char cc[200];
			sprintf(cc,"%02d:%02d:%02d",	t.nHours, t.nMinutes,  t.nSeconds);//.%03d, t.nFractions--医院不喜欢毫秒

			return cc;
		}
		else
		{
			return "?";
		}
	}

	QString sRet = "";
	nRet = Hs_GetStringValue(pEle,sRet,nValueIndex);
	if (nRet==Ret_Success)
		return sRet;
	else
		return "?";

	
}


int CHsBaseFile::Hs_GetImageInfo(pHsElement pPixEle, ImageInfo& ImgInfo,int iFrame)
{
	if (m_pMainEle==NULL)
		return Ret_InvalidBuf;

	if(pPixEle==NULL)
		return Ret_InvalidPara;

	ImgInfo.sFileName = m_BaseFileInfo.sFileName;
	ImgInfo.bValid = true;
	ImgInfo.iFrame = iFrame;

	//iCompress
	if(m_BaseFileInfo.nTsType==TS_IMPLICIT_VR_LITTLE_ENDIAN || m_BaseFileInfo.nTsType==TS_EXPLICIT_VR_LITTLE_ENDIAN || m_BaseFileInfo.nTsType==TS_EXPLICIT_VR_BIG_ENDIAN)
		ImgInfo.iCompress = 0;
	else
		ImgInfo.iCompress = m_BaseFileInfo.nTsType;

	ImgInfo.bBigEndia = m_BaseFileInfo.nTsType==TS_EXPLICIT_VR_BIG_ENDIAN ? true : false;

	int nRet = 0;
	pHsElement p = NULL;

	//iAcquisitionNum
	p = Hs_FindFirstEle(NULL,TAG_ACQUISITION_NUMBER,true);
	if (p)
		Hs_GetLongValue(p,ImgInfo.iAcquisitionNum,0);

	//ScanOptions
	p = Hs_FindFirstEle(NULL,TAG_SCAN_OPTIONS,true);
	if (p)
		Hs_GetStringValue(p,ImgInfo.sScanOptions,0);

	//sPhotometric
	p = Hs_FindFirstEle(pPixEle,TAG_PHOTOMETRIC_INTERPRETATION,true);
	nRet = Hs_GetStringValue(p,ImgInfo.sPhotometric,0);

	if (ImgInfo.sPhotometric.isEmpty())
		ImgInfo.sPhotometric = "MONOCHROME2";

	if(int( ImgInfo.sPhotometric.indexOf("MONOCHROME1") )>=0)
	{
		ImgInfo.bInverse = true;
		ImgInfo.bGrayImg = true;
	}
	else if (int( ImgInfo.sPhotometric.indexOf("MONOCHROME2") )>=0)
	{
		ImgInfo.bInverse = false;
		ImgInfo.bGrayImg = true;
	}
	else 
	{
		ImgInfo.bInverse = false;
		ImgInfo.bGrayImg = false;
	}

	//Rows
	p = Hs_FindFirstEle(pPixEle,TAG_ROWS,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nRows,0);
	ImgInfo.nOriRows = ImgInfo.nRows;

	//Col
	p = Hs_FindFirstEle(pPixEle,TAG_COLUMNS,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nCols,0);
	ImgInfo.nOriCols = ImgInfo.nCols;

	//nSamplePerPixel
	p = Hs_FindFirstEle(pPixEle,TAG_SAMPLES_PER_PIXEL,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nSamplePerPixel,0);
	if(ImgInfo.nSamplePerPixel==0)
		ImgInfo.nSamplePerPixel = 1;
	if(ImgInfo.nSamplePerPixel>=256)//三中心的MR怎么理解标准的,竟然x256
		ImgInfo.nSamplePerPixel /=256;

	//iPlanarConfig
	p = Hs_FindFirstEle(pPixEle,TAG_PLANAR_CONFIGURATION,true);
	nRet = Hs_GetLongValue(p,ImgInfo.iPlanarConfig,0);	

	//nBitsAllocated
	p = Hs_FindFirstEle(pPixEle,TAG_BITS_ALLOCATED,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nBitsAllocated,0);
	if(ImgInfo.nBitsAllocated>=256)//三中心的MR怎么理解标准的,竟然x256
		ImgInfo.nBitsAllocated /=256;

	//nBitStored
	p = Hs_FindFirstEle(pPixEle,TAG_BITS_STORED,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nBitStored,0);
	if(ImgInfo.nBitStored>=256)//三中心的MR怎么理解标准的,竟然x256
		ImgInfo.nBitStored /=256;

	//iHighBit
	p = Hs_FindFirstEle(pPixEle,TAG_HIGH_BIT,true);
	nRet = Hs_GetLongValue(p,ImgInfo.iHighBit,0);
	if(ImgInfo.iHighBit>=256)//三中心的MR怎么理解标准的,竟然x256
		ImgInfo.iHighBit /=256;

	//nPixelRepresentation 高位补码
	p = Hs_FindFirstEle(pPixEle,TAG_PIXEL_REPRESENTATION,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nPixelRepresentation,0);

	//fPixelSpace
	p = Hs_FindFirstEle(pPixEle,TAG_PIXEL_SPACING,true);//0028,0030//TAG_IMAGER_PIXEL_SPACING;0018,1164//TAG_NOMINAL_SCANNED_PIXEL_SPACING;0018,2010
	nRet = Hs_GetDoubleValue(p,ImgInfo.fPixelSpaceY,0);
	nRet = Hs_GetDoubleValue(p,ImgInfo.fPixelSpaceX,1);

	if (nRet!=0)
	{
		p = Hs_FindFirstEle(pPixEle,TAG_IMAGER_PIXEL_SPACING,true);
		nRet = Hs_GetDoubleValue(p,ImgInfo.fPixelSpaceY,0);
		nRet = Hs_GetDoubleValue(p,ImgInfo.fPixelSpaceX,1);

		//不能直接用Imager Pixel Spacing
		if (nRet==Ret_Success)
		{
			pHsElement pZoom = Hs_FindFirstEle(pPixEle,TAG_ESTIMATED_RADIOGRAPHIC_MAGNIFICATION_FACTOR,true);
			double fv = -1.00;

			if (pZoom)
				Hs_GetDoubleValue(pZoom,fv,0);
			
			if(fv < 0.00001)//没得到正常值,自己去计算一下，也未必成功
			{
				pHsElement pd = Hs_FindFirstEle(pPixEle,TAG_DISTANCE_SOURCE_TO_DETECTOR,true);
				pHsElement pp = Hs_FindFirstEle(pPixEle,TAG_DISTANCE_SOURCE_TO_PATIENT,true);
				if (pd && pp)
				{
					double fd = 0.00;
					Hs_GetDoubleValue(pd,fd,0);
					double fp = 0.00;
					Hs_GetDoubleValue(pp,fp,0);

					if (fd>0.00001 && fp>0.00001)
						fv = fd/fp;
				}
			}

			if(fv>0.00001)//若成功了
			{
				ImgInfo.fPixelSpaceX/=fv;
				ImgInfo.fPixelSpaceY/=fv;
			}

		}

	}

	//sImageType
	p = Hs_FindFirstEle(pPixEle,TAG_IMAGE_TYPE,true);
	if (p)
	{
		int n = Hs_GetValueCount(p,nRet);
		for(int i=0;i<n;i++)
		{
			QString s = "";
			Hs_GetStringValue(p,s,i);
			if(i>0)
				ImgInfo.sImageType += "\\";

			ImgInfo.sImageType += s;
		}
	}

	//BitsPerPixel
	ImgInfo.nBitsPerPixel = ImgInfo.nBitsAllocated * ImgInfo.nSamplePerPixel;

	//nSmallestPixelValue
	p = Hs_FindFirstEle(pPixEle,TAG_SMALLEST_IMAGE_PIXEL_VALUE,true);
	Hs_GetLongValue(p,ImgInfo.nSmallestPixelValue,0);

	//nLargestPixelValue
	p = Hs_FindFirstEle(pPixEle,TAG_SMALLEST_IMAGE_PIXEL_VALUE,true);
	Hs_GetLongValue(p,ImgInfo.nLargestPixelValue,0);

	//nFrame
	ImgInfo.nFrame = m_BaseFileInfo.nFrame;

	//p = Hs_FindFirstEle(pPixEle,TAG_NUMBER_OF_FRAMES,true);
	//if(p)//如果是多帧，必须有这个Tag,单帧的可以没有
	//	Hs_GetLongValue(p,ImgInfo.nFrame,0);
	//else
	//	ImgInfo.nFrame = 1;

	/*
	if(ImgInfo.nFrame==0)//没找到
	{//能否替代leadtools 找到帧数？
		if (ImgInfo.iCompress==0)
		{ 
			if(pPixEle->nLen==0)
				ImgInfo.nFrame = 0;
			else if(pPixEle->nLen==0xFFFFFFFF)
				ImgInfo.nFrame = -1;//待定之意
			else
			{
				unsigned long nBytePerFrame = (ImgInfo.nBitsAllocated/8) * ImgInfo.nRows * ImgInfo.nCols * ImgInfo.nSamplePerPixel;
				if(nBytePerFrame!=0)
					ImgInfo.nFrame = pPixEle->nLen/nBytePerFrame;
				else
					ImgInfo.nFrame = -1;
			}
		}
		else//LeadTool来确定帧数
		{
			LDicomDS ds;
			char cfile[512];
			strcpy(cfile,m_BaseFileInfo.sFileName.c_str());
			if(ds.LoadDS(cfile, 0)!=DICOM_SUCCESS)
				return Ret_LoadFileError;

			pDICOMELEMENT pLtPix = GetLtEleByMyEle(pPixEle,&ds);

			DICOMIMAGE LtInfo;
			ds.GetInfoImage(pLtPix,&LtInfo,0);
			ImgInfo.nFrame = LtInfo.nFrames;
		}

	}
	*/

	//////////////////////////////////////////////////////////////////////////

	ImgInfo.pEle = pPixEle;

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_TYPE,true);
	Hs_GetStringValue(p,ImgInfo.sOverlayType,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_MAGNIFICATION_TYPE,true);
	Hs_GetStringValue(p,ImgInfo.sOverlayMagnificationType,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_OR_IMAGE_MAGNIFICATION,true);
	Hs_GetStringValue(p,ImgInfo.sOverlayOrImageMagnification,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_SMOOTHING_TYPE,true);
	Hs_GetStringValue(p,ImgInfo.sOverlaySmoothingType,0);

	p = Hs_FindFirstEle(pPixEle,TAG_FRAME_LABEL_VECTOR,true);
	Hs_GetStringValue(p,ImgInfo.sFrameLabelVector,0);

	p = Hs_FindFirstEle(pPixEle,TAG_RESCALE_INTERCEPT,true);
	Hs_GetDoubleValue(p,ImgInfo.fRescaleIntercept,0);

	p = Hs_FindFirstEle(pPixEle,TAG_RESCALE_SLOPE,true);
	if(Hs_GetDoubleValue(p,ImgInfo.fRescaleSlope,0) != Ret_Success)
		ImgInfo.fRescaleSlope = 1.00;

	p = Hs_FindFirstEle(pPixEle,TAG_WINDOW_CENTER,true);
	Hs_GetDoubleValue(p,ImgInfo.fWinCenter,0);

	p = Hs_FindFirstEle(pPixEle,TAG_WINDOW_WIDTH,true);
	Hs_GetDoubleValue(p,ImgInfo.fWinWidth,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_BIT_POSITION,true);
	Hs_GetLongValue(p,ImgInfo.iOverlayBitPosition,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_BITS_ALLOCATED,true);
	Hs_GetLongValue(p,ImgInfo.nOverlayBitsAllocated,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_COLUMNS,true);
	Hs_GetLongValue(p,ImgInfo.nOverlayCols,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_ROWS,true);
	Hs_GetLongValue(p,ImgInfo.nOverlayRows,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_ORIGIN,true);
	Hs_GetLongValue(p,ImgInfo.nOverlayOrigin1,0);

	p = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_ORIGIN,true);
	Hs_GetLongValue(p,ImgInfo.nOverlayOrigin2,1);

	p = Hs_FindFirstEle(pPixEle,TAG_MAGNIFY_TO_NUMBER_OF_COLUMNS,true);
	Hs_GetLongValue(p,ImgInfo.nMagnifyToNumberOfColumns,0);

	p = Hs_FindFirstEle(pPixEle,TAG_LUT_DESCRIPTOR,true);
	Hs_GetLongValue(p,ImgInfo.nLutDescriptor1,0);

	p = Hs_FindFirstEle(pPixEle,TAG_LUT_DESCRIPTOR,true);
	Hs_GetLongValue(p,ImgInfo.nLutDescriptor2,1);

	p = Hs_FindFirstEle(pPixEle,TAG_MODALITY,true);
	Hs_GetStringValue(p,ImgInfo.sModality,0);

	p = Hs_FindFirstEle(pPixEle,TAG_IMAGE_POSITION_PATIENT,true);
	Hs_GetDoubleValue(p,ImgInfo.fImagePosition[0],0);

	p = Hs_FindFirstEle(pPixEle,TAG_IMAGE_POSITION_PATIENT,true);
	Hs_GetDoubleValue(p,ImgInfo.fImagePosition[1],1);

	p = Hs_FindFirstEle(pPixEle,TAG_IMAGE_POSITION_PATIENT,true);
	if(Hs_GetDoubleValue(p,ImgInfo.fImagePosition[2],2) != Ret_Success)
	{
		p = Hs_FindFirstEle(pPixEle,TAG_SLICE_LOCATION,true);
		Hs_GetDoubleValue(p,ImgInfo.fImagePosition[2],0);
	}

	p = Hs_FindFirstEle(pPixEle,TAG_ULTRASOUND_COLOR_DATA_PRESENT,true);
	Hs_GetLongValue(p,ImgInfo.nUltrasound_Color_Data_Present,0);


	//计算WL_Lut的长度

	if ( ImgInfo.nPixelRepresentation == 1 ) //高位补码
		ImgInfo.nWcLutLen = 1<<ImgInfo.nBitsAllocated;
	else
		ImgInfo.nWcLutLen = 1<<(ImgInfo.iHighBit+1);

	if(ImgInfo.nWcLutLen>256)
		ImgInfo.fWinLevelStep = min(ImgInfo.nWcLutLen/1000,10);


	if (ImgInfo.nPixelRepresentation==0)
	{
		ImgInfo.iLutStart = 0;
		ImgInfo.iLutEnd = ImgInfo.nWcLutLen;
	}
	else //高位补码(像素值内有负数)
	{
		ImgInfo.iLutStart = -(ImgInfo.nWcLutLen/2);
		ImgInfo.iLutEnd = ImgInfo.nWcLutLen/2;
	}

	pHsElement pOverlayData = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_DATA,true);
	if(ImgInfo.iOverlayBitPosition>ImgInfo.iHighBit)//嵌入式ImgInfo.iOverlayBitPosition==16 || ImgInfo.iOverlayBitPosition==8
	{
		ImgInfo.nOverLayType = OverLay_Pixel;
	}
	else if (pOverlayData!=NULL && ImgInfo.nOverlayCols>0 && ImgInfo.nOverlayRows>0)//独立式
	{
		//if (ImgInfo.nOverlayCols*ImgInfo.nOverlayRows == pOverlayData->nLen*8)
			ImgInfo.nOverLayType = OverLay_Bits;
		//else if(ImgInfo.nOverlayCols*ImgInfo.nOverlayRows*ImgInfo.nOverlayBitsAllocated == pOverlayData->nLen*8)
		//	ImgInfo.nOverLayType = OverLay_Byte;
		//else
		//	ImgInfo.nOverLayType = OverLay_None;
	}
	else//没OverLay
	{
		ImgInfo.nOverLayType = OverLay_None;
	}

	//if (ImgInfo.iOverlayBitPosition!=16 && ImgInfo.iOverlayBitPosition!=8 && ImgInfo.iOverlayBitPosition!=0 )
	//{
	//	AtlTrace("\r\n不符合标准的OverLay");
	//	return Ret_UnKnownCase;
	//}
	if (ImgInfo.nPixelRepresentation == 1 && ImgInfo.nOverLayType == 2)
	{
		qDebug("\r\nOverLay:既有高位补码,又有嵌入式Overlay");
	}

	//获取定位线基本元素
	GetImageLocPara(ImgInfo);

	pHsElement pFunGpSeqEle = Hs_FindFirstEle(NULL,0x52009230,true);
	while(pFunGpSeqEle)
	{
		//获取pixspacing
		pHsElement pItemEle = GetItemFromPerFramFunGpSeq(pFunGpSeqEle,iFrame);
		if(pItemEle == NULL)
			break;

		//Pixel Spacing /Slice thick
		pHsElement pPixMeasSeqEle = Hs_GetChildEle(pItemEle,0x00289110,-1);
		if(pPixMeasSeqEle)
		{
			pHsElement tItemEle0 = Hs_GetChildEle(pPixMeasSeqEle,TAG_ITEM,-1);
			if(tItemEle0)
			{
				pHsElement pPixSpacingEle = Hs_GetChildEle(tItemEle0,TAG_PIXEL_SPACING,true);

				if (pPixSpacingEle)
				{
					Hs_GetDoubleValue(pPixSpacingEle,ImgInfo.fPixelSpaceY,0);
					Hs_GetDoubleValue(pPixSpacingEle,ImgInfo.fPixelSpaceX,1);
				}
			}
		}
		//Image Position
		pHsElement pPlanePosSeqEle = Hs_GetChildEle(pItemEle,0x00209113,-1);
		if (pPlanePosSeqEle)
		{
			pHsElement tItemEle0 = Hs_GetChildEle(pPlanePosSeqEle,TAG_ITEM,-1);
			if (tItemEle0)
			{
				pHsElement pImgPosition = Hs_GetChildEle(tItemEle0,TAG_IMAGE_POSITION_PATIENT,true);
				if (pImgPosition)
				{
					Hs_GetDoubleValue(pImgPosition,ImgInfo.fImagePosition[0],0);
					Hs_GetDoubleValue(pImgPosition,ImgInfo.fImagePosition[1],1);
					Hs_GetDoubleValue(pImgPosition,ImgInfo.fImagePosition[2],2);
				}

			}
		}
		//
		//Rescale Para
		if (m_sModality.compare("CT") == 0)
		{
			pHsElement pPixValueTrsSeqEle = Hs_GetChildEle(pItemEle,0x00289145,-1);
			if (pPixValueTrsSeqEle)
			{
				pHsElement tItemEle0 = Hs_GetChildEle(pPixValueTrsSeqEle,TAG_ITEM,-1);
				if (tItemEle0)
				{
					pHsElement pRescale = Hs_GetChildEle(tItemEle0,TAG_RESCALE_INTERCEPT,true);
					if (pRescale)
						Hs_GetDoubleValue(pRescale,ImgInfo.fRescaleIntercept,0);

					pRescale = Hs_GetChildEle(tItemEle0,TAG_RESCALE_SLOPE,true);
					if (pRescale)
						Hs_GetDoubleValue(pRescale,ImgInfo.fRescaleSlope,0);
				}
			}
		}

		//WL
		if (m_sModality.compare("CT") == 0)
		{
			pHsElement pWLEle = Hs_GetChildEle(pItemEle,0x00289132,-1);
			if (pWLEle)
			{
				pHsElement tItemEle0 = Hs_GetChildEle(pWLEle,TAG_ITEM,-1);
				if (tItemEle0)
				{
					pHsElement pWL = Hs_GetChildEle(tItemEle0,TAG_WINDOW_CENTER,true);
					if (pWL)
						Hs_GetDoubleValue(pWL,ImgInfo.fWinCenter,0);

					pWL = Hs_GetChildEle(tItemEle0,TAG_WINDOW_WIDTH,true);
					if (pWL)
						Hs_GetDoubleValue(pWL,ImgInfo.fWinWidth,0);
				}
			}
		}

		//获取一下定位线项目
		if (ImgInfo.ImgLocPara.bValide == false)
		{
			GetImageLocParaMulityFrame(iFrame,ImgInfo);
		}

		break;
	}


	//一些特殊的处理：
	if (1)
	{
		//301医院佳能的RF图像，当Tag:0x10411002=1时，需要我再进行一次负片
		if (m_sModality.compare("RF") == 0)
		{
			QString sv = "";
			Hs_GetStringValueA(0x00080070,sv);
			QString sManufacture = sv;
			sManufacture.toLower();
			int iPos = sManufacture.indexOf("shimadzu");//佳能设备的标志
			if (iPos >= 0)
			{
				p = Hs_FindFirstEle(NULL,0x10411002,true);
				if(p)
				{
					long lv = 0;
					Hs_GetLongValue(p,lv,0);
					if(lv == 1)
					{
						ImgInfo.bInverse = !ImgInfo.bInverse;
						ImgInfo.bGrayImg = true;
					}
				}
			}

		}
	}

	//弥散成像
	if (m_sModality.compare("MR") == 0)
	{
		pHsElement pDiffisionSq = Hs_FindFirstEle(NULL,0x00189117,true);
		if (pDiffisionSq)
		{
			pHsElement tItemEle0 =Hs_GetChildEle(pDiffisionSq,TAG_ITEM,-1);
			if (tItemEle0)
			{
				pHsElement pDiffusionBvalueEle = Hs_GetChildEle(tItemEle0,TAG_DIFFUSION_B_VALUE,true);
				if (pDiffusionBvalueEle)
					Hs_GetDoubleValue(pDiffusionBvalueEle,ImgInfo.fDifusionBvalue,0);
			}
		}
		else
		{
			pHsElement pFunGpSeqEle = Hs_FindFirstEle(NULL,0x52009230,true);
			if(pFunGpSeqEle)
			{
				pHsElement pItemEle = GetItemFromPerFramFunGpSeq(pFunGpSeqEle,iFrame);
				if (pItemEle)
				{
					pHsElement pDiffisionSq = Hs_GetChildEle(pItemEle,0x00189117,-1);
					if(pDiffisionSq)
					{
						pHsElement tItemEle0 = Hs_GetChildEle(pDiffisionSq,TAG_ITEM,-1);
						if(tItemEle0)
						{
							pHsElement pDiffusionBvalueEle = Hs_GetChildEle(tItemEle0,TAG_DIFFUSION_B_VALUE,true);
							if (pDiffusionBvalueEle)
								Hs_GetDoubleValue(pDiffusionBvalueEle,ImgInfo.fDifusionBvalue,0);
							if (pDiffusionBvalueEle == NULL)
								ImgInfo.fDifusionBvalue = -100;
						}
					}
				}
			}
			else
			{
				pHsElement pDiffisionEle = Hs_FindFirstEle(NULL,TAG_DIFFUSION_B_VALUE,true);
				if (pDiffisionEle)
					Hs_GetDoubleValue(pDiffisionEle,ImgInfo.fDifusionBvalue,0);
				else
					ImgInfo.fDifusionBvalue = -10240;
			}
		}
	}

	return 0;
}

//E:\1图片\秦皇岛中医院\秦皇岛中医院-DR\1.2.826.0.1.3680043.1.1.10606269.20080227.151635.0.0.1173.dcm
//这个图像骗人的,说没有OverLay其实还是偷偷的放了OverLay数据在像素空位,搞惨了我
int CHsBaseFile::Hs_GetImage(pHsElement pPixEle,CHsBaseImg &Img, int iFrame)
{
	//CTstRunTime tsm("Hs_GetImage");
//CLog a;
//a.Log("\r\n开始读取像素");
	//基本条件判断
	if(pPixEle==NULL)
		return Ret_InvalidPara;

	if(pPixEle->nLen==0 )
		return Ret_NoValue;

	int nRet = 0;

	bool bCloseFile = false;//是否需要关闭文件?----如果原来文件是关闭的,则读完给关闭,否则保持打开状态

	//获取像素信息
	if(Img.m_ImgInfo.bValid == false)
	{
		nRet = Hs_GetImageInfo(pPixEle,Img.m_ImgInfo,iFrame);
		if(nRet) 
			return nRet;
	}

	////这里是一个图像最先获得PixSpacing的地方。
	//Img.m_AnnoManager.SetPixSpacing(Img.m_ImgInfo.fPixelSpaceX,Img.m_ImgInfo.fPixelSpaceY);

	//名字太长了.找个变量代替
	unsigned long nRow = Img.m_ImgInfo.nOriRows;
	unsigned long nCol = Img.m_ImgInfo.nOriCols;

	if(nRow==0 || nCol==0)
		return Ret_NoValue;

	//初始化一下,状态参数
	if(Img.m_ImgState.bImgStateFilled==false)
		Img.Hs_InitImgState();

	Img.m_ImgState.nCurOriPixCol = nCol;
	Img.m_ImgState.nCurOriPixRow = nRow;
	Img.m_ImgState.bSubstracted = false;

	long nFrame = Img.m_ImgInfo.nFrame;
	int iCompress = Img.m_ImgInfo.iCompress;
	int nSample = Img.m_ImgInfo.nSamplePerPixel;

	if (iFrame>=Img.m_ImgInfo.nFrame)
		return Ret_OutOfValueCount;

	//一个像素要几字节?
	unsigned long nBytePerPix = Img.m_ImgInfo.nBitsPerPixel/8;
	//计算每帧多少字节
	unsigned long nSizePerFrame = nBytePerPix * nRow * nCol;
	//这一帧的起始位置
	unsigned long nFrameStartPos = pPixEle->nTagPos + pPixEle->nOffset +  iFrame*nSizePerFrame;
	//每行几个有效字节,4的倍数导致的补充字节不算
	unsigned long nBytePerRow = nCol*nBytePerPix;

	//超出文件末尾,--dcm文件残缺一部分像素.没关系,有多少像素显示多少
	//if(nSizePerFrame + nFrameStartPos>m_BaseFileInfo.nFullSize)
	//	return Ret_GetImgError;


	LDicomDS *pDs = NULL;//ds(_T("c:\\"));
	LBitmap lbm;

	if (Img.m_ImgInfo.iCompress==0)
	{
		//打开文件
		if(m_fp == NULL)
		{
			QByteArray ba = m_BaseFileInfo.sFileName.toLatin1();
			errno_t err = fopen_s( &m_fp, ba.data(), "rb" );
			if( err !=0 )
				return Ret_LoadFileError;

			bCloseFile = true;
		}
	}
	else
	{
        pDs = new LDicomDS("c:\\");
		//LeadTools加载该文件
		char cfile[512];
		QByteArray ba = m_BaseFileInfo.sFileName.toLatin1();
		strcpy(cfile,ba.data());
		if(pDs->LoadDS(cfile, 0)!=DICOM_SUCCESS)
		{
			delete pDs;
			pDs = NULL;
			return Ret_LoadFileError;
		}
		
		pDICOMELEMENT pLtPix = GetLtEleByMyEle(pPixEle,pDs);

		L_UINT16 r = pDs->GetImage(pLtPix,lbm.GetHandle(),sizeof(BITMAPHANDLE),iFrame,0,0,0,0,0);
		if(r)
		{
			delete pDs;
			pDs = NULL;
			return Ret_LedtolsGetImgError;
		}

		delete pDs;
		pDs = NULL;
	}


	//读取一帧的数据
	//开辟内存准备接受该帧数据
	if (Img.m_pOriData)
	{
		ArrayFree((void**)Img.m_pOriData);
		Img.m_pOriData = NULL;
	}

	unsigned long nNewRow = 0;
	unsigned long nNewCol = 0;
	Img.m_pOriData = (BYTE**)ArrayNew(nRow,		nCol,		nBytePerPix,&nNewRow, &nNewCol);

	if(Img.m_pOriData==NULL)
		return Ret_AllocateMemFailed;

	unsigned long nSamplePerRow = nCol*nSample;//每行几个Sample
	unsigned long nBt = Img.m_ImgInfo.nBitsAllocated/8;//每个Sample有几个字节?
	int nStep = int(nBt/2);//BigEnian情况下,Sample内部字节对换时,要调换几次

	bool bGetMaxMinValue = true;//需要得到最大值最小值吗?
	//if (Img.m_ImgInfo.nSmallestPixelValue == 0 && Img.m_ImgInfo.nLargestPixelValue == 0)
	//	bGetMaxMinValue = true;

	long nMax = -2147483640;
	long nMin = 2147483640;

	if (iCompress == 0)//无压缩的话
	{
		fseek(m_fp,nFrameStartPos, SEEK_SET);//在文件中来到该帧起始处
		size_t nCount = fread(Img.m_pOriData[0], nRow*nCol*nSample*nBt, 1,m_fp);//连续内存m_pOriData[0]一下子读取该帧所有像素
		if(nCount != 1)//失败处理,不要返回了,这样可以看到残缺的坏图像.
		{
			ArrayFree((void**)Img.m_pOriData);
			return Ret_GetImgError;
		}

		//R1 R2 R3 R4...Rx. G1 G2 G3 G4...Gx. B1 B2 B3 B4...Bx.摆放形式.整理成RGB RGB....
		if (Img.m_ImgInfo.iPlanarConfig==1 && Img.m_ImgInfo.nBitsPerPixel==24 && int(Img.m_ImgInfo.sPhotometric.indexOf("RGB"))>=0)
		{
			unsigned long sz = nCol*nRow*3;//像素总共多少字节

			BYTE * pByte = new BYTE[sz];//分配临时内存
			memcpy(pByte,Img.m_pOriData[0],sz);//把像素拷到临时内存

			unsigned long nSec = sz/3;
			BYTE *pImgData = Img.m_pOriData[0];

			unsigned long nSec2 = 2*nSec;

			//从临时内存逐个挑拣回来
			for (unsigned long i=0;i<nSec;i++)
			{
				pImgData[3*i] = pByte[i];
				pImgData[3*i+1] = pByte[nSec+i];
				pImgData[3*i+2] = pByte[nSec2+i];
			}

			delete []pByte;
		}

		int nAppend = nNewCol - nCol;
		if (nAppend)//若因为4字节对齐，而发生补字节的情况
		{
			unsigned long nBytePerRow4 = nNewCol * nBytePerPix;//补4之后的每行字节数

			BYTE** pTmpData = (BYTE**)ArrayNew(nNewRow,	nNewCol,nBytePerPix);

			for (int r=0;r<nNewRow;r++)//nNewRow和nRow永远会一致的
			{
				BYTE* p = Img.m_pOriData[0] + r * nBytePerRow;//原始矩阵中这一行的起始位置
				BYTE* pTmp = pTmpData[0] + r * nBytePerRow4;

				memcpy(pTmp,p,nBytePerRow);//nBytePerRow4
			}
			ArrayFree((void**)Img.m_pOriData);
			Img.m_pOriData = pTmpData;
		}
	}
	
	//开始读:一次读一行,直接放到Img里,不必再辟新内存
	for (int r=0;r<nRow;r++)
	{
		if(iCompress==0)
		{
			//下面注释掉的读取方式，会导致nRow次磁盘读取，若是远程图像的话，还会导致nRow次的网络传输，不如上面一下子读取所有像素到内存，再在内存里倒腾的好
			//fseek(m_fp,nFrameStartPos + r*nBytePerRow, SEEK_SET);//在文件中来到该帧起始处
			//size_t nCount = fread(Img.m_pOriData[r], nBytePerRow, 1,m_fp);//连续内存m_pOriData[r]
			//if(nCount != 1)//失败处理,不要返回了,这样可以看到残缺的坏图像.
			//{
			//	nRet = Ret_GetImgError;
			//	break;
			//}

			// 若为大端还要在Sample内部把字节顺序颠倒过来.其实应该说CPU与文件大小端不一致就得换.
			if (pPixEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia && Img.m_ImgInfo.nBitsAllocated>8)
			{
				BYTE *pSamleData0 = Img.m_pOriData[r];

				for (unsigned long i=0;i<nSamplePerRow;i++)
				{
					BYTE *pSamleDatai = pSamleData0 + i*nBt;

					for (int b=0;b<nStep;b++)//12->21
					{
						BYTE t = pSamleDatai[b];
						pSamleDatai[b] = pSamleDatai[nBt-b-1];
						pSamleDatai[nBt-b-1] = t;
					}
				}
			}

			//无高位补码时,灰度图需要求最大值最小值 或者有嵌入式的OverLay要扣掉,则需要遍历每个像素
			if (Img.m_ImgInfo.nSamplePerPixel==1 && Img.m_ImgInfo.nPixelRepresentation==0)
			{
				SeparatePixdataAndOverlayByRow(Img,r,nCol,nMin,nMax);	
			}
			else
			{
				if (nBytePerPix == 1)
				{
					if (Img.m_ImgInfo.nPixelRepresentation==0)
					{
						unsigned char *t = (unsigned char*)Img.m_pOriData[r];
						for ( int i=0; i<nCol; i++)
						{
							if(t[i] > nMax)
								nMax = t[i];

							if(t[i] < nMin)
								nMin = t[i];
						}
					}
					else
					{
						char *t = (char*)(Img.m_pOriData[r]);
						for ( int i=0; i<nCol; i++)
						{
							if(t[i] > nMax)
								nMax = t[i];

							if(t[i] < nMin)
								nMin = t[i];
						}
					}
				}
				else if (nBytePerPix == 2)
				{
					if (Img.m_ImgInfo.nPixelRepresentation==0)
					{
						unsigned short *t = (unsigned short*)Img.m_pOriData[r];
						for ( int i=0; i<nCol; i++)
						{
							if(t[i] > nMax)
								nMax = t[i];

							if(t[i] < nMin)
								nMin = t[i];
						}
					}
					else
					{
						short *t = (short*)Img.m_pOriData[r];
						for ( int i=0; i<nCol; i++)
						{
							if(t[i] > nMax)
								nMax = t[i];

							if(t[i] < nMin)
								nMin = t[i];
						}
					}

				}
			}
		}
		else//Leadtools帮忙解压后读给我
		{
			if(nBytePerPix==1)
			{
				unsigned short nBitOffset = 8 - Img.m_ImgInfo.nBitStored;
				for ( int i=0; i<nCol; i++)
				{
					lbm.GetPixelData(&(Img.m_pOriData[r][i]) , r, i,  nBytePerPix);//从LeadTools处得到解压后的数据

					//1.不存在大端小端问题
					
					//2.分离Overlay和像素、求最大值最小值
					if (Img.m_ImgInfo.bGrayImg && Img.m_ImgInfo.nPixelRepresentation==0)
					{
						
						if (Img.m_pOriData[r][i] >> Img.m_ImgInfo.nBitStored )
						{
							if(Img.m_pOriOverlayData==NULL)
							{
								Img.m_pOriOverlayData = (BYTE**)ArrayNew(Img.m_ImgInfo.nRows,Img.m_ImgInfo.nCols,1);
								Img.m_ImgInfo.nOverLayType = OverLay_Pixel;
								Img.m_ImgInfo.nOverlayRows = Img.m_ImgInfo.nRows;
								Img.m_ImgInfo.nOverlayCols = Img.m_ImgInfo.nCols;
							}

							Img.m_pOriOverlayData[r][i] = OverlayValue;
						}


						//我不管你OverLay第几位,全部滚蛋:左移挤出去,右移抽回来,就只剩像素了.这一步可确保像素在可控范围内;有的Dcm文件是骗人的.没说明OverLay,却偷偷塞了OverLay数据.导致像素值超出nBitStored指定的范围
						Img.m_pOriData[r][i] = Img.m_pOriData[r][i] << nBitOffset;
						Img.m_pOriData[r][i] = Img.m_pOriData[r][i] >> nBitOffset;

						if(Img.m_pOriData[r][i] > nMax)
							nMax = Img.m_pOriData[r][i];

						if(Img.m_pOriData[r][i] < nMin)
							nMin = Img.m_pOriData[r][i];
					}

				}
			}
			else if (nBytePerPix==2)
			{
				unsigned short nBitOffset = 16 - Img.m_ImgInfo.nBitStored;

				unsigned short **pOutData = (unsigned short **)(Img.m_pOriData);

				for ( int i=0; i<nCol; i++)
				{
					lbm.GetPixelData(&pOutData[r][i] , r, i,  nBytePerPix);

					//存在大小端问题
					if (pPixEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
					{
						BYTE *pSamleData0 = (BYTE*)pOutData[r][i];

						BYTE t = pSamleData0[0];
						pSamleData0[0] = pSamleData0[1];
						pSamleData0[1] = t;

					}

					if (Img.m_ImgInfo.bGrayImg && Img.m_ImgInfo.nPixelRepresentation==0)
					{
						//分离OverLay和像素、求最大值最小值					
						if (pOutData[r][i] >> Img.m_ImgInfo.nBitStored )//我不管你有没有Overlay,也不管放到第几位.反正除去像素部分剩下的不是0，那就是OverLay
						{
							if(Img.m_pOriOverlayData==NULL)
							{
								Img.m_pOriOverlayData = (BYTE**)ArrayNew(Img.m_ImgInfo.nRows,Img.m_ImgInfo.nCols,1);
								Img.m_ImgInfo.nOverLayType = OverLay_Pixel;
								Img.m_ImgInfo.nOverlayRows = Img.m_ImgInfo.nRows;
								Img.m_ImgInfo.nOverlayCols = Img.m_ImgInfo.nCols;
							}

							Img.m_pOriOverlayData[r][i] = OverlayValue;
						}

						//我不管你OverLay第几位,全部滚蛋,左移挤出去,右移抽回来,就只剩像素了.这一步可确保像素在可控范围内,有的Dcm文件是骗人的.没说明OverLay,却偷偷塞了OverLay数据.导致像素值超出nBitStored指定的范围
						pOutData[r][i] = pOutData[r][i] << nBitOffset;
						pOutData[r][i] = pOutData[r][i] >> nBitOffset;

						if(pOutData[r][i] > nMax)
							nMax = pOutData[r][i];

						if(pOutData[r][i] < nMin)
							nMin = pOutData[r][i];
					}
				}
			}
			else if (nBytePerPix==3)
			{
				MYDATA24 **pOutData = (MYDATA24 **)(Img.m_pOriData);

				for ( int i=0; i<nCol; i++)
				{
					lbm.GetPixelData(&pOutData[r][i] , r, i,  nBytePerPix);
					BYTE t = pOutData[r][i].pData[0];
					pOutData[r][i].pData[0] = pOutData[r][i].pData[2];
					pOutData[r][i].pData[2] = t;
				}
			}
		}
	}



	if (Img.m_ImgInfo.nPixelRepresentation==1)//对于有符号的图像，尤其是两字节的图像，其最大值有可能超出范围
	{//E:\1图片\胶片质量测试图像\vlut_p06.dcm
		if (nBytePerPix==2)//两字节的//先只做两字节的。
		{
			short nCorrectMin = -(2<<(Img.m_ImgInfo.iHighBit-1));//理论最小值
			short nCorrectMax = (2<<(Img.m_ImgInfo.iHighBit-1)) - 1;//理论最大值

			if (nMax > nCorrectMax || nMin < nCorrectMin)//违规了
			{
				short nOffset = nCorrectMax - nCorrectMin;
				CorrectPixelValue((short**)Img.m_pOriData, Img.m_ImgInfo.nRows,Img.m_ImgInfo.nCols,nCorrectMin,nCorrectMax,nOffset);

				nMin = nCorrectMin;
				nMax = nCorrectMax;
			}			
		}
	}


	if(bGetMaxMinValue && nMax!=-2147483640 && nMin!=2147483640)
	{
		Img.m_ImgInfo.nLargestPixelValue = nMax;
		Img.m_ImgInfo.nSmallestPixelValue = nMin;

		//if (Img.m_ImgInfo.fWinWidth==0.00)
		//{
		//	Img.m_ImgInfo.fWinCenter = (nMin+nMax)*1.00/2;
		//	Img.m_ImgInfo.fWinWidth = nMax - nMin;
		//}

		if (Img.m_ImgInfo.fWinWidth==0.00)
		{
			//如果图像有斜率截距，那么像素值经过斜率截距加工才能使用，所以要根据像素值算窗宽窗位，务必考虑斜率截距
			int tMax = nMax;
			int tMin = nMin;
			if (Img.m_ImgInfo.fRescaleSlope!=0.00 && Img.m_ImgInfo.fRescaleIntercept!=0.00)
			{
				tMax = tMax*Img.m_ImgInfo.fRescaleSlope + Img.m_ImgInfo.fRescaleIntercept;
				tMin = tMin*Img.m_ImgInfo.fRescaleSlope + Img.m_ImgInfo.fRescaleIntercept;
			}

			Img.m_ImgInfo.fWinCenter = (tMin+tMax)*1.00/2;
			Img.m_ImgInfo.fWinWidth = tMax - tMin;
		}

	}



	Img.m_ImgInfo.iFrame = iFrame;

	//如果有独立OverLay,此处读出来
	if (Img.m_ImgInfo.nOverLayType==OverLay_Bits)
	{
		pHsElement pOverlayData = Hs_FindFirstEle(pPixEle,TAG_OVERLAY_DATA,true);
		if(m_fp == NULL)
		{
			QByteArray sFileName = m_BaseFileInfo.sFileName.toLatin1();
			errno_t err = fopen_s(&m_fp, sFileName.data(), "rb");
			if( err !=0 )
				return Ret_LoadFileError;

			bCloseFile = true;
		}

		fseek(m_fp,pOverlayData->nTagPos+pOverlayData->nOffset, SEEK_SET);//在文件中来到该帧起始处

		unsigned long nNeedLen = pOverlayData->nLen;//Img.m_ImgInfo.nOverlayCols * Img.m_ImgInfo.nOverlayRows/8 +1; 
		BYTE *pByte = new BYTE[nNeedLen];//pOverlayData->nLen
		size_t nCount = fread(pByte,nNeedLen , 1,m_fp);//pOverlayData->nLen
		if(nCount!=1)
		{
			delete []pByte;
			Img.m_pOriOverlayData = NULL;
		}
		else
		{
			//读成功了,若是大端且OW型的，还得颠倒一下
			if (pOverlayData->nVR == VR_OW && pPixEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia && nNeedLen%2==0)
			{
				int nTimes = nNeedLen/2;
				for (unsigned long i=0;i<nTimes;i++)
				{
					qSwap(pByte[2*i],pByte[2*i+1]);
				}
			}

			unsigned long nNewRow = 0;
			unsigned long nNewCol = 0;

			if(Img.m_pOriOverlayData==NULL)//有独立式的OverLay
				Img.m_pOriOverlayData = (BYTE**)ArrayNew(Img.m_ImgInfo.nOverlayRows,Img.m_ImgInfo.nOverlayCols,1,&nNewRow,&nNewCol);


			//因为4对齐而补充了几个像素
			unsigned long nAppend = nNewCol - Img.m_ImgInfo.nOverlayCols;
			unsigned long nAllLen = nNewCol*nNewRow;

			BYTE b[8] = {1, 2, 4, 8, 16, 32, 64, 128};
			unsigned long iOverlay = 0;


			for (long iByte=0;iByte<pOverlayData->nLen;iByte++)//nNeedLen
			{
				for(int iBit=0;iBit<8;iBit++)
				{
					if (nAppend > 0)
					{
						int x = nNewCol - (iOverlay+1)%nNewCol;

						if (x<nAppend)
						{
							iOverlay += nAppend;
						}
					}

					if (iOverlay>=nAllLen)
						break;

					if(pByte[iByte] & b[iBit])
					{
						Img.m_pOriOverlayData[0][iOverlay] = OverlayValue;	
					}
					else
					{
						Img.m_pOriOverlayData[0][iOverlay] = 0;
					}

					iOverlay++;	
				}

				if (iOverlay>=nAllLen)
					break;
			}

			delete []pByte;
		}
	}


	if (Img.m_pDisplayData)
	{
		ArrayFree((void**)Img.m_pDisplayData,0);
		Img.m_pDisplayData = NULL;
	}

	if (Img.m_bMpr == false)
	{
		Img.m_pDisplayData = (BYTE**)ArrayCopy((void**)Img.m_pOriData,nRow,nCol,nBytePerPix);
		Img.m_ImgState.nDispalyCol = nCol;
		Img.m_ImgState.nDispalyRow = nRow;
	}
	
	if (Img.m_pDisplayOverlayData)
	{
		ArrayFree((void**)Img.m_pDisplayOverlayData);
		Img.m_pDisplayOverlayData = NULL;
	}

	if (Img.m_pOriOverlayData)
	{
		if (Img.m_ImgInfo.nOverlayRows==Img.m_ImgInfo.nRows && Img.m_ImgInfo.nOverlayCols == Img.m_ImgInfo.nCols)
		{
			Img.m_pDisplayOverlayData = (BYTE**)ArrayCopy((void**)Img.m_pOriOverlayData,nRow,nCol,1);
		}
		else
		{
			//sos 在此处要记得保证OverLaydata行列要与像素行列一致.---有的文件并不一致.遇到再处理
			//::MessageBox(0,"OverLay行列与像素行列不同","新情况",0);
			ArrayFree((void **)Img.m_pOriOverlayData);
			Img.m_pOriOverlayData = NULL;
		}

	}

	Hs_GetLuts(pPixEle,Img);

	//解决某些图像没有设置wl或者wl为0的情况下，窗宽床位不对的问题
	if(Img.m_ImgInfo.fWinWidth <= 0.000001 )//没找到WC的情况下,
	{
		if (Img.m_ImgInfo.nSmallestPixelValue!=0 || Img.m_ImgInfo.nLargestPixelValue!=0)
		{
			//Img.m_ImgInfo.fWinWidth =  Img.m_ImgInfo.nLargestPixelValue - Img.m_ImgInfo.nSmallestPixelValue;
			//Img.m_ImgInfo.fWinCenter = (Img.m_ImgInfo.nLargestPixelValue + Img.m_ImgInfo.nSmallestPixelValue)/2;

			int tMax = Img.m_ImgInfo.nLargestPixelValue;
			int tMin = Img.m_ImgInfo.nSmallestPixelValue;
			if (Img.m_ImgInfo.fRescaleSlope!=0.00 && Img.m_ImgInfo.fRescaleIntercept!=0.00)
			{
				tMax = tMax*Img.m_ImgInfo.fRescaleSlope + Img.m_ImgInfo.fRescaleIntercept;
				tMin = tMin*Img.m_ImgInfo.fRescaleSlope + Img.m_ImgInfo.fRescaleIntercept;
			}

			Img.m_ImgInfo.fWinCenter = (tMin+tMax)*1.00/2;
			Img.m_ImgInfo.fWinWidth = tMax - tMin;
		}
		else
		{
			//Img.m_ImgInfo.fWinWidth =  1 << Img.m_ImgInfo.nBitStored;
			//Img.m_ImgInfo.fWinCenter = 1 << (Img.m_ImgInfo.nBitStored-1);

			int tMax = 1 << Img.m_ImgInfo.nBitStored;
			int tMin = 0;
			if (Img.m_ImgInfo.fRescaleSlope!=0.00 && Img.m_ImgInfo.fRescaleIntercept!=0.00)
			{
				tMax = tMax*Img.m_ImgInfo.fRescaleSlope + Img.m_ImgInfo.fRescaleIntercept;
				tMin = tMin*Img.m_ImgInfo.fRescaleSlope + Img.m_ImgInfo.fRescaleIntercept;
			}

			Img.m_ImgInfo.fWinCenter = (tMin+tMax)*1.00/2;
			Img.m_ImgInfo.fWinWidth = tMax - tMin;
		}

	}


	if (lbm.IsAllocated())//Leadtools应该自己会释放吧.不确定所以此处再释放一次
		lbm.Free();
	
	//注释信息
	//CAppConfig::GetInfoSet()

	if(bCloseFile==true)//如果原来文件是关闭的,则关闭.本函数不应该改变文件的打开状态.
		Hs_CloseFile();

	//病人的位置
	if (Img.m_ImgInfo.ImgLocPara.bValide==true)
	{
		
		Img.Hs_GetPatientPos(Img.m_ImgInfo.ImgLocPara.fFirstColCosX,
							Img.m_ImgInfo.ImgLocPara.fFirstColCosY,
							Img.m_ImgInfo.ImgLocPara.fFirstColCosZ,
							Img.m_ImgState.sTopPatientPos,
							Img.m_ImgState.sBottomPatientPos);

		Img.Hs_GetPatientPos(Img.m_ImgInfo.ImgLocPara.fFirstRowCosX,
							Img.m_ImgInfo.ImgLocPara.fFirstRowCosY,
							Img.m_ImgInfo.ImgLocPara.fFirstRowCosZ,
							Img.m_ImgState.sLeftPatientPos,
							Img.m_ImgState.sRightPatientPos);
	}

	//a.Log("\r\n读取像素完毕");
	return nRet;
}
//获得Modality Lut和Voi Lut两个 module的内容(Lut和Wl都要得到)
int CHsBaseFile::Hs_GetLuts(pHsElement pPixelEle,CHsBaseImg &Img)
{//寻到所有LutData
	
	int nRet = 0;

	//第一部分:VoiLut======================================================================
	pHsElement pVoiSeq = Hs_FindFirstEle(pPixelEle,TAG_VOI_LUT_SEQUENCE,true);

	if(pVoiSeq)
	{
		int i = 0;
		while(1)
		{
			pHsElement pItem = Hs_GetChildEle(pVoiSeq,0,i++);
			if(pItem==NULL)
				break;

			LutData *pNewLut = new LutData;
			pNewLut->bModality = false;
			pNewLut->bWc = false;
			pNewLut->iLutID = i;

			if (Hs_GetLutDataItem(pItem,*pNewLut) == false)
			{
				delete pNewLut;
				continue;
			}
			else
			{
				Img.m_pLutV.push_back(pNewLut);
			}	
		}

	}

	//VOI WinLevel:
	Hs_GetWcLutItem(NULL,Img.m_pLutV);
	/*
	pHsElement pWinCenter = Hs_FindFirstEle(0,TAG_WINDOW_CENTER,true);
	pHsElement pWinWidth = Hs_FindFirstEle(0,TAG_WINDOW_WIDTH,true);
	pHsElement pWinExplan = Hs_FindFirstEle(0,0x00281055,true);

	if (pWinCenter)
	{
		int i = -1;
		while(1)
		{
			i++;
			LutData *pNewWcLut = new LutData;
			pNewWcLut->bModality = false;
			pNewWcLut->bWc = true;
			nRet = Hs_GetDoubleValue(pWinCenter,pNewWcLut->nC,i);
			if(nRet)
			{
				delete pNewWcLut;
				break;
			}

			nRet = Hs_GetDoubleValue(pWinWidth,pNewWcLut->nW,i);
			if(nRet)
			{
				delete pNewWcLut;
				break;
			}

			nRet = Hs_GetStringValue(pWinExplan,pNewWcLut->sName,i);
			if(pNewWcLut->sName.empty())
			{
				char cName[100];
				sprintf(cName,"WL:%d",i+1);
				pNewWcLut->sName = cName;
			}
			else
			{
				char cName[200] = {'\0'};
				sprintf(cName,"WL:%s",pNewWcLut->sName.c_str());
				pNewWcLut->sName = cName;
			}

			Img.m_pLutV.push_back(pNewWcLut);

		}
	}
*/
	//第二部分:Modality Lut================================================================
	pHsElement pModSeq = Hs_FindFirstEle(pPixelEle,TAG_MODALITY_LUT_SEQUENCE,true);

	if(pModSeq)
	{
		int i = 0;
		while(1)
		{
			pHsElement pItem = Hs_GetChildEle(pVoiSeq,0,i++);
			if(pItem==NULL)
				break;

			LutData *pNewLut = new LutData;
			pNewLut->bModality = true;
			pNewLut->bWc = false;
			pNewLut->iLutID = i;

			if (Hs_GetLutDataItem(pItem,*pNewLut) == false)
			{
				delete pNewLut;
				continue;
			}
			else
			{
				Img.m_pLutV.push_back(pNewLut);
			}
		}

	}

	//第三部分:PALETTE COLOR LUT================================================================
	int bPaletteClor = int(Img.m_ImgInfo.sPhotometric.indexOf("PALETTE COLOR"));//不用compare.因为老有空格问题
	if (bPaletteClor>=0)
	{
		unsigned long nTagDes = TAG_RED_PALETTE_COLOR_LOOKUP_TABLE_DESCRIPTOR;
		unsigned long nTagData = TAG_RED_PALETTE_COLOR_LOOKUP_TABLE_DATA;
		QString sName = "R";

		int nColorLutCount = 0;//数一下,是三个lut不是?
		for(int i=0;i<3;i++)
		{
			if(i==1)
			{
				nTagDes = TAG_GREEN_PALETTE_COLOR_LOOKUP_TABLE_DESCRIPTOR;
				nTagData = TAG_GREEN_PALETTE_COLOR_LOOKUP_TABLE_DATA;
				sName = "G";
			}
			if (i==2)
			{
				nTagDes = TAG_BLUE_PALETTE_COLOR_LOOKUP_TABLE_DESCRIPTOR;
				nTagData = TAG_BLUE_PALETTE_COLOR_LOOKUP_TABLE_DATA;
				sName = "B";
			}

			LutData *pNewLut = new LutData;
			pNewLut->bModality = false;
			pNewLut->bWc = false;
			pNewLut->sName = sName;


			// Des
			pHsElement pLutDes = Hs_FindFirstEle(pPixelEle,nTagDes,true);
			nRet = Hs_GetLongValue(pLutDes,pNewLut->nLutLen,0);//Lut多长? 含多少个Data?
			if(nRet)
			{
				delete pNewLut;
				break;
			}

			nRet = Hs_GetLongValue(pLutDes,pNewLut->nMinValue,1);//最小值
			if(nRet)
			{
				delete pNewLut;
				break;
			}

			nRet = Hs_GetLongValue(pLutDes,pNewLut->nBitsPerData,2);//Lut的每个Data数据占几位
			if(nRet)
			{
				delete pNewLut;
				break;
			}

			// Data
			pHsElement pLutData = Hs_FindFirstEle(pPixelEle,nTagData,true);
			if (pNewLut->nLutLen==0)
				pNewLut->nLutLen= Hs_GetValueCount(pLutData,nRet);

			pNewLut->nBytePerData = pNewLut->nBitsPerData/8 + (pNewLut->nBitsPerData%8 > 0 ? 1:0);

			unsigned long nSize = pNewLut->nLutLen * pNewLut->nBytePerData ;//预计应有长度
			if(nSize==0)
			{
				delete pNewLut;
				break;
			}

			unsigned long nSizeGot = 0;
			BYTE *pBuf = Hs_GetByteValue(pLutData,nSizeGot,nRet);			

			if(nRet)
			{
				delete pNewLut;
				delete []pBuf;
				break;
			}

			if(nSizeGot!=nSize)//预计长度与实际长度不符合
			{
				delete pNewLut;
				delete []pBuf;
				break;
			}

			//将pBuf整理成0-255
			long nMaxLutData = 1<<pNewLut->nBitsPerData;
			double f = 256.00/nMaxLutData;
			pNewLut->pLutData = new BYTE[pNewLut->nLutLen];

			if (pNewLut->nBytePerData==1)
			{
				BYTE *tData = (BYTE*)pBuf;
				for (int k=0;k<pNewLut->nLutLen;k++)
				{
					pNewLut->pLutData[k] = tData[k]*f;
					if(pNewLut->pLutData[k]>=255)
						pNewLut->pLutData[k] = 255;

					//AtlTrace("\r\n%d-%d",tData[k],pNewLut->pLutData[k]);
				}

			}
			if (pNewLut->nBytePerData==2)
			{
				unsigned short *tData = (unsigned short*)pBuf;
				for (int k=0;k<pNewLut->nLutLen;k++)
				{
					pNewLut->pLutData[k] = tData[k]*f;
					if(pNewLut->pLutData[k]>=255)
						pNewLut->pLutData[k] = 255;

					//BYTE *pp = (BYTE*)(&tData[k]);

					//if(k==236)
					//	AtlTrace("\r\n%d:%d-%d  (%d %d)",k,tData[k],pNewLut->pLutData[k],pp[0],pp[1]);
				}
			}

			delete []pBuf;

			pNewLut->nMaxValue = pNewLut->nMinValue + pNewLut->nLutLen - 1;

			Img.m_pLutV.push_back(pNewLut);
			nColorLutCount++;

		} 

		////看三个Lut的最大值最小值.把初始WC搞出来.下面代码依赖于:
		////1.必须有三个LUT.
		////2.三个Lut的描述DESCRIPTOR必须完全一致,也就是三大件要求一致.否则我没法办.
		//if (nColorLutCount==3)
		//{
		//	LutData *tLut = Img.m_pLutV[Img.m_pLutV.size()-1];
		//	Img.m_ImgInfo.fWinWidth = tLut->nMaxValue - tLut->nMinValue;
		//	Img.m_ImgInfo.fWinCenter = (tLut->nMaxValue + tLut->nMinValue)/2;
		//}

	}

	return Ret_Success;
}
/*
图像分为压缩图像可原始图像:

一、原始图像
1、	每帧图像是紧紧连在一起的,没有分界符号.

2、	每个像素分为n个Sample,每个Sample分配了nBitAllocate(8的倍数)位、
	每个Sample实际占用nBitsStored位,
	每个Sample的最高位在第iHighBit位(竟然有大于nBitStored的情况),
	所以每个像素需要的BitsPerPixel = Sample*nBitAllocate位.

3、	任何图像都是需要WL_LUT的,这个WL_LUT是我们自己创建的.像素值(unsigned short型)最为索引,要去WL_LUT中对应到一个0-255之间的值作为最终显示值.
	那么Lut表的长度是pow(2,nBitsStored);
	如果最高位iHighBit后面还有其他数值,比如OverLay紧随HighBit在后,那么可考虑建立长度为pow(2,nBitsStored+1)的WL_LUT表,使程序运行更快.代码也简洁.
	只不过这时候的Lut的上半部分不受OVerlay的影响,下半部分要考虑一下OverLay的显示和突出即可.(突出:分为一直显示最白色,还是比上半部分该值稍亮一点,这两种情况)

	如果最高位有高位补码,那Lut长度就要是pow(2,nBitAllocate)了.

4、	ModalityLUT:标准中规定必须应用的LUT.这个Lut并不会对应出0-255的数字,对应出的数字仍然在0-pow(2,上述所述情况).
	最终将从ModalityLut处对应到的数字,再去我们自己的WL_LUT中对应到0-255的数字.
	VOI Lut是可选的lut,是设备推荐的几种Lut效果. 其实医院实际应用时,Modality Lut有时候医院也希望可选.--要做成配置.

5、	有时并无Lut,但需要ax+b : 0x00281053*x + 0x00281052

二、压缩图像,先想办法解压吧.

*/

pHsElement CHsBaseFile::DestroyEle(pHsElement pEle)
{
	if (pEle==NULL)
		return NULL;

	if (pEle->pPreEle)
		pEle->pPreEle->pNextEle = NULL;
	
	if (pEle->pParentEle)
	{
		int n = pEle->pParentEle->pChildEleV.size();
		for(int i=0;i<n;i++)
		{
			if(pEle->pParentEle->pChildEleV[i] == pEle)
				pEle->pParentEle->pChildEleV.erase(pEle->pParentEle->pChildEleV.begin()+i);
		}
	}

	delete pEle;

	return NULL;
}

pHsElement CHsBaseFile::BuildListRelation(pHsElement pEle)
{//有孩子的,请交出最后一个孩子,没孩子交出你自己,以便作为下次递归的 "上一个"
	int n = pEle->pChildEleV.size();
	pHsElement pPre = pEle;

	for (int i=0;i<n;i++)
	{
		pEle->pChildEleV[i]->pPreEle = pPre;
		if(pPre)
			pPre->pNextEle = pEle->pChildEleV[i];

		pPre = BuildListRelation(pEle->pChildEleV[i]);
	}

	return pPre;
}

pDICOMELEMENT CHsBaseFile::GetLtEleByMyEle(pHsElement pMyEle, LDicomDS *pDS)
{
	if(pDS==NULL)
		return NULL;

	//pMyEle在本文件中是第几个同类型Ele
	int nPixID0 = 0;
	pHsElement tHsPixEle = Hs_FindFirstEle(0,pMyEle->nTag,false);
	while(tHsPixEle)
	{
		tHsPixEle = Hs_FindNextEle(tHsPixEle,FALSE);
		if(tHsPixEle)
			nPixID0++;
	}

	//在pDS中也应该是第nPixID0个
	int nPixID1 = 0;
	pDICOMELEMENT pLtPix = pDS->FindFirstElement(NULL,pMyEle->nTag,FALSE);
	while(pLtPix)
	{
		if(nPixID1==nPixID0)
			return pLtPix;

		pLtPix = pDS->FindNextElement(pLtPix,FALSE);
		nPixID1++;
	}

	return NULL;
}

int CHsBaseFile::SeparatePixdataAndOverlayByRow(CHsBaseImg &Img,unsigned long iRow,unsigned long nCol,long &nMin,long &nMax)
{
	//三中心的DSA你大爷的,你的Overlay到底TMD放在第几位?
	if(Img.m_ImgInfo.nBitsPerPixel==8)
	{
		unsigned short nBitOffset = 8 - Img.m_ImgInfo.nBitStored;

		for(unsigned long i=0;i<nCol;i++)
		{
			//如果含嵌入式OverLay要抠出来,放到单独的二维数组内				

			//我不管你Overlay放到第几位.除去像素部分剩下的不是0，那就是OverLay
			if (Img.m_pOriData[iRow][i] >> Img.m_ImgInfo.nBitStored )
			{
				if(Img.m_pOriOverlayData==NULL)
				{
					Img.m_pOriOverlayData = (BYTE**)ArrayNew(Img.m_ImgInfo.nRows,Img.m_ImgInfo.nCols,1);
					Img.m_ImgInfo.nOverLayType = OverLay_Pixel;
					Img.m_ImgInfo.nOverlayRows = Img.m_ImgInfo.nRows;
					Img.m_ImgInfo.nOverlayCols = Img.m_ImgInfo.nCols;
				}

				Img.m_pOriOverlayData[iRow][i] = OverlayValue;
			}


			//我不管你OverLay第几位,全部滚蛋:左移挤出去,右移抽回来,就只剩像素了.这一步可确保像素在可控范围内;有的Dcm文件是骗人的.没说明OverLay,却偷偷塞了OverLay数据.导致像素值超出nBitStored指定的范围
			Img.m_pOriData[iRow][i] = Img.m_pOriData[iRow][i] << nBitOffset;
			Img.m_pOriData[iRow][i] = Img.m_pOriData[iRow][i] >> nBitOffset;

			if(Img.m_pOriData[iRow][i] > nMax)
				nMax = Img.m_pOriData[iRow][i];

			if(Img.m_pOriData[iRow][i] < nMin)
				nMin = Img.m_pOriData[iRow][i];

		}
	}
	else if (Img.m_ImgInfo.nBitsPerPixel==16)
	{
		unsigned short nBitOffset = 16 - Img.m_ImgInfo.nBitStored;

		unsigned short* pRowDataAsShort = (unsigned short*)(Img.m_pOriData[iRow]);//把这一行看做每俩字节表示一个数字的short数组
		for(unsigned long i=0;i<nCol;i++)
		{
			//我不管你有没有Overlay,也不管放到第几位.反正除去像素部分剩下的不是0，那就是OverLay
			if (pRowDataAsShort[i] >> Img.m_ImgInfo.nBitStored )
			{
				if(Img.m_pOriOverlayData==NULL)
				{
					Img.m_pOriOverlayData = (BYTE**)ArrayNew(Img.m_ImgInfo.nRows,Img.m_ImgInfo.nCols,1);
					Img.m_ImgInfo.nOverLayType = OverLay_Pixel;
					Img.m_ImgInfo.nOverlayRows = Img.m_ImgInfo.nRows;
					Img.m_ImgInfo.nOverlayCols = Img.m_ImgInfo.nCols;
				}
				Img.m_pOriOverlayData[iRow][i] = OverlayValue;
			}

			//我不管你OverLay第几位,全部滚蛋,左移挤出去,右移抽回来,就只剩像素了.这一步可确保像素在可控范围内,有的Dcm文件是骗人的.没说明OverLay,却偷偷塞了OverLay数据.导致像素值超出nBitStored指定的范围
			pRowDataAsShort[i] = pRowDataAsShort[i] << nBitOffset;
			pRowDataAsShort[i] = pRowDataAsShort[i] >> nBitOffset;

			if(pRowDataAsShort[i] > nMax)
				nMax = pRowDataAsShort[i];

			if(pRowDataAsShort[i] < nMin)
				nMin = pRowDataAsShort[i];

		}
	}
	else if (Img.m_ImgInfo.nBitsPerPixel==32)
	{
		unsigned short nBitOffset = 32 - Img.m_ImgInfo.nBitStored;

		unsigned long* pRowDataAsShort = (unsigned long*)(Img.m_pOriData[iRow]);//把这一行看做每俩字节表示一个数字的short数组
		for(unsigned long i=0;i<nCol;i++)
		{
			//我不管你有没有Overlay,也不管放到第几位.反正除去像素部分剩下的不是0，那就是OverLay
			if (pRowDataAsShort[i] >> Img.m_ImgInfo.nBitStored )
			{
				if(Img.m_pOriOverlayData==NULL)
				{
					Img.m_pOriOverlayData = (BYTE**)ArrayNew(Img.m_ImgInfo.nRows,Img.m_ImgInfo.nCols,1);
					Img.m_ImgInfo.nOverLayType = OverLay_Pixel;
					Img.m_ImgInfo.nOverlayRows = Img.m_ImgInfo.nRows;
					Img.m_ImgInfo.nOverlayCols = Img.m_ImgInfo.nCols;
				}
				Img.m_pOriOverlayData[iRow][i] = OverlayValue;
			}

			//我不管你OverLay第几位,全部滚蛋,左移挤出去,右移抽回来,就只剩像素了.这一步可确保像素在可控范围内,有的Dcm文件是骗人的.没说明OverLay,却偷偷塞了OverLay数据.导致像素值超出nBitStored指定的范围
			pRowDataAsShort[i] = pRowDataAsShort[i] << nBitOffset;
			pRowDataAsShort[i] = pRowDataAsShort[i] >> nBitOffset;

			if(pRowDataAsShort[i] > nMax)
				nMax = pRowDataAsShort[i];

			if(pRowDataAsShort[i] < nMin)
				nMin = pRowDataAsShort[i];

		}
	}

	return Ret_Success;
}

bool CHsBaseFile::GetImageLocPara( ImageInfo &info )
{
	if(info.ImgLocPara.bValide == true)
		return true;

	memset(&info.ImgLocPara,0,sizeof(ImageLoc));

	//if (m_buf==NULL)
	//	return false;
	if(m_pMainEle==NULL)
		return false;

	//Row
	pHsElement pRowEle = Hs_FindFirstEle(NULL,TAG_ROWS,true);

	if(Hs_GetLongValue(pRowEle,info.ImgLocPara.nRow)!=Ret_Success) 
		return false;

	//Col
	pHsElement pColEle = Hs_FindFirstEle(NULL,TAG_COLUMNS,true);

	if(Hs_GetLongValue(pColEle,info.ImgLocPara.nCol)!=Ret_Success) 
		return false;

	//行列有效性
	if(info.ImgLocPara.nRow==0 || info.ImgLocPara.nCol==0)
		return false;


	//Pixel Spacing
	pHsElement pPixSpacingEle = Hs_FindFirstEle(NULL,TAG_PIXEL_SPACING,true);

	if(Hs_GetDoubleValue(pPixSpacingEle,info.ImgLocPara.fPixSpacingY,0)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pPixSpacingEle,info.ImgLocPara.fPixSpacingX,1)!=Ret_Success) 
		return false;

	// TAG_IMAGE_POSITION_PATIENT 0x00200032
	pHsElement pPosEle = Hs_FindFirstEle(NULL,TAG_IMAGE_POSITION_PATIENT,true);

	if(Hs_GetDoubleValue(pPosEle,info.ImgLocPara.fOriLeftTopPixX,0)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pPosEle,info.ImgLocPara.fOriLeftTopPixY,1)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pPosEle,info.ImgLocPara.fOriLeftTopPixZ,2)!=Ret_Success) 
		return false;

	info.ImgLocPara.fLeftTopPixX = info.ImgLocPara.fOriLeftTopPixX;
	info.ImgLocPara.fLeftTopPixY = info.ImgLocPara.fOriLeftTopPixY;
	info.ImgLocPara.fLeftTopPixZ = info.ImgLocPara.fOriLeftTopPixZ;

	// TAG_IMAGE_ORIENTATION_PATIENT 0x00200037UL
	pHsElement pOriEle = Hs_FindFirstEle(NULL,TAG_IMAGE_ORIENTATION_PATIENT,true);

	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstRowCosX,0)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstRowCosY,1)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstRowCosZ,2)!=Ret_Success) 
		return false;

	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstColCosX,3)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstColCosY,4)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstColCosZ,5)!=Ret_Success) 
		return false;


	info.ImgLocPara.fFirstRowCosX = info.ImgLocPara.fOriFirstRowCosX;
	info.ImgLocPara.fFirstRowCosY = info.ImgLocPara.fOriFirstRowCosY;
	info.ImgLocPara.fFirstRowCosZ = info.ImgLocPara.fOriFirstRowCosZ;

	info.ImgLocPara.fFirstColCosX = info.ImgLocPara.fOriFirstColCosX;
	info.ImgLocPara.fFirstColCosY = info.ImgLocPara.fOriFirstColCosY;
	info.ImgLocPara.fFirstColCosZ = info.ImgLocPara.fOriFirstColCosZ;


	info.ImgLocPara.bValide = true;

	info.ImgLocPara.fRowmm = info.ImgLocPara.nRow * info.ImgLocPara.fPixSpacingY;
	info.ImgLocPara.fColmm = info.ImgLocPara.nCol * info.ImgLocPara.fPixSpacingX;

	if (info.fPixelSpaceX < 0.000001)
		info.fPixelSpaceX = info.ImgLocPara.fPixSpacingX;
	if (info.fPixelSpaceY < 0.000001)
		info.fPixelSpaceY = info.ImgLocPara.fPixSpacingY;

	//层厚也搞到手吧
	Hs_GetDoubleValueA(TAG_SLICE_THICKNESS,info.ImgLocPara.fSliceThickness);

	//层位置也搞
	Hs_GetDoubleValueA(TAG_SLICE_LOCATION,info.ImgLocPara.fSliceLoction);

	return true;
}

int CHsBaseFile::Hs_CopyTo( CHsBaseFile &NewFile)
{
	NewFile.m_BaseFileInfo = m_BaseFileInfo;
	NewFile.m_fp = NULL;
	NewFile.m_nDefaultReadSize = m_nDefaultReadSize;
	NewFile.m_pPreEle = NULL;
	NewFile.m_nBufLen = m_nBufLen;

	//copy buf
	if (NewFile.m_buf)
			delete []NewFile.m_buf;
	
	NewFile.m_buf = NULL;

	if(m_buf)
	{
		NewFile.m_buf = new BYTE[m_nBufLen];
		memcpy(NewFile.m_buf,m_buf, m_nBufLen);
	}

	//copy element
	if(NewFile.m_pMainEle)
		delete NewFile.m_pMainEle;

	NewFile.m_pMainEle = NULL;

	if (m_pMainEle)
	{
		NewFile.m_pMainEle = new HsElement;
		CopyEle(m_pMainEle,NewFile.m_pMainEle);

		m_pMainEle->pParentEle = NULL;

		NewFile.BuildListRelation(NewFile.m_pMainEle);
	}
	
	return 0;

}

void CHsBaseFile::CopyEle( HsElement* pFrom,HsElement* pTo )
{//pFrom有NUll的情况，700多幅的那个序列可引发

	if(pFrom==NULL || pTo==NULL)
	{
		QByteArray ba = m_BaseFileInfo.sFileName.toLatin1();
		qDebug("\r\nFileName = %s;pFrom==0x%08x || pTo==0x%08x",ba.data(),pFrom,pTo);
		return;
	}

	*pTo = *pFrom;
	//pTo->bBigEndian = pFrom->bBigEndian;
	//pTo->bVirtualItem = pFrom->bVirtualItem;
	//pTo->nLen = pFrom->nLen;
	//pTo->nLenDesc = pFrom->nLenDesc;
	//pTo->nOffset = pFrom->nOffset;
	//pTo->nTag = pFrom->nTag;
	//pTo->nTagPos = pFrom->nTagPos;
	//pTo->nValueCount = pFrom->nValueCount;
	//pTo->nVR = pFrom->nVR;
	//pTo->pNextEle = NULL;
	//pTo->pPreEle = NULL;

	int n = int(pFrom->pChildEleV.size());
	for (int i=0;i<n;i++)
	{
		HsElement *pNewEle = new HsElement;
		CopyEle(pFrom->pChildEleV[i],pNewEle);
		pNewEle->pParentEle = pTo;

		pTo->pChildEleV.push_back(pNewEle);
	}
	
}


bool TAGSORT(const pHsElement &a,const pHsElement &b)
{
	return a->nTag < b->nTag;
}

//编辑DICOM文件
pHsElement CHsBaseFile::Hs_InsertElement(pHsElement pNeighbor,bool bChild, unsigned long nTag,unsigned long nVR,bool bSquence,int nIndex,int &nRet)
{
	if (m_pMainEle == NULL)
	{
		m_pMainEle = new HsElement;
	}

	if (pNeighbor == NULL)
	{
		pNeighbor = m_pMainEle;
		pNeighbor->nVR = VR_SQ;
		bChild = true;
	}

	if ((nTag/65536)%2 == 0 && nTag!=TAG_ITEM)
	{
		if (CDcmTag::Find(nTag)->nVR!=nVR)
		{
			nRet = Ret_InvalidPara;
			return NULL;
		}
		else if (nVR != VR_SQ && bSquence == true)
		{
			nRet = Ret_InvalidPara;
			return NULL;
		}
	}

	if (nTag == TAG_ITEM)
	{
		int nItemNum;
		if (bChild == 0)
			nItemNum = pNeighbor->pParentEle->pChildEleV.size();
		else
			nItemNum = pNeighbor->pChildEleV.size();
		if (nIndex+1-nItemNum>1)
			nIndex = nItemNum;
	}


	HsElement *pInsertEle = new HsElement;
	pInsertEle->nLenDesc = CDcmVR::Find(nVR)->nLenOfLenDes;
	pInsertEle->nLen = 0;
	pInsertEle->bSquence = bSquence;
	pInsertEle->bNewTag = TRUE;

	if (nTag == TAG_ITEM)
		pInsertEle->nOffset = 8;	
	else
		pInsertEle->nOffset = 4 + CDcmVR::Find(nVR)->nLenOfVrDes + CDcmVR::Find(nVR)->nLenOfLenDes;

	if (nTag!=0)
	{
		pInsertEle->nTag = nTag;
	}
	else
	{
		DestroyEle(pInsertEle);
		nRet = Ret_InvalidPara;
		return NULL;
	}

	if (nTag == TAG_ITEM)
	{
		nVR = NULL;
	}
	else
	{
		if ((nTag/65536)%2!=0 && bSquence==true)
			nVR = VR_SQ;
		else
		{
			if(nVR)
			{
				pDcmVR pVR = CDcmVR::Find(nVR);
				if (pVR->nCode == 0)	
				{
					nRet = Ret_InvalidPara;
					return NULL;
				}
				pInsertEle->nVR = nVR;
			}
			else
			{
				pInsertEle->nVR = CDcmTag::Find(nTag)->nVR;
			}

		}
	}


	if (bChild ==0)
	{
		if (pNeighbor->pParentEle)
		{
			pInsertEle->pParentEle = pNeighbor->pParentEle;
		}
		else
		{
			DestroyEle(pInsertEle);
			nRet = Ret_InvalidPara;
			return NULL;
		}
	}
	else
	{
		if ((pNeighbor->nVR == VR_SQ && pInsertEle->nTag == TAG_ITEM)|| pNeighbor->nTag == TAG_ITEM || pNeighbor->nVR == VR_SQ)
		{
			pInsertEle->pParentEle = pNeighbor;
		}
		else
		{
			DestroyEle(pInsertEle);
			nRet = Ret_InvalidPara;
			return NULL;
		}		
	}

	if (nTag == TAG_ITEM)
	{
		if ((bChild == 0 && pNeighbor->pParentEle->nVR != VR_SQ)||
			(bChild && pNeighbor->nVR != VR_SQ) )
		{
			DestroyEle(pInsertEle);
			nRet = Ret_InvalidPara;
			return NULL;
		}

		if (nIndex == 0)
		{
			pInsertEle->pPreEle = pInsertEle->pParentEle;
			if (pInsertEle->pParentEle->pChildEleV.size() != 0)
				pInsertEle->pNextEle = pInsertEle->pParentEle->pChildEleV[0];
			else
				pInsertEle->pNextEle = pInsertEle->pPreEle->pNextEle;

		}
		else
		{
			pInsertEle->pPreEle = pInsertEle->pParentEle->pChildEleV[nIndex-1];
			pInsertEle->pNextEle = pInsertEle->pParentEle->pChildEleV[nIndex-1]->pNextEle;
		}
	}
	else
	{
		int nOldChildNum = pInsertEle->pParentEle->pChildEleV.size();
		for (int i = 0; i<nOldChildNum; i++)
		{
			unsigned long nPreTag = pInsertEle->pParentEle->pChildEleV[i]->nTag;
			if (nTag == nPreTag)
			{
				DestroyEle(pInsertEle);
				return NULL;
			}
			else if (nTag<nPreTag)
			{
				if (i == 0)
				{
					pInsertEle->pPreEle = pInsertEle->pParentEle;
					pInsertEle->pNextEle = pInsertEle->pParentEle->pChildEleV[i];
				}
				else
				{
					pInsertEle->pPreEle = pInsertEle->pParentEle->pChildEleV[i-1];
					pInsertEle->pNextEle = pInsertEle->pParentEle->pChildEleV[i];
					break;
				}
			}
		}
	}

	pInsertEle->pParentEle->pChildEleV.push_back(pInsertEle);
	qSort(pInsertEle->pParentEle->pChildEleV.begin(),pInsertEle->pParentEle->pChildEleV.end(),TAGSORT);

	nRet = Ret_Success;
	return pInsertEle;

}


int CHsBaseFile::Hs_DeleteElement(pHsElement pEle)
{
	if(pEle==NULL)
		return Ret_InvalidPara;

	if (pEle->pParentEle)
	{
		for (QVector<pHsElement>::iterator i =pEle->pParentEle->pChildEleV.begin(); i<pEle->pParentEle->pChildEleV.end();i++ )
		{
			if ((*i) == pEle)
			{
				pEle->pParentEle->pChildEleV.erase(i);
				break;
			}
		}
	}
	if (pEle->pPreEle)
	{
		pEle->pPreEle->pNextEle = pEle->pNextEle;
	}
	if (pEle->pNextEle)
	{
		pEle->pNextEle->pPreEle = pEle->pPreEle;
	}

	delete pEle;

	return Ret_Success;
}


int CHsBaseFile::Hs_SetLongValue(pHsElement pEle, long nValue,bool bCover=false,int nValueIndex=0)
{
	if (pEle == NULL)
		return Ret_InvalidElement;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_SL&& pEle->nVR != VR_UL &&pEle->nVR != VR_AT&&pEle->nVR!=VR_IS)
		return Ret_InvalidPara;

	this->ValueToEle((void*)&nValue,pEle,sizeof(long),bCover,nValueIndex);

	pEle->bNewTag = TRUE;

	return Ret_Success;
}

int CHsBaseFile::Hs_SetShortValue(pHsElement pEle, short nValue,bool bCover=false,int nValueIndex=0)
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_SS && pEle->nVR != VR_US && pEle->nVR != VR_OW && pEle->nVR != VR_OB)
		return Ret_InvalidPara;

	if (pEle->nVR == VR_OB)
		this->ValueToEle((void*)&nValue,pEle,1,bCover,nValueIndex);
	else
		this->ValueToEle((void*)&nValue,pEle,sizeof(short),bCover,nValueIndex);


	pEle->bNewTag = TRUE;

	return Ret_Success;
}

int CHsBaseFile::Hs_SetDoubleValue(pHsElement pEle, double nValue,bool bCover=false,int nValueIndex=0 )
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR !=VR_FD && pEle->nVR != VR_DS)// &&pEle->nVR != VR_OF && pEle->nVR != VR_FL  )
		return Ret_InvalidPara;

	this->ValueToEle((void*)&nValue,pEle,sizeof(double),bCover,nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}

int CHsBaseFile::Hs_SetFloatValue(pHsElement pEle, float nValue,bool bCover=false,int nValueIndex=0 )
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_FL)
		return Ret_InvalidPara;

	this->ValueToEle((void*)&nValue,pEle,sizeof(float),bCover,nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}

int CHsBaseFile::Hs_SetStringValue(pHsElement pEle, QString nValue,bool bCover=false,int nValueIndex=0 )
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR!=VR_CS && pEle->nVR!=VR_SH && pEle->nVR!=VR_LO && pEle->nVR!=VR_AE && pEle->nVR!=VR_LT
		&& pEle->nVR!=VR_ST && pEle->nVR!=VR_UI && pEle->nVR!=VR_UT && pEle->nVR!=VR_PN)
		return Ret_InvalidPara;

	QByteArray ba = nValue.toLatin1();
	this->ValueToEle((void*)ba.data(), pEle, nValue.length(), bCover, nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}

int CHsBaseFile::Hs_SetDataValue(pHsElement pEle, HsDateTime nValue,bool bCover=false,int nValueIndex=0)
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_DA )
		return Ret_InvalidPara;
	if (nValue.nMonth>12|| nValue.nDay>31)
		return Ret_InvalidPara;

	char temp[9] ;
	memset(temp,0,9);
	sprintf_s(temp,"%04d%02d%02d",nValue.nYear,nValue.nMonth,nValue.nDay);

	this->ValueToEle((void*)&temp,pEle,8,bCover,nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}

int CHsBaseFile::Hs_SetTimeValue(pHsElement pEle, HsDateTime nValue,bool bCover=false,int nValueIndex=0)
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_TM )
		return Ret_InvalidPara;
	if (nValue.nHours>23|| nValue.nMinutes>59||nValue.nSeconds>59||nValue.nFractions>999999)
		return Ret_InvalidPara;

	char temp[100] ;
	memset(temp,0,100);
	if (nValue.nFractions!=0)
		sprintf_s(temp,"%02d%02d%02d.%d",nValue.nHours,nValue.nMinutes,nValue.nSeconds,nValue.nFractions);
	else
		sprintf_s(temp,"%02d%02d%02d",nValue.nHours,nValue.nMinutes,nValue.nSeconds);


	int nLen = strlen(temp);
	this->ValueToEle((void*)&temp,pEle,nLen,bCover,nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}
int CHsBaseFile::Hs_SetAgeValue(pHsElement pEle,int nAge,char cAgeType,bool bCover=false,int nValueIndex=0)
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_AS )
		return Ret_InvalidPara;
	if (nAge>999)
		return Ret_InvalidPara;

	int nRet = 0;
	int nCount = Hs_GetValueCount(pEle,nRet);
	if (nCount>0)
		return Ret_InvalidPara;

	QString sAgeType;
	sAgeType = QString("%1").arg(cAgeType);
	if (!(sAgeType.compare("D") || sAgeType.compare("M") || sAgeType.compare("Y")))
		return Ret_InvalidPara;

	char Temp[100];
	memset(Temp,0,100);

	sprintf(Temp,"%1%2",nAge,cAgeType);	


	this->ValueToEle((void*)&Temp,pEle,4,bCover,nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}
int CHsBaseFile::Hs_SetDataTimeValue(pHsElement pEle, HsDateTime nValue,bool bCover=false,int nValueIndex=0)
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nValueIndex<0)
		return Ret_OutOfValueCount;
	if (pEle->nVR != VR_DT )
		return Ret_InvalidPara;
	if (nValue.nHours>23|| nValue.nMinutes>59||nValue.nSeconds>59||nValue.nFractions>999999||nValue.nMonth>12|| nValue.nDay>31)
		return Ret_InvalidPara;

	char temp[100] ;
	memset(temp,0,100);
	QString sOffsetState="";
	if (nValue.nOffset>=0)
		sOffsetState = "+";
	else
		sOffsetState = "-";

	QByteArray ba = sOffsetState.toLatin1();
	sprintf_s(temp,"%04d%02d%02d%02d%02d%02d.%d%c%04d",nValue.nYear,nValue.nMonth,nValue.nDay,nValue.nFractions,ba.data(),nValue.nOffset);

	int nLen = strlen(temp);

	this->ValueToEle((void*)&temp,pEle,nLen,bCover,nValueIndex);

	pEle->bNewTag = TRUE;
	return Ret_Success;
}

int CHsBaseFile::ValueToEle(void *pData,pHsElement pEle, int nNewValueLen,bool bCover, int nValueIndex)
{
	if (nNewValueLen ==0 || pData == NULL || pEle == NULL || nValueIndex<0)
	{
		return 0;
	}

	if (pEle->nVR == VR_LT || pEle->nVR == VR_ST || pEle->nVR == VR_UT)  //这三个VR类型不能有多值
	{
		if (nValueIndex>0)
			return Ret_OutOfValueCount;
	}
	int iFunState;
	unsigned long nValueCount = Hs_GetValueCount(pEle,iFunState);
	BYTE *pDataByte = new BYTE[nNewValueLen];
	memcpy(pDataByte,pData,nNewValueLen);
	BYTE *pTempValue = new BYTE[nNewValueLen];

	if (pEle->nVR == VR_SL  || pEle->nVR == VR_UL || pEle->nVR == VR_FL || pEle->nVR == VR_OF)
	{		
		if (pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			pTempValue[0] = pDataByte[3];pTempValue[1] = pDataByte[2]; pTempValue[2] = pDataByte[1]; pTempValue[3] = pDataByte[0];
		}
		else
		{
			memcpy(pTempValue,pDataByte,nNewValueLen);
		}
	}
	else if (pEle->nVR == VR_AT)
	{		
		if (pEle->bBigEndian== false)
		{
			if (m_BaseFileInfo.bCpuBigEndia  == false)
			{
				pTempValue[0] = pDataByte[2]; pTempValue[1] = pDataByte[3]; pTempValue[2] = pDataByte[0]; pTempValue[3] = pDataByte[1];
			}
			else
			{
				pTempValue[0] = pDataByte[3]; pTempValue[1] = pDataByte[2]; pTempValue[2] = pDataByte[1]; pTempValue[3] = pDataByte[0];
			}
		}
		else
		{
			if (m_BaseFileInfo.bCpuBigEndia  ==false)
			{
				pTempValue[0] = pDataByte[1]; pTempValue[1] = pDataByte[0]; pTempValue[2] = pDataByte[3]; pTempValue[3] = pDataByte[2];
			}
			else
			{
				memcpy(&pTempValue,&pDataByte,4);
			}
		}
	}
	else if (pEle->nVR == VR_SS|| pEle->nVR == VR_US|| pEle->nVR == VR_OW )
	{
		if (pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			pTempValue[0] = pDataByte[1]; pTempValue[1] = pDataByte[0];
		}
		else
		{
			memcpy(pTempValue,pDataByte,nNewValueLen);
		}
	}
	else if (pEle->nVR ==VR_FD )
	{
		if (pEle->bBigEndian!=m_BaseFileInfo.bCpuBigEndia)
		{
			pTempValue[0] = pDataByte[7]; pTempValue[1] = pDataByte[6];pTempValue[2] = pDataByte[5];pTempValue[3] = pDataByte[4];
			pTempValue[4] = pDataByte[3]; pTempValue[5] = pDataByte[2];pTempValue[6] = pDataByte[1];pTempValue[7] = pDataByte[0];
		}
		else
		{
			memcpy(pTempValue,pDataByte,nNewValueLen);
		}
	}
	else if (pEle->nVR == VR_IS || pEle->nVR == VR_DS)
	{
		char Is[100];
		memset(Is,0,100);
		if (pEle->nVR == VR_IS)
			sprintf(Is,"%d",*(long*)pDataByte);
		else
			sprintf(Is,"%f",*(double*)pDataByte);		
		nNewValueLen = min(nNewValueLen,(int)strlen(Is));//memcpy(pTempValue,Is,nNewValueLen);pTempValue new了8个，此处复制10个，焉能不错

		if (pEle->nVR == VR_IS)
		{
			if (nNewValueLen>12)
				return 0;
		}
		else
		{
			if (nNewValueLen>16)
				return 0;
		}

		memcpy(pTempValue,Is,nNewValueLen);
	}
	else if (pEle->nVR==VR_CS || pEle->nVR==VR_SH || pEle->nVR==VR_LO || pEle->nVR==VR_AE || pEle->nVR==VR_LT|| pEle->nVR==VR_ST  
		|| pEle->nVR==VR_UT || pEle->nVR==VR_PN||pEle->nVR == VR_DA || pEle->nVR == VR_DT|| pEle->nVR == VR_TM || pEle->nVR == VR_AS)
	{
		memcpy(pTempValue,pDataByte,nNewValueLen);
	}
	else if (pEle->nVR == VR_OB|| pEle->nVR==VR_UI)
	{
		memcpy(pTempValue,pDataByte,nNewValueLen);
	}
	else if(pEle->nVR == VR_UN || pEle->nVR == VR_00)
	{
		//char Is[100];
		//memset(Is,0,100);
		//sprintf(Is,"%c",pDataByte);
		//iTypeSize = strlen(Is);
		//if (iTypeSize%2!=0)
		//	iNewTypeSize = iTypeSize + 1;	
		//if (pTempValue)
		//{
		//	delete pTempValue;
		//	pTempValue = NULL;
		//}
		//memset(pTempValue,0x20,iNewTypeSize);
		//if (m_BaseFileInfo.bCpuBigEndia == true)
		//{
		//	for (int i =0; i<iTypeSize;i++)
		//	{
		//		pTempValue[i] = pDataByte[iTypeSize-1-i];
		//	}
		//}
		//else
		//{
		//	memcpy(pTempValue,pDataByte,iTypeSize);
		//}
	}
	if (nValueCount == 0)//情况1：原来Ele并没有值域
	{
		pEle->nLen = pEle->nLen + nNewValueLen;
		int nOldLen = pEle->nLen;
		if (pEle->nVR !=VR_UL && pEle->nVR != VR_FL && pEle->nVR != VR_FD && pEle->nVR != VR_SS  && pEle->nVR != VR_SL  && pEle->nVR != VR_US && pEle->nVR != VR_AT && pEle->nVR != VR_OB)//add by wxs for hmy
		{
			if (pEle->nLen%2 != 0)
				pEle->nLen = pEle->nLen + 1;
		}
		pEle->pValue = new BYTE[pEle->nLen];
		memset(pEle->pValue,0,pEle->nLen);//DICOM规定值域长度为奇数时，应以0x20(即:空格)补成偶数个字节---需要再验证？？？
		memcpy(pEle->pValue,pTempValue,nOldLen);	

		//for (int ii=0;ii<pEle->nLen;ii++)
		//{
		//	if(ii==55)
		//		int g = 0;
		//	AtlTrace("%02d: --------  [%c] -------  [%d] --------  [%x]\r\n",ii,pEle->pValue[ii],pEle->pValue[ii],pEle->pValue[ii]);
		//}
	}
	else if (nValueCount-1<nValueIndex)//情况2：插入到原本Ele值域最后；这种情况下，需分此Ele是否在本次保存文件前被修改过
	{
		BYTE *pNewValue = NULL;
		if (pEle->nVR ==VR_UL || pEle->nVR == VR_FL || pEle->nVR == VR_FD || pEle->nVR == VR_SS || pEle->nVR == VR_SL || pEle->nVR == VR_US || pEle->nVR == VR_AT)
			pNewValue = new BYTE[pEle->nLen + nNewValueLen];
		else
			pNewValue = new BYTE[pEle->nLen + nNewValueLen + 1];

		memcpy(pNewValue,pEle->pValue,pEle->nLen);
		delete pEle->pValue;
		pEle->pValue = NULL;

		pNewValue += pEle->nLen;

		if (pEle->nVR!=VR_UL && pEle->nVR != VR_FL && pEle->nVR != VR_FD && pEle->nVR != VR_SS && pEle->nVR != VR_SL && pEle->nVR != VR_US && pEle->nVR != VR_AT && pEle->nVR != VR_OB)
		{
			pNewValue[0] = 92;
			pNewValue += 1;
			memcpy(pNewValue,pTempValue,nNewValueLen);
			pNewValue =pNewValue - pEle->nLen - 1;
			pEle->nLen = pEle->nLen + nNewValueLen + 1;
		}
		else
		{		
			memcpy(pNewValue,pTempValue,nNewValueLen);
			pNewValue -= pEle->nLen;
			pEle->nLen = pEle->nLen + nNewValueLen;
		}

		int nOldLen = pEle->nLen;
		if (pEle->nLen%2 != 0)
			pEle->nLen = pEle->nLen + 1;

		pEle->pValue = new BYTE[pEle->nLen];
		memset(pEle->pValue,0,pEle->nLen);
		memcpy(pEle->pValue,pNewValue,nOldLen);
		delete []pNewValue;
	}
	else if (nValueCount-1>=nValueIndex)//情况3：插入原来值域中，并且分为是否对应位置原先值
	{
		BYTE *pNewValue = NULL;
		if (pEle->nVR ==VR_UL || pEle->nVR == VR_FL || pEle->nVR == VR_FD || pEle->nVR == VR_SS || pEle->nVR == VR_SL || pEle->nVR == VR_US || pEle->nVR == VR_AT || pEle->nVR == VR_OB)
		{
			if (bCover == false)
				pNewValue = new BYTE[pEle->nLen+nNewValueLen];
			else
				pNewValue = new BYTE[pEle->nLen];
			BYTE *pOldPos = pNewValue;
			memcpy(pNewValue,pEle->pValue,nValueIndex*nNewValueLen);
			pNewValue += nValueIndex*nNewValueLen;
			memcpy(pNewValue,pTempValue,nNewValueLen);
			pNewValue += nNewValueLen;
			if (bCover == false)
			{
				memcpy(pNewValue,pEle->pValue+nValueIndex*nNewValueLen,pEle->nLen - nValueIndex*nNewValueLen);
				pEle->nLen += nNewValueLen;
			}
			else
			{
				memcpy(pNewValue,pEle->pValue+nValueIndex*nNewValueLen,pEle->nLen - nValueIndex*nNewValueLen);
			}
			pNewValue = pNewValue - nValueIndex*nNewValueLen - nNewValueLen; 
			if (pEle->pValue!=NULL)
			{
				delete []pEle->pValue;
				pEle->pValue = NULL;
			}
			pEle->pValue = new BYTE[pEle->nLen];
			memcpy(pEle->pValue,pNewValue,pEle->nLen);
			delete []pOldPos;
		}
		else
		{
			QVector<QString> OldValues;
			QVector<int> ValueLen;			
			for (int i = 0; i<nValueCount; i++)
			{
				QString Value;
				Hs_GetStringValue(pEle,Value,i);
				OldValues.push_back(Value);
				int Len = Value.length();
				ValueLen.push_back(Len);
			}
			if (bCover == false)
				pNewValue = new BYTE[pEle->nLen+nNewValueLen+1];
			else
				pNewValue = new BYTE[pEle->nLen - ValueLen[nValueIndex] + nNewValueLen];

			int iPreLen = 0;
			for (int i = 0; i<nValueIndex;i++)
			{
				iPreLen = iPreLen + ValueLen[i] + 1;
			}

			memcpy(pNewValue,pEle->pValue,iPreLen);			

			pNewValue += iPreLen;
			memcpy(pNewValue,pTempValue,nNewValueLen);
			pNewValue += nNewValueLen;
			if (bCover == false)
			{
				pNewValue[0] = 92;//'\';
				pNewValue += 1;
				memcpy(pNewValue,pEle->pValue+iPreLen,pEle->nLen - iPreLen);
				pEle->nLen = pEle->nLen + nNewValueLen + 1;
				pNewValue = pNewValue - iPreLen - nNewValueLen -1;
			}
			else
			{
				memcpy(pNewValue,pEle->pValue+iPreLen+ValueLen[nValueIndex],pEle->nLen - iPreLen - ValueLen[nValueIndex]);
				pEle->nLen = pEle->nLen - ValueLen[nValueIndex] + nNewValueLen;
				pNewValue = pNewValue - iPreLen - nNewValueLen;
			}
			if (pEle->pValue!=NULL)
			{
				delete pEle->pValue;
				pEle->pValue = NULL;
			}

			int nOldLen = pEle->nLen;
			if (pEle->nLen%2 != 0)
				pEle->nLen = pEle->nLen + 1;
			pEle->pValue = new BYTE[pEle->nLen];
			memset(pEle->pValue,0,pEle->nLen);
			memcpy(pEle->pValue,pNewValue,nOldLen);
			delete []pNewValue;
		}		
	}
	delete []pDataByte;
	delete []pTempValue;
	//TRACE("\r\n %d %d %c %d",pEle->pValue[6],pEle->pValue[7],pEle->pValue[8],pEle->pValue[9]);
	return Ret_Success;
}

int CHsBaseFile::Hs_SaveFile(const char *cFileName,int nTsType)
{
	bool bBigEndia = false;
	bool bConvert = false;

	CalcInformationLength(nTsType);

	if (nTsType == TS_EXPLICIT_VR_BIG_ENDIAN)
		bBigEndia = true; 

    QString str = QString(QLatin1String(cFileName));
    HANDLE hf = ::CreateFile(str.toStdWString().data(),GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hf==INVALID_HANDLE_VALUE)
		return Ret_CreateFileFail;

	//清除以前内容
	::SetFilePointer(hf,0,NULL,FILE_BEGIN);
	::SetEndOfFile(hf);

	DWORD nBt=0;

	char ucFirstTag[132];
	memset(ucFirstTag,0,132);
	ucFirstTag[128] = 'D'; ucFirstTag[129] = 'I';ucFirstTag[130] = 'C';ucFirstTag[131] = 'M';
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,ucFirstTag,132,&nBt,0);

	/*int nRet = 0;
	pHsElement pTransEle = Hs_FindFirstEle(NULL,TAG_TRANSFER_SYNTAX_UID,true);
	if(pTransEle == NULL)
		Hs_InsertElement(NULL,true,TAG_TRANSFER_SYNTAX_UID,CDcmTag::Find(TAG_TRANSFER_SYNTAX_UID)->nVR,false,0,nRet);
	*/

	int n = m_pMainEle->pChildEleV.size();

	for (int i = 0; i<n; i++)
	{
		pHsElement pTempEle = m_pMainEle->pChildEleV[i];

		unsigned long nTag = pTempEle->nTag;

		if ((nTag>>16)%2!=0)
			continue;

		if (pTempEle->nTag == TAG_TRANSFER_SYNTAX_UID)
		{
			if (m_BaseFileInfo.nTsType != nTsType)
			{
				SaveTsType(pTempEle,nTsType,hf);
			}
			else
			{
				this->SaveNormalEle(pTempEle,hf,bBigEndia,bConvert);
			}
			if ((m_BaseFileInfo.nTsType == TS_EXPLICIT_VR_BIG_ENDIAN && nTsType != TS_EXPLICIT_VR_BIG_ENDIAN) ||
				(m_BaseFileInfo.nTsType != TS_EXPLICIT_VR_BIG_ENDIAN && nTsType == TS_EXPLICIT_VR_BIG_ENDIAN))
				bConvert = TRUE;
		}
		else if (pTempEle->nVR == VR_SQ)
		{
			this->SaveSquenceEle(pTempEle,hf,bBigEndia,bConvert);
		}
		else if (pTempEle->nTag == TAG_ITEM)
		{
			this->SaveItemEle(pTempEle,hf,bBigEndia,bConvert);
		}	
		else if (pTempEle->nTag == TAG_PIXEL_DATA)
		{
			this->SavePixelDataEle(pTempEle,hf,bBigEndia,bConvert);
		}
		else
		{
			this->SaveNormalEle(pTempEle,hf,bBigEndia,bConvert);
		}		

	}

	CloseHandle(hf);
	return Ret_Success;
}

int CHsBaseFile::SaveTsType(pHsElement pEle,int nTsType,HANDLE &hf)
{
	DWORD nBt=0;

	//1、写TagNumber
	BYTE *TempTagNum = (BYTE*) &pEle->nTag;
	BYTE *TagNum = WritePara(pEle,TempTagNum,4,false,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagNum,4,&nBt,0);
	delete []TagNum;

	//2、VR
	pDcmVR pVR = CDcmVR::Find(pEle->nVR);
	BYTE *TempTagVR = (BYTE*)&pEle->nVR;
	BYTE *TagVR = WritePara(pEle,TempTagVR,pVR->nLenOfVrDes,false,Para_VR);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagVR,pVR->nLenOfVrDes,&nBt,0);
	delete []TagVR;


	char *cTsTypeName = new char[100];
	memset(cTsTypeName,0,100);
	unsigned long iValueLen;

	if (nTsType == TS_IMPLICIT_VR_LITTLE_ENDIAN)
	{	
		strcpy(	cTsTypeName, "1.2.840.10008.1.2");
	}
	else if (nTsType == TS_EXPLICIT_VR_LITTLE_ENDIAN)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.1");
	}
	else if (nTsType == TS_EXPLICIT_VR_BIG_ENDIAN)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.2");
	}
	else if (nTsType == TS_RLE_LOSSLESS)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.5");
	}
	else if (nTsType == TS_JPEG_BASELINE_1)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.4.50");
	}
	else if (nTsType == TS_JPEG_EXTENDED_2_4)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.4.51");
	}
	else if (nTsType == TS_JPEG_LOSSLESS_NONHIER_14)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.4.57");
	}
	else if (nTsType == TS_JPEG_LOSSLESS_NONHIER_14B)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.4.70");
	}
	else if (nTsType == TS_JPEG2000_LOSSLESS_ONLY)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.4.90");
	}
	else if (nTsType == TS_JPEG2000)
	{
		strcpy(	cTsTypeName,"1.2.840.10008.1.2.4.91");
	}
	else
	{
		strcpy(	cTsTypeName, "1.2.840.10008.1.2");
	}

	iValueLen = strlen(cTsTypeName);
	if (iValueLen%2 != 0)
	{
		iValueLen += 1;
	}

	//3、LEN
	BYTE *TempTagLen = (BYTE *) &iValueLen;
	BYTE *TagLen = WritePara(pEle,TempTagLen,pVR->nLenOfLenDes,false,Para_Len);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagLen,pVR->nLenOfLenDes,&nBt,0);
	delete []TagLen;

	//值域
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,cTsTypeName,iValueLen,&nBt,0);
	delete []cTsTypeName;

	return Ret_Success;
}

int CHsBaseFile::SaveSquenceEle(pHsElement pEle,HANDLE &hf,bool bBigEndia,bool bConvert)
{
	if (pEle == NULL)
	{
		return 0;
	}

	DWORD nBt=0;

	//1、写TagNumber
	BYTE *TempTagNum = (BYTE*) &pEle->nTag;
	BYTE *TagNum = WritePara(pEle,TempTagNum,4,bBigEndia,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagNum,4,&nBt,0);
	delete []TagNum;

	//2、VR
	pDcmVR pVR = CDcmVR::Find(pEle->nVR);
	if (m_BaseFileInfo.nTsType != TS_IMPLICIT_VR_LITTLE_ENDIAN)
	{
		BYTE *TempTagVR = (BYTE*)&pEle->nVR;
		BYTE *TagVR = WritePara(pEle,TempTagVR,pVR->nLenOfVrDes,bBigEndia,Para_VR);
		::SetFilePointer(hf,0,NULL,FILE_END);
		::WriteFile(hf,TagVR,pVR->nLenOfVrDes,&nBt,0);
		delete []TagVR;
	}

	//3、LEN = 0xFFFFFFFF
	pEle->nLen = 0xFFFFFFFF;
	BYTE *TempTagLen = (BYTE *) &pEle->nLen;
	BYTE *TagLen = WritePara(pEle,TempTagLen,pVR->nLenOfLenDes,bBigEndia,Para_Len);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagLen,pVR->nLenOfLenDes,&nBt,0);
	delete []TagLen;

	//4、值域:
	int n = pEle->pChildEleV.size();

	for (int i=0; i<n; i++)
	{
		pHsElement pTempEle = pEle->pChildEleV[i];
		if (pTempEle->nVR == VR_SQ)
		{
			this->SaveSquenceEle(pTempEle,hf,bBigEndia,bConvert);
		}
		else if (pTempEle->nTag == TAG_ITEM)
		{
			this->SaveItemEle(pTempEle,hf,bBigEndia,bConvert);
		}
		else
		{
			this->SaveNormalEle(pTempEle,hf,bBigEndia,bConvert);
		}	
	}	

	//5、SQ结束符
	unsigned long nEndTagNum= 0xFFFEE0DD;
	BYTE *TempSQEndTag = (BYTE*) &nEndTagNum;
	BYTE *SQEndTagNum = WritePara(pEle,TempSQEndTag,4,bBigEndia,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,SQEndTagNum,4,&nBt,0);
	delete []SQEndTagNum;

	unsigned long nSqEnd = 0;
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,&nSqEnd,4,&nBt,0);	

	return Ret_Success;
}

int CHsBaseFile::SaveItemEle(pHsElement pEle,HANDLE &hf,bool bBigEndia,bool bConvert)
{	
	if (pEle == NULL)
	{
		return 0;
	}

	DWORD nBt=0;

	//1、写TagNumber
	BYTE *TempTagNum = (BYTE*) &pEle->nTag;
	BYTE *TagNum = WritePara(pEle,TempTagNum,4,bBigEndia,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagNum,4,&nBt,0);
	delete []TagNum;

	//2、LEN = 0xFFFFFFFF
	pEle->nLen = 0xFFFFFFFF;
	BYTE *TempTagLen = (BYTE *) &pEle->nLen;
	BYTE *TagLen = WritePara(pEle,TempTagLen,4,bBigEndia,Para_Len);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagLen,4,&nBt,0);
	delete []TagLen;

	//3、值域:
	int n = pEle->pChildEleV.size();

	for (int i=0; i<n; i++)
	{
		pHsElement pTempEle = pEle->pChildEleV[i];
		if (pTempEle->nVR == VR_SQ)
		{
			this->SaveSquenceEle(pTempEle,hf,bBigEndia,bConvert);
		}
		else if (pTempEle->nTag == TAG_ITEM)
		{
			this->SaveItemEle(pTempEle,hf,bBigEndia,bConvert);
		}
		else
		{
			this->SaveNormalEle(pTempEle,hf,bBigEndia,bConvert);
		}	
	}	

	//4、ITEM结束符
	unsigned long nEndTagNum= 0xFFFEE00D;
	BYTE *TempItemEndTag = (BYTE*) &nEndTagNum;
	BYTE *ItemEndTagNum = WritePara(pEle,TempItemEndTag,4,bBigEndia,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,ItemEndTagNum,4,&nBt,0);
	delete []ItemEndTagNum;

	unsigned long nItemEnd = 0;
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,&nItemEnd,4,&nBt,0);

	return Ret_Success;
}

int CHsBaseFile::SaveNormalEle(pHsElement pEle,HANDLE &hf,bool bBigEndia,bool bConvert)
{
	if (pEle == NULL)
	{
		return 0;
	}

	DWORD nBt=0;

	//1、写TagNumber
	BYTE *TempTagNum = (BYTE*) &pEle->nTag;
	BYTE *TagNum = WritePara(pEle,TempTagNum,4,bBigEndia,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagNum,4,&nBt,0);
	delete []TagNum;

	//2、VR
	pDcmVR pVR = CDcmVR::Find(pEle->nVR);
	if (m_BaseFileInfo.nTsType != TS_IMPLICIT_VR_LITTLE_ENDIAN)
	{
		if (pEle->nVR == VR_00)
			pEle->nVR = VR_UN;
		BYTE *TempTagVR = (BYTE*)&pEle->nVR;
		BYTE *TagVR = WritePara(pEle,TempTagVR,pVR->nLenOfVrDes,false,Para_VR);
		::SetFilePointer(hf,0,NULL,FILE_END);
		::WriteFile(hf,TagVR,pVR->nLenOfVrDes,&nBt,0);
		delete []TagVR;
	}

	//3、LEN
	BYTE *TempTagLen = (BYTE *) &pEle->nLen;
	int nLenOfLenDes;
	if (m_BaseFileInfo.nTsType == TS_IMPLICIT_VR_LITTLE_ENDIAN)
		nLenOfLenDes = 4;
	else
		nLenOfLenDes = pVR->nLenOfLenDes;	  
	BYTE *TagLen = WritePara(pEle,TempTagLen,nLenOfLenDes,bBigEndia,Para_Len);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagLen,nLenOfLenDes,&nBt,0);
	delete []TagLen;

	//4、值域:
	//if (pEle->bNewTag == TRUE)
	//{
		if (pEle->pValue != NULL)
		{
			BYTE *TempTagValue = pEle->pValue;
			BYTE *TagValue = WriteValue(pEle,TempTagValue,pEle->nLen,false);
			::SetFilePointer(hf,0,NULL,FILE_END);
			::WriteFile(hf,TagValue,pEle->nLen,&nBt,0);
			delete []TagValue;
		}
		else
		{
			return Ret_Success;
		}
	/*}
	else
	{
		BYTE *TempTagValue = new BYTE[pEle->nLen];
		memcpy(TempTagValue,&(m_buf[pEle->nTagPos+pEle->nOffset]),pEle->nLen);		
		BYTE *TagValue = WriteValue(pEle,TempTagValue,pEle->nLen,bConvert);
		::SetFilePointer(hf,0,NULL,FILE_END);
		::WriteFile(hf,TagValue,pEle->nLen,&nBt,0);
		delete []TempTagValue;
		delete []TagValue;
	}*/

	return Ret_Success;
}

int CHsBaseFile::SavePixelDataEle(pHsElement pPixelEle,HANDLE &hf,bool bBigEndia,bool bConvert)
{
	if (pPixelEle == NULL)
		return Ret_InvalidElement;
	int nRet;

	DWORD nBt =0;

	//1、写TagNumber
	BYTE *TempTagNum = (BYTE*) &pPixelEle->nTag;
	BYTE *TagNum = WritePara(pPixelEle,TempTagNum,4,bBigEndia,Para_TagNum);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagNum,4,&nBt,0);
	delete []TagNum;

	//2、VR
	pDcmVR pVR = CDcmVR::Find(pPixelEle->nVR);
	if (m_BaseFileInfo.nTsType != TS_IMPLICIT_VR_LITTLE_ENDIAN)
	{
		if (pPixelEle->nVR == VR_00)
			pPixelEle->nVR = VR_OW;
		BYTE *TempTagVR = (BYTE*)&pPixelEle->nVR;
		BYTE *TagVR = WritePara(pPixelEle,TempTagVR,pVR->nLenOfVrDes,false,Para_VR);
		::SetFilePointer(hf,0,NULL,FILE_END);
		::WriteFile(hf,TagVR,pVR->nLenOfVrDes,&nBt,0);
		delete []TagVR;
	}

	//3、LEN
	BYTE *TempTagLen = (BYTE *) &pPixelEle->nLen;
	BYTE *TagLen = WritePara(pPixelEle,TempTagLen,pVR->nLenOfLenDes,bBigEndia,Para_Len);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,TagLen,pVR->nLenOfLenDes,&nBt,0);
	delete []TagLen;

	// 值域
	pHsElement p;	
	ImageInfo ImgInfo;

	//nSamplePerPixel
	p = Hs_FindFirstEle(pPixelEle,TAG_SAMPLES_PER_PIXEL,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nSamplePerPixel,0);
	if(ImgInfo.nSamplePerPixel==0)
		ImgInfo.nSamplePerPixel = 1;

	//nBitsAllocated
	p = Hs_FindFirstEle(pPixelEle,TAG_BITS_ALLOCATED,true);
	nRet = Hs_GetLongValue(p,ImgInfo.nBitsAllocated,0);

	FILE *m_File;
	QByteArray ba = m_BaseFileInfo.sFileName.toLatin1();
	errno_t err = fopen_s(&m_File,ba.data(),"rb");

	fseek(m_File,0,SEEK_END);
	int iFileEndPos = ftell(m_File);
	int iPerByte = iFileEndPos - pPixelEle->nTagPos-pPixelEle->nOffset;
	BYTE *pPixel = new BYTE[iPerByte];


	fseek(m_File,pPixelEle->nTagPos+pPixelEle->nOffset,SEEK_SET); 
	size_t nReadCount = fread(pPixel, iPerByte, 1,m_File);
	BYTE *pFinalPixel = WriteImage(ImgInfo,pPixel,iPerByte,bConvert);
	::SetFilePointer(hf,0,NULL,FILE_END);
	::WriteFile(hf,pFinalPixel,iPerByte,&nBt,0);
	delete []pFinalPixel;

	fclose(m_File);
	delete []pPixel;

	return Ret_Success;
}
/*
关于TagNum大小端存储方式：
1.本机CPU为小端，保存DICOM文件为小端，先将4个字节全部颠倒，再分别将前两个字节，后两个字节颠倒;
2.本机CPU为小端，保存DICOM文件为大端，将4个字节全部颠倒;
3.本机CPU为大端，保存DICOM文件为小端，分别将前两个字节，后两个字节颠倒;
4.本机CPU为大端，保存DICOM文件为大端，不做任何调换;

关于TagVR保存特点：
当长度为四个字节时，不分大小端，全为"UL0000"排列;
*/

BYTE *CHsBaseFile::WritePara(pHsElement pEle,BYTE *pData,int iDataSize,bool bBigEndia,int iPareType)
{
	BYTE *pResult = new BYTE[iDataSize];

	if (iPareType == Para_TagNum)
	{
		if (m_BaseFileInfo.bCpuBigEndia == false)
		{
			if (bBigEndia == false)
			{
				BYTE temp[4];
				temp[0] = pData[3]; temp[1] = pData[2]; temp[2] = pData[1]; temp[3] = pData[0];
				pResult[0] = temp[1]; pResult[1] = temp[0]; pResult[2] = temp[3]; pResult[3] = temp[2];
			}
			else
			{
				if (pEle->nTag!=0x00020000UL && pEle->nTag!=0x00020102UL && pEle->nTag!=0x00020001UL && pEle->nTag!=0x00020002UL && pEle->nTag!=0x00020003UL && 
					pEle->nTag!=0x00020010UL && pEle->nTag!=0x00020012UL && pEle->nTag!=0x00020013UL && pEle->nTag!=0x00020016UL && pEle->nTag!=0x00020100UL )
				{
					pResult[0] = pData[3]; pResult[1] = pData[2]; pResult[2] = pData[1]; pResult[3] = pData[0];
				}
				else
				{
					BYTE temp[4];
					temp[0] = pData[3]; temp[1] = pData[2]; temp[2] = pData[1]; temp[3] = pData[0];
					pResult[0] = temp[1]; pResult[1] = temp[0]; pResult[2] = temp[3]; pResult[3] = temp[2];
				}
			}
		}
		else
		{
			if (bBigEndia == false)
			{
				pResult[0] = pData[3]; pResult[1] = pData[2]; pResult[2] = pData[1]; pResult[3] = pData[0];
			}
			else
			{
				if (pEle->nTag!=0x00020000UL && pEle->nTag!=0x00020102UL && pEle->nTag!=0x00020001UL && pEle->nTag!=0x00020002UL && pEle->nTag!=0x00020003UL && 
					pEle->nTag!=0x00020010UL && pEle->nTag!=0x00020012UL && pEle->nTag!=0x00020013UL && pEle->nTag!=0x00020016UL && pEle->nTag!=0x00020100UL )
				{
					pResult[0] = pData[3]; pResult[1] = pData[2]; pResult[2] = pData[1]; pResult[3] = pData[0];
				}
				else
				{
					pResult = pData;
				}
			}
		}
	}
	else if (iPareType == Para_VR)
	{
		BYTE temp[4];
		memset(temp,0,4);
		if (m_BaseFileInfo.bCpuBigEndia == false)
		{
			temp[0] = pData[1]; temp[1] = pData[0] ;
		}
		else
		{
			temp[0] = pData[2]; temp[1] = pData[3];
		}
		if (iDataSize == 2)
		{
			pResult[0] = temp[0]; pResult[1] = temp[1];
		}
		else
		{
			pResult[0] = temp[0]; pResult[1] = temp[1];pResult[2] = temp[2]; pResult[3] = temp[3];
		}
	}
	else if (iPareType == Para_Len)
	{
		BYTE temp[4];
		memset(temp,0,4);
		if (bBigEndia == false)
		{
			if (m_BaseFileInfo.bCpuBigEndia == false)
			{
				temp[0] = pData[0]; temp[1] = pData[1]; temp[2] = pData[2]; temp[3] = pData[3];
			}				
			else
			{
				temp[0] = pData[3]; temp[1] = pData[2]; temp[2] = pData[1]; temp[3] = pData[0];			
			}	
			if(iDataSize == 2)
			{
				pResult[0] = temp[0]; pResult[1] = temp[1];
			}
			else
			{
				pResult[0] = temp[0]; pResult[1] = temp[1];pResult[2] = temp[2]; pResult[3] = temp[3];
			}
		}
		else
		{
			if (m_BaseFileInfo.bCpuBigEndia == false)
			{
				temp[0] = pData[3]; temp[1] = pData[2]; temp[2] = pData[1]; temp[3] = pData[0];
			}
			else
			{
				temp[0] = pData[0]; temp[1] = pData[1]; temp[2] = pData[2]; temp[3] = pData[3];
			}
			if (iDataSize == 2)
			{
				if (pEle->nTag!=0x00020000UL && pEle->nTag!=0x00020102UL && pEle->nTag!=0x00020001UL && pEle->nTag!=0x00020002UL && pEle->nTag!=0x00020003UL && 
					pEle->nTag!=0x00020010UL && pEle->nTag!=0x00020012UL && pEle->nTag!=0x00020013UL && pEle->nTag!=0x00020016UL && pEle->nTag!=0x00020100UL )
				{
					pResult[0] = temp[2]; pResult[1] = temp[3];
				}
				else
				{
					pResult[0] = temp[3]; pResult[1] = temp[2];
				}
			}
			else
			{
				if (pEle->nTag!=0x00020000UL && pEle->nTag!=0x00020102UL && pEle->nTag!=0x00020001UL && pEle->nTag!=0x00020002UL && pEle->nTag!=0x00020003UL && 
					pEle->nTag!=0x00020010UL && pEle->nTag!=0x00020012UL && pEle->nTag!=0x00020013UL && pEle->nTag!=0x00020016UL && pEle->nTag!=0x00020100UL )
				{
					pResult[0] = temp[0]; pResult[1] = temp[1];pResult[2] = temp[2]; pResult[3] = temp[3];
				}
				else
				{
					pResult[0] = temp[3]; pResult[1] = temp[2]; pResult[2] = temp[1]; pResult[3] =temp[0]; 
				}

			}		
		}
	}
	return pResult;
}

BYTE *CHsBaseFile::WriteValue(pHsElement pEle,BYTE *pData, int iDataSize,bool bConvert)
{
	BYTE *pResult = new BYTE[iDataSize];	
	if (bConvert != false)
	{
		if (pEle->nTag==0x00020000UL || pEle->nTag==0x00020102UL || pEle->nTag==0x00020001UL || pEle->nTag==0x00020002UL || pEle->nTag==0x00020003UL || 
			pEle->nTag==0x00020010UL || pEle->nTag==0x00020012UL || pEle->nTag==0x00020013UL || pEle->nTag==0x00020016UL || pEle->nTag==0x00020100UL )
		{	
			memcpy(pResult,pData,iDataSize);
		}
		else
		{
			if (pEle->nVR == VR_SL  || pEle->nVR == VR_UL || pEle->nVR == VR_FL || pEle->nVR == VR_OF)
			{
				int iFunState;                            
				unsigned long nValueCount = Hs_GetValueCount(pEle,iFunState);
				for (int i = 0; i<nValueCount;i++)
				{
					for (int j = 0; j<4;j++)
					{
						pResult[i*4 +j] = pData[(i+1)*4-1-j];
					}
				}
			}
			else if(pEle->nVR == VR_SS||pEle->nVR == VR_US||pEle->nVR == VR_OW)
			{
				int iFunState;                            
				unsigned long nValueCount = Hs_GetValueCount(pEle,iFunState);
				for (int i = 0; i<nValueCount;i++)
				{
					for (int j = 0; j<2;j++)
					{
						pResult[i*2 +j] = pData[(i+1)*2-1-j];
					}
				}
			}
			else if (pEle->nVR ==VR_FD)
			{
				int iFunState;                            
				unsigned long nValueCount = Hs_GetValueCount(pEle,iFunState);
				for (int i = 0; i<nValueCount;i++)
				{
					for (int j = 0; j<8;j++)
					{
						pResult[i*8 +j] = pData[(i+1)*8-1-j];
					}
				}
			}
			//else if (pEle->nVR == VR_AT)
			//{
			//	pResult[0] = pData[1]; pResult[1] = pData[0]; pResult[2] = pData[3]; pResult[3] = pData[2]; 
			//}
			else
			{
				memcpy(pResult,pData,iDataSize);
			}
		}
	}		
	else
	{
		memcpy(pResult,pData,iDataSize);
	}
	//TRACE("\r\n %d %d %d %d",pResult[0],pResult[1],pResult[2],pResult[3]);
	return pResult;
}

BYTE *CHsBaseFile::WriteImage(ImageInfo ImgInfo,BYTE *pImageData,int iImageByte,bool bConvert)
{
	BYTE *pResult = new BYTE[iImageByte];

	int	iPerPixelByte = ImgInfo.nBitsAllocated/ImgInfo.nSamplePerPixel/8;


	if (bConvert != false)
	{
		if (iPerPixelByte == 1)
		{
			memcpy(pResult,pImageData,iImageByte);
		}
		else if (iPerPixelByte == 2)
		{
			int iPixelNum = iImageByte/iPerPixelByte;

			for (int i=0; i<iPixelNum; i++)
			{
				pResult[i*iPerPixelByte] = pImageData[i*iPerPixelByte+1];pResult[i*iPerPixelByte+1] = pImageData[i*iPerPixelByte];
			}
		}
		else if (iPerPixelByte == 4)
		{
			int iPixelNum = iImageByte/iPerPixelByte;

			for (int i=0; i<iPixelNum; i++)
			{
				pResult[i*iPerPixelByte] = pImageData[i*iPerPixelByte+3];pResult[i+iPerPixelByte+1] = pImageData[i*iPerPixelByte+2];
				pResult[i*iPerPixelByte+2] = pImageData[i*iPerPixelByte+1];pResult[i+iPerPixelByte+3] = pImageData[i*iPerPixelByte];
			}
		}
	}
	else
	{
		memcpy(pResult,pImageData,iImageByte);
	}

	return pResult;
}


int CHsBaseFile::Hs_DeleteValues(pHsElement pEle,int nIndex)
{
	if (pEle == NULL)
		return Ret_InvalidPara;
	if(nIndex<0)
		return Ret_OutOfValueCount;

	int nRet = 0;
	unsigned long nValueCount = Hs_GetValueCount(pEle,nRet);

	if (nValueCount == 0)
	{
		return Ret_OutOfValueCount;
	}

	//if (nIndex+1-nValueCount>0)
	//	nIndex = nValueCount-1;

	if (nValueCount == 1&& nIndex==0)
	{
		if (pEle->pValue)
		{
			delete pEle->pValue;
			pEle->pValue = NULL;
		}
		pEle->pValue = new BYTE[0];
		pEle->nLen = 0;
	}
	else
	{
		BYTE *pNewValue;
		if (pEle->nVR ==VR_UL || pEle->nVR == VR_FL || pEle->nVR == VR_FD || pEle->nVR == VR_SS || pEle->nVR == VR_SL || pEle->nVR == VR_US || pEle->nVR == VR_AT)
		{
			int iTypeSize = CDcmVR::Find(pEle->nVR)->nValueLeng;
			pNewValue = new BYTE[(nValueCount-1)*iTypeSize];

			//if (pEle->pValue == NULL)//未修改
			//	memcpy(pNewValue,&(m_buf[pEle->nTagPos+pEle->nOffset]),(nIndex)*iTypeSize);
			//else//已修改
			memcpy(pNewValue,pEle->pValue,nIndex*iTypeSize);

			pNewValue += nIndex*iTypeSize;

			//if (pEle->pValue == NULL)//未修改
			//	memcpy(pNewValue,&(m_buf[pEle->nTagPos+pEle->nOffset+(nIndex)*iTypeSize + iTypeSize]),pEle->nLen  - (nIndex+1)*iTypeSize);
			//else//已修改
			memcpy(pNewValue,pEle->pValue+(nIndex)*iTypeSize + iTypeSize,pEle->nLen  - (nIndex+1)*iTypeSize);

			pEle->nLen = pEle->nLen - iTypeSize;
			pNewValue -= nIndex*iTypeSize;

			if (pEle->pValue!=NULL)
			{
				delete pEle->pValue;
				pEle->pValue = NULL;
			}	
			pEle->pValue = new BYTE[pEle->nLen];
			memcpy(pEle->pValue,pNewValue,pEle->nLen);
			delete []pNewValue;
		}
		else
		{	
			QVector<QString> OldValues;
			QVector<int> ValueLen;
			for (int i = 0; i<nValueCount; i++)
			{
				QString Value;
				Hs_GetStringValue(pEle,Value,i);
				OldValues.push_back(Value);
				int Len = Value.length();
				ValueLen.push_back(Len);
			}		

			int iPreLen = 0;

			for (int i = 0; i<nIndex;i++)
			{
				iPreLen = iPreLen + ValueLen[i] + 1;
			}

			pNewValue = new BYTE[pEle->nLen - ValueLen[nIndex]];

			if (nIndex == 0)
			{				
				//if (pEle->pValue == NULL)//未修改
				//	memcpy(pNewValue,&(m_buf[pEle->nTagPos+pEle->nOffset+ValueLen[nIndex]+1]),pEle->nLen-ValueLen[nIndex]-1);
				//else//已修改
				memcpy(pNewValue,pEle->pValue+ValueLen[nIndex]+1,pEle->nLen-ValueLen[nIndex]-1);
			}
			else
			{
				//if (pEle->pValue == NULL)//未修改
				//	memcpy(pNewValue,&(m_buf[pEle->nTagPos+pEle->nOffset]),iPreLen-1);
				//else//已修改
				memcpy(pNewValue,pEle->pValue,iPreLen-1);

				if (nIndex < nValueCount-1)				
				{
					pNewValue += iPreLen-1;

					//if (pEle->pValue == NULL)//未修改
					//	memcpy(pNewValue,&(m_buf[pEle->nTagPos+pEle->nOffset+iPreLen+ValueLen[nIndex]+1]),pEle->nLen - iPreLen - ValueLen[nIndex]-1);
					//else//已修改
					memcpy(pNewValue,pEle->pValue+iPreLen+ValueLen[nIndex]+1,pEle->nLen - iPreLen - ValueLen[nIndex]-1);
				}				
			}
			pEle->nLen = pEle->nLen - ValueLen[nIndex] -1;

			if (pEle->pValue!=NULL)
			{
				delete pEle->pValue;
				pEle->pValue = NULL;
			}

			if (pEle->nLen%2 != 0)
				pEle->nLen = pEle->nLen + 1;

			pEle->pValue = new BYTE[pEle->nLen];
			memset(pEle->pValue,0x00,pEle->nLen);
			memcpy(pEle->pValue,pNewValue,pEle->nLen);
			delete []pNewValue;			
		}				
	}

	pEle->bNewTag = true;

	return 0;
}

unsigned long CHsBaseFile::Hs_GetSequenceItemEle( pHsElement pEle,QVector<pHsElement> &ItemEleV )
{
	if (m_pMainEle == NULL || pEle == NULL)
		return 0;

	if(pEle->nVR != VR_SQ)
		return 0;

	int n = int(pEle->pChildEleV.size());
	for (int i=0; i<n; i++)
	{
		if (pEle->pChildEleV[i]->nTag == TAG_ITEM)
		{
			ItemEleV.push_back(pEle->pChildEleV[i]);
		}
	}

	return unsigned long(ItemEleV.size());
}



bool CHsBaseFile::Hs_GetLutDataItem( pHsElement pLutItemEle,LutData &lut )
{//给我一个包含Lut三大项的ITEM。我把Lut读给lut
	if (m_pMainEle == NULL || pLutItemEle == NULL)
		return false;

	int nRet = 0;
	//Lut Descriptor中的三要员,没谁都不行
	pHsElement pLutDes = Hs_GetChildEle(pLutItemEle,TAG_LUT_DESCRIPTOR);
	nRet = Hs_GetLongValue(pLutDes,lut.nLutLen,0);//Lut多长? 含多少个Data?
	if(nRet)
		return false;

	nRet = Hs_GetLongValue(pLutDes,lut.nMinValue,1);//最小值
	if(nRet)
		return false;

	nRet = Hs_GetLongValue(pLutDes,lut.nBitsPerData,2);//Lut的每个Data数据占几位
	if(nRet)
		return false;

	//这个是Lut的名称
	pHsElement pExplan = Hs_GetChildEle(pLutItemEle,TAG_LUT_EXPLANATION);
	nRet = Hs_GetStringValue(pExplan,lut.sName,0);
	if(lut.sName.isEmpty())
	{
		char cName[200] = {'\0'};
		sprintf(cName,"Lut:%d",lut.iLutID);
		lut.sName = cName;
	}
	else
	{
		char cName[200] = {'\0'};
		QByteArray ba = lut.sName.toLatin1();
		sprintf(cName,"Lut:%s",ba.data());
		lut.sName = cName;
	}

	//Lut数组数据
	lut.nBytePerData = lut.nBitsPerData/8 + (lut.nBitsPerData%8 > 0 ? 1:0);

	unsigned long nSize = lut.nLutLen * lut.nBytePerData ;//预计应有长度
	if(nSize==0)
		return false;

	pHsElement pLutDataEle = Hs_GetChildEle(pLutItemEle,TAG_LUT_DATA);
	unsigned long nSizeGot = 0;
	lut.pLutData = Hs_GetByteValue(pLutDataEle,nSizeGot,nRet);

	if(nRet)
		return false;

	if(nSizeGot!=nSize)//预计长度与实际长度不符合
		return false;

	//如果是大端需要每相邻的两个字节需要互换
	if (pLutDataEle->bBigEndian)
	{
		int nTime = nSizeGot/2;
		for (int e=0;e<nTime;e++)
		{
			std::swap(lut.pLutData[2*e],lut.pLutData[2*e+1]);
		}
	}

	lut.nMaxValue = lut.nMinValue + lut.nLutLen - 1;

	return true;
}


bool CHsBaseFile::Hs_GetWcLutItem( pHsElement pLutItemEle,QVector<LutData*> &LutV )
{//某Tag:pLutItemEle下含有三个子Tag：TAG_WINDOW_CENTER  TAG_WINDOW_WIDTH 0x00281055，这其实形成了一个Wl队列，因为这三个tag下都可以多值

	if (m_pMainEle == NULL)
		return false;

	pHsElement pWinCenter = Hs_GetChildEle(pLutItemEle,TAG_WINDOW_CENTER);
	pHsElement pWinWidth = Hs_GetChildEle(pLutItemEle,TAG_WINDOW_WIDTH);
	pHsElement pWinExplan = Hs_GetChildEle(pLutItemEle,TAG_WINDOW_CENTER_WIDTH_EXPLANATION);

	int nRet = 0;

	if (pWinCenter)
	{
		int i = -1;
		while(1)
		{
			i++;
			LutData *pNewWcLut = new LutData;
			pNewWcLut->bModality = false;
			pNewWcLut->bWc = true;
			pNewWcLut->iLutID = i;
			nRet = Hs_GetDoubleValue(pWinCenter,pNewWcLut->nC,i);
			if(nRet)
			{
				delete pNewWcLut;
				break;
			}

			nRet = Hs_GetDoubleValue(pWinWidth,pNewWcLut->nW,i);
			if(nRet)
			{
				delete pNewWcLut;
				break;
			}

			nRet = Hs_GetStringValue(pWinExplan,pNewWcLut->sName,i);
			if(pNewWcLut->sName.isEmpty())
			{
				char cName[100];
				sprintf(cName,"WL:%d",i+1);
				pNewWcLut->sName = cName;
			}
			else
			{
				char cName[200] = {'\0'};
				QByteArray ba = pNewWcLut->sName.toLatin1();
				sprintf(cName,"WL:%s",ba.data());
				pNewWcLut->sName = QString(QLatin1String(cName));
			}

			LutV.push_back(pNewWcLut);
		}
	}
//	此函数如果无误，则应用到CHsFile::GetLuts;
	return true;
}

int CHsBaseFile::Hs_GetFrameCount(pHsElement pPixEle)
{
	long nFrame = 0;
	pHsElement p = Hs_FindFirstEle(pPixEle,TAG_NUMBER_OF_FRAMES,true);
	if(p)//如果是多帧，必须有这个Tag,单帧的可以没有
	{
		Hs_GetLongValue(p,nFrame,0);
	}
	else
	{
		nFrame = 1;
	}

	m_BaseFileInfo.nFrame = nFrame;

	return nFrame;
}

pHsElement CHsBaseFile::GetItemFromPerFramFunGpSeq( pHsElement pFrmFunGpSeqEle,int iFrame )
{
	if(pFrmFunGpSeqEle == NULL || m_pMainEle == NULL)
		return NULL;

	QVector<pHsElement> ItemEleV;
	int nItem = Hs_GetSequenceItemEle(pFrmFunGpSeqEle,ItemEleV);
	if(nItem==0)
		return NULL;

	if(iFrame>=0 && iFrame<nItem)
		return ItemEleV[iFrame];

	return NULL;
	//for (int i=0;i<nItem;i++)
	//{
	//	pHsElement pItemEle = ItemEleV[i];
	//	pHsElement pFrmConSeqEle = Hs_GetChildEle(pItemEle,0x00209111,-1);
	//	if(pFrmConSeqEle==NULL)
	//		return NULL;

	//	pHsElement tItemEle = Hs_GetChildEle(pFrmConSeqEle,TAG_ITEM,-1);
	//	if(tItemEle==NULL)
	//		return NULL;

	//	pHsElement pInStackFrameNumEle = Hs_GetChildEle(tItemEle,0x00209057,-1);
	//	if(pInStackFrameNumEle==NULL)
	//		return NULL;

	//	long tFrame = -1;
	//	if(Hs_GetLongValue(pInStackFrameNumEle,tFrame,0) != Ret_Success)
	//		return NULL;

	//	if(tFrame-1 == iFrame)
	//	{
	//		return pItemEle;
	//	}
	//}

	return NULL;
}

bool CHsBaseFile::GetImageLocParaMulityFrame( int iFrame,ImageInfo &info )
{
	pHsElement pFunGpSeqEle = Hs_FindFirstEle(NULL,0x52009230,true);
	if(pFunGpSeqEle==NULL)
		return false;

	pHsElement pItemEle = GetItemFromPerFramFunGpSeq(pFunGpSeqEle,iFrame);

	//Pixel Spacing 
	pHsElement pPixMeasureSeqEle = Hs_GetChildEle(pItemEle,0x00289110,-1);
	if(pPixMeasureSeqEle==NULL)
		return false;

	pHsElement tItemEle0 = Hs_GetChildEle(pPixMeasureSeqEle,TAG_ITEM,-1);
	if(tItemEle0 == NULL)
		return false;

	pHsElement pPixSpacingEle = Hs_GetChildEle(tItemEle0,TAG_PIXEL_SPACING,true);

	if(Hs_GetDoubleValue(pPixSpacingEle,info.ImgLocPara.fPixSpacingY,0)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pPixSpacingEle,info.ImgLocPara.fPixSpacingX,1)!=Ret_Success) 
		return false;

	// TAG_IMAGE_POSITION_PATIENT 0x00200032
	pHsElement pPlanePosSeqEle = Hs_GetChildEle(pItemEle,0x00209113,-1);
	if(pPlanePosSeqEle==NULL)
		return false;

	pHsElement tItemEle1 = Hs_GetChildEle(pPlanePosSeqEle,TAG_ITEM,-1);
	if(tItemEle1 == NULL)
		return false;

	pHsElement pPosEle = Hs_GetChildEle(tItemEle1,TAG_IMAGE_POSITION_PATIENT,-1);

	if(Hs_GetDoubleValue(pPosEle,info.ImgLocPara.fOriLeftTopPixX,0)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pPosEle,info.ImgLocPara.fOriLeftTopPixY,1)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pPosEle,info.ImgLocPara.fOriLeftTopPixZ,2)!=Ret_Success) 
		return false;

	info.ImgLocPara.fLeftTopPixX = info.ImgLocPara.fOriLeftTopPixX;
	info.ImgLocPara.fLeftTopPixY = info.ImgLocPara.fOriLeftTopPixY;
	info.ImgLocPara.fLeftTopPixZ = info.ImgLocPara.fOriLeftTopPixZ;

	// TAG_IMAGE_ORIENTATION_PATIENT 0x00200037UL
	pHsElement pPlaneOriSeqEle = Hs_GetChildEle(pItemEle,0x00209116,-1);
	if(pPlaneOriSeqEle==NULL)
		return false;

	pHsElement tItemEle2 = Hs_GetChildEle(pPlaneOriSeqEle,TAG_ITEM,-1);
	if(tItemEle2==NULL)
		return false;
	pHsElement pOriEle = Hs_GetChildEle(tItemEle2,TAG_IMAGE_ORIENTATION_PATIENT,-1);

	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstRowCosX,0)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstRowCosY,1)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstRowCosZ,2)!=Ret_Success) 
		return false;

	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstColCosX,3)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstColCosY,4)!=Ret_Success) 
		return false;
	if(Hs_GetDoubleValue(pOriEle,info.ImgLocPara.fOriFirstColCosZ,5)!=Ret_Success) 
		return false;

	info.ImgLocPara.fFirstRowCosX = info.ImgLocPara.fOriFirstRowCosX;
	info.ImgLocPara.fFirstRowCosY = info.ImgLocPara.fOriFirstRowCosY;
	info.ImgLocPara.fFirstRowCosZ = info.ImgLocPara.fOriFirstRowCosZ;

	info.ImgLocPara.fFirstColCosX = info.ImgLocPara.fOriFirstColCosX;
	info.ImgLocPara.fFirstColCosY = info.ImgLocPara.fOriFirstColCosY;
	info.ImgLocPara.fFirstColCosZ = info.ImgLocPara.fOriFirstColCosZ;

	info.ImgLocPara.bValide = true;

	info.ImgLocPara.fRowmm = info.ImgLocPara.nRow * info.ImgLocPara.fPixSpacingY;
	info.ImgLocPara.fColmm = info.ImgLocPara.nCol * info.ImgLocPara.fPixSpacingX;

	if (info.fPixelSpaceX < 0.000001)
		info.fPixelSpaceX = info.ImgLocPara.fPixSpacingX;
	if (info.fPixelSpaceY < 0.000001)
		info.fPixelSpaceY = info.ImgLocPara.fPixSpacingY;

	/// Image SliceThick
	pHsElement pSliceThickEle = Hs_GetChildEle(tItemEle0,TAG_SLICE_THICKNESS,true);

	if (Hs_GetDoubleValue(pSliceThickEle,info.ImgLocPara.fSliceThickness,0) != Ret_Success)
	{
		pHsElement pPhilpPriEle = Hs_GetChildEle(pItemEle,0x2005140f,-1);//飞利浦多帧私有tag
		if(pPhilpPriEle==NULL)
			return false;

		pHsElement tPriItemEle = Hs_GetChildEle(pPhilpPriEle,TAG_ITEM,-1);
		if (tPriItemEle == NULL)
			return false;

		pSliceThickEle = Hs_GetChildEle(tPriItemEle,TAG_SLICE_THICKNESS,true);

		if (Hs_GetDoubleValue(pSliceThickEle,info.ImgLocPara.fSliceThickness,0) != Ret_Success)
			return false;
	}
	//层位置也搞
	//Hs_GetDoubleValueA(TAG_SLICE_LOCATION,info.ImgLocPara.fSliceLoction);

	return true;
}

int CHsBaseFile::Hs_CopyRootTagTo(CHsBaseFile* pDstDs, unsigned long nTag)// ,pHsElement* pDstParentEle)
{
	if(nTag == 0 || pDstDs == NULL)
		return Ret_InvalidPara;

	if(nTag == TAG_ITEM)
		return Ret_UnSupportPara;

	pHsElement pSrcEle = Hs_FindFirstEle(NULL,nTag,true);
	if(pSrcEle==NULL)
		return Ret_Success;

	if(pSrcEle->nVR == VR_SQ || pSrcEle->pChildEleV.empty()==false)
		return Ret_UnSupportPara;

	int nRet = 0;
	//看看目的地是否已存在这个Tag---仅限根目录查找
	pHsElement pDstEle = NULL;

	pDstEle = pDstDs->Hs_FindFirstEle(NULL,nTag,true);
	if(pDstEle == NULL)
		pDstEle = pDstDs->Hs_InsertElement(NULL,true,pSrcEle->nTag,CDcmTag::Find(pSrcEle->nTag)->nVR,pSrcEle->bSquence,0,nRet);

	if(pDstEle == NULL)
		return nRet;

	unsigned long  nByte = 0;
	BYTE* pByte = Hs_GetByteValue(pSrcEle,nByte,nRet);
	if(pByte)
	{
		pDstDs->Hs_SetByteValue(pDstEle,pByte,nByte);
		delete []pByte;
	}

	return Ret_Success;

}
int CHsBaseFile::Hs_SetByteValue( pHsElement pEle,BYTE *pValue,int nCount)
{
	if (pEle == NULL )
		return Ret_InvalidPara;

	if(pEle->pValue)
	{
		delete [](pEle->pValue);
		pEle->pValue = NULL;
		pEle->nLen = 0;
	}

	if (pValue && nCount > 0)
	{
		pEle->pValue = new BYTE[nCount];
		memcpy(pEle->pValue,pValue,nCount);
		pEle->nLen = nCount;
	}

	pEle->bNewTag = TRUE;
	return Ret_Success;
}

int CHsBaseFile::Hs_InitFile(QString StorageClassUID, QString StorageInstancUID, int nTsType)
{
	if (m_pMainEle == NULL)
	{
		m_pMainEle = new HsElement;
	}

	int nRet;

	//
	pHsElement pInsertEle = Hs_InsertElement(NULL,false,TAG_FILE_META_INFORMATION_VERSION,VR_OB,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;
	Hs_SetShortValue(pInsertEle,01,true,0);
	Hs_SetShortValue(pInsertEle,00,true,1);

	//
	pInsertEle = Hs_InsertElement(NULL,false,TAG_MEDIA_STORAGE_SOP_CLASS_UID,CDcmTag::Find(TAG_MEDIA_STORAGE_SOP_CLASS_UID)->nVR,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;
	Hs_SetStringValue(pInsertEle,StorageClassUID,false,0);

	//
	pInsertEle = Hs_InsertElement(NULL,false,TAG_MEDIA_STORAGE_SOP_INSTANCE_UID,CDcmTag::Find(TAG_MEDIA_STORAGE_SOP_INSTANCE_UID)->nVR,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;
	Hs_SetStringValue(pInsertEle,StorageInstancUID,false,0);

	//
	pInsertEle = Hs_InsertElement(NULL,false,TAG_TRANSFER_SYNTAX_UID,CDcmTag::Find(TAG_TRANSFER_SYNTAX_UID)->nVR,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;

	QString sTsTypeName;
	if (nTsType == TS_IMPLICIT_VR_LITTLE_ENDIAN)
	{	
		sTsTypeName= "1.2.840.10008.1.2";
	}
	else if (nTsType == TS_EXPLICIT_VR_LITTLE_ENDIAN)
	{
		sTsTypeName= "1.2.840.10008.1.2.1";
	}
	else if (nTsType == TS_EXPLICIT_VR_BIG_ENDIAN)
	{
		sTsTypeName= "1.2.840.10008.1.2.2";
	}
	else if (nTsType == TS_RLE_LOSSLESS)
	{
		sTsTypeName= "1.2.840.10008.1.2.5";
	}
	else if (nTsType == TS_JPEG_BASELINE_1)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.50";
	}
	else if (nTsType == TS_JPEG_EXTENDED_2_4)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.51";
	}
	else if (nTsType == TS_JPEG_LOSSLESS_NONHIER_14)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.57";
	}
	else if (nTsType == TS_JPEG_LOSSLESS_NONHIER_14B)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.70";
	}
	else if (nTsType == TS_JPEG2000_LOSSLESS_ONLY)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.90";
	}
	else if (nTsType == TS_JPEG2000)
	{
		sTsTypeName="1.2.840.10008.1.2.4.91";
	}
	else
	{
		sTsTypeName="1.2.840.10008.1.2";
	}
	Hs_SetStringValue(pInsertEle,sTsTypeName,false,0);


	//
	pInsertEle = Hs_InsertElement(NULL,false,TAG_IMPLEMENTATION_CLASS_UID,VR_UI,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;
	QString sImplementationUID;
	sImplementationUID = "1.2.840.49110116101.99104";
	Hs_SetStringValue(pInsertEle,sImplementationUID,false,0);

	//
	pInsertEle = Hs_InsertElement(NULL,false,TAG_IMPLEMENTATION_VERSION_NAME,VR_SH,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;
	QString sImplementationVersion;
	sImplementationVersion = "1";
	Hs_SetStringValue(pInsertEle,sImplementationVersion,false,0);

	return 0;
}

int CHsBaseFile::CalcInformationLength( int nTsType )
{
	int nRet;

	pHsElement pTsType = Hs_FindFirstEle(NULL,TAG_TRANSFER_SYNTAX_UID,true);
	if (pTsType == NULL)
	{
		pTsType = Hs_InsertElement(NULL,false,TAG_TRANSFER_SYNTAX_UID,VR_UI,false,0,nRet);
		if (pTsType == NULL)
			return Ret_InvalidPara;
	}

	QString sTsTypeName;
	if (nTsType == TS_IMPLICIT_VR_LITTLE_ENDIAN)
	{	
		sTsTypeName= "1.2.840.10008.1.2";
	}
	else if (nTsType == TS_EXPLICIT_VR_LITTLE_ENDIAN)
	{
		sTsTypeName= "1.2.840.10008.1.2.1";
	}
	else if (nTsType == TS_EXPLICIT_VR_BIG_ENDIAN)
	{
		sTsTypeName= "1.2.840.10008.1.2.2";
	}
	else if (nTsType == TS_RLE_LOSSLESS)
	{
		sTsTypeName= "1.2.840.10008.1.2.5";
	}
	else if (nTsType == TS_JPEG_BASELINE_1)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.50";
	}
	else if (nTsType == TS_JPEG_EXTENDED_2_4)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.51";
	}
	else if (nTsType == TS_JPEG_LOSSLESS_NONHIER_14)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.57";
	}
	else if (nTsType == TS_JPEG_LOSSLESS_NONHIER_14B)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.70";
	}
	else if (nTsType == TS_JPEG2000_LOSSLESS_ONLY)
	{
		sTsTypeName= "1.2.840.10008.1.2.4.90";
	}
	else if (nTsType == TS_JPEG2000)
	{
		sTsTypeName="1.2.840.10008.1.2.4.91";
	}
	else
	{
		sTsTypeName="1.2.840.10008.1.2";
	}
	Hs_SetStringValue(pTsType,sTsTypeName,true,0);

	pHsElement pInsertEle = Hs_InsertElement(NULL,false,0x00020000,VR_UL,false,0,nRet);
	if (pInsertEle == NULL)
		return Ret_InvalidPara;	

	int n = m_pMainEle->pChildEleV.size();
	int nLength = 0;
	if (n>1)
	{	
		for (int i=0; i<n;i++)
		{
			if (m_pMainEle->pChildEleV[i]->nTag/65536 == 2 && m_pMainEle->pChildEleV[i]->nTag != 0x00020000)
			{
				nLength = nLength + m_pMainEle->pChildEleV[i]->nOffset + m_pMainEle->pChildEleV[i]->nLen;
			}
		}
	}
	else
	{
		nLength = 0;
	}

	Hs_SetLongValue(pInsertEle,nLength,true,0);

	return 0;
}

BYTE* CHsBaseFile::GetByteFromBuffer( pHsElement pEle, unsigned long &nValueLen, int &nRet )
{
	if(pEle == NULL)
	{
		nRet = Ret_InvalidPara;
		return NULL;
	}

	if(pEle->nLen==0)
	{
		nRet = Ret_NoValue;
		return NULL;
	}

	BYTE *pByte = new BYTE[pEle->nLen];
	memcpy(pByte,&(m_buf[pEle->nTagPos+pEle->nOffset]),pEle->nLen);

	nValueLen = pEle->nLen;
	nRet = Ret_Success;

	return pByte;
}

int CHsBaseFile::Hs_GetStringValueA( unsigned long uTag,QString &sRet, bool bRoot,int nValueIndex )
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetStringValue(p,sRet,nValueIndex);
}

int CHsBaseFile::Hs_GetDateValueA( unsigned long uTag,HsDateTime &DateValue,bool bRoot, int nValueIndex )
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetDateValue(p,DateValue,nValueIndex);
}

int CHsBaseFile::Hs_GetTimeValueA( unsigned long uTag, HsDateTime &TimeValue,bool bRoot, int nValueIndex)
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetTimeValue(p,TimeValue,nValueIndex);
}

int CHsBaseFile::Hs_GetDateTimeValueA( unsigned long uTag, HsDateTime &DateTimeValue, bool bRoot/*=true*/, int nValueIndex/*=0*/ )
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetDateTimeValue(p,DateTimeValue,nValueIndex);
}

int CHsBaseFile::Hs_GetAgeValueA( unsigned long uTag,int &nAge,char &cAgeType,bool bRoot)
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetAgeValue(p,nAge,cAgeType);
}

int CHsBaseFile::Hs_GetLongValueA( unsigned long uTag, long &nValue,bool bRoot,int nValueIndex)
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetLongValue(p,nValue,nValueIndex);
}

BYTE* CHsBaseFile::Hs_GetByteValueA( unsigned long uTag, unsigned long &nValueLen, int &nRet, bool bRoot/*=true*/ )
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
	{
		nRet = Ret_TagNotFound;
		return NULL;
	}

	return Hs_GetByteValue(p,nValueLen,nRet);
}

int CHsBaseFile::Hs_GetDoubleValueA( unsigned long uTag, double &fVal,bool bRoot,int nValueIndex )
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetDoubleValue(p,fVal,nValueIndex);
}

int CHsBaseFile::Hs_GetValueCountA( unsigned long uTag,int &nRet , bool bRoot)
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
		return Ret_TagNotFound;

	return Hs_GetValueCount(p,nRet);
}

QString CHsBaseFile::Hs_GetConvertValueA( unsigned long uTag,int nValueIndex, int& nRet,bool bRoot)
{
	pHsElement p = NULL;
	if(bRoot)
		p = Hs_FindFirstEle(NULL,uTag,true);
	else
		p = Hs_FindFirstEle(NULL,uTag,false);

	if (p == NULL)
	{
		nRet = Ret_TagNotFound;
		return "";
	}

	return Hs_GetConvertValue(p,nValueIndex,nRet);
}

void CHsBaseFile::IsXRay()//是不是普放图像
{
	if(m_pMainEle == NULL)
		return;

	QString sModality = m_sModality;
	if (sModality.compare("CT", Qt::CaseInsensitive) == 0 || //这些Modality毋庸置疑，认为是序列图像，不是普放图像
		sModality.compare("MR", Qt::CaseInsensitive) == 0 ||
		sModality.compare("PET",Qt::CaseInsensitive) == 0 ||
		sModality.compare("PT", Qt::CaseInsensitive) == 0 ||
		sModality.compare("MG",Qt::CaseInsensitive) == 0 ||
		sModality.compare("RF", Qt::CaseInsensitive) == 0)
	{
		m_bXRay = false;
		return;
	}

	//但：郑大一附院用DSA做CT扫描，导致Modality=="XA"，但图像实际上是CT图像，而且每张也是512K，图像数量很大，缩略图巨多，此处专门处理：
	if (sModality.compare("XA", Qt::CaseInsensitive) == 0 || sModality.compare("DSA", Qt::CaseInsensitive) == 0)
	{
		//有层厚的认为是序列图像
		pHsElement pThickEle = Hs_FindFirstEle(NULL,TAG_SLICE_THICKNESS,true);
		if(pThickEle)
		{
			m_bXRay = false;
			return;
		}

		//彩色DSA图像也认为是序列图像，不是普放图像
		QString sImgType = "";
		if(Hs_GetStringValueA(TAG_PHOTOMETRIC_INTERPRETATION,sImgType) == 0)
		{
			if (sImgType.indexOf("RGB", 0) >= 0 || sImgType.indexOf("Palette", 0) >= 0)
			{
				m_bXRay = false;
				return;
			}
		}
		
	}

	m_bXRay = true;
}

//void CHsFile::CopyElement( pHsElement pSrc,pHsElement pDst )
//{
//	if(pSrc == NULL || pDst == NULL)
//		return;
//
//	*pDst = *pSrc;
//
//	int n = pSrc->pChildEleV.size();
//	for (int i=0;i<n;i++)
//	{
//		pHsElement pNewChildEle = new HsElement;
//
//		CopyElement(pSrc->pChildEleV[i],pNewChildEle);
//		pDst->pChildEleV.push_back(pNewChildEle);
//		pNewChildEle->pParentEle = pDst;
//	}
//}
int CHsBaseFile::Hs_OffsetWndLevel( CHsBaseImg &Img )
{
   /* int ndv = CAppConfig::m_DevCfgV.size();
    if (ndv)
    {
        string sManu = "";
        Hs_GetStringValueA(0x00080070,sManu,true,0);

        string sSerial = "";
        Hs_GetStringValueA(0x00181000,sSerial,true,0);

        string sStaion = "";
        Hs_GetStringValueA(0x00081010,sStaion,true,0);

        for (int d=0;d<ndv;d++)
        {
            if (sManu.compare(CAppConfig::m_DevCfgV[d].sManuFacturer) == 0 &&
                sSerial.compare(CAppConfig::m_DevCfgV[d].sSerialNumber) == 0 &&
                sStaion.compare(CAppConfig::m_DevCfgV[d].sStationName) == 0 )
            {
                Img.m_ImgState.CurWc.x += CAppConfig::m_DevCfgV[d].w;
                Img.m_ImgState.CurWc.y  += CAppConfig::m_DevCfgV[d].l;
                if(CAppConfig::m_DevCfgV[d].bModalityLine == false)
                {
                    Img.m_ImgState.bUseSlope = false;
                }
                if (CAppConfig::m_DevCfgV[d].bModalityLUT == false)
                {
                    int nLut = Img.m_pLutV.size();
                    for (int L=0;L<nLut;L++)
                    {
                        if (Img.m_pLutV[L]->bModality)
                        {
                            delete Img.m_pLutV[L];
                            Img.m_pLutV.erase(Img.m_pLutV.begin() + L);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }*/

    return 0;
}
