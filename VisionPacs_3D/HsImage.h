#pragma once
#include "HsBaseImg.h"

class CHsFile;
class ImageWnd;

class CHsImage :
	public CHsBaseImg
{
public:
	CHsImage();
	~CHsImage();

private:
	CHsFile *m_pDs;

	//初始填充信息
	int InitInfoItem(CHsFile* pDs, INFOITEM &infoItem);
	//更新变化信息
	int RefreshInfoItem(CHsFile* pDs, INFOITEM &infoItem);
	//获取格式化字符串的类型0=不确定，1=字符串格式化，2=整数格式化，3=小数格式化
	int GetFormatType(QString &sFormat);
	//所属窗体
	ImageWnd *m_pBelongWnd;

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

	//窗口左边转图像坐标
	POINT ConvertWndToImg(RECT ImgRcOnWnd, long nImgW, long nImgH, POINT &pt);

	int Hs_ApplyCurImgState();

	//以QT的方式画图
	int Hs_QtDrawImg(QImage &qImage, RECT &rc);

	//因为QT现实图像颠倒，需将原始数据翻转
	int Hs_FilpOridata();

	//获得角标信息
	MODINFO m_CornorInfo;
	void Hs_GetCornerInfo(bool bFrist);

	//层厚
	float m_fOriSilceThick;
	float m_fCurSliceThick;

	//设置、获取所属窗体
	void SetBelongWnd(ImageWnd *pWnd) { m_pBelongWnd = pWnd; }
	ImageWnd *GetBelongWnd() { return m_pBelongWnd; }

};

