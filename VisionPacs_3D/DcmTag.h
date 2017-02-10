#pragma once
#include "stdafx.h"

class CDcmTag
{

public:
	CDcmTag(void);
	~CDcmTag(void);

private:
	static MAP_TAGPROPERTY m_TagProperty;	
	static bool m_bInit;
	static bool Init();
public:

	static pDcmTag Find(unsigned long nCode);
	static pDcmTag Insert(unsigned long nCode, unsigned long nVR,  unsigned long nMaxVM=1, unsigned long nMinVM=1,  unsigned long nMask=0, unsigned long nDivideVM=0, char *pszName="");
};
