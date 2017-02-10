#pragma once
#include "stdafx.h"

class CDcmVR
{
public:
	CDcmVR(void);
	~CDcmVR(void);

private:
	static MAP_VRPROPERTY m_VrProperty;
	static bool m_bInit;
	static bool Init();
public:

	static pDcmVR Find(unsigned long nVrCode);
	static pDcmVR Insert(int  nCode,char *pszName,int  nValueLen,int  nRestrict,int  nUnitSize,int nOffset,int nVrType,unsigned long nLenOfLenDes/*"值长度的描述"的长度*/,unsigned long nLenOfVrDes/*类型域长度*/,char* pShortName/*缩写VR_OF...*/);

};
