#pragma once
//////////////////////////////////////////////////////////////////////////
//解析Dicom文件
//本类主要构成就是一个m_pMainEle，其内部通过ChildV体现文件Ele的层次，每个ELe又通过pPreEle和pNextEle体现List结构
//只要理解了这一点，就可以任意制作自己喜欢的获取Ele的函数。
//////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "DcmVR.h"
#include "DcmTag.h"
#include "HsBaseImg.h"

template <class TP>	//对于超过最大值的有符号像素，进行修正
int CorrectPixelValue(TP **pPixData,unsigned long nRow,unsigned long nCol,const TP nMinPixValue,const TP nMaxPixValue,TP nOffset)
{
	if(pPixData == NULL)
		return Ret_InvalidPara;

	for (unsigned long r = 0;r<nRow; r++)
	{
		for (unsigned long c=0;c<nCol; c++)
		{
			if(pPixData[r][c] > nMaxPixValue)
				pPixData[r][c] -= nOffset;

			if(pPixData[r][c] < nMinPixValue)
				pPixData[r][c] += nOffset;
		}
	}

	return 0;
}

template <class T>
int SetNumberValue(CHsFile* pDs, pHsElement pEle, T nValue,bool bCover,int nValueIndex)
{
	if(pEle == NULL || pDs == NULL)
		return Ret_InvalidPara;

	if (pEle->nVR == VR_FD || pEle->nVR == VR_DS)
		return pDs->Hs_SetDoubleValue(pEle,(double)(nValue*1.00),bCover,nValueIndex);

	if (pEle->nVR == VR_OF || pEle->nVR == VR_FL)
		return pDs->Hs_SetFloatValue(pEle,(float)(nValue*1.00),bCover,nValueIndex);

	if (pEle->nVR == VR_SS || pEle->nVR == VR_US || pEle->nVR == VR_OW || pEle->nVR == VR_OB)
		return pDs->Hs_SetShortValue(pEle,(short)nValue,bCover,nValueIndex);

	if (pEle->nVR == VR_AT || pEle->nVR == VR_IS || pEle->nVR == VR_SL || pEle->nVR == VR_UL )
		return pDs->Hs_SetLongValue(pEle,(long)nValue,bCover,nValueIndex);

	qDebug("\r\nSetTagValue:无法处理的Tag类型！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！");

	return Ret_InvalidPara;
}
class CHsBaseFile
{
public:
	CHsBaseFile(void);
	~CHsBaseFile(void);

	//Member:
private:
	FILE *m_fp;
	unsigned char* m_buf;

	long m_nBufLen;//m_buf大小
	long m_nDefaultReadSize;//一个文件第一次读取多少字节?---不打算一次将文件全部读完,,占用内存太多

	pHsElement m_pPreEle;//为了表达元素的list形式.
public:
	pHsElement m_pMainEle;//假设整个Dicom文件就是一个大元素.

	HsBaseFileInfo m_BaseFileInfo;//关于本文件自身的关键信息

	bool m_bXRay;//是不是按照普放图像展示

	//Method:
private:
	// 找传输语法
	int GetTsType();

	// 获取Dcm文件第一个Tag的位置
	bool GetFirstTagPos(void);

	// 从nStartPos处读4个字节作为Tag
	int GetTagNumber(unsigned long nStartPos,unsigned long &nTag,bool &bBigEndia);

	// 得到nTag的类型
	unsigned long GetTagVR(unsigned long nPos,unsigned long nTag ,bool &bShowField,int &nRet);

	// 获取pParentEle所含的所有Element
	int GetChildElement(unsigned long &nPos/*父元素值域的第一个字节*/,pHsElement pParentEle/*父元素指针*/);

	// 获取非Item类型的元素
	pHsElement GetNormalElement(unsigned long &nStartPos,unsigned long nTag,unsigned long nVR,bool bBigEndia,bool bVrFiledShow,int &nRet);

	// 解析一个队列元素
	pHsElement GetSequenceElement(unsigned long &nStartPos,unsigned long nTag,bool bBigEndia,int &nRet);

	// 解析一个ITEM元素
	pHsElement GetItemElement(unsigned long &nStartPos/*一个Item元素的第一个字节*/,unsigned long nTag/*TAG_ITEM*/,bool bBigEndia/*传输语法非大即小*/,int &nRet);

	// 建立树形结构
	void BuildTreeRelation(pHsElement pCurEle,pHsElement pParentEle,pHsElement pPreEle);

	//释放m_buf
	int FreeBuffer(void);

	//拆分字符串获取指定的一段
	int DivString(char* pString,const char* pDelChar,int nID, QString &sValue);

	//LodaFile时销毁一个出错的元素.仅仅删除是不够的,还要将指向这个被销毁元素的指针处理一下
	pHsElement DestroyEle(pHsElement pEle);

	//将元素建立List结构
	pHsElement BuildListRelation(pHsElement pEle);

    int ShowElement(pHsElement p,int nLevel);

	//根据我的Element获得Leadtools的DS中对应的ELe
	pDICOMELEMENT GetLtEleByMyEle(pHsElement pMyEle, LDicomDS *pDS);

	//分离Overlay和像素,并求最大值最小值----这些工作都是需要循环每个像素的,所以放到一起.减少循环次数
	int SeparatePixdataAndOverlayByRow(CHsBaseImg &Img,unsigned long iRow,unsigned long nCol,long &nMin,long &nMax);

	//复制Element
	void CopyEle(HsElement* pFrom,HsElement* pTo);


	//hmy 
	int  ValueToEle(void *pData,pHsElement pEle, int iTypeSize,bool bCover, int nValueIndex);

	BYTE *WritePara(pHsElement pEle,BYTE *pData,int iDataSize/*参数描述的长度，根据字典查询*/,bool bBigEndia,int iPareType/*参数类型*/);

	BYTE *WriteValue(pHsElement pEle,BYTE *pData, int iDataSize,bool bConvert/*大小端是否转换*/);

	BYTE *WriteImage(ImageInfo ImgInfo,BYTE *pImageData,int iImageByte,bool bConvert);

	int SaveTsType(pHsElement pEle, int nTsType,HANDLE &hf);

	int SaveItemEle(pHsElement pEle,HANDLE &hf,bool bBigEndia,bool bConvert);

	int SaveSquenceEle(pHsElement pEle,HANDLE &hf,bool bBigEndia,bool bConvert);

	int SaveNormalEle(pHsElement pEle,HANDLE &hf,bool bBigEndia,bool bConvert);

	int SavePixelDataEle(pHsElement pPixelEle,HANDLE &hf,bool bBigEndia,bool bConvert);

	//为定位线量身定做--获取定位线所需材料-----一个小函数,为了方便
	bool GetImageLocPara(ImageInfo &info);

	//获取定位线所修材料第二种情况--有的多帧图像会将这些信息集中存储在一个队列下，本函数负责去抠
	bool GetImageLocParaMulityFrame(int iFrame,ImageInfo &info);

	//小工具函数，专门获取0x52009230(Per-frame Functional Groups Sequence)下某帧的Item
	pHsElement GetItemFromPerFramFunGpSeq(pHsElement pFrmFunGpSeqEle,int iFrame);

	int CalcInformationLength( int nTsType );

	BYTE* GetByteFromBuffer(pHsElement pEle,	unsigned long &nValueLen, int &nRet);

	//复制Element的小函数
	//void CopyElement(pHsElement pSrc,pHsElement pDst);

	//判断是不是普放图像
	void IsXRay();
public:
	// 加载一个文件
	int Hs_LoadFile(const char *cFileName,bool bFullLoad=false);

	// 释放CHsBaseFile对象
	int Hs_CloseFile();

	// 查找第一个Tag=nTag的元素,pSiblingEle==NULL:表示在第一级查找.否则在与pSiblingEle同级中查找第一个nTag
	pHsElement Hs_FindFirstEle(pHsElement pSiblingEle,unsigned long nTag,bool bAsTree);

	// 自pEle开始向下,查找与pElen的Tag相等的元素.bTree==true:只在同级查找,否则作为list查找整个文件
	pHsElement Hs_FindNextEle(pHsElement pEle,bool bTree);

	// 返回pEle的某个子元素
	pHsElement Hs_GetChildEle(pHsElement pEle,unsigned long nTag=0,int nIndex=0);

	// 返回SQ型的pEle的所有直接下级Item
	unsigned long Hs_GetSequenceItemEle(pHsElement pEle,QVector<pHsElement> &ItemEleV);

	// 自pEle向上查找与pELe的nTag相同的元素.bTree==true:只在pEle同级中查找,否则以list形式查找整个文件
	pHsElement Hs_FindPreEle(pHsElement pEle,bool bTree);

	// 取含有小数的Tag的值;(VR_DS   VR_FD   VR_FL)
	int Hs_GetDoubleValue(pHsElement pEle,	double &fVal,int nValueIndex=0);
	// 直接取含有小数的Tag的值;(VR_DS   VR_FD   VR_FL)
	int Hs_GetDoubleValueA(unsigned long uTag,	double &fVal,bool bRoot=true,int nValueIndex=0);

	//以字符串形式返回Tag的值
	int Hs_GetStringValue(pHsElement pEle, QString &sRet, int nValueIndex = 0);
	//直接获取根目录下这个Tag的值
	int Hs_GetStringValueA(unsigned long uTag, QString &sRet, bool bRoot = true, int nValueIndex = 0);

	// 取日期
	int Hs_GetDateValue(pHsElement pEle,	HsDateTime &DateValue,	int nValueIndex=0);
	//直接取日期
	int Hs_GetDateValueA(unsigned long uTag,HsDateTime &DateValue,bool bRoot=true,	int nValueIndex=0);

	// 取时间
	int Hs_GetTimeValue(pHsElement pEle,	HsDateTime &TimeValue,	int nValueIndex=0);
	// 直接取时间
	int Hs_GetTimeValueA(unsigned long uTag,	HsDateTime &TimeValue,bool bRoot=true,	int nValueIndex=0);

	// 取日期时间
	int Hs_GetDateTimeValue(pHsElement pEle,	HsDateTime &DateTimeValue,	int nValueIndex=0);
	// 直接取日期时间
	int Hs_GetDateTimeValueA(unsigned long uTag,	HsDateTime &DateTimeValue, bool bRoot=true, int nValueIndex=0);

	// 获取年龄
	int Hs_GetAgeValue(pHsElement pEle,int &nAge,char &cAgeType);
	// 直接获取年龄
	int Hs_GetAgeValueA(unsigned long uTag,int &nAge,char &cAgeType,bool bRoot=true);

	//获取整数Tag值
	int Hs_GetLongValue(pHsElement pEle,	long &nValue,int nValueIndex=0);
	//直接获取整数Tag值
	int Hs_GetLongValueA(unsigned long uTag, long &nValue,bool bRoot=true,int nValueIndex=0);

	//直接拷贝某Tag的值------需要delete []返回的指针
	BYTE* Hs_GetByteValue(pHsElement pEle,	unsigned long &nValueLen, int &nRet);
	//直接拷贝某Tag的值------需要delete []返回的指针
	BYTE* Hs_GetByteValueA(unsigned long uTag,	unsigned long &nValueLen, int &nRet, bool bRoot=true);

	//返回某Tag所含值的个数
	int Hs_GetValueCount(pHsElement pEle,int &nRet);
	//直接返回某Tag所含值的个数
	int Hs_GetValueCountA(unsigned long uTag,int &nRet, bool bRoot=true);

	//以字符串形式返回Tag值
	QString Hs_GetConvertValue(pHsElement pEle ,int nValueIndex, int& nRet);
	//直接以字符串形式返回Tag值
	QString Hs_GetConvertValueA(unsigned long uTag, int nValueIndex, int& nRet, bool bRoot = true);

	// 获取某Tag_Pixel_Data的信息
	int Hs_GetImageInfo(pHsElement pPixEle, ImageInfo& ImgInfo,int iFrame);

	// 读取某Tag_Pixel_Data的第iFrame帧像素字节,iFrame=-1.表示全部读取
	virtual int Hs_GetImage(pHsElement pPixEle,CHsBaseImg &Img, int iFrame);

	//获取所有Lut
	int Hs_GetLuts(pHsElement pPixelEle,CHsBaseImg &Img);

	//复制
	virtual int Hs_CopyTo( CHsBaseFile &NewFile);

	//读取Lut的一个小函数，通用
	bool Hs_GetLutDataItem( pHsElement pLutItemEle,LutData &lut );
	bool Hs_GetWcLutItem(pHsElement pLutItemEle, QVector<LutData*> &LutV);

	//专门搞个函数获取帧数吧
	int Hs_GetFrameCount(pHsElement pPixEle);

    //偏移窗宽窗位
    int Hs_OffsetWndLevel(CHsBaseImg &Img);

	//编辑DICOM********************************************************************************
	//初始化一个Dcm文件
	int Hs_InitFile( QString StorageClassUID, QString StorageInstancUID,int nTsType );

	//插入新Ele
	pHsElement Hs_InsertElement(pHsElement pNeighbor,bool bChild, unsigned long nTag,unsigned long nVR,bool bSquence/*当插入私有Tag时此Tag是否为SQ*/,int nIndex/*当插入私有Item时,Item在SQ下的位置序数*/,int &nRet);

	//删除Ele
	int Hs_DeleteElement(pHsElement pEle);

	//删除值域中特定的值,nIndex从0开始
	int Hs_DeleteValues(pHsElement pEle,int nIndex);

	//修改Tag 整型值 其中nValueIndex从0开始
	int Hs_SetLongValue(pHsElement pEle, long nValue, bool bCover,int  nValueIndex);

	//修改Tag 短整型值 其中nValueIndex从0开始
	int Hs_SetShortValue(pHsElement pEle, short nValue, bool bCover,int  nValueIndex);

	//修改Tag 双精度浮点型值 其中nValueIndex从0开始
	int Hs_SetDoubleValue(pHsElement pEle, double nValue,bool bCover,int nValueIndex);

	//修改Tag 单精度浮点型值 其中nValueIndex从0开始
	int Hs_SetFloatValue(pHsElement pEle, float nValue,bool bCover,int nValueIndex);

	//修改Tag 字符值 其中nValueIndex从0开始
	int Hs_SetStringValue(pHsElement pEle, QString nValue,bool bCover,int nValueIndex);

	//修改Tag 日期值 其中nValueIndex从0开始
	int Hs_SetDataValue(pHsElement pEle, HsDateTime nValue,bool bCover,int nValueIndex);

	//修改Tag 时间值 其中nValueIndex从0开始
	int Hs_SetTimeValue(pHsElement pEle, HsDateTime nValue,bool bCover,int nValueIndex);

	//修改Tag 日期时间值 其中nValueIndex从0开始
	int Hs_SetDataTimeValue(pHsElement pEle, HsDateTime nValue,bool bCover,int nValueIndex);

	//修改Tag 年龄值 其中nValueIndex从0开始
	int Hs_SetAgeValue(pHsElement pEle,int nAge,char cAgeType,bool bCover,int nValueIndex);

	//设置BYTE值域
	int Hs_SetByteValue( pHsElement pEle,BYTE *pValue,int nCount);

	//保存修改，形成新的DICOM
	int Hs_SaveFile(const char *cFileName,int nTsType=TS_EXPLICIT_VR_LITTLE_ENDIAN);

public:
	//俩文件之间复制Tag(仅限根目录下的Tag)
	int Hs_CopyRootTagTo(CHsBaseFile* pDstDs, unsigned long nTag);//,pHsElement* pDstParentEle);

	QString m_sModality;//直接读出来记住得了，省的重复
};
