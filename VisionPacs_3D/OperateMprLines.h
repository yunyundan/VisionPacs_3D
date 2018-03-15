#pragma once

class CResliceControl;
class HmyLine3D;
class HmyPlane3D;

class OperateMprLines : public QObject
{
	Q_OBJECT
public:
	OperateMprLines(QObject *parent,int nImgWndType);
	~OperateMprLines();	

private:
	int m_nWndType;	//父窗口的类型
	bool m_bShow;	//是否显示
	bool m_bActiveOblique; //是否允许斜切
	bool m_bInitLines; //是否初始化过

	int m_nNearFactor; //靠近系数
	bool m_bMousePressed;//鼠标是否按下

	QPoint m_prePt;
		
	MoveObject m_moveObject;//移动名称

	CResliceControl *m_pResliceControl;

private:
	//每个成分的位置
	MprLine *m_pLineAxis1;
	MprPoint *m_ptA1StartRotate;
	MprPoint *m_ptA1EndRotate;
	MprPoint *m_ptA1StartInter;
	MprPoint *m_ptA1EndInter;
	MprLine *m_pLineA1SliceTop;
	MprLine *m_pLineA1SliceBottom;

	MprLine *m_pLineAxis2;
	MprPoint *m_ptA2StartRotate;
	MprPoint *m_ptA2EndRotate;
	MprPoint *m_ptA2StartInter;
	MprPoint *m_ptA2EndInter;
	MprLine *m_pLineA2SliceTop;
	MprLine *m_pLineA2SliceBottom;

	MprPoint m_ptPreCenter;
	MprPoint *m_ptCenter;

	int m_iPointWidth;
	float m_fRotateLineRate;

private:
	//rc部分
	RECT m_rcImgOnWnd;
	long m_nImgW;
	long m_nImgH;
	double m_dSpacingX;
	double m_dSpacingY;

	//颜色部分
	QColor m_clrLineAxis1;
	QColor m_clrLineAxis2;
	QColor m_clrCenter;

	//层厚
	int m_nHWndSlicePos;
	int m_nVWndSlicePos;
	int m_nHImgSlicePos;
	int m_nVImgSlicePos;

public:
	bool RefreshMprLinesPara(RECT rcImg, int nImgWidth, int nImgHeigh, double dSpacingX, double dSpacingY);
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

	//根据新的中心点，设置主线的位置
	void OnCenterPointChanged();
	
	//设置主线
	void SetManiLinePos(MoveObject object, MprLine *pLine);

	//设置层厚
	void SetSliceLinePos(double nSliceThickm,int nIndex);//0：Axis1, 1:Axis2
	
	//设置control
	void SetResliceControl(CResliceControl *control) { m_pResliceControl = control; }
	CResliceControl *GetResliceControl() { return m_pResliceControl; }

private:
	//已知主线，更新两个旋转点的坐标
	void UpdateRotatePoint(MprLine *pMainLine, MprPoint *ptStartRotate, MprPoint *ptEndRotate);

	//根据窗口类型设置线的颜色
	void GetLinesColor();

	//图像窗口坐标转换
	bool ConvertImgToWnd(MprPoint &pt);

	//将窗口坐标转换为图像坐标
	bool ConvertWndToImg(MprPoint &pt);

	//已知线段p1-p2，求经过线段上一点PubLicPt的、且垂直于此线段的、并且距离此线段nLen的两点：newPt1和newPt2
	bool GetVerticalLine(QPoint &p1, QPoint &p2, QPoint &PublicPt, double nLen, QPoint &newPt1, QPoint &newPt2);

	//已知线段A和B，求交点
	bool GetInterPoint(FPOINT pA0, FPOINT pA1, FPOINT pB0, FPOINT pB1, FPOINT &retPt);
	bool GetInterPoint(QPoint pA0, QPoint pA1, QPoint pB0, QPoint pB1, QPoint &retPt);

	//鼠标位置激活直线
	bool GetPointNearLines(QPoint pt);
	//鼠标位置激活中间点
	bool GetPointNearPoint(QPoint pt);

	//判断是否为靠近
	bool DistanceToLines(MprLine* pLine, QPoint pt);
	bool DistanceToPoint(MprPoint *ptMpr,QPoint pt, int nDistance);
	float Dist2(QPoint pt1, QPoint pt2);

	//移动主直线
	bool MoveMainLines(QPoint deltaPt);

	//移动中心点
	bool MoveCenterPt(QPoint pt);
	//已知直线，求过直线外一点与此直线平行直线
	void GetParalleLine(MprLine *pMainLine, MprPoint *pt, MprPoint &pParalleLinePt1, MprPoint &pParalleLinePt2);

	//移动层厚线
	bool MoveSliceLines(QPoint deltaPt);
	void CalSliceLine(MprPoint *pStart, MprPoint *pEnd, MprLine *pTopSlice, MprLine *pBottomSlice);
	void CalDistanceOfParalleLine(QPoint pPt, MprLine *pLine, int &fdistance);
	void CalImageDistance(QPoint pPt, MprLine *pLine, int &fdistance);
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

	//获得两个轴的序号
	int GetAxis1();
	int GetAxis2();	

	//获得平面水平&垂直向量
	int GetPlaneAxis1();
	int GetPlaneAxis2();

	//移动中心时对control的影响
	void MoveResliceControlCenter();

	//计算旋转
	double RotateAxisByPoint(QPoint pt, MoveObject moveObject);
	void RotateAxisByAngle(double angle, MoveObject moveObject);

	void RotateVectorAboutVector(double vectorToBeRotated[3],
		double axis[3], // vector about which we rotate
		double angle, // angle in radians
		double output[3]);

	//计算两个平面的交线
	HmyLine3D CalInterSectingLine(HmyPlane3D *target, HmyPlane3D *reference);


signals:
	void MprLinesInfoChange(MprLinesInfo info);
};


