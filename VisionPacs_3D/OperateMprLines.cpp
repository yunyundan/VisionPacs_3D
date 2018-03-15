#include "stdafx.h"
#include "OperateMprLines.h"
#include "ResliceControl.h"
#include "Hmy3dMath.h"


OperateMprLines::OperateMprLines(QObject *parent,int nImgWndType)
	: QObject(parent)
{
	m_bShow = true;
	m_bInitLines = false;
	m_bActiveOblique = false;
	m_nNearFactor = 5;
	m_bMousePressed = false;
	m_moveObject = A1LINE;

	m_prePt = QPoint(0, 0);

	m_pLineAxis1 = nullptr;
	m_ptA1StartRotate = nullptr;
	m_ptA1EndRotate = nullptr;
	m_ptA1StartInter = nullptr;
	m_ptA1EndInter = nullptr;


	m_pLineA1SliceTop = nullptr;
	m_pLineA1SliceBottom = nullptr;

	m_pLineAxis2 = nullptr;
	m_ptA2StartRotate = nullptr;
	m_ptA2EndRotate = nullptr;
	m_ptA2StartInter = nullptr;
	m_ptA2EndInter = nullptr;
	m_pLineA2SliceTop = nullptr;
	m_pLineA2SliceBottom = nullptr;

	m_ptCenter = nullptr;

	m_pResliceControl = nullptr;

	m_iPointWidth = 2;
	m_fRotateLineRate = 0.1;

	m_nWndType = nImgWndType;

	m_nHWndSlicePos = 0;
	m_nVWndSlicePos = 0;
	m_nHImgSlicePos = 0;
	m_nVImgSlicePos = 0;

	m_dSpacingX = 0.00;
	m_dSpacingY = 0.00;
	GetLinesColor();
}

OperateMprLines::~OperateMprLines()
{
	if (m_pLineAxis1)
	{
		delete m_pLineAxis1;
		m_pLineAxis1 = nullptr;
	}

	if (m_ptA1StartRotate)
	{
		delete m_ptA1StartRotate;
		m_ptA1StartRotate = nullptr;
	}

	if (m_ptA1EndRotate)
	{
		delete m_ptA1EndRotate;
		m_ptA1EndRotate = nullptr;
	}

	if (m_ptA1StartInter)
	{
		delete m_ptA1StartInter;
		m_ptA1StartInter = nullptr;
	}

	if (m_ptA1EndInter)
	{
		delete m_ptA1EndInter;
		m_ptA1EndInter = nullptr;
	}

	if (m_pLineA1SliceTop)
	{
		delete m_pLineA1SliceTop;
		m_pLineA1SliceTop = nullptr;
	} 

	if (m_pLineA1SliceBottom)
	{
		delete m_pLineA1SliceBottom;
		m_pLineA1SliceBottom = nullptr;
	}

	if (m_pLineAxis2)
	{
		delete m_pLineAxis2;
		m_pLineAxis2 = nullptr;
	}

	if (m_ptA2StartRotate)
	{
		delete m_ptA2StartRotate;
		m_ptA2StartRotate = nullptr;
	}

	if (m_ptA2EndRotate)
	{
		delete m_ptA2EndRotate;
		m_ptA2EndRotate = nullptr;
	}

	if (m_ptA2StartInter)
	{
		delete m_ptA2StartInter;
		m_ptA2StartInter = nullptr;
	}

	if (m_ptA2EndInter)
	{
		delete m_ptA2EndInter;
		m_ptA2EndInter = nullptr;
	}

	if (m_pLineA2SliceTop)
	{
		delete m_pLineA2SliceTop;
		m_pLineA2SliceTop = nullptr;
	}

	if (m_pLineA2SliceBottom)
	{
		delete m_pLineA2SliceBottom;
		m_pLineA2SliceBottom = nullptr;
	}


	if (m_ptCenter)
	{
		delete m_ptCenter;
		m_ptCenter = nullptr;
	}
}


bool OperateMprLines::RefreshMprLinesPara(RECT rcImg, int nImgWidth, int nImgHeigh, double dSpacingX, double dSpacingY)
{
	if (rcImg.right - rcImg.left <= 0 || rcImg.bottom - rcImg.top <= 0)
		return false;
	if (nImgWidth <= 0 || nImgHeigh <= 0)
		return false;

	m_rcImgOnWnd = rcImg;
	m_nImgW = nImgWidth;
	m_nImgH = nImgHeigh;
	m_dSpacingX = dSpacingX;
	m_dSpacingY = dSpacingY;

	if (m_bInitLines == false)
	{
		if (1) //横向
		{
			m_pLineAxis1 = new MprLine;
			m_pLineAxis1->pt1.ImgPt.x = 0.00;
			m_pLineAxis1->pt1.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineAxis1->pt1);

			m_pLineAxis1->pt2.ImgPt.x = m_nImgW*1.00;
			m_pLineAxis1->pt2.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineAxis1->pt2);

			m_ptA1StartRotate = new MprPoint;
			m_ptA1EndRotate = new MprPoint;
			UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);

			int nRotateLineLength = (m_pLineAxis1->pt2.WndPt.x() - m_pLineAxis1->pt1.WndPt.x()) * m_fRotateLineRate;

			m_ptA1StartInter = new MprPoint;
			m_ptA1StartInter->WndPt.setX(m_pLineAxis1->pt1.WndPt.x() + nRotateLineLength);
			m_ptA1StartInter->WndPt.setY(m_pLineAxis1->pt1.WndPt.y());

			m_ptA1EndInter = new MprPoint;
			m_ptA1EndInter->WndPt.setX(m_pLineAxis1->pt2.WndPt.x() - nRotateLineLength);
			m_ptA1EndInter->WndPt.setY(m_pLineAxis1->pt2.WndPt.y());

			//横向的层厚线--初始值与主线重合
			m_pLineA1SliceTop = new MprLine;
			m_pLineA1SliceTop->pt1 = *m_ptA1StartInter;
			m_pLineA1SliceTop->pt2 = *m_ptA1EndInter;

			m_pLineA1SliceBottom = new MprLine;
			m_pLineA1SliceBottom->pt1 = *m_ptA1StartInter;
			m_pLineA1SliceBottom->pt2 = *m_ptA1EndInter;
		}

		if (2) //纵向
		{
			m_pLineAxis2 = new MprLine;
			m_pLineAxis2->pt1.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineAxis2->pt1.ImgPt.y = 0.00 ;
			ConvertImgToWnd(m_pLineAxis2->pt1);

			m_pLineAxis2->pt2.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineAxis2->pt2.ImgPt.y = m_nImgH*1.00;
			ConvertImgToWnd(m_pLineAxis2->pt2);

			m_ptA2StartRotate = new MprPoint;
			m_ptA2EndRotate = new MprPoint;
			UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);

			int nRotateLineLength = (m_pLineAxis2->pt2.WndPt.y() - m_pLineAxis2->pt1.WndPt.y()) * m_fRotateLineRate;

			m_ptA2StartInter = new MprPoint;
			m_ptA2StartInter->WndPt.setX(m_pLineAxis2->pt1.WndPt.x());
			m_ptA2StartInter->WndPt.setY(m_pLineAxis2->pt1.WndPt.y() + nRotateLineLength);

			m_ptA2EndInter = new MprPoint;
			m_ptA2EndInter->WndPt.setX(m_pLineAxis2->pt2.WndPt.x());
			m_ptA2EndInter->WndPt.setY(m_pLineAxis2->pt2.WndPt.y() - nRotateLineLength);

			//横向的层厚线--初始值与主线重合
			m_pLineA2SliceTop = new MprLine;
			m_pLineA2SliceTop->pt1 = *m_ptA2StartInter;
			m_pLineA2SliceTop->pt2 = *m_ptA2EndInter;

			m_pLineA2SliceBottom = new MprLine;
			m_pLineA2SliceBottom->pt1 = *m_ptA2StartInter;
			m_pLineA2SliceBottom->pt2 = *m_ptA2EndInter;

		}

		if (3)//交点
		{
			m_ptCenter = new MprPoint;
			GetInterPoint(m_pLineAxis2->pt1.ImgPt, m_pLineAxis2->pt2.ImgPt, m_pLineAxis1->pt1.ImgPt, m_pLineAxis1->pt2.ImgPt, m_ptCenter->ImgPt);
			ConvertImgToWnd(*m_ptCenter);

			m_ptPreCenter = *m_ptCenter;			
		}

		m_bInitLines = true;
		AllCovertWndToImg();
	}
	else
	{
		if (1) //横向
		{
			ConvertImgToWnd(m_pLineAxis1->pt1);
			ConvertImgToWnd(m_pLineAxis1->pt2);
			ConvertImgToWnd(*m_ptA1StartRotate);
			ConvertImgToWnd(*m_ptA1EndRotate);
			ConvertImgToWnd(*m_ptA1StartInter);
			ConvertImgToWnd(*m_ptA1EndInter);
			ConvertImgToWnd(m_pLineA1SliceTop->pt1);
			ConvertImgToWnd(m_pLineA1SliceTop->pt2);
			ConvertImgToWnd(m_pLineA1SliceBottom->pt1);
			ConvertImgToWnd(m_pLineA1SliceBottom->pt2);
		}

		if (2)//纵向
		{
			ConvertImgToWnd(m_pLineAxis2->pt1);
			ConvertImgToWnd(m_pLineAxis2->pt2);
			ConvertImgToWnd(*m_ptA2StartRotate);
			ConvertImgToWnd(*m_ptA2EndRotate);
			ConvertImgToWnd(*m_ptA2StartInter);
			ConvertImgToWnd(*m_ptA2EndInter);
			ConvertImgToWnd(m_pLineA2SliceTop->pt1);
			ConvertImgToWnd(m_pLineA2SliceTop->pt2);
			ConvertImgToWnd(m_pLineA2SliceBottom->pt1);
			ConvertImgToWnd(m_pLineA2SliceBottom->pt2);
		}

		if (3)//交点
		{
			ConvertImgToWnd(*m_ptCenter);
		}

		if (4)//层厚
		{
			CalDistanceOfParalleLine(m_pLineA1SliceBottom->pt1.ImgPt.Int(), m_pLineAxis1, m_nHImgSlicePos);
			CalDistanceOfParalleLine(m_pLineA2SliceBottom->pt1.ImgPt.Int(), m_pLineAxis2, m_nVImgSlicePos);
		}
	}
		
	return true;
}

void OperateMprLines::OnMprLinesMousePress(QMouseEvent *event)
{
	m_prePt = event->pos();

	if (m_pLineAxis1->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1LINE;
		return;
	}

	if (m_ptA1StartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1STARTROTATE;
		return;
	}

	if (m_ptA1EndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1ENDROTATE;
		return;
	}

	if (m_ptA1StartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1STARTROTATE;
		return;
	}

	if (m_ptA1EndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1ENDROTATE;
		return;
	}

	if (m_pLineA1SliceTop->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1TOPSLICE;
		return;
	}

	if (m_pLineA1SliceBottom->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A1BOTTOMSLICE;
		return;
	}

	if (m_pLineAxis2->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2LINE;
		return;
	}

	if (m_ptA2StartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2STARTROTATE;
		return;
	}

	if (m_ptA2EndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2ENDROTATE;
		return;
	}
	if (m_ptA2StartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2STARTROTATE;
		return;
	}

	if (m_ptA2EndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2ENDROTATE;
		return;
	}

	if (m_pLineA2SliceTop->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2TOPSLICE;
		return;
	}

	if (m_pLineA2SliceBottom->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = A2BOTTOMSLICE;
		return;
	}

	if (m_ptCenter->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = CENTERPOINT;
		return;
	}
}

bool OperateMprLines::OnMprLinesMouseMove(QMouseEvent *event)
{
	QPoint pt;
	pt = event->pos();

	
	if (m_bMousePressed == false)
	{
		bool bLinesUpdate = GetPointNearLines(pt);
		bool bCenterUpdate = GetPointNearPoint(pt);

		return (bLinesUpdate || bCenterUpdate);
	}
	else {
		QPoint ptDelta = pt - m_prePt;
		bool bChange = false;
		if (m_moveObject == A1LINE || m_moveObject == A2LINE)
		{
			bChange = MoveMainLines(ptDelta);
		}

		if (m_moveObject == A1TOPSLICE || m_moveObject == A1BOTTOMSLICE || m_moveObject == A2TOPSLICE || m_moveObject == A2BOTTOMSLICE)
		{
			bChange = MoveSliceLines(ptDelta);
		}

		if (m_moveObject == A1STARTROTATE || m_moveObject == A1ENDROTATE || m_moveObject == A2STARTROTATE || m_moveObject == A2ENDROTATE)
		{
			MoveRotateLine(pt);
		}

		if (m_moveObject == CENTERPOINT)
		{
			bChange = MoveCenterPt(pt);
		}

		AllCovertWndToImg();
		m_prePt = pt;
		if (bChange)
		{
			OutputLineInfo();
		}		
		return true;
	}
}

void OperateMprLines::OnMprLinesMouseRelease(QMouseEvent *event)
{
	m_bMousePressed = false;
}

void OperateMprLines::OnMprLinesPaint(QPainter *painter)
{
	if (m_bShow == false)
		return;
	painter->setRenderHint(QPainter::Antialiasing, true);
	if (1)//横向
	{
		//主线
		m_clrLineAxis1.setAlpha(125);
		if (m_pLineAxis1->bActive == true)
			m_clrLineAxis1.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis1, 2, Qt::SolidLine));
		QPoint ptTmp;
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineAxis1->pt1.WndPt)) < 10)
			ptTmp = m_pLineAxis1->pt1.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineAxis1->pt1.WndPt, 10, ptTmp);
		painter->drawLine(m_pLineAxis1->pt1.WndPt,ptTmp);
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineAxis1->pt2.WndPt)) < 10)
			ptTmp = m_pLineAxis1->pt2.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineAxis1->pt2.WndPt, 10, ptTmp);
		painter->drawLine(ptTmp, m_pLineAxis1->pt2.WndPt);

		//连接处圆形
		m_clrLineAxis1.setAlpha(125);
		if (m_ptA1StartInter->bActive == true)
			m_clrLineAxis1.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis1, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis1);
		painter->drawEllipse(m_ptA1StartInter->WndPt, 3, 3);
		m_clrLineAxis1.setAlpha(125);
		if (m_ptA1EndInter->bActive == true)
			m_clrLineAxis1.setAlpha(255);
		painter->setBrush(m_clrLineAxis1);
		painter->drawEllipse(m_ptA1EndInter->WndPt, 3, 3);

		//旋转圆
		m_clrLineAxis1.setAlpha(200);
		if (m_ptA1StartRotate->bActive == true)
			m_clrLineAxis1.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis1, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis1);
		painter->drawEllipse(m_ptA1StartRotate->WndPt, 5, 5);

		m_clrLineAxis1.setAlpha(200);
		if (m_ptA1EndRotate->bActive == true)
			m_clrLineAxis1.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis1, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis1);
		painter->drawEllipse(m_ptA1EndRotate->WndPt, 5, 5);

		//层厚线
		if (m_nHWndSlicePos != 0.00)
		{
			m_clrLineAxis1.setAlpha(125);
			painter->setPen(QPen(m_clrLineAxis1, 1, Qt::DashLine));
			painter->drawLine(m_ptA1StartInter->WndPt, m_pLineA1SliceTop->pt1.WndPt);
			painter->drawLine(m_pLineA1SliceTop->pt2.WndPt, m_ptA1EndInter->WndPt);
			if (m_pLineA1SliceTop->bActive == true)
				m_clrLineAxis1.setAlpha(255);
			painter->setPen(QPen(m_clrLineAxis1, 2, Qt::DashLine));			
			painter->drawLine(m_pLineA1SliceTop->pt1.WndPt, m_pLineA1SliceTop->pt2.WndPt);
			
			m_clrLineAxis1.setAlpha(125);
			painter->setPen(QPen(m_clrLineAxis1, 1, Qt::DashLine));
			painter->drawLine(m_ptA1StartInter->WndPt, m_pLineA1SliceBottom->pt1.WndPt);
			painter->drawLine(m_pLineA1SliceBottom->pt2.WndPt, m_ptA1EndInter->WndPt);
			if (m_pLineA1SliceBottom->bActive == true)
				m_clrLineAxis1.setAlpha(255);
			painter->setPen(QPen(m_clrLineAxis1, 2, Qt::DashLine));
			painter->drawLine(m_pLineA1SliceBottom->pt1.WndPt, m_pLineA1SliceBottom->pt2.WndPt);
		}	
	}

	if (2)//纵向
	{
		//主线
		m_clrLineAxis2.setAlpha(125);
		if (m_pLineAxis2->bActive)
			m_clrLineAxis2.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis2, 2, Qt::SolidLine));
		QPoint ptTmp;
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineAxis2->pt1.WndPt)) < 10)
			ptTmp = m_pLineAxis2->pt1.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineAxis2->pt1.WndPt, 10, ptTmp);
		painter->drawLine(m_pLineAxis2->pt1.WndPt, ptTmp);
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineAxis2->pt2.WndPt)) < 10)
			ptTmp = m_pLineAxis2->pt2.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineAxis2->pt2.WndPt, 10, ptTmp);		
		painter->drawLine(ptTmp,m_pLineAxis2->pt2.WndPt);

		//连接处圆形
		m_clrLineAxis2.setAlpha(125);
		if (m_ptA2StartInter->bActive)
			m_clrLineAxis2.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis2, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis2);	
		painter->drawEllipse(m_ptA2StartInter->WndPt, 3, 3);
		m_clrLineAxis2.setAlpha(125);
		if (m_ptA2EndInter->bActive)
			m_clrLineAxis2.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis2, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis2);
		painter->drawEllipse(m_ptA2EndInter->WndPt, 3, 3);

		//旋转圆
		m_clrLineAxis2.setAlpha(200);
		if (m_ptA2StartRotate->bActive)
			m_clrLineAxis2.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis2, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis2);
		painter->drawEllipse(m_ptA2StartRotate->WndPt, 5, 5);

		m_clrLineAxis2.setAlpha(200);
		if (m_ptA2EndRotate->bActive == true)
			m_clrLineAxis2.setAlpha(255);
		painter->setPen(QPen(m_clrLineAxis2, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineAxis2);
		painter->drawEllipse(m_ptA2EndRotate->WndPt, 5, 5);

		//层厚线
		if (m_nVWndSlicePos != 0.00)
		{
			m_clrLineAxis2.setAlpha(125);
			painter->setPen(QPen(m_clrLineAxis2, 1, Qt::DashLine));
			painter->drawLine(m_ptA2StartInter->WndPt, m_pLineA2SliceTop->pt1.WndPt);
			painter->drawLine(m_pLineA2SliceTop->pt2.WndPt, m_ptA2EndInter->WndPt);
			if (m_pLineA2SliceTop->bActive == true)
				m_clrLineAxis2.setAlpha(255);
			painter->setPen(QPen(m_clrLineAxis2, 2, Qt::DashLine));
			painter->drawLine(m_pLineA2SliceTop->pt1.WndPt, m_pLineA2SliceTop->pt2.WndPt);

			m_clrLineAxis2.setAlpha(125);
			painter->setPen(QPen(m_clrLineAxis2, 1, Qt::DashLine));
			painter->drawLine(m_ptA2StartInter->WndPt, m_pLineA2SliceBottom->pt1.WndPt);
			painter->drawLine(m_pLineA2SliceBottom->pt2.WndPt, m_ptA2EndInter->WndPt);
			if (m_pLineA2SliceBottom->bActive == true)
				m_clrLineAxis2.setAlpha(255);
			painter->setPen(QPen(m_clrLineAxis2, 2, Qt::DashLine));
			painter->drawLine(m_pLineA2SliceBottom->pt1.WndPt, m_pLineA2SliceBottom->pt2.WndPt);
		}
	}

	if (3)
	{
		m_clrCenter.setAlpha(125);
		if (m_ptCenter->bActive)
			m_clrCenter.setAlpha(255);
		painter->setPen(QPen(m_clrCenter, 2, Qt::SolidLine));
		painter->setBrush(Qt::NoBrush);
		painter->drawEllipse(m_ptCenter->WndPt, 2, 2);
	}
}

void OperateMprLines::OutputLineInfo()
{
	MprLinesInfo info;
	if (m_moveObject == A1LINE || m_moveObject == A2LINE || m_moveObject == CENTERPOINT)
	{
		info.moveObject = CENTERPOINT;
	}
	else if (m_moveObject == A1STARTROTATE || m_moveObject == A1ENDROTATE || m_moveObject == A2STARTROTATE || m_moveObject == A2ENDROTATE)
	{
		info.moveObject = A1STARTROTATE;
	}

	info.nWndType = m_nWndType;

	emit MprLinesInfoChange(info);
}

void OperateMprLines::OnCenterPointChanged()
{
	Hmy3DVector vecDelta = m_pResliceControl->GetCenter().Sub(m_pResliceControl->GetPreCenter());

	int nPlaneAxis1Id = GetPlaneAxis1();
	int nPlaneAxis2Id = GetPlaneAxis2();

	Hmy3DVector vecA1 = m_pResliceControl->GetAxis(nPlaneAxis1Id);
	Hmy3DVector vecA2 = m_pResliceControl->GetAxis(nPlaneAxis2Id);
	Hmy3DVector thisNormal = m_pResliceControl->GetPlane(m_nWndType)->normal();

	double dDeltaX = 0.00, dDeltaY = 0.00;
	double xyX = vecA2.x()*vecA1.y() - vecA1.x()*vecA2.y();
	double xzX = vecA2.x()*vecA1.z() - vecA1.x()*vecA2.z();
	double yzX = vecA2.y()*vecA1.z() - vecA1.y()*vecA2.z();
	vector<double> vecDeltaX;
	if (abs(xyX - 0.0000) > 0.00001)
	{
		double delta = (vecDelta.y()*vecA2.x() - vecA2.y()*vecDelta.x()) / xyX;
		vecDeltaX.push_back(delta);
	}
	if (abs(xzX - 0.0000) > 0.00001)
	{
		double delta = (vecDelta.z()*vecA2.x() - vecA2.z()*vecDelta.x()) / xzX;
		vecDeltaX.push_back(delta);
	}
	if(abs(yzX - 0.0000) > 0.00001)
	{
		double delta = (vecDelta.z()*vecA2.y() - vecA2.z()*vecDelta.y()) / yzX;
		vecDeltaX.push_back(delta);
	}
	sort(vecDeltaX.begin(), vecDeltaX.end());
	dDeltaX = vecDeltaX[vecDeltaX.size() - 1];

	double xyY= vecA1.x()*vecA2.y() - vecA2.x()*vecA1.y();
	double xzY = vecA1.x()*vecA2.z() - vecA2.x()*vecA1.z();
	double yzY = vecA1.y()*vecA2.z() - vecA2.y()*vecA1.z();
	vector<double> vecDeltaY;
	if (abs(xyY - 0.0000) > 0.00001)
	{
		double delta = (vecDelta.y()*vecA1.x() - vecA1.y()*vecDelta.x()) / xyY;
		vecDeltaY.push_back(delta);
	}
	if (abs(xzY - 0.0000) > 0.00001)
	{
		double delta = (vecDelta.z()*vecA1.x() - vecA1.z()*vecDelta.x()) / xzY;
		vecDeltaY.push_back(delta);
	}
	if (abs(yzY - 0.0000) > 0.00001)
	{
		double delta = (vecDelta.z()*vecA1.y() - vecA1.z()*vecDelta.y()) / yzY;
		vecDeltaY.push_back(delta);
	}
	sort(vecDeltaY.begin(), vecDeltaY.end());
	dDeltaY = vecDeltaY[vecDeltaY.size() - 1];

	double deltaX = dDeltaX / m_dSpacingX;
	double deltaY = dDeltaY / m_dSpacingY;

	m_ptCenter->ImgPt.x += deltaX;
	m_ptCenter->ImgPt.y += deltaY;

	ConvertImgToWnd(*m_ptCenter);

	m_ptPreCenter = *m_ptCenter;

	if (1)//H
	{
		GetParalleLine(m_pLineAxis1, m_ptCenter, m_pLineAxis1->pt1, m_pLineAxis1->pt2);
		ReCalInterPoint(m_pLineAxis1, m_ptA1StartInter, m_ptA1EndInter);
		UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	}

	if (2)//V
	{
		GetParalleLine(m_pLineAxis2, m_ptCenter, m_pLineAxis2->pt1, m_pLineAxis2->pt2);
		ReCalInterPoint(m_pLineAxis2, m_ptA2StartInter, m_ptA2EndInter);
		UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);
	}
	AllCovertWndToImg();
}

void OperateMprLines::SetManiLinePos(MoveObject object, MprLine *pLine)
{
	ConvertImgToWnd(pLine->pt1);
	ConvertImgToWnd(pLine->pt2);
	QPoint ptInter;
	if (object == A1LINE)
	{
		m_pLineAxis1->pt1 = pLine->pt1;
		m_pLineAxis1->pt2 = pLine->pt2;	
		GetInterPoint(m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt, m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt, ptInter);
		ReCalInterPoint(m_pLineAxis1, m_ptA1StartInter, m_ptA1EndInter);
		UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);	
	}

	if (object == A2LINE)
	{
		m_pLineAxis2->pt1 = pLine->pt1;
		m_pLineAxis2->pt2 = pLine->pt2;
		GetInterPoint(m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt, m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt, ptInter);
		ReCalInterPoint(m_pLineAxis2, m_ptA2StartInter, m_ptA2EndInter);
		UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);
	}

	CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);

	m_ptCenter->WndPt = ptInter;
	AllCovertWndToImg();
}

void OperateMprLines::SetSliceLinePos(double nSliceThick, int nIndex)
{
	if (nIndex == 0)
	{
		m_nHWndSlicePos = qRound(nSliceThick); // m_fSpacingX
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	}
	else
	{
		m_nVWndSlicePos = qRound(nSliceThick );// m_fSpacingX
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);
	}
	AllCovertWndToImg();
}

void OperateMprLines::UpdateRotatePoint(MprLine *pMainLine, MprPoint *ptStartRotate, MprPoint *ptEndRotate)
{
	*ptStartRotate = pMainLine->pt1;
	*ptEndRotate = pMainLine->pt2;
}

void OperateMprLines::GetLinesColor()
{
	if (m_nWndType == ORIIMG_AXIAL)
	{
		m_clrLineAxis1 = QColor(124, 252, 0, 125);
		m_clrLineAxis2 = QColor(255, 0, 0, 125);
		m_clrCenter = QColor(0, 191, 255, 125);
	}
	else if(m_nWndType == ORIIMG_CORONAL)
	{
		m_clrLineAxis1 = QColor(0, 191, 255, 125);
		m_clrLineAxis2 = QColor(255, 0, 0, 125);
		m_clrCenter = QColor(124, 252, 0, 125);
	}
	else if (m_nWndType == ORIIMG_SAGITTAL)
	{
		m_clrLineAxis1 = QColor(0, 191, 255, 125);
		m_clrLineAxis2 = QColor(124, 252, 0, 125);
		m_clrCenter = QColor(255, 0, 0, 125);
	}
}

bool OperateMprLines::ConvertImgToWnd(MprPoint &pt)
{
	if (m_nImgW == 0 || m_nImgH == 0)
		return false;

	double fx = (m_rcImgOnWnd.right - m_rcImgOnWnd.left)*1.00 / m_nImgW;
	pt.WndPt.setX(qRound(fx * pt.ImgPt.x + m_rcImgOnWnd.left));

	double fy = (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top)*1.00 / m_nImgH;
	pt.WndPt.setY(qRound(fy * pt.ImgPt.y + m_rcImgOnWnd.top));

	return true;
}

bool OperateMprLines::ConvertWndToImg(MprPoint & pt)
{
	if (m_rcImgOnWnd.left == m_rcImgOnWnd.right || m_rcImgOnWnd.top == m_rcImgOnWnd.bottom)
		return false;

	double fx = m_nImgW*1.00 / (m_rcImgOnWnd.right - m_rcImgOnWnd.left);
	pt.ImgPt.x = fx * (pt.WndPt.x() - m_rcImgOnWnd.left);

	double fy = m_nImgH*1.00 / (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top);
	pt.ImgPt.y = fy * (pt.WndPt.y() - m_rcImgOnWnd.top);

	return true;
}

bool OperateMprLines::GetVerticalLine(QPoint &p1, QPoint &p2, QPoint &PublicPt, double nLen, QPoint &newPt1, QPoint &newPt2)
{
	int x1 = p1.x();
	int y1 = p1.y();
	int x2 = p2.x();
	int y2 = p2.y();

	if (x1 != x2 && y1 != y2)
	{
		//新斜率=原斜率的负倒数(-1/k)
		double k = -(x2 - x1)*1.00 / (y2 - y1);

		//y = kx + b;代入PublicPt求b:
		double b = PublicPt.y() - k*PublicPt.x();

		//两点间距离公式 ：nLen = [ sqrt(1 + k*k) ]*abs(x2 - x1)

		double fx1 = nLen*1.00 /sqrt(1 + k*k) + PublicPt.x();
		newPt1.setX(qRound(fx1));

		double fx2 = PublicPt.x() - nLen*1.00 / sqrt(1 + k*k);
		newPt2.setX(qRound(fx2));

		newPt1.setY(qRound(k * fx1 + b));
		newPt2.setY(qRound(k * fx2 + b));

		return true;
	}

	if (x1 == x2)//|
	{
		newPt1.setX(PublicPt.x() - nLen);
		newPt1.setY(PublicPt.y());

		newPt2.setX(PublicPt.x() + nLen);
		newPt2.setY(PublicPt.y());

		return true;
	}

	if (y1 == y2)//-
	{
		newPt1.setX(PublicPt.x());
		newPt1.setY(PublicPt.y()- nLen);

		newPt2.setX(PublicPt.x() );
		newPt2.setY(PublicPt.y()+ nLen);

		return true;
	}

	return false;
}

bool OperateMprLines::GetInterPoint(FPOINT pA0, FPOINT pA1, FPOINT pB0, FPOINT pB1, FPOINT &retPt)
{
	double x0 = pA0.x;	 double y0 = pA0.y;
	double x1 = pA1.x;	 double y1 = pA1.y;
	double x2 = pB0.x;	 double y2 = pB0.y;
	double x3 = pB1.x;	 double y3 = pB1.y;

	//直线A
	//已知x求y: y = (x - x0) * (y1 - y0) / (x1 - x0) + y0;
	//已知y求x: x = (x1 - x0) * (y - y0) / (y1 - y0) + x0;

	//直线B
	//已知x求y: y = (x - x2) * (y3 - y2) / (x3 - x2) + y2;
	//已知y求x: x = (x3 - x2) * (y - y2) / (y3 - y2) + x2;

	if (abs(x0 - x1) < 0.000001)//A垂直
	{
		if (abs(y2 - y3) < 0.000001)//B水平
		{
			retPt.x = x0;
			retPt.y = y2;
			return true;
		}
		else if (abs(x2 - x3) < 0.000001)//B垂直
		{
			return false;
		}
		else//B斜
		{
			retPt.x = x0;
			retPt.y = ((retPt.x - x2) * (y3 - y2) *1.00 / (x3 - x2) + y2);
			return true;
		}
	}
	else if (abs(y0 - y1) < 0.000001)//A水平
	{
		if (abs(y2 - y3) < 0.0000001)//B水平
		{
			return false;
		}
		else if (abs(x2 - x3) < 0.0000001)//B垂直
		{
			retPt.x = x2;
			retPt.y = y0;
			return true;
		}
		else//B斜
		{
			retPt.y = y0;
			retPt.x = ((retPt.y - y3)*(x2 - x3)*1.00 / (y2 - y3) + x3);//(long)(x3 - x2) * (retPt.y - y2) *1.00 / (y3 - y2) + x2;
			return true;
		}
	}
	else//A斜
	{
		if (abs(y2 - y3) < 0.0000001)//B水平
		{
			retPt.y = y2;
			retPt.x = ((x1 - x0) * (retPt.y - y0) *1.00 / (y1 - y0) + x0);
			return true;
		}
		else if (abs(x2 - x3) < 0.000001)//B垂直
		{
			retPt.x = x2;
			retPt.y = ((retPt.x - x0) * (y1 - y0) *1.00 / (x1 - x0) + y0);
		}
		else//B斜
		{
			double kA = (y1 - y0)*1.00 / (x1 - x0);
			double kB = (y3 - y2)*1.00 / (x3 - x2);
			if (abs(kA - kB) < 0.00001)// 平行了
				return false;

			double y = ((y0 - y1)*(y3 - y2)*x0 + (y3 - y2)*(x1 - x0)*y0 + (y1 - y0)*(y3 - y2)*x2 + (x2 - x3)*(y1 - y0)*y2)*1.00 / ((x1 - x0)*(y3 - y2) + (y0 - y1)*(x3 - x2));
			double x = x2 + (x3 - x2)*(y - y2)*1.00 / (y3 - y2);

			retPt.x = x;
			retPt.y = y;
			return true;
		}
	}

	return true;
}


bool OperateMprLines::GetInterPoint(QPoint pA0, QPoint pA1, QPoint pB0, QPoint pB1, QPoint &retPt)
{
	FPOINT fA0; fA0.x = pA0.x()*1.00; fA0.y = pA0.y()*1.00;
	FPOINT fA1; fA1.x = pA1.x()*1.00; fA1.y = pA1.y()*1.00;
	FPOINT fB0; fB0.x = pB0.x()*1.00; fB0.y = pB0.y()*1.00;
	FPOINT fB1; fB1.x = pB1.x()*1.00; fB1.y = pB1.y()*1.00;
	FPOINT rPt;
	bool b = GetInterPoint(fA0, fA1, fB0, fB1, rPt);
	if (!b)
		return false;
	retPt = rPt.Int();
	return true;	
}

bool OperateMprLines::GetPointNearLines(QPoint pt)
{
	bool bUpdate = false;
	if (1)//横向几个成员判断
	{
		bool bNear = DistanceToLines(m_pLineAxis1, pt);
		if ((bNear && !m_pLineAxis1->bActive) || (!bNear && m_pLineAxis1->bActive) )
		{
			m_pLineAxis1->bActive = !m_pLineAxis1->bActive;
			bUpdate = true;
		}	
	}

	if (2)
	{
		bool bNear = DistanceToLines(m_pLineAxis2, pt);
		if ((bNear && !m_pLineAxis2->bActive) || (!bNear && m_pLineAxis2->bActive))
		{
			m_pLineAxis2->bActive = !m_pLineAxis2->bActive;
			bUpdate = true;
		}
	}

	if (3 && m_nHWndSlicePos !=0)
	{
		m_ptA1EndInter->bActive = false;
		m_ptA1StartInter->bActive = false;
		bool bNear = DistanceToLines(m_pLineA1SliceTop, pt);
		if ((bNear && !m_pLineA1SliceTop->bActive) || (!bNear && m_pLineA1SliceTop->bActive))
		{
			m_pLineA1SliceTop->bActive = !m_pLineA1SliceTop->bActive;
			bUpdate = true;
		}

		bNear = DistanceToLines(m_pLineA1SliceBottom, pt);
		if ((bNear && !m_pLineA1SliceBottom->bActive) || (!bNear && m_pLineA1SliceBottom->bActive))
		{
			m_pLineA1SliceBottom->bActive = !m_pLineA1SliceBottom->bActive;
			bUpdate = true;
		}
	}

	if (4 && m_nVWndSlicePos != 0)
	{
		m_ptA2EndInter->bActive = false;
		m_ptA2StartInter->bActive = false;
		bool bNear = DistanceToLines(m_pLineA2SliceTop, pt);
		if ((bNear && !m_pLineA2SliceTop->bActive) || (!bNear && m_pLineA2SliceTop->bActive))
		{
			m_pLineA2SliceTop->bActive = !m_pLineA2SliceTop->bActive;
			bUpdate = true;
		}

		bNear = DistanceToLines(m_pLineA2SliceBottom, pt);
		if ((bNear && !m_pLineA2SliceBottom->bActive) || (!bNear && m_pLineA2SliceBottom->bActive))
		{
			m_pLineA2SliceBottom->bActive = !m_pLineA2SliceBottom->bActive;
			bUpdate = true;
		}
	}

	return bUpdate;
}

bool OperateMprLines::GetPointNearPoint(QPoint pt)
{
	bool bUpdate = false;

	if (1)//横向
	{
		bool bNear = DistanceToPoint(m_ptA1StartRotate, pt, 8);
		if ((bNear && !m_ptA1StartRotate->bActive) || (!bNear && m_ptA1StartRotate->bActive))
		{
			m_ptA1StartRotate->bActive = !m_ptA1StartRotate->bActive;
			bUpdate = true;
		}

		bNear = DistanceToPoint(m_ptA1EndRotate, pt, 8);
		if ((bNear && !m_ptA1EndRotate->bActive) || (!bNear && m_ptA1EndRotate->bActive))
		{
			m_ptA1EndRotate->bActive = !m_ptA1EndRotate->bActive;
			bUpdate = true;
		}

		if (m_ptA1StartRotate->bActive || m_ptA1EndRotate->bActive)
		{
			m_pLineAxis1->bActive = false;
		}
	}

	if (2)//纵向
	{
		bool bNear = DistanceToPoint(m_ptA2StartRotate, pt, 8);
		if ((bNear && !m_ptA2StartRotate->bActive) || (!bNear && m_ptA2StartRotate->bActive))
		{
			m_ptA2StartRotate->bActive = !m_ptA2StartRotate->bActive;
			bUpdate = true;
		}

		bNear = DistanceToPoint(m_ptA2EndRotate, pt, 8);
		if ((bNear && !m_ptA2EndRotate->bActive) || (!bNear && m_ptA2EndRotate->bActive))
		{
			m_ptA2EndRotate->bActive = !m_ptA2EndRotate->bActive;
			bUpdate = true;
		}

		if (m_ptA2StartRotate->bActive || m_ptA2EndRotate->bActive)
		{
			m_pLineAxis2->bActive = false;
		}
	}

	if (3) //中心点
	{

		bool bNear = DistanceToPoint(m_ptCenter, pt, 8);
		if ((bNear && !m_ptCenter->bActive) || (!bNear && m_ptCenter->bActive))
		{
			m_ptCenter->bActive = !m_ptCenter->bActive;
			bUpdate = true;
		}

		if (m_ptCenter->bActive)
		{
			m_pLineAxis2->bActive = false;
			m_pLineAxis1->bActive = false;
		}
	}

	if (4 && m_nHWndSlicePos == 0)
	{
		bool bSNear = DistanceToPoint(m_ptA1StartInter, pt, 4);
		bool bENear = DistanceToPoint(m_ptA1EndInter, pt, 4);
		m_ptA1StartInter->bActive = bSNear;
		m_ptA1EndInter->bActive = bENear;
		bool bNear = (bSNear || bENear);
		if ((bNear && !m_pLineA1SliceBottom->bActive) || (!bNear && m_pLineA1SliceBottom->bActive))
		{
			m_pLineA1SliceBottom->bActive = !m_pLineA1SliceBottom->bActive;
			bUpdate = true;
		}
		
		if (m_pLineA1SliceBottom->bActive)
		{
			m_pLineAxis1->bActive = false;
		}
	}

	if (5 && m_nVWndSlicePos == 0)
	{
		bool bSNear = DistanceToPoint(m_ptA2StartInter, pt, 4);
		bool bENear = DistanceToPoint(m_ptA2EndInter, pt, 4);
		m_ptA2StartInter->bActive = bSNear;
		m_ptA2EndInter->bActive = bENear;
		bool bNear = (bSNear || bENear);
		if ((bNear && !m_pLineA2SliceTop->bActive) || (!bNear && m_pLineA2SliceTop->bActive))
		{
			m_pLineA2SliceTop->bActive = !m_pLineA2SliceTop->bActive;
			bUpdate = true;
		}

		if (m_pLineA2SliceTop->bActive)
		{
			m_pLineAxis2->bActive = false;
		}
	}

	return bUpdate;
}

bool OperateMprLines::DistanceToLines(MprLine* pLine, QPoint pt)
{
	float fLineLength = Dist2(pLine->pt1.WndPt,pLine->pt2.WndPt);

	if (fLineLength < 0.0001)
	{
		float dist = sqrt(Dist2(pt, pLine->pt1.WndPt));
		return (dist < m_nNearFactor);
	}
	
	float t = ((pt.x() - pLine->pt1.WndPt.x()) * (pLine->pt2.WndPt.x() - pLine->pt1.WndPt.x()) +
		(pt.y() - pLine->pt1.WndPt.y()) * (pLine->pt2.WndPt.y() - pLine->pt1.WndPt.y())) / fLineLength;

	if (t < 0.00)
	{
		float dist = sqrt(Dist2(pt, pLine->pt1.WndPt));
		return (dist < m_nNearFactor);
	}
	
	if (t > 1.00)
	{
		float dist = sqrt(Dist2(pt, pLine->pt1.WndPt));
		return (dist < m_nNearFactor);
	}

	QPoint ptTemp;
	ptTemp.setX(pLine->pt1.WndPt.x() + t * (pLine->pt2.WndPt.x() - pLine->pt1.WndPt.x()));
	ptTemp.setY(pLine->pt1.WndPt.y() + t * (pLine->pt2.WndPt.y() - pLine->pt1.WndPt.y()));

	return (sqrt(Dist2(pt, ptTemp)) < m_nNearFactor);
}

bool OperateMprLines::DistanceToPoint(MprPoint *ptMpr, QPoint pt, int nDistance)
{
	MprPoint ptLT;
	ptLT.WndPt.setX(ptMpr->WndPt.x() - nDistance);
	ptLT.WndPt.setY(ptMpr->WndPt.y() - nDistance);

	MprPoint ptLB;
	ptLB.WndPt.setX(ptMpr->WndPt.x() - nDistance);
	ptLB.WndPt.setY(ptMpr->WndPt.y() + nDistance);

	MprPoint ptRT;
	ptRT.WndPt.setX(ptMpr->WndPt.x() + nDistance);
	ptRT.WndPt.setY(ptMpr->WndPt.y() - nDistance);

	MprPoint ptRB;
	ptRB.WndPt.setX(ptMpr->WndPt.x() + nDistance);
	ptRB.WndPt.setY(ptMpr->WndPt.y() + nDistance);

	MprLine *lineLeft = new MprLine;
	lineLeft->pt1 = ptLT;
	lineLeft->pt2 = ptLB;

	MprLine *lineBottom = new MprLine;
	lineBottom->pt1 = ptLB;
	lineBottom->pt2 = ptRB;

	MprLine *lineRight = new MprLine;
	lineRight->pt1 = ptRB;
	lineRight->pt2 = ptRT;

	MprLine *lineTop = new MprLine;
	lineTop->pt1 = ptRT;
	lineTop->pt2 = ptLT;

	bool bNearRect = (DistanceToLines(lineLeft, pt) || DistanceToLines(lineRight, pt) || DistanceToLines(lineTop, pt) || DistanceToLines(lineBottom, pt));

	delete lineLeft;
	delete lineBottom;
	delete lineRight;
	delete lineTop;

	bool bInRect = false;
	if (pt.x() > ptLT.WndPt.x() && pt.x() < ptRT.WndPt.x() && pt.y() > ptLT.WndPt.y() && pt.y() < ptLB.WndPt.y())
		bInRect = true;

	return (bNearRect || bInRect);
}

float OperateMprLines::Dist2(QPoint pt1, QPoint pt2)
{
	return (pt1.x() - pt2.x())*(pt1.x() - pt2.x()) + (pt1.y() - pt2.y())*(pt1.y() - pt2.y());
}

bool OperateMprLines::MoveMainLines(QPoint deltaPt)
{
	if (m_moveObject == A1LINE)
	{
		MprLine moveLine;
		moveLine.pt1.WndPt.setX(m_pLineAxis1->pt1.WndPt.x());
		moveLine.pt1.WndPt.setY(m_pLineAxis1->pt1.WndPt.y());
		moveLine.pt2.WndPt.setX(m_pLineAxis1->pt2.WndPt.x());
		moveLine.pt2.WndPt.setY(m_pLineAxis1->pt2.WndPt.y());
		QPoint ptInter;
		if (m_pLineAxis1->pt1.WndPt.y() == m_pLineAxis1->pt2.WndPt.y())
		{
			moveLine.pt1.WndPt.setY(m_pLineAxis1->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setY(m_pLineAxis1->pt2.WndPt.y() + deltaPt.y());
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt, ptInter);
			if (!IsPointInRc(m_rcImgOnWnd, ptInter))
				return false;
			m_pLineAxis1->pt1.WndPt.setY(moveLine.pt1.WndPt.y());
			m_pLineAxis1->pt2.WndPt.setY(moveLine.pt2.WndPt.y());
		}
		else if (m_pLineAxis1->pt1.WndPt.x() == m_pLineAxis1->pt2.WndPt.x())
		{
			moveLine.pt1.WndPt.setX(m_pLineAxis1->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setX(m_pLineAxis1->pt2.WndPt.x() + deltaPt.x());
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt, ptInter);
			if (!IsPointInRc(m_rcImgOnWnd, ptInter))
				return false;
			m_pLineAxis1->pt1.WndPt.setX(moveLine.pt1.WndPt.x());
			m_pLineAxis1->pt2.WndPt.setX(moveLine.pt2.WndPt.x());
		}
		else 
		{
			moveLine.pt1.WndPt.setX(m_pLineAxis1->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt1.WndPt.setY(m_pLineAxis1->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setX(m_pLineAxis1->pt2.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setY(m_pLineAxis1->pt2.WndPt.y() + deltaPt.y());
			MprLine *pNewLineA1 = new MprLine;
			GetLineAcrossRc(m_rcImgOnWnd, moveLine.pt1.WndPt, moveLine.pt2.WndPt, pNewLineA1->pt1.WndPt, pNewLineA1->pt2.WndPt);
			GetInterPoint(pNewLineA1->pt1.WndPt, pNewLineA1->pt2.WndPt, m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt, ptInter);
			if (!IsPointInRc(m_rcImgOnWnd, ptInter))
			{
				delete pNewLineA1;
				return false;
			}
			*m_pLineAxis1 = *pNewLineA1;
			delete pNewLineA1;
		}	
		ReCalInterPoint(m_pLineAxis1, m_ptA1StartInter, m_ptA1EndInter);
		UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);
		m_ptCenter->WndPt = ptInter;
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	}
	else if (m_moveObject == A2LINE)
	{
		MprLine moveLine;
		moveLine.pt1.WndPt.setX(m_pLineAxis2->pt1.WndPt.x());
		moveLine.pt1.WndPt.setY(m_pLineAxis2->pt1.WndPt.y());
		moveLine.pt2.WndPt.setX(m_pLineAxis2->pt2.WndPt.x());
		moveLine.pt2.WndPt.setY(m_pLineAxis2->pt2.WndPt.y());
		QPoint ptInter;
		if (m_pLineAxis2->pt1.WndPt.y() == m_pLineAxis2->pt2.WndPt.y())
		{
			moveLine.pt1.WndPt.setY(m_pLineAxis2->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setY(m_pLineAxis2->pt2.WndPt.y() + deltaPt.y());			
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return false;
			m_pLineAxis2->pt1.WndPt.setY(m_pLineAxis2->pt1.WndPt.y() + deltaPt.y());
			m_pLineAxis2->pt2.WndPt.setY(m_pLineAxis2->pt2.WndPt.y() + deltaPt.y());
		}
		else if (m_pLineAxis2->pt1.WndPt.x() == m_pLineAxis2->pt2.WndPt.x())
		{
			moveLine.pt1.WndPt.setX(m_pLineAxis2->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setX(m_pLineAxis2->pt2.WndPt.x() + deltaPt.x());
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return false;
			m_pLineAxis2->pt1.WndPt.setX(m_pLineAxis2->pt1.WndPt.x() + deltaPt.x());
			m_pLineAxis2->pt2.WndPt.setX(m_pLineAxis2->pt2.WndPt.x() + deltaPt.x());
		}
		else
		{
			moveLine.pt1.WndPt.setX(m_pLineAxis2->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt1.WndPt.setY(m_pLineAxis2->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setX(m_pLineAxis2->pt2.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setY(m_pLineAxis2->pt2.WndPt.y() + deltaPt.y());
			MprLine *pNewLineV = new MprLine;
			GetLineAcrossRc(m_rcImgOnWnd, moveLine.pt1.WndPt, moveLine.pt2.WndPt, pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt);
			GetInterPoint(pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt, m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
			{
				delete pNewLineV;
				return false;
			}
			*m_pLineAxis2 = *pNewLineV;
			delete pNewLineV;
		}
		ReCalInterPoint(m_pLineAxis2, m_ptA2StartInter, m_ptA2EndInter);
		UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);
		m_ptCenter->WndPt = ptInter;
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);
	}
	MoveResliceControlCenter();
	return true;
}

bool OperateMprLines::MoveCenterPt(QPoint pt)
{
	m_ptCenter->WndPt.setX(pt.x());
	m_ptCenter->WndPt.setY(pt.y());

	m_ptCenter->WndPt.setX(max(m_ptCenter->WndPt.x(),int(m_rcImgOnWnd.left)));
	m_ptCenter->WndPt.setX(min(m_ptCenter->WndPt.x(),int(m_rcImgOnWnd.right)));
	m_ptCenter->WndPt.setY(max(m_ptCenter->WndPt.y(), int(m_rcImgOnWnd.top)));
	m_ptCenter->WndPt.setY(min(m_ptCenter->WndPt.y(), int(m_rcImgOnWnd.bottom)));

	if (1)//H
	{
		GetParalleLine(m_pLineAxis1, m_ptCenter, m_pLineAxis1->pt1, m_pLineAxis1->pt2);
		ReCalInterPoint(m_pLineAxis1, m_ptA1StartInter, m_ptA1EndInter);
		UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	}

	if (2)//V
	{
		GetParalleLine(m_pLineAxis2, m_ptCenter, m_pLineAxis2->pt1, m_pLineAxis2->pt2);
		ReCalInterPoint(m_pLineAxis2, m_ptA2StartInter, m_ptA2EndInter);
		UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);
	}

	MoveResliceControlCenter();
	return true;
}

void OperateMprLines::GetParalleLine(MprLine *pMainLine, MprPoint *pt, MprPoint &pParalleLinePt1, MprPoint &pParalleLinePt2)
{
	if (pMainLine->pt1.WndPt.y() == pMainLine->pt2.WndPt.y())
	{
		pParalleLinePt1.WndPt.setX(pMainLine->pt1.WndPt.x());
		pParalleLinePt1.WndPt.setY(pt->WndPt.y());
		pParalleLinePt2.WndPt.setX(pMainLine->pt2.WndPt.x());
		pParalleLinePt2.WndPt.setY(pt->WndPt.y());
	}
	else if (pMainLine->pt1.WndPt.x() == pMainLine->pt2.WndPt.x())
	{
		pParalleLinePt1.WndPt.setX(pt->WndPt.x());
		pParalleLinePt1.WndPt.setY(pMainLine->pt1.WndPt.y());
		pParalleLinePt2.WndPt.setX(pt->WndPt.x());
		pParalleLinePt2.WndPt.setY(pMainLine->pt2.WndPt.y());
	}
	else
	{
		double x1 = pMainLine->pt1.WndPt.x() * 1.00;
		double y1 = pMainLine->pt1.WndPt.y() * 1.00;
		double x2 = pMainLine->pt2.WndPt.x() * 1.00;
		double y2 = pMainLine->pt2.WndPt.y() * 1.00;

		double k = (y2 - y1) / (x2 - x1);
		double b = pt->WndPt.y() - k * pt->WndPt.x();

		QPoint pt1; 
		pt1.setX(pt->WndPt.x()+100);
		pt1.setY(qRound(k*pt1.x() + b));

		GetLineAcrossRc(m_rcImgOnWnd, pt->WndPt, pt1, pParalleLinePt1.WndPt, pParalleLinePt2.WndPt);
	}
}

bool OperateMprLines::MoveSliceLines(QPoint deltaPt)
{
	if (m_moveObject == A1TOPSLICE)
	{
		if (m_pLineAxis1->pt1.WndPt.y() == m_pLineAxis1->pt2.WndPt.y())
		{
			m_nHWndSlicePos -= deltaPt.y();
		}
		else if (m_pLineAxis1->pt1.WndPt.x() == m_pLineAxis1->pt2.WndPt.x())
		{
			m_nHWndSlicePos += deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineA1SliceTop->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineAxis1, m_nHWndSlicePos);
		}
		m_nHWndSlicePos = m_nHWndSlicePos < 2 ? 0:m_nHWndSlicePos;
		m_nHWndSlicePos = min((LONG)m_nHWndSlicePos, (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2);
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	}
	else if (m_moveObject == A1BOTTOMSLICE)
	{
		if (m_pLineAxis1->pt1.WndPt.y() == m_pLineAxis1->pt2.WndPt.y())
		{
			m_nHWndSlicePos += deltaPt.y();
		}
		else if (m_pLineAxis1->pt1.WndPt.x() == m_pLineAxis1->pt2.WndPt.x())
		{
			m_nHWndSlicePos -= deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineA1SliceBottom->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineAxis1, m_nHWndSlicePos);
		}
		m_nHWndSlicePos = m_nHWndSlicePos < 2 ? 0 : m_nHWndSlicePos;
		m_nHWndSlicePos = min((LONG)m_nHWndSlicePos, (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2);
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);
	}
	else if (m_moveObject == A2TOPSLICE)
	{
		if (m_pLineAxis2->pt1.WndPt.y() == m_pLineAxis2->pt2.WndPt.y())
		{
			m_nVWndSlicePos -= deltaPt.y();
		}
		else if (m_pLineAxis2->pt1.WndPt.x() == m_pLineAxis2->pt2.WndPt.x())
		{
			m_nVWndSlicePos += deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineA2SliceTop->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineAxis2, m_nVWndSlicePos);
		}
		m_nVWndSlicePos = m_nVWndSlicePos < 2 ? 0 : m_nVWndSlicePos;
		m_nVWndSlicePos = min((LONG)m_nVWndSlicePos, (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2);
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);

	}
	else if(m_moveObject == A2BOTTOMSLICE)
	{
		if (m_pLineAxis2->pt1.WndPt.y() == m_pLineAxis2->pt2.WndPt.y())
		{
			m_nVWndSlicePos += deltaPt.y();
		}
		else if (m_pLineAxis2->pt1.WndPt.x() == m_pLineAxis2->pt2.WndPt.x())
		{
			m_nVWndSlicePos -= deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineA2SliceBottom->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());
			CalDistanceOfParalleLine(pt1, m_pLineAxis2, m_nVWndSlicePos);
		}
		m_nVWndSlicePos = m_nVWndSlicePos < 2 ? 0 : m_nVWndSlicePos;
		m_nVWndSlicePos = min((LONG)m_nVWndSlicePos, (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2);
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop,m_pLineA2SliceBottom);
	}
	return true;
}

void OperateMprLines::CalSliceLine(MprPoint *pStart, MprPoint *pEnd, MprLine *pTopSlice, MprLine *pBottomSlice)
{
	int nSlicePos = 0;
	if (pStart == m_ptA1StartInter)
		nSlicePos = m_nHWndSlicePos;
	else
		nSlicePos = m_nVWndSlicePos;

	if (pStart->WndPt.y() == pEnd->WndPt.y())
	{
		pTopSlice->pt1.WndPt.setX(pStart->WndPt.x());
		pTopSlice->pt1.WndPt.setY(pStart->WndPt.y() - nSlicePos);
		pTopSlice->pt2.WndPt.setX(pEnd->WndPt.x());
		pTopSlice->pt2.WndPt.setY(pEnd->WndPt.y() - nSlicePos);

		pBottomSlice->pt1.WndPt.setX(pStart->WndPt.x());
		pBottomSlice->pt1.WndPt.setY(pStart->WndPt.y() + nSlicePos);
		pBottomSlice->pt2.WndPt.setX(pEnd->WndPt.x());
		pBottomSlice->pt2.WndPt.setY(pEnd->WndPt.y() + nSlicePos);
	}
	else if (pStart->WndPt.x() == pEnd->WndPt.x())
	{
		pTopSlice->pt1.WndPt.setX(pStart->WndPt.x() + nSlicePos);
		pTopSlice->pt1.WndPt.setY(pStart->WndPt.y());
		pTopSlice->pt2.WndPt.setX(pEnd->WndPt.x() + nSlicePos);
		pTopSlice->pt2.WndPt.setY(pEnd->WndPt.y());

		pBottomSlice->pt1.WndPt.setX(pStart->WndPt.x() - nSlicePos);
		pBottomSlice->pt1.WndPt.setY(pStart->WndPt.y());
		pBottomSlice->pt2.WndPt.setX(pEnd->WndPt.x() - nSlicePos);
		pBottomSlice->pt2.WndPt.setY(pEnd->WndPt.y());
	}
	else
	{
		double k = (pEnd->WndPt.y() - pStart->WndPt.y()) * 1.00 / (pEnd->WndPt.x() - pStart->WndPt.x());
		if ((k > 0.00 && (pStart->WndPt.x() < pEnd->WndPt.x())) || (k < 0.00 && (pStart->WndPt.x() > pEnd->WndPt.x())))
		{
			GetVerticalLine(pStart->WndPt, pEnd->WndPt, pStart->WndPt, nSlicePos, pTopSlice->pt1.WndPt, pBottomSlice->pt1.WndPt);
			GetVerticalLine(pEnd->WndPt, pStart->WndPt, pEnd->WndPt, nSlicePos, pTopSlice->pt2.WndPt, pBottomSlice->pt2.WndPt);
		}
		else
		{
			GetVerticalLine(pStart->WndPt, pEnd->WndPt, pStart->WndPt, nSlicePos, pBottomSlice->pt1.WndPt, pTopSlice->pt1.WndPt);
			GetVerticalLine(pEnd->WndPt, pStart->WndPt, pEnd->WndPt, nSlicePos, pBottomSlice->pt2.WndPt, pTopSlice->pt2.WndPt);
		}
	}
}

void OperateMprLines::CalDistanceOfParalleLine(QPoint pPt, MprLine *pLine, int &fdistance)
{
	FPOINT fA0; fA0.x = pLine->pt1.WndPt.x()*1.00; fA0.y = pLine->pt1.WndPt.y()*1.00;
	FPOINT fA1; fA1.x = pLine->pt2.WndPt.x()*1.00; fA1.y = pLine->pt2.WndPt.y()*1.00;

	double A = fA1.y - fA0.y;
	double B = fA0.x - fA1.x;
	double C = fA1.x * fA0.y - fA1.y * fA0.x;
	
	double fdis = abs(A * pPt.x() + B * pPt.y() + C) / sqrt(pow(A, 2) + pow(B, 2));
	fdistance = qRound(fdis);
}

void OperateMprLines::CalImageDistance(QPoint pPt, MprLine *pLine, int &fdistance)
{
	FPOINT fA0; fA0.x = pLine->pt1.ImgPt.Int().x()*1.00; fA0.y = pLine->pt1.ImgPt.Int().y()*1.00;
	FPOINT fA1; fA1.x = pLine->pt2.ImgPt.Int().x()*1.00; fA1.y = pLine->pt2.ImgPt.Int().y()*1.00;

	double A = fA1.y - fA0.y;
	double B = fA0.x - fA1.x;
	double C = fA1.x * fA0.y - fA1.y * fA0.x;

	double fdis = abs(A * pPt.x() + B * pPt.y() + C) / sqrt(pow(A, 2) + pow(B, 2));
	fdistance = qRound(fdis);
}

void OperateMprLines::MoveRotateLine(QPoint pt)
{
	if (m_bActiveOblique == false)
	{
		return;
	}

	if (m_moveObject == A1STARTROTATE || m_moveObject == A1ENDROTATE)
	{
		if (m_moveObject == A1STARTROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt);
		else if (m_moveObject == A1ENDROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineAxis1->pt2.WndPt, m_pLineAxis1->pt1.WndPt);
		
		MprLine pNewA2Line;
		GetVerticalLine(m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt, m_ptCenter->WndPt, 20, pNewA2Line.pt1.WndPt, pNewA2Line.pt2.WndPt);
		GetLineAcrossRc(m_rcImgOnWnd, pNewA2Line.pt1.WndPt, pNewA2Line.pt2.WndPt, m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt);

		ReCalInterPoint(m_pLineAxis1, m_ptA1StartInter,m_ptA1EndInter);
		UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);

		ReCalInterPoint(m_pLineAxis2, m_ptA2StartInter, m_ptA2EndInter);
		UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);

		double dRotateAngle = RotateAxisByPoint(pt, m_moveObject);
		RotateAxisByAngle(dRotateAngle, A2STARTROTATE);
	}

	if (m_moveObject == A2STARTROTATE || m_moveObject == A2ENDROTATE)
	{
		if (m_moveObject == A2STARTROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt);
		else if (m_moveObject == A2ENDROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineAxis2->pt2.WndPt, m_pLineAxis2->pt1.WndPt);

		MprLine pNewA1Line;
		GetVerticalLine(m_pLineAxis2->pt1.WndPt, m_pLineAxis2->pt2.WndPt, m_ptCenter->WndPt, 20, pNewA1Line.pt1.WndPt, pNewA1Line.pt2.WndPt);
		GetLineAcrossRc(m_rcImgOnWnd, pNewA1Line.pt1.WndPt, pNewA1Line.pt2.WndPt, m_pLineAxis1->pt1.WndPt, m_pLineAxis1->pt2.WndPt);

		ReCalInterPoint(m_pLineAxis2, m_ptA2StartInter, m_ptA2EndInter);
		UpdateRotatePoint(m_pLineAxis2, m_ptA2StartRotate, m_ptA2EndRotate);
		CalSliceLine(m_ptA2StartInter, m_ptA2EndInter, m_pLineA2SliceTop, m_pLineA2SliceBottom);

		ReCalInterPoint(m_pLineAxis1, m_ptA1StartInter, m_ptA1EndInter);
		UpdateRotatePoint(m_pLineAxis1, m_ptA1StartRotate, m_ptA1EndRotate);
		CalSliceLine(m_ptA1StartInter, m_ptA1EndInter, m_pLineA1SliceTop, m_pLineA1SliceBottom);

		double dRotateAngle = RotateAxisByPoint(pt, m_moveObject);
		RotateAxisByAngle(dRotateAngle, A1STARTROTATE);
	}
}

bool OperateMprLines::GetLineAcrossRc(RECT rc, QPoint p1, QPoint p2, QPoint &retP1, QPoint &retP2)
{
	//判断这两点在同一条边上
	if (p1.y() == rc.top && p1.y() == p2.y()) return false;
	if (p1.y() == rc.bottom && p1.y() == p2.y()) return false;
	if (p1.x() == rc.left && p1.x() == p2.x()) return false;
	if (p1.x() == rc.right && p1.x() == p2.x()) return false;

	do
	{
		if (p1.y() == p2.y())//水平的
		{
			retP1.setX(rc.left);
			retP1.setY(p1.y());

			retP2.setX(rc.right);
			retP2.setY(p1.y());
		}
		else if (p1.x() == p2.x())//垂直的
		{
			retP1.setX(p1.x());
			retP1.setY(rc.top);

			retP2.setX(p1.x());
			retP2.setY(rc.bottom);
		}
		else
		{
			long x1 = p1.x(); long y1 = p1.y();
			long x2 = p2.x(); long y2 = p2.y();

			//一般的
			//直线方程:(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
			//已知x求y: y = (x - x1) * (y2 - y1) / (x2 - x1) + y1
			//已知y求x: x = (x2 - x1) * (y - y1) / (y2 - y1) + x1

			bool b1 = false;//第一个交点找到没

			//1.求与top的交点,已知y = rc.top;
			if (1)
			{
				long y = rc.top;
				long x = (long)((x2 - x1) * (y - y1) * 1.00 / (y2 - y1) + x1);
				if (x >= rc.left && x <= rc.right)
				{
					retP1.setX(x);
					retP1.setY(y);
					b1 = true;
				}
			}

			//2.求与bottom的交点,已知y = rc.bottom;
			if (2)
			{
				long y = rc.bottom;
				long x = (long)((x2 - x1) * (y - y1) * 1.00 / (y2 - y1) + x1);
				if (x >= rc.left && x <= rc.right)//有交点了
				{
					if (b1 == false)//赋值要讲究一下
					{
						retP1.setX(x);
						retP1.setY(y);
						b1 = true;
					}
					else
					{
						retP2.setX(x);
						retP2.setY(y);
						break;
					}
				}
			}

			//3.求与left的交点,已知x = rc.left;
			if (3)
			{
				long x = rc.left;
				long y = (long)((x - x1) * (y2 - y1)* 1.00 / (x2 - x1) + y1);

				if (y >= rc.top && y <= rc.bottom)//有交点了
				{
					if (b1 == false)//赋值要讲究一下
					{
						retP1.setX(x);
						retP1.setY(y);
						b1 = true;
					}
					else
					{
						retP2.setX(x);
						retP2.setY(y);
						break;
					}

				}
			}

			//4.求与left的交点,已知x = rc.right;
			if (4)
			{
				long x = rc.right;
				long y = (long)((x - x1) * (y2 - y1)* 1.00 / (x2 - x1) + y1);

				if (y >= rc.top && y <= rc.bottom)//有交点了
				{
					if (b1 == false)//赋值要讲究一下
					{
						return false;//到最后这里了，这才找到一个交点--实际上这是不可能的
					}
					else
					{
						retP2.setX(x);
						retP2.setY(y);
						break;
					}

				}
			}
		}

	} while (false);

	int R1ToP1 = (retP1.x() - p1.x())*(retP1.x() - p1.x()) + (retP1.y() - p1.y())*(retP1.y() - p1.y());//RetP1到p1的距离的平方
	int R2ToP1 = (retP2.x() - p1.x())*(retP2.x() - p1.x()) + (retP2.y() - p1.y())*(retP2.y() - p1.y());//RetP2到p1的距离的平方

	if (R1ToP1 > R2ToP1)//应该是(RetP1到p1的距离)小于(RetP2到p1的距离),若不是的话，互换
		swap(retP1, retP2);

	return true;
}

bool OperateMprLines::ReCalInterPoint(MprLine *pMainLine, MprPoint *pStartInter, MprPoint *pEndInter)
{
	int x1 = pMainLine->pt1.WndPt.x();
	int y1 = pMainLine->pt1.WndPt.y();
	int x2 = pMainLine->pt2.WndPt.x();
	int y2 = pMainLine->pt2.WndPt.y();
	if (y1 == y2)
	{
		int nRotateLineLength = abs(pMainLine->pt2.WndPt.x() - pMainLine->pt1.WndPt.x()) * m_fRotateLineRate;
		if (x1 < x2)
		{
			pStartInter->WndPt.setX(x1 + nRotateLineLength);
			pStartInter->WndPt.setY(y1);
			pEndInter->WndPt.setX(x2 - nRotateLineLength);
			pEndInter->WndPt.setY(y2);
		}
		else 
		{
			pStartInter->WndPt.setX(x1 - nRotateLineLength);
			pStartInter->WndPt.setY(y1);
			pEndInter->WndPt.setX(x2 + nRotateLineLength);
			pEndInter->WndPt.setY(y2);
		}
	}
	else if (x1 == x2)
	{
		int nRotateLineLength = abs(pMainLine->pt2.WndPt.y() - pMainLine->pt1.WndPt.y()) * m_fRotateLineRate;
		if (y1 < y2)
		{
			pStartInter->WndPt.setX(x1);
			pStartInter->WndPt.setY(y1 + nRotateLineLength);
			pEndInter->WndPt.setX(x2);
			pEndInter->WndPt.setY(y2 - nRotateLineLength);
		}
		else
		{
			pStartInter->WndPt.setX(x1);
			pStartInter->WndPt.setY(y1 - nRotateLineLength);
			pEndInter->WndPt.setX(x2);
			pEndInter->WndPt.setY(y2 + nRotateLineLength);
		}
	}
	else
	{
		int nRotateLineLength = sqrt(pow((pMainLine->pt2.WndPt.x() - pMainLine->pt1.WndPt.x()), 2) + pow((pMainLine->pt2.WndPt.y() - pMainLine->pt1.WndPt.y()), 2)) * m_fRotateLineRate;
		
		GetInterPtByLength(pMainLine->pt1.WndPt, pMainLine->pt2.WndPt, nRotateLineLength, pStartInter->WndPt);
		GetInterPtByLength(pMainLine->pt2.WndPt, pMainLine->pt1.WndPt, nRotateLineLength, pEndInter->WndPt);

	}

	return true;
}

bool OperateMprLines::GetInterPtByLength(QPoint &p1, QPoint &p2, double nLen, QPoint &RetPt)
{
	//1、先求距离p1处距离为nLen的点
	double x1 = p1.x() * 1.00;
	double y1 = p1.y() * 1.00;
	double x2 = p2.x() * 1.00;
	double y2 = p2.y() * 1.00;

	RetPt.setX(-1000);
	RetPt.setY(-1000);
	if (p1.x() != p2.x() && p1.y() != p2.y())
	{

		double k = (y2 - y1) / (x2 - x1);
		//两点间距离公式 ：nLen1 = [ sqrt(1 + k*k) ]*abs(? - x1)
		//得到:abs(?-x1) = nLen1/sqrt(1+k*k);
		//得到:? = nLen1/sqrt(1+k*k) + x1; 和 ? = x1 - [nLen1/sqrt(1+k*k)];
		double X1 = nLen / sqrt(1 + k*k) + x1;
		double Y1 = ((X1 - x1) / (x2 - x1))*(y2 - y1) + y1;

		double X2 = x1 - (nLen / sqrt(1 + k*k));
		double Y2 = ((X2 - x1) / (x2 - x1))*(y2 - y1) + y1;

		//因为绝对值的问题，得到俩点，那么谁距离p2点近，就留谁
		double L1 = (X1 - x2)*(X1 - x2) + (Y1 - y2)*(Y1 - y2);//( sqrt(1 + k*k) )*fabs(X1 - x2);
		double L2 = (X2 - x2)*(X2 - x2) + (Y2 - y2)*(Y2 - y2);//( sqrt(1 + k*k) )*fabs(X2 - x2);

		if (min(L1, L2) == L1)
		{
			RetPt.setX(X1);
			RetPt.setY(Y1);
		}
		else
		{
			RetPt.setX(X2);
			RetPt.setY(Y2);
		}

	}
	if (p1.x() == p2.x())//|
	{
		int Y1 = y1 + nLen;
		int Y2 = y1 - nLen;

		//谁距离点2近，就留谁
		int L1 = abs(Y1 - y2);
		int L2 = abs(Y2 - y2);

		if (L1 == min(L1, L2))
			RetPt.setY(Y1);
		else
			RetPt.setY(Y2);
		
		RetPt.setX(x1);
	}
	if (p1.y() == p2.y())//-
	{
		int X1 = x1 + nLen;
		int X2 = x1 - nLen;

		int L1 = abs(X1 - x2);
		int L2 = abs(X2 - x2);

		if (L1 == min(L1, L2))
			RetPt.setX(X1);
		else
			RetPt.setX(X2);
		
		RetPt.setY(y1);
	}

	return true;
}

bool OperateMprLines::IsPointInRc(RECT rc, QPoint pt)
{
	if (pt.x() >= rc.left && pt.x() < rc.right && pt.y() >= rc.top && pt.y() < rc.bottom)
		return true;
	return false;
}

void OperateMprLines::AllCovertWndToImg()
{
	if (1) //横向
	{
		ConvertWndToImg(m_pLineAxis1->pt1);
		ConvertWndToImg(m_pLineAxis1->pt2);
		ConvertWndToImg(*m_ptA1StartRotate);
		ConvertWndToImg(*m_ptA1EndRotate);
		ConvertWndToImg(*m_ptA1StartInter);
		ConvertWndToImg(*m_ptA1EndInter);
		ConvertWndToImg(m_pLineA1SliceTop->pt1);
		ConvertWndToImg(m_pLineA1SliceTop->pt2);
		ConvertWndToImg(m_pLineA1SliceBottom->pt1);
		ConvertWndToImg(m_pLineA1SliceBottom->pt2);
	}

	if (2)//纵向
	{
		ConvertWndToImg(m_pLineAxis2->pt1);
		ConvertWndToImg(m_pLineAxis2->pt2);
		ConvertWndToImg(*m_ptA2StartRotate);
		ConvertWndToImg(*m_ptA2EndRotate);
		ConvertWndToImg(*m_ptA2StartInter);
		ConvertWndToImg(*m_ptA2EndInter);
		ConvertWndToImg(m_pLineA2SliceTop->pt1);
		ConvertWndToImg(m_pLineA2SliceTop->pt2);
		ConvertWndToImg(m_pLineA2SliceBottom->pt1);
		ConvertWndToImg(m_pLineA2SliceBottom->pt2);
	}

	if (3)//交点
	{
		ConvertWndToImg(*m_ptCenter);
	}

}

int OperateMprLines::GetAxis1()
{
	if (m_nWndType == ORIIMG_SAGITTAL)
		return 1;

	if (m_nWndType == ORIIMG_CORONAL)
		return 0;

	return 0;
}

int OperateMprLines::GetAxis2()
{
	if (m_nWndType == ORIIMG_SAGITTAL)
		return 2;

	if (m_nWndType == ORIIMG_CORONAL)
		return 2;

	return 1;
}

int OperateMprLines::GetPlaneAxis1()
{
	if (m_nWndType == ORIIMG_AXIAL)
	{
		return 0;
	}
	if (m_nWndType == ORIIMG_CORONAL)
	{
		return 0;
	}
	return 1;
}

int OperateMprLines::GetPlaneAxis2()
{
	if (m_nWndType == ORIIMG_AXIAL)
	{
		return 1;
	}
	if (m_nWndType == ORIIMG_CORONAL)
	{
		return 2;
	}
	return 2;
}

void OperateMprLines::MoveResliceControlCenter()
{
	ConvertWndToImg(*m_ptCenter);
	double deltaX = m_ptCenter->ImgPt.x - m_ptPreCenter.ImgPt.x;
	double deltaY = m_ptCenter->ImgPt.y - m_ptPreCenter.ImgPt.y;

	int nPlaneAxis1Id = GetPlaneAxis1();
	int nPlaneAxis2Id = GetPlaneAxis2();

	Hmy3DVector vecA1 = m_pResliceControl->GetAxis(nPlaneAxis1Id);
	Hmy3DVector vecA2 = m_pResliceControl->GetAxis(nPlaneAxis2Id);

	Hmy3DVector vecDeltaX = vecA1.Scale(deltaX*m_dSpacingY);
	Hmy3DVector vecDeltaY = vecA2.Scale(deltaY*m_dSpacingX);
	Hmy3DVector deltaVector = vecDeltaX.Add(vecDeltaY);
	Hmy3DVector vecPreCenter = m_pResliceControl->GetCenter();
	Hmy3DVector vecNewCenter = vecPreCenter.Add(deltaVector);
	m_pResliceControl->SetCenter(vecNewCenter);

	m_ptPreCenter = *m_ptCenter;

}

double OperateMprLines::RotateAxisByPoint(QPoint pt, MoveObject moveObject)
{
	double lastVector[2], curVector[2];
	lastVector[0] = m_prePt.x() - m_ptCenter->WndPt.x();
	lastVector[1] = m_prePt.y() - m_ptCenter->WndPt.y();
	curVector[0] = pt.x() - m_ptCenter->WndPt.x();
	curVector[1] = pt.y() - m_ptCenter->WndPt.y();

	double lastNorm = sqrt(lastVector[0] * lastVector[0] + lastVector[1] * lastVector[1]);
	double curNorm = sqrt(curVector[0] * curVector[0] + curVector[1] * curVector[1]);

	double angle = acos((lastVector[0] * curVector[0] + lastVector[1] * curVector[1])/(lastNorm*curNorm));

	double corss = lastVector[0] * curVector[1] - lastVector[1] * curVector[0];
	if (corss < 0)
		angle *= -1;

	if (angle == 0)
		return 0;

	RotateAxisByAngle(angle, moveObject);
	return angle;
}

void OperateMprLines::RotateAxisByAngle(double angle, MoveObject moveObject)
{
	int axis = -1;
	if (moveObject == A1STARTROTATE || moveObject == A1ENDROTATE)
		axis = GetPlaneAxis2();
	else if (moveObject == A2STARTROTATE || moveObject == A2ENDROTATE)
		axis = GetPlaneAxis1();

	HmyPlane3D *planeToBeRotated = m_pResliceControl->GetPlane(axis);
	HmyPlane3D *normalPlane = m_pResliceControl->GetPlane(m_nWndType);

	double vectorToBeRotated[3], aboutAxis[3], rotatedVector[3];
	planeToBeRotated->GetNormal(vectorToBeRotated);
	normalPlane->GetNormal(aboutAxis);

	this->RotateVectorAboutVector(vectorToBeRotated,
		aboutAxis, angle, rotatedVector);
	planeToBeRotated->SetNormal(rotatedVector);
}

void OperateMprLines::RotateVectorAboutVector(double vectorToBeRotated[3], double axis[3], /* vector about which we rotate */ double angle, /* angle in radians */ double output[3])
{
	//  let
	//        [v] = [vx, vy, vz]      the vector to be rotated.
	//        [l] = [lx, ly, lz]      the vector about rotation
	//              | 1  0  0|
	//        [i] = | 0  1  0|           the identity matrix
	//              | 0  0  1|
	//
	//              |   0  lz -ly |
	//        [L] = | -lz   0  lx |
	//              |  ly -lx   0 |
	//
	//        d = sqrt(lx*lx + ly*ly + lz*lz)
	//        a                       the angle of rotation
	//
	//    then
	//
	//   matrix operations gives:
	//
	//    [v] = [v]x{[i] + sin(a)/d*[L] + ((1 - cos(a))/(d*d)*([L]x[L]))}

	// normalize the axis vector
	double v[3] = { vectorToBeRotated[0],
		vectorToBeRotated[1], vectorToBeRotated[2] };
	double l[3] = { axis[0], axis[1], axis[2] };
	vtkMath::Normalize(v);
	vtkMath::Normalize(l);
	const double u = sin(angle);
	const double w = 1.0 - cos(angle);

	output[0] = v[0] * (1 - w * (l[2] * l[2] + l[1] * l[1])) +
		v[1] * (-u * l[2] + w * l[0] * l[1]) +
		v[2] * (u * l[1] + w * l[0] * l[1]);
	output[1] = v[0] * (u * l[2] + w * l[0] * l[1]) +
		v[1] * (1 - w * (l[0] * l[0] + l[2] * l[2])) +
		v[2] * (-u * l[0] + w * l[1] * l[2]);
	output[2] = v[0] * (-u * l[1] + w * l[0] * l[2]) +
		v[1] * (u * l[0] + w * l[1] * l[2]) +
		v[2] * (1 - w * (l[1] * l[1] + l[0] * l[0]));
}

HmyLine3D OperateMprLines::CalInterSectingLine(HmyPlane3D *target, HmyPlane3D *reference)
{
	Hmy3DVector targetNormal = target->normal();
	Hmy3DVector referenceNormal = reference->normal();

	Hmy3DVector ptCenter = m_pResliceControl->GetCenter();

	target->SetFromNormalAndCoplanarPoint(targetNormal, ptCenter);
	reference->SetFromNormalAndCoplanarPoint(referenceNormal, ptCenter);

	Hmy3DVector lineDirection, lineOrigin;
	reference->IntersectPlane(*target, lineDirection,lineOrigin);
	
	HmyLine3D a;

	return a;


}
