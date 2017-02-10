#include "stdafx.h"
#include "DcmVR.h"


MAP_VRPROPERTY CDcmVR::m_VrProperty;
bool CDcmVR::m_bInit = CDcmVR::Init();

CDcmVR::CDcmVR(void)
{
}

CDcmVR::~CDcmVR(void)
{
}

bool CDcmVR::Init()
{
	Insert(VR_00,"私有Tag",						0,				4,		1,10,0,4,2,"VR_00");

	Insert(VR_SQ,"Sequence of Items",			0,				4,		1,12,0,4,4,"VR_SQ");
	Insert(VR_OB,"Other Byte String",			0,				259,	1,12,1,4,4,"VR_OB");
	Insert(VR_OW,"Other Word String",			0,				259,	2,12,1,4,4,"VR_OW");
	Insert(VR_UN,"Unknown",						0,				259,	1,12,1,4,4,"VR_UN");
	Insert(VR_UT,"Unlimited Text",				4294967294,		1025,	1,12,1,4,4,"VR_UT");
	Insert(VR_OF,"Other Float String",			4294967292,		257,	4,12,1,4,4,"VR_OF");

	Insert(VR_AE,"Application Entity",			16,				513,	1,8,2,2,2,"VR_AE");
	Insert(VR_AS,"Age String",					4,				512,	1,8,2,2,2,"VR_AS");
	Insert(VR_AT,"Attribute Tag",				4,				256,	4,8,2,2,2,"VR_AT");
	Insert(VR_CS,"Code String",					16,				513,	1,8,2,2,2,"VR_CS");
	Insert(VR_DA,"Date",						8,				512,	1,8,2,2,2,"VR_DA");
	Insert(VR_DS,"Decimal String",				16,				513,	1,8,2,2,2,"VR_DS");
	Insert(VR_DT,"Date Time",					26,				513,	1,8,2,2,2,"VR_DT");
	Insert(VR_FD,"Floating Point Double",		8,				256,	8,8,2,2,2,"VR_FD");
	Insert(VR_FL,"Floating Point Single",		4,				256,	4,8,2,2,2,"VR_FL");
	Insert(VR_IS,"Integer String",				12,				513,	1,8,2,2,2,"VR_IS");
	Insert(VR_LO,"Long String",					64,				513,	1,8,2,2,2,"VR_LO");
	Insert(VR_LT,"Long Text",					10240,			1025,	1,8,2,2,2,"VR_LT");
	Insert(VR_PN,"Person Name",					64,				514,	1,8,2,2,2,"VR_PN");
	Insert(VR_SH,"Short String",				16,				513,	1,8,2,2,2,"VR_SH");
	Insert(VR_SL,"Signed Long",					4,				256,	4,8,2,2,2,"VR_SL");
	Insert(VR_SS,"Signed Short",				2,				256,	2,8,2,2,2,"VR_SS");
	Insert(VR_ST,"Short Text",					1024,			1025,	1,8,2,2,2,"VR_ST");
	Insert(VR_TM,"Time",						16,				513,	1,8,2,2,2,"VR_TM");
	Insert(VR_UI,"Unique Identifier",			64,				513,	1,8,2,2,2,"VR_UI");
	Insert(VR_UL,"Unsigned Long",				4,				256,	4,8,2,2,2,"VR_UL");
	Insert(VR_US,"Unsigned Short",				2,				256,	2,8,2,2,2,"VR_US");


	return true;
}

pDcmVR CDcmVR::Find(unsigned long nVrCode)
{
	return &(m_VrProperty[nVrCode]);
}
pDcmVR CDcmVR::Insert(int  nCode,char *pszName,int  nValueLength,int  nRestrict,int  nUnitSize,int nOffset,int nVrType,unsigned long nLenOfLenDes,unsigned long nLenOfVrDes,char* pShortName)
{
	m_VrProperty[nCode].nCode = nCode;
	m_VrProperty[nCode].nValueLeng = nValueLength;
	m_VrProperty[nCode].nOffset = nOffset;//这里的offset不是绝对的.还有个重要因素:该Tag是否含有VR域
	m_VrProperty[nCode].nRestrict = nRestrict;
	m_VrProperty[nCode].nUnitSize = nUnitSize;
	m_VrProperty[nCode].nLenOfVrDes = nLenOfVrDes;


	m_VrProperty[nCode].nVrType = nVrType;
	strcpy_s(m_VrProperty[nCode].pszName,256,pszName);
	strcpy_s(m_VrProperty[nCode].pszShortName,10,pShortName);
	m_VrProperty[nCode].nLenOfLenDes = nLenOfLenDes;

	return &(m_VrProperty[nCode]);
}