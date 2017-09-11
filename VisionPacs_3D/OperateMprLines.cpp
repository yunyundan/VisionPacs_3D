#include "stdafx.h"
#include "OperateMprLines.h"


OperateMprLines::OperateMprLines(QObject *parent,int nImgWndType)
	: QObject(parent)
{
	m_bShow = true;
	m_bInitLines = false;
	m_bActiveOblique = false;
	m_nNearFactor = 5;
	m_bMousePressed = false;
	m_moveObject = HLINE;

	m_prePt = QPoint(0, 0);

	m_pLineH = nullptr;
	m_ptHStartRotate = nullptr;
	m_ptHEndRotate = nullptr;
	m_ptHStartInter = nullptr;
	m_ptHEndInter = nullptr;
	m_pLineHSliceTop = nullptr;
	m_pLineHSliceBottom = nullptr;
	m_pTriHStartTop = nullptr;
	m_pTriHStartBottom = nullptr;
	m_pTriHEndTop = nullptr;
	m_pTriHEndBottom = nullptr;

	m_pLineV = nullptr;
	m_ptVStartRotate = nullptr;
	m_ptVEndRotate = nullptr;
	m_ptVStartInter = nullptr;
	m_ptVEndInter = nullptr;
	m_pLineVSliceTop = nullptr;
	m_pLineVSliceBottom = nullptr;
	m_pTriVStartTop = nullptr;
	m_pTriVStartBottom = nullptr;
	m_pTriVEndTop = nullptr;
	m_pTriVEndBottom = nullptr;

	m_ptCenter = nullptr;

	m_iPointWidth = 2;
	m_fRotateLineRate = 0.1;

	m_nTriAngleWidth = 15;
	m_nTriAngleDis = 6;

	m_nWndType = nImgWndType;

	m_nSlicePos = 0.00;

	GetLinesColor();

}

OperateMprLines::~OperateMprLines()
{
	if (m_pLineH)
	{
		delete m_pLineH;
		m_pLineH = nullptr;
	}

	if (m_ptHStartRotate)
	{
		delete m_ptHStartRotate;
		m_ptHStartRotate = nullptr;
	}

	if (m_ptHEndRotate)
	{
		delete m_ptHEndRotate;
		m_ptHEndRotate = nullptr;
	}

	if (m_ptHStartInter)
	{
		delete m_ptHStartInter;
		m_ptHStartInter = nullptr;
	}

	if (m_ptHEndInter)
	{
		delete m_ptHEndInter;
		m_ptHEndInter = nullptr;
	}

	if (m_pLineHSliceTop)
	{
		delete m_pLineHSliceTop;
		m_pLineHSliceTop = nullptr;
	} 

	if (m_pLineHSliceBottom)
	{
		delete m_pLineHSliceBottom;
		m_pLineHSliceBottom = nullptr;
	}
	if (m_pTriHStartTop)
	{
		delete m_pTriHStartTop;
		m_pTriHStartTop = nullptr;
	}
	if (m_pTriHStartBottom)
	{
		delete m_pTriHStartBottom;
		m_pTriHStartBottom = nullptr;
	}
	if (m_pTriHEndTop)
	{
		delete m_pTriHEndTop;
		m_pTriHEndTop = nullptr;
	}
	if (m_pTriHEndBottom)
	{
		delete m_pTriHEndBottom;
		m_pTriHEndBottom = nullptr;
	}

	if (m_pLineV)
	{
		delete m_pLineV;
		m_pLineV = nullptr;
	}

	if (m_ptVStartRotate)
	{
		delete m_ptVStartRotate;
		m_ptVStartRotate = nullptr;
	}

	if (m_ptVEndRotate)
	{
		delete m_ptVEndRotate;
		m_ptVEndRotate = nullptr;
	}

	if (m_ptVStartInter)
	{
		delete m_ptVStartInter;
		m_ptVStartInter = nullptr;
	}

	if (m_ptVEndInter)
	{
		delete m_ptVEndInter;
		m_ptVEndInter = nullptr;
	}

	if (m_pLineVSliceTop)
	{
		delete m_pLineVSliceTop;
		m_pLineVSliceTop = nullptr;
	}

	if (m_pLineVSliceBottom)
	{
		delete m_pLineVSliceBottom;
		m_pLineVSliceBottom = nullptr;
	}

	if (m_pTriVStartTop)
	{
		delete m_pTriVStartTop;
		m_pTriVStartTop = nullptr;
	}

	if (m_pTriVStartBottom)
	{
		delete m_pTriVStartBottom;
		m_pTriHStartBottom = nullptr;
	}

	if (m_pTriVEndTop)
	{
		delete m_pTriVEndTop;
		m_pTriVEndTop = nullptr;
	}

	if (m_pTriVEndBottom)
	{
		delete m_pTriVEndBottom;
		m_pTriVEndBottom = nullptr;
	}

	if (m_ptCenter)
	{
		delete m_ptCenter;
		m_ptCenter = nullptr;
	}
}


bool OperateMprLines::RefreshMprLinesPara(RECT rcImg, int nImgWidth, int nImgHeigh, double fspacingX, double fspacingY)
{
	if (rcImg.right - rcImg.left <= 0 || rcImg.bottom - rcImg.top <= 0)
		return false;
	if (nImgWidth <= 0 || nImgHeigh <= 0)
		return false;

	m_rcImgOnWnd = rcImg;
	m_nImgW = nImgWidth;
	m_nImgH = nImgHeigh;
	m_fSpacingX = fspacingX;
	m_fSpacingY = fspacingY;
	if (m_bInitLines == false)
	{
		if (1) //横向
		{
			m_pLineH = new MprLine;
			m_pLineH->pt1.ImgPt.x = 0.00;
			m_pLineH->pt1.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineH->pt1);

			m_pLineH->pt2.ImgPt.x = m_nImgW*1.00;
			m_pLineH->pt2.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineH->pt2);

			m_ptHStartRotate = new MprPoint;
			m_ptHEndRotate = new MprPoint;
			UpdateRotatePoint(m_pLineH, m_ptHStartRotate, m_ptHEndRotate);

			int nRotateLineLength = (m_pLineH->pt2.WndPt.x() - m_pLineH->pt1.WndPt.x()) * m_fRotateLineRate;

			m_ptHStartInter = new MprPoint;
			m_ptHStartInter->WndPt.setX(m_pLineH->pt1.WndPt.x() + nRotateLineLength);
			m_ptHStartInter->WndPt.setY(m_pLineH->pt1.WndPt.y());

			m_ptHEndInter = new MprPoint;
			m_ptHEndInter->WndPt.setX(m_pLineH->pt2.WndPt.x() - nRotateLineLength);
			m_ptHEndInter->WndPt.setY(m_pLineH->pt2.WndPt.y());

			//横向的层厚线--初始值与主线重合
			m_pLineHSliceTop = new MprLine;
			m_pLineHSliceTop->pt1 = *m_ptHStartInter;
			m_pLineHSliceTop->pt2 = *m_ptHEndInter;

			m_pLineHSliceBottom = new MprLine;
			m_pLineHSliceBottom->pt1 = *m_ptHStartInter;
			m_pLineHSliceBottom->pt2 = *m_ptHEndInter;

			//三角形
			m_pTriHStartTop = new MprTriAngle;
			m_pTriHEndTop= new MprTriAngle;
			GetTrianglePoint(m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriHStartTop, *m_pTriHEndTop, true);

			m_pTriHStartBottom = new MprTriAngle;
			m_pTriHEndBottom = new MprTriAngle;
			GetTrianglePoint(m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriHStartBottom, *m_pTriHEndBottom, false);
		}

		if (2) //纵向
		{
			m_pLineV = new MprLine;
			m_pLineV->pt1.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineV->pt1.ImgPt.y = 0.00 ;
			ConvertImgToWnd(m_pLineV->pt1);

			m_pLineV->pt2.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineV->pt2.ImgPt.y = m_nImgH*1.00;
			ConvertImgToWnd(m_pLineV->pt2);

			m_ptVStartRotate = new MprPoint;
			m_ptVEndRotate = new MprPoint;
			UpdateRotatePoint(m_pLineV, m_ptVStartRotate, m_ptVEndRotate);

			int nRotateLineLength = (m_pLineV->pt2.WndPt.y() - m_pLineV->pt1.WndPt.y()) * m_fRotateLineRate;

			m_ptVStartInter = new MprPoint;
			m_ptVStartInter->WndPt.setX(m_pLineV->pt1.WndPt.x());
			m_ptVStartInter->WndPt.setY(m_pLineV->pt1.WndPt.y() + nRotateLineLength);

			m_ptVEndInter = new MprPoint;
			m_ptVEndInter->WndPt.setX(m_pLineV->pt2.WndPt.x());
			m_ptVEndInter->WndPt.setY(m_pLineV->pt2.WndPt.y() - nRotateLineLength);

			//横向的层厚线--初始值与主线重合
			m_pLineVSliceTop = new MprLine;
			m_pLineVSliceTop->pt1 = *m_ptVStartInter;
			m_pLineVSliceTop->pt2 = *m_ptVEndInter;

			m_pLineVSliceBottom = new MprLine;
			m_pLineVSliceBottom->pt1 = *m_ptVStartInter;
			m_pLineVSliceBottom->pt2 = *m_ptVEndInter;

			//三角形
			m_pTriVStartTop = new MprTriAngle;
			m_pTriVStartBottom = new MprTriAngle;
			GetTrianglePoint(m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriVStartTop, *m_pTriVStartBottom, true);

			m_pTriVEndTop = new MprTriAngle;
			m_pTriVEndBottom = new MprTriAngle;
			GetTrianglePoint(m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriVEndTop, *m_pTriVEndBottom, false);
		}

		if (3)//交点
		{
			m_ptCenter = new MprPoint;
			GetInterPoint(m_pLineV->pt1.ImgPt, m_pLineV->pt2.ImgPt, m_pLineH->pt1.ImgPt, m_pLineH->pt2.ImgPt, m_ptCenter->ImgPt);
			ConvertImgToWnd(*m_ptCenter);
		}
		m_bInitLines = true;
		AllCovertWndToImg();
	}
	else
	{
		if (1) //横向
		{
			ConvertImgToWnd(m_pLineH->pt1);
			ConvertImgToWnd(m_pLineH->pt2);
			ConvertImgToWnd(*m_ptHStartRotate);
			ConvertImgToWnd(*m_ptHEndRotate);
			ConvertImgToWnd(*m_ptHStartInter);
			ConvertImgToWnd(*m_ptHEndInter);
			ConvertImgToWnd(m_pLineHSliceTop->pt1);
			ConvertImgToWnd(m_pLineHSliceTop->pt2);
			ConvertImgToWnd(m_pLineHSliceBottom->pt1);
			ConvertImgToWnd(m_pLineHSliceBottom->pt2);
			GetTrianglePoint(m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriHStartTop, *m_pTriHEndTop, true);
			GetTrianglePoint(m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriHStartBottom, *m_pTriHEndBottom, false);
		}

		if (2)//纵向
		{
			ConvertImgToWnd(m_pLineV->pt1);
			ConvertImgToWnd(m_pLineV->pt2);
			ConvertImgToWnd(*m_ptVStartRotate);
			ConvertImgToWnd(*m_ptVEndRotate);
			ConvertImgToWnd(*m_ptVStartInter);
			ConvertImgToWnd(*m_ptVEndInter);
			ConvertImgToWnd(m_pLineVSliceTop->pt1);
			ConvertImgToWnd(m_pLineVSliceTop->pt2);
			ConvertImgToWnd(m_pLineVSliceBottom->pt1);
			ConvertImgToWnd(m_pLineVSliceBottom->pt2);
			GetTrianglePoint(m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriVStartTop, *m_pTriVEndTop, true);
			GetTrianglePoint(m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *m_pTriVStartBottom, *m_pTriVEndBottom, false);
		}

		if (3)//交点
		{
			ConvertImgToWnd(*m_ptCenter);
		}
	}
		
	return true;
}

void OperateMprLines::OnMprLinesMousePress(QMouseEvent *event)
{
	m_prePt = event->pos();

	if (m_pLineH->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HLINE;
		return;
	}

	if (m_ptHStartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HSTARTROTATE;
		return;
	}

	if (m_ptHEndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HENDROTATE;
		return;
	}

	if (m_ptHStartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HSTARTROTATE;
		return;
	}

	if (m_ptHEndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HENDROTATE;
		return;
	}

	if (m_pLineHSliceTop->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HTOPSLICE;
		return;
	}

	if (m_pLineHSliceBottom->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HBOTTOMSLICE;
		return;
	}

	if (m_pLineV->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VLINE;
		return;
	}

	if (m_ptVStartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VSTARTROTATE;
		return;
	}

	if (m_ptVEndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VENDROTATE;
		return;
	}
	if (m_ptVStartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VSTARTROTATE;
		return;
	}

	if (m_ptVEndRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VENDROTATE;
		return;
	}

	if (m_pLineVSliceTop->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VTOPSLICE;
		return;
	}

	if (m_pLineVSliceBottom->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VBOTTOMSLICE;
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
		bool bTriangleUpdate = GetPointNearTriAngle(pt);
		bool bCenterUpdate = GetPointNearPoint(pt);

		return (bLinesUpdate || bTriangleUpdate || bCenterUpdate);
	}
	else {

		QPoint ptDelta = pt - m_prePt;

		if (m_moveObject == HLINE || m_moveObject == VLINE)
		{
			MoveMainLines(ptDelta);
		}

		if (m_moveObject == HTOPSLICE || m_moveObject == HBOTTOMSLICE || m_moveObject == VTOPSLICE || m_moveObject == VBOTTOMSLICE)
		{
			MoveSliceTriangle(ptDelta);
		}

		if (m_moveObject == HSTARTROTATE || m_moveObject == HENDROTATE || m_moveObject == VSTARTROTATE || m_moveObject == VENDROTATE)
		{
			MoveRotateLine(pt);
		}

		if (m_moveObject == CENTERPOINT)
		{
			MoveCenterPt(pt);
		}

		m_prePt = pt;
		OutputLineInfo();
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
		m_clrLineH.setAlpha(125);
		if (m_pLineH->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 2, Qt::SolidLine));
		QPoint ptTmp;
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineH->pt1.WndPt)) < 10)
			ptTmp = m_pLineH->pt1.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineH->pt1.WndPt, 10, ptTmp);
		painter->drawLine(m_pLineH->pt1.WndPt,ptTmp);
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineH->pt2.WndPt)) < 10)
			ptTmp = m_pLineH->pt2.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineH->pt2.WndPt, 10, ptTmp);
		painter->drawLine(ptTmp, m_pLineH->pt2.WndPt);

		//连接处圆形
		painter->setBrush(m_clrLineH);
		painter->drawEllipse(m_ptHStartInter->WndPt, 3, 3);
		painter->drawEllipse(m_ptHEndInter->WndPt, 3, 3);

		//旋转圆
		m_clrLineH.setAlpha(200);
		if (m_ptHStartRotate->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineH);
		painter->drawEllipse(m_ptHStartRotate->WndPt, 5, 5);

		m_clrLineH.setAlpha(200);
		if (m_ptHEndRotate->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineH);
		painter->drawEllipse(m_ptHEndRotate->WndPt, 5, 5);

		//层厚线
		if (m_nSlicePos != 0.00)
		{
			m_clrLineH.setAlpha(125);
			painter->setPen(QPen(m_clrLineH, 1, Qt::DashLine));
			painter->drawLine(m_ptHStartInter->WndPt, m_pLineHSliceTop->pt1.WndPt);
			painter->drawLine(m_pLineHSliceTop->pt2.WndPt, m_ptHEndInter->WndPt);
			if (m_pLineHSliceTop->bActive == true)
				m_clrLineH.setAlpha(255);
			painter->setPen(QPen(m_clrLineH, 2, Qt::DashLine));			
			QPoint pEnd1; GetInterPoint(m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, pEnd1);
			QPoint pStart1; GetInterPoint(m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, pStart1);
			bool bSwitch = false;
			double dEd = (pEnd1 - m_pLineHSliceTop->pt1.WndPt).manhattanLength();
			double dSd = (pStart1 - m_pLineHSliceTop->pt1.WndPt).manhattanLength();
			if (dSd < dEd)
				bSwitch = true;
			if (bSwitch == false)
			{
				painter->drawLine(m_pLineHSliceTop->pt1.WndPt, pEnd1);
				painter->drawLine(pStart1, m_pLineHSliceTop->pt2.WndPt);
			}
			else
			{
				painter->drawLine(m_pLineHSliceTop->pt1.WndPt, pStart1);
				painter->drawLine(pEnd1, m_pLineHSliceTop->pt2.WndPt);
			}
			
			m_clrLineH.setAlpha(125);
			painter->setPen(QPen(m_clrLineH, 1, Qt::DashLine));
			painter->drawLine(m_ptHStartInter->WndPt, m_pLineHSliceBottom->pt1.WndPt);
			painter->drawLine(m_pLineHSliceBottom->pt2.WndPt, m_ptHEndInter->WndPt);
			if (m_pLineHSliceBottom->bActive == true)
				m_clrLineH.setAlpha(255);
			painter->setPen(QPen(m_clrLineH, 2, Qt::DashLine));
			GetInterPoint(m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, pEnd1);	
			GetInterPoint(m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, pStart1);
			if (bSwitch == false) 
			{
				painter->drawLine(m_pLineHSliceBottom->pt1.WndPt, pEnd1);
				painter->drawLine(pStart1, m_pLineHSliceBottom->pt2.WndPt);
			}
			else
			{
				painter->drawLine(m_pLineHSliceBottom->pt1.WndPt, pStart1);
				painter->drawLine(pEnd1, m_pLineHSliceBottom->pt2.WndPt);
			}

		}

		//三角形
		m_clrLineH.setAlpha(125);
		painter->setBrush(Qt::NoBrush);
		if (m_pTriHStartTop->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 2, Qt::SolidLine));
		const QPointF points[3] = { m_pTriHStartTop->ptBtm1,m_pTriHStartTop->ptBtm2,m_pTriHStartTop->ptTop };
		painter->drawPolygon(points, 3);

		m_clrLineH.setAlpha(125);
		if (m_pTriHEndTop->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 2, Qt::SolidLine));
		const QPointF points1[3] = { m_pTriHEndTop->ptBtm1,m_pTriHEndTop->ptBtm2,m_pTriHEndTop->ptTop };
		painter->drawPolygon(points1, 3);

		m_clrLineH.setAlpha(125);
		if (m_pTriHEndBottom->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 2, Qt::SolidLine));
		const QPointF points2[3] = { m_pTriHEndBottom->ptBtm1,m_pTriHEndBottom->ptBtm2,m_pTriHEndBottom->ptTop };
		painter->drawPolygon(points2, 3);

		m_clrLineH.setAlpha(125);
		if (m_pTriHStartBottom->bActive == true)
			m_clrLineH.setAlpha(255);
		painter->setPen(QPen(m_clrLineH, 2, Qt::SolidLine));
		const QPointF points3[3] = { m_pTriHStartBottom->ptBtm1,m_pTriHStartBottom->ptBtm2,m_pTriHStartBottom->ptTop };
		painter->drawPolygon(points3, 3);
	}

	if (2)//纵向
	{
		//主线
		m_clrLineV.setAlpha(125);
		if (m_pLineV->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 2, Qt::SolidLine));
		QPoint ptTmp;
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineV->pt1.WndPt)) < 10)
			ptTmp = m_pLineV->pt1.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineV->pt1.WndPt, 10, ptTmp);
		painter->drawLine(m_pLineV->pt1.WndPt, ptTmp);
		if (sqrt(Dist2(m_ptCenter->WndPt, m_pLineV->pt2.WndPt)) < 10)
			ptTmp = m_pLineV->pt2.WndPt;
		else
			GetInterPtByLength(m_ptCenter->WndPt, m_pLineV->pt2.WndPt, 10, ptTmp);		
		painter->drawLine(ptTmp,m_pLineV->pt2.WndPt);

		//连接处圆形
		painter->setPen(QPen(m_clrLineV, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineV);
		painter->drawEllipse(m_ptVStartInter->WndPt, 3, 3);
		painter->drawEllipse(m_ptVEndInter->WndPt, 3, 3);

		//旋转圆
		m_clrLineV.setAlpha(200);
		if (m_ptVStartRotate->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineV);
		painter->drawEllipse(m_ptVStartRotate->WndPt, 5, 5);

		m_clrLineV.setAlpha(200);
		if (m_ptVEndRotate->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineV);
		painter->drawEllipse(m_ptVEndRotate->WndPt, 5, 5);

		//层厚线
		if (m_nSlicePos != 0.00)
		{
			m_clrLineV.setAlpha(125);
			painter->setPen(QPen(m_clrLineV, 1, Qt::DashLine));
			painter->drawLine(m_ptVStartInter->WndPt, m_pLineVSliceTop->pt1.WndPt);
			painter->drawLine(m_pLineVSliceTop->pt2.WndPt, m_ptVEndInter->WndPt);
			if (m_pLineVSliceTop->bActive == true)
				m_clrLineV.setAlpha(255);
			painter->setPen(QPen(m_clrLineV, 2, Qt::DashLine));
			QPoint pStart1; GetInterPoint(m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, pStart1);
			QPoint pEnd1; GetInterPoint(m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, pEnd1);
			bool bSwitch = false;
			double dEd = (pEnd1 - m_pLineVSliceTop->pt1.WndPt).manhattanLength();
			double dSd = (pStart1 - m_pLineVSliceTop->pt1.WndPt).manhattanLength();
			if (dSd < dEd)
				bSwitch = true;
			if (bSwitch == false)
			{
				painter->drawLine(m_pLineVSliceTop->pt1.WndPt, pEnd1);
				painter->drawLine(pStart1, m_pLineVSliceTop->pt2.WndPt);
			}
			else
			{
				painter->drawLine(m_pLineVSliceTop->pt1.WndPt, pStart1);
				painter->drawLine(pEnd1, m_pLineVSliceTop->pt2.WndPt);
			}

			m_clrLineV.setAlpha(125);
			painter->setPen(QPen(m_clrLineV, 1, Qt::DashLine));
			painter->drawLine(m_ptVStartInter->WndPt, m_pLineVSliceBottom->pt1.WndPt);
			painter->drawLine(m_pLineVSliceBottom->pt2.WndPt, m_ptVEndInter->WndPt);
			if (m_pLineVSliceBottom->bActive == true)
				m_clrLineV.setAlpha(255);
			painter->setPen(QPen(m_clrLineV, 2, Qt::DashLine));
			GetInterPoint(m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, pEnd1);
			GetInterPoint(m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, pStart1);
			if (bSwitch == false)
			{
				painter->drawLine(m_pLineVSliceBottom->pt1.WndPt, pEnd1);
				painter->drawLine(pStart1, m_pLineVSliceBottom->pt2.WndPt);
			}
			else
			{
				painter->drawLine(m_pLineVSliceBottom->pt1.WndPt, pStart1);
				painter->drawLine(pEnd1, m_pLineVSliceBottom->pt2.WndPt);
			}

		}

		//三角形
		m_clrLineV.setAlpha(125);
		painter->setBrush(Qt::NoBrush);
		if (m_pTriVStartTop->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 2, Qt::SolidLine));
		const QPointF points[3] = { m_pTriVStartTop->ptBtm1,m_pTriVStartTop->ptBtm2,m_pTriVStartTop->ptTop };
		painter->drawPolygon(points, 3);

		m_clrLineV.setAlpha(125);
		if (m_pTriVStartBottom->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 2, Qt::SolidLine));
		const QPointF points1[3] = { m_pTriVStartBottom->ptBtm1,m_pTriVStartBottom->ptBtm2,m_pTriVStartBottom->ptTop };
		painter->drawPolygon(points1, 3);

		m_clrLineV.setAlpha(125);
		if (m_pTriVEndTop->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 2, Qt::SolidLine));
		const QPointF points2[3] = { m_pTriVEndTop->ptBtm1,m_pTriVEndTop->ptBtm2,m_pTriVEndTop->ptTop };
		painter->drawPolygon(points2, 3);

		m_clrLineV.setAlpha(125);
		if (m_pTriVEndBottom->bActive == true)
			m_clrLineV.setAlpha(255);
		painter->setPen(QPen(m_clrLineV, 2, Qt::SolidLine));
		const QPointF points3[3] = { m_pTriVEndBottom->ptBtm1,m_pTriVEndBottom->ptBtm2,m_pTriVEndBottom->ptTop };
		painter->drawPolygon(points3, 3);
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
	AllCovertWndToImg();

	MprLinesInfo info;
	info.ptH1 = m_pLineH->pt1.ImgPt.Int();
	info.ptH2 = m_pLineH->pt2.ImgPt.Int();
	info.ptV1 = m_pLineV->pt1.ImgPt.Int();
	info.ptV2 = m_pLineV->pt2.ImgPt.Int();
	info.dSliceThick = m_nSlicePos * m_fSpacingX;
	info.ptCenter = m_ptCenter->ImgPt.Int();
	info.nWndType = m_nWndType;
	emit MprLinesInfoOutput(info);
}



void OperateMprLines::SetManiLinePos(MoveObject object, MprLine *pLine)
{
	ConvertImgToWnd(pLine->pt1);
	ConvertImgToWnd(pLine->pt2);
	QPoint ptInter;
	if (object == HLINE)
	{
		m_pLineH->pt1 = pLine->pt1;
		m_pLineH->pt2 = pLine->pt2;	
		GetInterPoint(m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
		ReCalInterPoint(m_pLineH, m_ptHStartInter, m_ptHEndInter);
		UpdateRotatePoint(m_pLineH, m_ptHStartRotate, m_ptHEndRotate);	
	}

	if (object == VLINE)
	{
		m_pLineV->pt1 = pLine->pt1;
		m_pLineV->pt2 = pLine->pt2;
		GetInterPoint(m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
		ReCalInterPoint(m_pLineV, m_ptVStartInter, m_ptVEndInter);
		UpdateRotatePoint(m_pLineV, m_ptVStartRotate, m_ptVEndRotate);
	}

	CalSliceLine(m_ptHStartInter, m_ptHEndInter, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);
	CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);

	m_ptCenter->WndPt = ptInter;
	AllCovertWndToImg();
}

void OperateMprLines::SetSliceLinePos(double nSliceThick)
{
	m_nSlicePos = qRound(nSliceThick / m_fSpacingX);

	CalSliceLine(m_ptHStartInter, m_ptHEndInter, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);
	CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
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
		m_clrLineH = QColor(124, 252, 0, 125);
		m_clrLineV = QColor(255, 0, 0, 125);
		m_clrCenter = QColor(0, 191, 255, 125);
	}
	else if(m_nWndType == ORIIMG_CORONAL)
	{
		m_clrLineH = QColor(0, 191, 255, 125);
		m_clrLineV = QColor(255, 0, 0, 125);
		m_clrCenter = QColor(124, 252, 0, 125);
	}
	else if (m_nWndType == ORIIMG_SAGITTAL)
	{
		m_clrLineH = QColor(0, 191, 255, 125);
		m_clrLineV = QColor(124, 252, 0, 125);
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

bool OperateMprLines::GetTrianglePoint(QPoint &p1, QPoint &p2, int iDis, UINT nHeight, UINT nWidth, MprTriAngle &T1, MprTriAngle &T2, bool bUseLeft)
{
	//1、求垂直于p1-p2，且经过 p1,p2的，且与p1-p2距离为nHeight + iDis的点---应该有4个(顶点)
	QPoint ptTopL1, ptTopL2;
	GetVerticalLine(p1, p2, p1, nHeight + iDis, ptTopL1, ptTopL2);
	QPoint ptTopR1, ptTopR2;
	GetVerticalLine(p1, p2, p2, nHeight + iDis, ptTopR1, ptTopR2);

	//2、这回求出4个点，我只要两个(从p1出发到p2，要左侧的点)
	QPoint ptTopL = SelectPtSideLine(p1, p2, ptTopL1, ptTopL2, bUseLeft);//去掉下面的那个点
	QPoint ptTopR = SelectPtSideLine(p1, p2, ptTopR1, ptTopR2, bUseLeft);//去掉下面的那个点

	//3、求垂直于p1-p2，且经过 p1,p2的，且与p1-p2距离为iDis的点---应该有4个(底部中点)
	QPoint ptBottonCenterL1, ptBottonCenterL2;
	GetVerticalLine(p1, p2, p1, iDis, ptBottonCenterL1, ptBottonCenterL2);
	QPoint ptBottonCenterR1, ptBottonCenterR2;
	GetVerticalLine(p1, p2, p2, iDis, ptBottonCenterR1, ptBottonCenterR2);

	QPoint ptBottonCenterL = SelectPtSideLine(p1, p2, ptBottonCenterL1, ptBottonCenterL2, bUseLeft);//去掉下面的那个点
	QPoint ptBottonCenterR = SelectPtSideLine(p1, p2, ptBottonCenterR1, ptBottonCenterR2, bUseLeft);//去掉下面的那个点

	//4、分别以两个底部中点各向两侧求距离为nWidth的两点，共四个点
	QPoint ptBtmLL, ptBtmLR;//左侧三角形的底部两点
	QPoint ptBtmRL, ptBtmRR;//右侧三角形的底部两点
	GetPtByLength(ptBottonCenterL,p1, p2, nWidth, ptBtmLL, ptBtmLR);
	GetPtByLength(ptBottonCenterR,p1, p2, nWidth, ptBtmRL, ptBtmRR);

	//5、完成
	T1.ptTop = ptTopL;	T1.ptBtm1 = ptBtmLL; T1.ptBtm2 = ptBtmLR;
	T2.ptTop = ptTopR;	T2.ptBtm1 = ptBtmRL; T2.ptBtm2 = ptBtmRR;

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
		newPt2.setY(qRound(k *fx2 + b));

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

QPoint OperateMprLines::SelectPtSideLine(QPoint &p1, QPoint &p2, QPoint &newPt1, QPoint &newPt2, bool bUseLeft)
{
	if (p2.x() > p1.x() && p2.y() < p1.y())//p1左下--p2右上
	{

		if (bUseLeft)//留y()小的
			return newPt1.y() < newPt2.y() ? newPt1 : newPt2;
		else
			return newPt1.y() < newPt2.y() ? newPt2 : newPt1;
	}
	else if (p2.x() == p1.x() && p2.y() < p1.y())//垂直，p1在下，p2在上
	{
		if (bUseLeft)//留x()小的
			return newPt1.x() < newPt2.x() ? newPt1 : newPt2;
		else
			return newPt1.x() < newPt2.x() ? newPt2 : newPt1;
	}
	else if (p2.x() < p1.x() && p2.y() < p1.y())//p2左上，p1右下
	{
		if (bUseLeft)//留y()大的
			return newPt1.y() > newPt2.y() ? newPt1 : newPt2;
		else
			return newPt1.y() > newPt2.y() ? newPt2 : newPt1;
	}
	else if (p2.y() == p1.y() && p2.x() < p1.x())//水平，p2在左，p1在右
	{
		if (bUseLeft)//留y()大的
			return newPt1.y() > newPt2.y() ? newPt1 : newPt2;
		else
			return newPt1.y() > newPt2.y() ? newPt2 : newPt1;
	}
	else if (p2.x() < p1.x() && p2.y() > p1.y())//p2左下，p1右上
	{
		if (bUseLeft)//留y()大的
			return newPt1.y() > newPt2.y() ? newPt1 : newPt2;
		else
			return newPt1.y() > newPt2.y() ? newPt2 : newPt1;
	}
	else if (p2.x() == p1.x() && p2.y() > p1.y())//垂直p2在下，p1在上
	{
		if (bUseLeft)//留x()大的
			return newPt1.x() > newPt2.x() ? newPt1 : newPt2;
		else
			return newPt1.x() > newPt2.x() ? newPt2 : newPt1;
	}
	else if (p2.x() > p1.x() && p2.y() > p1.y())//p2右下，p1左上
	{
		if (bUseLeft)//留y()小的
			return newPt1.y() < newPt2.y() ? newPt1 : newPt2;
		else
			return newPt1.y() < newPt2.y() ? newPt2 : newPt1;
	}
	else if (p2.y() == p1.y() && p2.x() > p1.x())//水平p2在右，p1在左
	{
		if (bUseLeft)//留y()小的
			return newPt1.y() < newPt2.y() ? newPt1 : newPt2;
		else
			return newPt1.y() < newPt2.y() ? newPt2 : newPt1;
	}

	return newPt1;
}

bool OperateMprLines::GetPtByLength(QPoint&pCenterPoint, QPoint &p1, QPoint &p2, double nLen, QPoint &RetPtL, QPoint &RetPtR)
{
	//1、先求距离p1处距离为nLen的点
	double X = pCenterPoint.x()*1.00;
	double Y = pCenterPoint.y()*1.00;
	double x1 = p1.x() * 1.00;
	double y1 = p1.y() * 1.00;
	double x2 = p2.x() * 1.00;
	double y2 = p2.y()* 1.00;

	if (p1.x() != p2.x() && p1.y() != p2.y())
	{
		double k = (y2 - y1) / (x2 - x1);
		double b = Y - k * X;
		//两点间距离公式 ：nLen1 = [ sqrt(1 + k*k) ]*abs(? - x1)
		//得到:abs(?-x1) = nLen1/sqrt(1+k*k);
		//得到:? = nLen1/sqrt(1+k*k) + x1; 和 ? = x1 - [nLen1/sqrt(1+k*k)];
		double X1 = nLen / sqrt(1 + k*k) + X;
		double Y1 = k * X1 + b;

		double X2 = X - (nLen / sqrt(1 + k*k));
		double Y2 = k * X2 + b;


		RetPtL.setX(qRound(X1));
		RetPtL.setY(qRound(Y1));

		RetPtR.setX(qRound(X2));
		RetPtR.setY(qRound(Y2));

	}
	if (p1.x() == p2.x())//|
	{
		int Y1 = Y + nLen;
		int Y2 = Y - nLen;

		RetPtL.setX(X);
		RetPtL.setY(Y1);

		RetPtR.setX(X);
		RetPtR.setY(Y2);

	}
	if (p1.y() == p2.y())//-
	{
		int X1 = X + nLen;
		int X2 = X - nLen;

		RetPtL.setX(X1);
		RetPtL.setY(Y);

		RetPtR.setX(X2);
		RetPtR.setY(Y);
	}

	return true;
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
	if (b = false)
		return false;
	retPt = rPt.Int();
	return true;	
}

bool OperateMprLines::GetPointNearLines(QPoint pt)
{
	bool bUpdate = false;
	if (1)//横向几个成员判断
	{
		bool bNear = DistanceToLines(m_pLineH, pt);
		if ((bNear && !m_pLineH->bActive) || (!bNear && m_pLineH->bActive) )
		{
			m_pLineH->bActive = !m_pLineH->bActive;
			bUpdate = true;
		}	
	}

	if (2)
	{
		bool bNear = DistanceToLines(m_pLineV, pt);
		if ((bNear && !m_pLineV->bActive) || (!bNear && m_pLineV->bActive))
		{
			m_pLineV->bActive = !m_pLineV->bActive;
			bUpdate = true;
		}
	}

	return bUpdate;
}

bool OperateMprLines::GetPointNearTriAngle(QPoint pt)
{
	bool bUpdate = false;
	if (1)//横向的几个三角形
	{
		bool bNear = DistanceToTriangle(m_pTriHStartTop, pt);
		if ((bNear && !m_pTriHStartTop->bActive) || (!bNear && m_pTriHStartTop->bActive))
		{
			m_pTriHStartTop->bActive = !m_pTriHStartTop->bActive;
			bUpdate = true;
		}

		bNear = DistanceToTriangle(m_pTriHEndTop, pt);
		if ((bNear && !m_pTriHEndTop->bActive) || (!bNear && m_pTriHEndTop->bActive))
		{
			m_pTriHEndTop->bActive = !m_pTriHEndTop->bActive;
			bUpdate = true;
		}

		if (m_pTriHStartTop->bActive || m_pTriHEndTop->bActive)
			m_pLineHSliceTop->bActive = true;
		
		if (!m_pTriHStartTop->bActive && !m_pTriHEndTop->bActive)
			m_pLineHSliceTop->bActive = false;

		bNear = DistanceToTriangle(m_pTriHStartBottom, pt);
		if ((bNear && !m_pTriHStartBottom->bActive) || (!bNear && m_pTriHStartBottom->bActive))
		{
			m_pTriHStartBottom->bActive = !m_pTriHStartBottom->bActive;
			bUpdate = true;
		}

		bNear = DistanceToTriangle(m_pTriHEndBottom, pt);
		if ((bNear && !m_pTriHEndBottom->bActive) || (!bNear && m_pTriHEndBottom->bActive))
		{
			m_pTriHEndBottom->bActive = !m_pTriHEndBottom->bActive;
			bUpdate = true;
		}

		if (m_pTriHStartBottom->bActive || m_pTriHEndBottom->bActive)
			m_pLineHSliceBottom->bActive = true;

		if (!m_pTriHStartBottom->bActive && !m_pTriHEndBottom->bActive)
			m_pLineHSliceBottom->bActive = false;
	}

	if (2)//纵向的几个三角形
	{
		bool bNear = DistanceToTriangle(m_pTriVStartTop, pt);
		if ((bNear && !m_pTriVStartTop->bActive) || (!bNear && m_pTriVStartTop->bActive))
		{
			m_pTriVStartTop->bActive = !m_pTriVStartTop->bActive;
			bUpdate = true;
		}

		bNear = DistanceToTriangle(m_pTriVEndTop, pt);
		if ((bNear && !m_pTriVEndTop->bActive) || (!bNear && m_pTriVEndTop->bActive))
		{
			m_pTriVEndTop->bActive = !m_pTriVEndTop->bActive;
			bUpdate = true;
		}

		if (m_pTriVStartTop->bActive || m_pTriVEndTop->bActive)
			m_pLineVSliceTop->bActive = true;

		if (!m_pTriVStartTop->bActive && !m_pTriVEndTop->bActive)
			m_pLineVSliceTop->bActive = false;

		bNear = DistanceToTriangle(m_pTriVStartBottom, pt);
		if ((bNear && !m_pTriVStartBottom->bActive) || (!bNear && m_pTriVStartBottom->bActive))
		{
			m_pTriVStartBottom->bActive = !m_pTriVStartBottom->bActive;
			bUpdate = true;
		}

		bNear = DistanceToTriangle(m_pTriVEndBottom, pt);
		if ((bNear && !m_pTriVEndBottom->bActive) || (!bNear && m_pTriVEndBottom->bActive))
		{
			m_pTriVEndBottom->bActive = !m_pTriVEndBottom->bActive;
			bUpdate = true;
		}

		if (m_pTriVStartBottom->bActive || m_pTriVEndBottom->bActive)
			m_pLineVSliceBottom->bActive = true;

		if (!m_pTriVStartBottom->bActive && !m_pTriVEndBottom->bActive)
			m_pLineVSliceBottom->bActive = false;
	}

	return bUpdate;
}

bool OperateMprLines::GetPointNearPoint(QPoint pt)
{
	bool bUpdate = false;

	if (1)//横向
	{
		bool bNear = DistanceToPoint(m_ptHStartRotate, pt, 8);
		if ((bNear && !m_ptHStartRotate->bActive) || (!bNear && m_ptHStartRotate->bActive))
		{
			m_ptHStartRotate->bActive = !m_ptHStartRotate->bActive;
			bUpdate = true;
		}

		bNear = DistanceToPoint(m_ptHEndRotate, pt, 8);
		if ((bNear && !m_ptHEndRotate->bActive) || (!bNear && m_ptHEndRotate->bActive))
		{
			m_ptHEndRotate->bActive = !m_ptHEndRotate->bActive;
			bUpdate = true;
		}

		if (m_ptHStartRotate->bActive || m_ptHEndRotate->bActive)
		{
			m_pLineH->bActive = false;
		}
	}

	if (2)//纵向
	{
		bool bNear = DistanceToPoint(m_ptVStartRotate, pt, 8);
		if ((bNear && !m_ptVStartRotate->bActive) || (!bNear && m_ptVStartRotate->bActive))
		{
			m_ptVStartRotate->bActive = !m_ptVStartRotate->bActive;
			bUpdate = true;
		}

		bNear = DistanceToPoint(m_ptVEndRotate, pt, 8);
		if ((bNear && !m_ptVEndRotate->bActive) || (!bNear && m_ptVEndRotate->bActive))
		{
			m_ptVEndRotate->bActive = !m_ptVEndRotate->bActive;
			bUpdate = true;
		}

		if (m_ptVStartRotate->bActive || m_ptVEndRotate->bActive)
		{
			m_pLineV->bActive = false;
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
			m_pLineV->bActive = false;
			m_pLineH->bActive = false;
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

bool OperateMprLines::DistanceToTriangle(MprTriAngle *pTriangle, QPoint pt)
{
	MprLine *pLineLeft = new MprLine;
	pLineLeft->pt1.WndPt = pTriangle->ptTop;
	pLineLeft->pt2.WndPt = pTriangle->ptBtm1;
	bool bNearLineLeft = DistanceToLines(pLineLeft, pt);
	delete pLineLeft;

	MprLine *pLineBottom = new MprLine;
	pLineBottom->pt1.WndPt = pTriangle->ptBtm1;
	pLineBottom->pt2.WndPt = pTriangle->ptBtm2;
	bool bNearLineBottom = DistanceToLines(pLineBottom, pt);
	delete pLineBottom;

	MprLine *pLineRight = new MprLine;
	pLineRight->pt1.WndPt = pTriangle->ptTop;
	pLineRight->pt2.WndPt = pTriangle->ptBtm2;
	bool bNearLineRight = DistanceToLines(pLineRight, pt);
	delete pLineRight;

	return (bNearLineLeft || bNearLineRight || bNearLineBottom);
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

void OperateMprLines::MoveMainLines(QPoint deltaPt)
{
	if (m_moveObject == HLINE)
	{
		MprLine moveLine;
		moveLine.pt1.WndPt.setX(m_pLineH->pt1.WndPt.x());
		moveLine.pt1.WndPt.setY(m_pLineH->pt1.WndPt.y());
		moveLine.pt2.WndPt.setX(m_pLineH->pt2.WndPt.x());
		moveLine.pt2.WndPt.setY(m_pLineH->pt2.WndPt.y());
		QPoint ptInter;
		if (m_pLineH->pt1.WndPt.y() == m_pLineH->pt2.WndPt.y())
		{
			moveLine.pt1.WndPt.setY(m_pLineH->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setY(m_pLineH->pt2.WndPt.y() + deltaPt.y());
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineH->pt1.WndPt.setY(moveLine.pt1.WndPt.y());
			m_pLineH->pt2.WndPt.setY(moveLine.pt2.WndPt.y());
		}
		else if (m_pLineH->pt1.WndPt.x() == m_pLineH->pt2.WndPt.x())
		{
			moveLine.pt1.WndPt.setX(m_pLineH->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setX(m_pLineH->pt2.WndPt.x() + deltaPt.x());
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineH->pt1.WndPt.setX(moveLine.pt1.WndPt.x());
			m_pLineH->pt2.WndPt.setX(moveLine.pt2.WndPt.x());
		}
		else 
		{
			moveLine.pt1.WndPt.setX(m_pLineH->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt1.WndPt.setY(m_pLineH->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setX(m_pLineH->pt2.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setY(m_pLineH->pt2.WndPt.y() + deltaPt.y());
			MprLine *pNewLineH = new MprLine;
			GetLineAcrossRc(m_rcImgOnWnd, moveLine.pt1.WndPt, moveLine.pt2.WndPt, pNewLineH->pt1.WndPt, pNewLineH->pt2.WndPt);
			GetInterPoint(pNewLineH->pt1.WndPt, pNewLineH->pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
			{
				delete pNewLineH;
				return;
			}
			*m_pLineH = *pNewLineH;
			delete pNewLineH;
		}	
		ReCalInterPoint(m_pLineH, m_ptHStartInter, m_ptHEndInter);
		UpdateRotatePoint(m_pLineH, m_ptHStartRotate, m_ptHEndRotate);
		m_ptCenter->WndPt = ptInter;
		CalSliceLine(m_ptHStartInter, m_ptHEndInter,m_pLineHSliceTop,m_pTriHStartTop,m_pTriHEndTop,m_pLineHSliceBottom,m_pTriHStartBottom,m_pTriHEndBottom);
	}
	else if (m_moveObject == VLINE)
	{
		MprLine moveLine;
		moveLine.pt1.WndPt.setX(m_pLineV->pt1.WndPt.x());
		moveLine.pt1.WndPt.setY(m_pLineV->pt1.WndPt.y());
		moveLine.pt2.WndPt.setX(m_pLineV->pt2.WndPt.x());
		moveLine.pt2.WndPt.setY(m_pLineV->pt2.WndPt.y());
		QPoint ptInter;
		if (m_pLineV->pt1.WndPt.y() == m_pLineV->pt2.WndPt.y())
		{
			moveLine.pt1.WndPt.setY(m_pLineV->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setY(m_pLineV->pt2.WndPt.y() + deltaPt.y());			
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineV->pt1.WndPt.setY(m_pLineV->pt1.WndPt.y() + deltaPt.y());
			m_pLineV->pt2.WndPt.setY(m_pLineV->pt2.WndPt.y() + deltaPt.y());
		}
		else if (m_pLineV->pt1.WndPt.x() == m_pLineV->pt2.WndPt.x())
		{
			moveLine.pt1.WndPt.setX(m_pLineV->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setX(m_pLineV->pt2.WndPt.x() + deltaPt.x());
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineV->pt1.WndPt.setX(m_pLineV->pt1.WndPt.x() + deltaPt.x());
			m_pLineV->pt2.WndPt.setX(m_pLineV->pt2.WndPt.x() + deltaPt.x());
		}
		else
		{
			moveLine.pt1.WndPt.setX(m_pLineV->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt1.WndPt.setY(m_pLineV->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setX(m_pLineV->pt2.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setY(m_pLineV->pt2.WndPt.y() + deltaPt.y());
			MprLine *pNewLineV = new MprLine;
			GetLineAcrossRc(m_rcImgOnWnd, moveLine.pt1.WndPt, moveLine.pt2.WndPt, pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt);
			GetInterPoint(pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
			{
				delete pNewLineV;
				return;
			}
			*m_pLineV = *pNewLineV;
			delete pNewLineV;
		}
		ReCalInterPoint(m_pLineV, m_ptVStartInter, m_ptVEndInter);
		UpdateRotatePoint(m_pLineV, m_ptVStartRotate, m_ptVEndRotate);
		m_ptCenter->WndPt = ptInter;
		CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
	}	
}

void OperateMprLines::MoveCenterPt(QPoint pt)
{
	m_ptCenter->WndPt.setX(pt.x());
	m_ptCenter->WndPt.setY(pt.y());

	if (m_ptCenter->WndPt.x() < m_rcImgOnWnd.left)
		m_ptCenter->WndPt.setX(m_rcImgOnWnd.left);
	if (m_ptCenter->WndPt.x() > m_rcImgOnWnd.right)
		m_ptCenter->WndPt.setX(m_rcImgOnWnd.right);
	if (m_ptCenter->WndPt.y() < m_rcImgOnWnd.top)
		m_ptCenter->WndPt.setY(m_rcImgOnWnd.top);
	if (m_ptCenter->WndPt.y() > m_rcImgOnWnd.bottom)
		m_ptCenter->WndPt.setY(m_rcImgOnWnd.bottom);

	if (1)//H
	{
		GetParalleLine(m_pLineH, m_ptCenter, m_pLineH->pt1, m_pLineH->pt2);
		ReCalInterPoint(m_pLineH, m_ptHStartInter, m_ptHEndInter);
		UpdateRotatePoint(m_pLineH, m_ptHStartRotate, m_ptHEndRotate);
		CalSliceLine(m_ptHStartInter, m_ptHEndInter, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);
	}

	if (2)//V
	{
		GetParalleLine(m_pLineV, m_ptCenter, m_pLineV->pt1, m_pLineV->pt2);
		ReCalInterPoint(m_pLineV, m_ptVStartInter, m_ptVEndInter);
		UpdateRotatePoint(m_pLineV, m_ptVStartRotate, m_ptVEndRotate);
		CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
	}
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

void OperateMprLines::MoveSliceTriangle(QPoint deltaPt)
{
	if (m_moveObject == HTOPSLICE)
	{
		if (m_pLineH->pt1.WndPt.y() == m_pLineH->pt2.WndPt.y())
		{
			m_nSlicePos -= deltaPt.y();
		}
		else if (m_pLineH->pt1.WndPt.x() == m_pLineH->pt2.WndPt.x())
		{
			m_nSlicePos += deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineHSliceTop->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineH, m_nSlicePos);			
		}
	}
	else if (m_moveObject == HBOTTOMSLICE)
	{
		if (m_pLineH->pt1.WndPt.y() == m_pLineH->pt2.WndPt.y())
		{
			m_nSlicePos += deltaPt.y();
		}
		else if (m_pLineH->pt1.WndPt.x() == m_pLineH->pt2.WndPt.x())
		{
			m_nSlicePos -= deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineHSliceBottom->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineH, m_nSlicePos);
		}
	}
	else if (m_moveObject == VTOPSLICE)
	{
		if (m_pLineV->pt1.WndPt.y() == m_pLineV->pt2.WndPt.y())
		{
			m_nSlicePos -= deltaPt.y();
		}
		else if (m_pLineV->pt1.WndPt.x() == m_pLineV->pt2.WndPt.x())
		{
			m_nSlicePos += deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineVSliceTop->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineV, m_nSlicePos);
		}
	}
	else if(m_moveObject == VBOTTOMSLICE)
	{
		if (m_pLineV->pt1.WndPt.y() == m_pLineV->pt2.WndPt.y())
		{
			m_nSlicePos += deltaPt.y();
		}
		else if (m_pLineV->pt1.WndPt.x() == m_pLineV->pt2.WndPt.x())
		{
			m_nSlicePos -= deltaPt.x();
		}
		else
		{
			QPoint pt1 = m_pLineVSliceBottom->pt1.WndPt;
			pt1.setX(pt1.x() + deltaPt.x());
			pt1.setY(pt1.y() + deltaPt.y());

			CalDistanceOfParalleLine(pt1, m_pLineV, m_nSlicePos);
		}
	}

	if (m_nSlicePos < 0)
		m_nSlicePos = 0;
	if (m_nSlicePos > ((m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2))
		m_nSlicePos = (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2;

	CalSliceLine(m_ptHStartInter, m_ptHEndInter, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);
	CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
}

void OperateMprLines::CalSliceLine(MprPoint *pStart, MprPoint *pEnd, MprLine *pTopSlice, MprTriAngle *pTriST, MprTriAngle *pTriET, MprLine *pBottomSlice, MprTriAngle *pTriSB, MprTriAngle *pTriEB)
{
	if (pStart->WndPt.y() == pEnd->WndPt.y())
	{
		pTopSlice->pt1.WndPt.setX(pStart->WndPt.x());
		pTopSlice->pt1.WndPt.setY(pStart->WndPt.y() - m_nSlicePos);
		pTopSlice->pt2.WndPt.setX(pEnd->WndPt.x());
		pTopSlice->pt2.WndPt.setY(pEnd->WndPt.y() - m_nSlicePos);

		pBottomSlice->pt1.WndPt.setX(pStart->WndPt.x());
		pBottomSlice->pt1.WndPt.setY(pStart->WndPt.y() + m_nSlicePos);
		pBottomSlice->pt2.WndPt.setX(pEnd->WndPt.x());
		pBottomSlice->pt2.WndPt.setY(pEnd->WndPt.y() + m_nSlicePos);

		if (pStart->WndPt.x() < pEnd->WndPt.x())
		{
			GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, true);
			GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, false);
		}
		else
		{
			GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, false);
			GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, true);
		}
	}
	else if (pStart->WndPt.x() == pEnd->WndPt.x())
	{
		pTopSlice->pt1.WndPt.setX(pStart->WndPt.x() + m_nSlicePos);
		pTopSlice->pt1.WndPt.setY(pStart->WndPt.y());
		pTopSlice->pt2.WndPt.setX(pEnd->WndPt.x() + m_nSlicePos);
		pTopSlice->pt2.WndPt.setY(pEnd->WndPt.y());

		pBottomSlice->pt1.WndPt.setX(pStart->WndPt.x() - m_nSlicePos);
		pBottomSlice->pt1.WndPt.setY(pStart->WndPt.y());
		pBottomSlice->pt2.WndPt.setX(pEnd->WndPt.x() - m_nSlicePos);
		pBottomSlice->pt2.WndPt.setY(pEnd->WndPt.y());

		if (pStart->WndPt.y() < pEnd->WndPt.y())
		{
			GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, true);
			GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, false);
		}
		else
		{
			GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, false);
			GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, true);
		}
	}
	else
	{
		double k = -(pEnd->WndPt.x() - pStart->WndPt.x()) * 1.00/ (pEnd->WndPt.y() - pStart->WndPt.y());
		if ((k > 0.00 && (pStart->WndPt.x() < pEnd->WndPt.x())) || (k < 0.00 && (pStart->WndPt.x() > pEnd->WndPt.x())))
		{
			GetVerticalLine(pStart->WndPt, pEnd->WndPt, pStart->WndPt, m_nSlicePos, pBottomSlice->pt1.WndPt, pTopSlice->pt1.WndPt);
			GetVerticalLine(pEnd->WndPt, pStart->WndPt, pEnd->WndPt, m_nSlicePos, pBottomSlice->pt2.WndPt, pTopSlice->pt2.WndPt);
		}
		else
		{
			GetVerticalLine(pStart->WndPt, pEnd->WndPt, pStart->WndPt, m_nSlicePos, pTopSlice->pt1.WndPt, pBottomSlice->pt1.WndPt);
			GetVerticalLine(pEnd->WndPt, pStart->WndPt, pEnd->WndPt, m_nSlicePos, pTopSlice->pt2.WndPt, pBottomSlice->pt2.WndPt);
		}
		GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, true);
		GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, false);
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

void OperateMprLines::MoveRotateLine(QPoint pt)
{
	if (m_bActiveOblique == false)
	{
		return;
	}

	if (m_moveObject == HSTARTROTATE || m_moveObject == HENDROTATE)
	{
		if (m_moveObject == HSTARTROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt);
		else if (m_moveObject == HENDROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineH->pt2.WndPt, m_pLineH->pt1.WndPt);
		
		MprLine pNewVLine;
		GetVerticalLine(m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, m_ptCenter->WndPt, 20, pNewVLine.pt1.WndPt, pNewVLine.pt2.WndPt);
		GetLineAcrossRc(m_rcImgOnWnd, pNewVLine.pt1.WndPt, pNewVLine.pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt);

		ReCalInterPoint(m_pLineH, m_ptHStartInter,m_ptHEndInter);
		UpdateRotatePoint(m_pLineH, m_ptHStartRotate, m_ptHEndRotate);
		CalSliceLine(m_ptHStartInter, m_ptHEndInter, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);

		ReCalInterPoint(m_pLineV, m_ptVStartInter, m_ptVEndInter);
		UpdateRotatePoint(m_pLineV, m_ptVStartRotate, m_ptVEndRotate);
		CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
	}

	if (m_moveObject == VSTARTROTATE || m_moveObject == VENDROTATE)
	{
		if (m_moveObject == VSTARTROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt);
		else if (m_moveObject == VENDROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, m_pLineV->pt2.WndPt, m_pLineV->pt1.WndPt);

		MprLine pNewHLine;
		GetVerticalLine(m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, m_ptCenter->WndPt, 20, pNewHLine.pt1.WndPt, pNewHLine.pt2.WndPt);
		GetLineAcrossRc(m_rcImgOnWnd, pNewHLine.pt1.WndPt, pNewHLine.pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt);

		ReCalInterPoint(m_pLineV, m_ptVStartInter, m_ptVEndInter);
		UpdateRotatePoint(m_pLineV, m_ptVStartRotate, m_ptVEndRotate);
		CalSliceLine(m_ptVStartInter, m_ptVEndInter, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);


		ReCalInterPoint(m_pLineH, m_ptHStartInter, m_ptHEndInter);
		UpdateRotatePoint(m_pLineH, m_ptHStartRotate, m_ptHEndRotate);
		CalSliceLine(m_ptHStartInter, m_ptHEndInter, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);
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
		ConvertWndToImg(m_pLineH->pt1);
		ConvertWndToImg(m_pLineH->pt2);
		ConvertWndToImg(*m_ptHStartRotate);
		ConvertWndToImg(*m_ptHEndRotate);
		ConvertWndToImg(*m_ptHStartInter);
		ConvertWndToImg(*m_ptHEndInter);
		ConvertWndToImg(m_pLineHSliceTop->pt1);
		ConvertWndToImg(m_pLineHSliceTop->pt2);
		ConvertWndToImg(m_pLineHSliceBottom->pt1);
		ConvertWndToImg(m_pLineHSliceBottom->pt2);
	}

	if (2)//纵向
	{
		ConvertWndToImg(m_pLineV->pt1);
		ConvertWndToImg(m_pLineV->pt2);
		ConvertWndToImg(*m_ptVStartRotate);
		ConvertWndToImg(*m_ptVEndRotate);
		ConvertWndToImg(*m_ptVStartInter);
		ConvertWndToImg(*m_ptVEndInter);
		ConvertWndToImg(m_pLineVSliceTop->pt1);
		ConvertWndToImg(m_pLineVSliceTop->pt2);
		ConvertWndToImg(m_pLineVSliceBottom->pt1);
		ConvertWndToImg(m_pLineVSliceBottom->pt2);
	}

	if (3)//交点
	{
		ConvertWndToImg(*m_ptCenter);
	}

}
