#pragma once
class OperateMprLines : public QObject
{
	Q_OBJECT
public:
	OperateMprLines(QObject *parent,int nImgWndType);
	~OperateMprLines();

private:
	

private:
	int m_nWndType;	//父窗口的类型
	bool m_bShow;	//是否显示
	bool m_bActiveOblique; //是否允许斜切
	bool m_bInitLines; //是否初始化过

	int m_nNearFactor; //靠近系数
	bool m_bMousePressed;//鼠标是否按下

	QPoint m_prePt;
	
	MoveObject m_moveObject;//移动名称

private:
	//每个成分的位置
	MprLine *m_pLineH;
	MprPoint *m_ptHStartRotate;
	MprPoint *m_ptHEndRotate;
	MprPoint *m_ptHStartInter;
	MprPoint *m_ptHEndInter;
	MprLine *m_pLineHSliceTop;
	MprLine *m_pLineHSliceBottom;
	MprTriAngle *m_pTriHStartTop;
	MprTriAngle *m_pTriHStartBottom;
	MprTriAngle *m_pTriHEndTop;
	MprTriAngle *m_pTriHEndBottom;

	MprLine *m_pLineV;
	MprPoint *m_ptVStartRotate;
	MprPoint *m_ptVEndRotate;
	MprPoint *m_ptVStartInter;
	MprPoint *m_ptVEndInter;
	MprLine *m_pLineVSliceTop;
	MprLine *m_pLineVSliceBottom;
	MprTriAngle *m_pTriVStartTop;
	MprTriAngle *m_pTriVStartBottom;
	MprTriAngle *m_pTriVEndTop;
	MprTriAngle *m_pTriVEndBottom;

	MprPoint *m_ptCenter;

	int m_iPointWidth;
	float m_fRotateLineRate;
	int m_nTriAngleWidth;//三角形底部宽度的一半.此值得两倍才是三角形底部宽度
	int m_nTriAngleDis;//三角形底部离虚线的距离
private:
	//rc部分
	RECT m_rcImgOnWnd;
	long m_nImgW;
	long m_nImgH;

	//颜色部分
	QColor m_clrLineH;
	QColor m_clrLineV;
	QColor m_clrCenter;

	//层厚
	int m_nSlicePos;

	//图像分辨率
	double m_fSpacingX;
	double m_fSpacingY;
public:
	bool RefreshMprLinesPara(RECT rcImg, int nImgWidth, int nImgHeigh, double fspacingX, double fspacingY);
	void OnMprLinesMousePress(QMouseEvent *event);
	bool OnMprLinesMouseMove(QMouseEvent *event);
	void OnMprLinesMouseRelease(QMouseEvent *event);
	void OnMprLinesPaint(QPainter *painter);

public:
	void SetMprLineShow(bool isShow) { m_bShow = isShow; }
	void ActiveOblique() { m_bActiveOblique = true; }
	void DisactiveOblique() { m_bActiveOblique = false; }
	bool IsMprLineShow() { return m_bShow; }
	void OutputLineInfo();
	
	//设置主线
	void SetManiLinePos(MoveObject object, MprLine *pLine);

	//设置层厚
	void SetSliceLinePos(double nSliceThick);
private:
	//已知主线，更新两个旋转点的坐标
	void UpdateRotatePoint(MprLine *pMainLine, MprPoint *ptStartRotate, MprPoint *ptEndRotate);

	//根据窗口类型设置线的颜色
	void GetLinesColor();

	//图像窗口坐标转换
	bool ConvertImgToWnd(MprPoint &pt);

	//将窗口坐标转换为图像坐标
	bool ConvertWndToImg(MprPoint &pt);

	//已知线段p1-p2,在内侧与p1、p2相距[线段长度*f，其中1.00>f>0.00]处，求一个高度为nHeight的、底部宽度为nWidth*2的三角形(共4个),箭头朝外,bUseLeft:是要p1-p2方向上，左侧的三角形吗
	bool GetTrianglePoint(QPoint &p1, QPoint &p2, int iDis, UINT nHeight, UINT nWidth, MprTriAngle &T1, MprTriAngle &T2, bool bUseLeft);

	//已知线段p1-p2，求经过线段上一点PubLicPt的、且垂直于此线段的、并且距离此线段nLen的两点：newPt1和newPt2
	bool GetVerticalLine(QPoint &p1, QPoint &p2, QPoint &PublicPt, double nLen, QPoint &newPt1, QPoint &newPt2);

	//GetVerticalLine的配套函数：只留p1-p2方向上左侧的点？还是右侧的点？,bUseLeft:是要p1-p2方向上，左侧的三角形吗
	QPoint SelectPtSideLine(QPoint &p1, QPoint &p2, QPoint &newPt1, QPoint &newPt2, bool bUseLeft);

	//已知线段p1-p2,求与p1距离为nLen的两个点	
	bool GetPtByLength(QPoint&pCenterPoint,QPoint &p1, QPoint &p2, double nLen, QPoint &RetPtL, QPoint &RetPtR);

	//已知线段A和B，求交点
	bool GetInterPoint(FPOINT pA0, FPOINT pA1, FPOINT pB0, FPOINT pB1, FPOINT &retPt);
	bool GetInterPoint(QPoint pA0, QPoint pA1, QPoint pB0, QPoint pB1, QPoint &retPt);

	//鼠标位置激活直线
	bool GetPointNearLines(QPoint pt);
	//鼠标位置激活三角形
	bool GetPointNearTriAngle(QPoint pt);
	//鼠标位置激活中间点
	bool GetPointNearPoint(QPoint pt);

	//判断是否为靠近
	bool DistanceToLines(MprLine* pLine, QPoint pt);
	bool DistanceToTriangle(MprTriAngle *pTriangle, QPoint pt);
	bool DistanceToPoint(MprPoint *ptMpr,QPoint pt, int nDistance);
	float Dist2(QPoint pt1, QPoint pt2);

	//移动主直线
	void MoveMainLines(QPoint deltaPt);

	//移动中心点
	void MoveCenterPt(QPoint pt);
	//已知直线，求过直线外一点与此直线平行直线
	void GetParalleLine(MprLine *pMainLine, MprPoint *pt, MprPoint &pParalleLinePt1, MprPoint &pParalleLinePt2);

	//移动层厚线
	void MoveSliceTriangle(QPoint deltaPt);
	void CalSliceLine(MprPoint *pStart, MprPoint *pEnd, MprLine *pTopSlice, MprTriAngle *pTriST, MprTriAngle *pTriET, MprLine *pBottomSlice, MprTriAngle *pTriSB, MprTriAngle *pTriEB);
	void CalDistanceOfParalleLine(QPoint pPt, MprLine *pLine, int &fdistance);
	//移动旋转线
	void MoveRotateLine(QPoint pt);

	//已知矩形内部任意两点，求所在直线与矩形的两个交点
	bool GetLineAcrossRc(RECT rc, QPoint p1, QPoint p2, QPoint &retP1, QPoint &retP2);

	//重新求层厚点
	bool ReCalInterPoint(MprLine *pManiLine, MprPoint *pStartInter, MprPoint *pEndInter);

	//已知线段p1-p2,求与p1距离为nLen的、并且靠近p2的一个点	
	bool GetInterPtByLength(QPoint &p1, QPoint &p2, double nLen,QPoint &RetPt);

	//判断此点是否在rc去欲孽
	bool IsPointInRc(RECT rc, QPoint pt);

	//将所有组件的窗口坐标转为图像坐标
	void AllCovertWndToImg();

signals:
	void MprLinesInfoOutput(MprLinesInfo);
};


