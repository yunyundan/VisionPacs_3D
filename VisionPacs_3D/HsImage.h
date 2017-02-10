#pragma once
#include "HsBaseImg.h"

class CHsFile;

class CHsImage :
	public CHsBaseImg
{
public:
	CHsImage();
	~CHsImage();

private:
	CHsFile *m_pDs;

protected:
	//在窗体上占有的区域
	RECT m_WndRc;
public:
	void SetDs(CHsFile *pDS);
	CHsFile *GetDs();

	//获取当前图像大小.
	SIZE Hs_GetImgSize(bool bDisplaySize = false);

	//设置在窗体上的显示区域
	void SetWndRc(RECT rc);

	//获取在窗体上的显示区域
	RECT GetWndRc();

	//还原
	int Hs_Restore();

	//调窗,bChangeValue=true意为w和c是变化量.否则w和c是绝对值
	int Hs_WinLevel(long w, long c, bool bChangeValue = false, long *pRetW = NULL, long *pRetC = NULL);

	int Hs_Reload(int islicenum, bool bApplyCurImgState = true);

	//删除图像
	int Hs_FreeMem();

	//窗口左边转图像坐标
	POINT ConvertWndToImg(RECT ImgRcOnWnd, long nImgW, long nImgH, POINT &pt);

	int Hs_ApplyCurImgState();

	//以QT的方式画图
	int Hs_QtDrawImg(QImage &qImage, RECT &rc);

	//因为QT现实图像颠倒，需将原始数据翻转
	int Hs_FilpOridata();
};

