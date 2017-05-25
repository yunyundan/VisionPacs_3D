#include "stdafx.h"
#include "OperateMprLines.h"


OperateMprLines::OperateMprLines(QObject *parent,int nImgWndType)
	: QObject(parent)
{
	m_bShow = true;
	m_bInitLines = false;
	m_nNearFactor = 5;
	m_bMousePressed = false;
	m_bCenterPoint = false;
	m_moveObject = HLINE;

	m_prePt = QPoint(0, 0);

	m_pLineH = nullptr;
	m_pLineHStartRotate = nullptr;
	m_pLineHEndRotate = nullptr;
	m_pLineHSliceTop = nullptr;
	m_pLineHSliceBottom = nullptr;
	m_pTriHStartTop = nullptr;
	m_pTriHStartBottom = nullptr;
	m_pTriHEndTop = nullptr;
	m_pTriHEndBottom = nullptr;

	m_pLineV = nullptr;
	m_pLineVStartRotate = nullptr;
	m_pLineVEndRotate = nullptr;
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

	m_fSlicePos = 0.00;

	GetLinesColor();

}

OperateMprLines::~OperateMprLines()
{
	if (m_pLineH)
	{
		delete m_pLineH;
		m_pLineH = nullptr;
	}

	if (m_pLineHStartRotate)
	{
		delete m_pLineHStartRotate;
		m_pLineHStartRotate = nullptr;
	}

	if (m_pLineHEndRotate)
	{
		delete m_pLineHEndRotate;
		m_pLineHEndRotate = nullptr;
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

	if (m_pLineVStartRotate)
	{
		delete m_pLineVStartRotate;
		m_pLineVStartRotate = nullptr;
	}

	if (m_pLineVEndRotate)
	{
		delete m_pLineVEndRotate;
		m_pLineVEndRotate = nullptr;
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


bool OperateMprLines::RefreshMprLinesPara(RECT rcImg, int nImgWidth, int nImgHeigh)
{
	if (rcImg.right - rcImg.left <= 0 || rcImg.bottom - rcImg.top <= 0)
		return false;
	if (nImgWidth <= 0 || nImgHeigh <= 0)
		return false;

	m_rcImgOnWnd = rcImg;
	m_nImgW = nImgWidth;
	m_nImgH = nImgHeigh;
	if (m_bInitLines == false)
	{
		if (1) //横向
		{
			int nRotateLineLength = nImgWidth * m_fRotateLineRate;

			m_pLineH = new MprLine;
			m_pLineH->pt1.ImgPt.x = 0.00 + nRotateLineLength;
			m_pLineH->pt1.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineH->pt1);

			m_pLineH->pt2.ImgPt.x = m_nImgW*1.00 - nRotateLineLength;
			m_pLineH->pt2.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineH->pt2);

			m_pLineHStartRotate = new MprLine;
			m_pLineHStartRotate->pt1.ImgPt.x = 0.00;
			m_pLineHStartRotate->pt1.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineHStartRotate->pt1);

			m_pLineHStartRotate->pt2.ImgPt.x = nRotateLineLength;
			m_pLineHStartRotate->pt2.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineHStartRotate->pt2);

			m_pLineHEndRotate = new MprLine;
			m_pLineHEndRotate->pt1.ImgPt.x = m_nImgW*1.00 - nRotateLineLength;
			m_pLineHEndRotate->pt1.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineHEndRotate->pt1);

			m_pLineHEndRotate->pt2.ImgPt.x = m_nImgW*1.00;
			m_pLineHEndRotate->pt2.ImgPt.y = m_nImgH*1.00 / 2;
			ConvertImgToWnd(m_pLineHEndRotate->pt2);

			//横向的层厚线--初始值与主线重合
			m_pLineHSliceTop = new MprLine;
			*m_pLineHSliceTop = *m_pLineH;

			m_pLineHSliceBottom = new MprLine;
			*m_pLineHSliceBottom = *m_pLineH;

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
			int nRotateLineLength = nImgHeigh * m_fRotateLineRate;

			m_pLineV = new MprLine;
			m_pLineV->pt1.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineV->pt1.ImgPt.y = 0.00 + nRotateLineLength;
			ConvertImgToWnd(m_pLineV->pt1);

			m_pLineV->pt2.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineV->pt2.ImgPt.y = m_nImgH*1.00 - nRotateLineLength;
			ConvertImgToWnd(m_pLineV->pt2);

			m_pLineVStartRotate = new MprLine;
			m_pLineVStartRotate->pt1.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineVStartRotate->pt1.ImgPt.y = 0.00;
			ConvertImgToWnd(m_pLineVStartRotate->pt1);

			m_pLineVStartRotate->pt2.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineVStartRotate->pt2.ImgPt.y = nRotateLineLength;
			ConvertImgToWnd(m_pLineVStartRotate->pt2);

			m_pLineVEndRotate = new MprLine;
			m_pLineVEndRotate->pt1.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineVEndRotate->pt1.ImgPt.y = m_nImgH*1.00 - nRotateLineLength;
			ConvertImgToWnd(m_pLineVEndRotate->pt1);

			m_pLineVEndRotate->pt2.ImgPt.x = m_nImgW*1.00 / 2;
			m_pLineVEndRotate->pt2.ImgPt.y = m_nImgH*1.00;
			ConvertImgToWnd(m_pLineVEndRotate->pt2);

			//横向的层厚线--初始值与主线重合
			m_pLineVSliceTop = new MprLine;
			*m_pLineVSliceTop = *m_pLineV;

			m_pLineVSliceBottom = new MprLine;
			*m_pLineVSliceBottom = *m_pLineV;

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
	}
	else
	{
		if (1) //横向
		{
			ConvertImgToWnd(m_pLineH->pt1);
			ConvertImgToWnd(m_pLineH->pt2);
			ConvertImgToWnd(m_pLineHStartRotate->pt1);
			ConvertImgToWnd(m_pLineHStartRotate->pt2);
			ConvertImgToWnd(m_pLineHEndRotate->pt1);
			ConvertImgToWnd(m_pLineHEndRotate->pt2);
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
			ConvertImgToWnd(m_pLineVStartRotate->pt1);
			ConvertImgToWnd(m_pLineVStartRotate->pt2);
			ConvertImgToWnd(m_pLineVEndRotate->pt1);
			ConvertImgToWnd(m_pLineVEndRotate->pt2);
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

	if (m_pLineHStartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = HSTARTROTATE;
		return;
	}

	if (m_pLineHEndRotate->bActive)
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

	if (m_pLineVStartRotate->bActive)
	{
		m_bMousePressed = true;
		m_moveObject = VSTARTROTATE;
		return;
	}

	if (m_pLineVEndRotate->bActive)
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

	if (m_bCenterPoint)
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
		bool bCenterUpdate = GetPointNearCenter(pt);

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
		m_prePt = pt;
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

	if (1)//横向
	{
		int nLinesWidth = 1;

		//主线
		if (m_pLineH->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::SolidLine));
		QPoint ptTmp;
		GetInterPtByLength(m_ptCenter->WndPt, m_pLineH->pt1.WndPt, 10, ptTmp);
		painter->drawLine(m_pLineH->pt1.WndPt, ptTmp);
		GetInterPtByLength(m_ptCenter->WndPt, m_pLineH->pt2.WndPt, 10, ptTmp);
		painter->drawLine(ptTmp,m_pLineH->pt2.WndPt);

		//旋转线
		nLinesWidth = 1;
		if (m_pLineHStartRotate->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::DashLine));
		painter->drawLine(m_pLineHStartRotate->pt1.WndPt, m_pLineHStartRotate->pt2.WndPt);

		nLinesWidth = 1;
		if (m_pLineHEndRotate->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::DashLine));
		painter->drawLine(m_pLineHEndRotate->pt1.WndPt, m_pLineHEndRotate->pt2.WndPt);

		//连接处圆形
		painter->setPen(QPen(m_clrLineH, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineH);
		painter->drawEllipse(m_pLineHStartRotate->pt2.WndPt, 2, 2);
		painter->drawEllipse(m_pLineHEndRotate->pt1.WndPt, 2, 2);

		//层厚线
		if (m_fSlicePos != 0.00)
		{
			nLinesWidth = 1;
			painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::DashLine));
			painter->drawLine(m_pLineH->pt1.WndPt, m_pLineHSliceTop->pt1.WndPt);
			painter->drawLine(m_pLineHSliceTop->pt2.WndPt, m_pLineH->pt2.WndPt);
			if (m_pLineHSliceTop->bActive == true)
				nLinesWidth = 2;
			painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::DashLine));
			QPoint pEnd1; GetInterPoint(m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, pEnd1);
			painter->drawLine(m_pLineHSliceTop->pt1.WndPt, pEnd1);
			QPoint pStart1; GetInterPoint(m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, pStart1);
			painter->drawLine(pStart1, m_pLineHSliceTop->pt2.WndPt);
			
			nLinesWidth = 1;
			painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::DashLine));
			painter->drawLine(m_pLineH->pt1.WndPt, m_pLineHSliceBottom->pt1.WndPt);
			painter->drawLine(m_pLineHSliceBottom->pt2.WndPt, m_pLineH->pt2.WndPt);
			if (m_pLineHSliceBottom->bActive == true)
				nLinesWidth = 2;
			painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::DashLine));
			GetInterPoint(m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, pEnd1);
			painter->drawLine(m_pLineHSliceBottom->pt1.WndPt, pEnd1);
			GetInterPoint(m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, pStart1);
			painter->drawLine(pStart1, m_pLineHSliceBottom->pt2.WndPt);
		}

		//三角形
		nLinesWidth = 1;
		painter->setBrush(Qt::NoBrush);
		if (m_pTriHStartTop->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::SolidLine));
		const QPointF points[3] = { m_pTriHStartTop->ptBtm1,m_pTriHStartTop->ptBtm2,m_pTriHStartTop->ptTop };
		painter->drawPolygon(points, 3);

		nLinesWidth = 1;
		if (m_pTriHEndTop->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::SolidLine));
		const QPointF points1[3] = { m_pTriHEndTop->ptBtm1,m_pTriHEndTop->ptBtm2,m_pTriHEndTop->ptTop };
		painter->drawPolygon(points1, 3);

		nLinesWidth = 1;
		if (m_pTriHEndBottom->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::SolidLine));
		const QPointF points2[3] = { m_pTriHEndBottom->ptBtm1,m_pTriHEndBottom->ptBtm2,m_pTriHEndBottom->ptTop };
		painter->drawPolygon(points2, 3);

		nLinesWidth = 1;
		if (m_pTriHStartBottom->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineH, nLinesWidth, Qt::SolidLine));
		const QPointF points3[3] = { m_pTriHStartBottom->ptBtm1,m_pTriHStartBottom->ptBtm2,m_pTriHStartBottom->ptTop };
		painter->drawPolygon(points3, 3);
	}

	if (2)//纵向
	{
		int nLinesWidth = 1;

		//主线
		if (m_pLineV->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::SolidLine));
		QPoint ptTmp;
		GetInterPtByLength(m_ptCenter->WndPt, m_pLineV->pt1.WndPt, 10, ptTmp);
		painter->drawLine(m_pLineV->pt1.WndPt, ptTmp);
		GetInterPtByLength(m_ptCenter->WndPt, m_pLineV->pt2.WndPt, 10, ptTmp);
		painter->drawLine(ptTmp,m_pLineV->pt2.WndPt);
		
		//旋转线
		nLinesWidth = 1;
		if (m_pLineVStartRotate->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::DashLine));
		painter->drawLine(m_pLineVStartRotate->pt1.WndPt, m_pLineVStartRotate->pt2.WndPt);

		nLinesWidth = 1;
		if (m_pLineVEndRotate->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::DashLine));
		painter->drawLine(m_pLineVEndRotate->pt1.WndPt, m_pLineVEndRotate->pt2.WndPt);

		//连接处圆形
		painter->setPen(QPen(m_clrLineV, 1, Qt::SolidLine));
		painter->setBrush(m_clrLineV);
		painter->drawEllipse(m_pLineVStartRotate->pt2.WndPt, 2, 2);
		painter->drawEllipse(m_pLineVEndRotate->pt1.WndPt, 2, 2);

		//层厚线
		if (m_fSlicePos != 0.00)
		{
			nLinesWidth = 1;
			painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::DashLine));
			painter->drawLine(m_pLineV->pt1.WndPt, m_pLineVSliceTop->pt1.WndPt);
			painter->drawLine(m_pLineVSliceTop->pt2.WndPt, m_pLineV->pt2.WndPt);
			if (m_pLineVSliceTop->bActive == true)
				nLinesWidth = 2;
			painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::DashLine));
			QPoint pEnd1; GetInterPoint(m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, pEnd1);
			painter->drawLine(m_pLineVSliceTop->pt1.WndPt, pEnd1);
			QPoint pStart1; GetInterPoint(m_pLineVSliceTop->pt1.WndPt, m_pLineVSliceTop->pt2.WndPt, m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, pStart1);
			painter->drawLine(pStart1, m_pLineVSliceTop->pt2.WndPt);

			nLinesWidth = 1;
			painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::DashLine));
			painter->drawLine(m_pLineV->pt1.WndPt, m_pLineVSliceBottom->pt1.WndPt);
			painter->drawLine(m_pLineVSliceBottom->pt2.WndPt, m_pLineV->pt2.WndPt);
			if (m_pLineVSliceBottom->bActive == true)
				nLinesWidth = 2;
			painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::DashLine));
			GetInterPoint(m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, m_pLineHSliceTop->pt1.WndPt, m_pLineHSliceTop->pt2.WndPt, pEnd1);
			painter->drawLine(m_pLineVSliceBottom->pt1.WndPt, pEnd1);
			GetInterPoint(m_pLineVSliceBottom->pt1.WndPt, m_pLineVSliceBottom->pt2.WndPt, m_pLineHSliceBottom->pt1.WndPt, m_pLineHSliceBottom->pt2.WndPt, pStart1);
			painter->drawLine(pStart1, m_pLineVSliceBottom->pt2.WndPt);
		}

		//三角形
		nLinesWidth = 1;
		painter->setBrush(Qt::NoBrush);
		if (m_pTriVStartTop->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::SolidLine));
		const QPointF points[3] = { m_pTriVStartTop->ptBtm1,m_pTriVStartTop->ptBtm2,m_pTriVStartTop->ptTop };
		painter->drawPolygon(points, 3);

		nLinesWidth = 1;
		if (m_pTriVStartBottom->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::SolidLine));
		const QPointF points1[3] = { m_pTriVStartBottom->ptBtm1,m_pTriVStartBottom->ptBtm2,m_pTriVStartBottom->ptTop };
		painter->drawPolygon(points1, 3);

		nLinesWidth = 1;
		if (m_pTriVEndTop->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::SolidLine));
		const QPointF points2[3] = { m_pTriVEndTop->ptBtm1,m_pTriVEndTop->ptBtm2,m_pTriVEndTop->ptTop };
		painter->drawPolygon(points2, 3);

		nLinesWidth = 1;
		if (m_pTriVEndBottom->bActive == true)
			nLinesWidth = 2;
		painter->setPen(QPen(m_clrLineV, nLinesWidth, Qt::SolidLine));
		const QPointF points3[3] = { m_pTriVEndBottom->ptBtm1,m_pTriVEndBottom->ptBtm2,m_pTriVEndBottom->ptTop };
		painter->drawPolygon(points3, 3);
	}

	if (3)
	{
		int nLinesWidth = 1;
		if (m_bCenterPoint)
			nLinesWidth = 2;
		painter->setPen(QPen(QColor(255, 255, 0), nLinesWidth, Qt::SolidLine));
		painter->setBrush(Qt::NoBrush);
		painter->drawEllipse(m_ptCenter->WndPt, 2, 2);
	}

}

void OperateMprLines::GetLinesColor()
{
	if (m_nWndType == ORIIMG_AXIAL)
	{
		m_clrLineH = QColor(124, 252, 0);
		m_clrLineV = QColor(255, 0, 0);
	}
	else if(m_nWndType == ORIIMG_CORONAL)
	{
		m_clrLineH = QColor(0, 191, 255);
		m_clrLineV = QColor(255, 0, 0);
	}
	else if (m_nWndType == ORIIMG_SAGITTAL)
	{
		m_clrLineH = QColor(0, 191, 255);
		m_clrLineV = QColor(124, 252, 0);
	}
}

bool OperateMprLines::ConvertImgToWnd(MprPoint &pt)
{
	if (m_nImgW == 0 || m_nImgH == 0)
		return false;

	double fx = (m_rcImgOnWnd.right - m_rcImgOnWnd.left)*1.00 / m_nImgW;
	pt.WndPt.setX(fx * pt.ImgPt.x + m_rcImgOnWnd.left);

	double fy = (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top)*1.00 / m_nImgH;
	pt.WndPt.setY(fy * pt.ImgPt.y + m_rcImgOnWnd.top);

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

		double fx1 = sqrt(nLen*nLen*1.00 / (1 + k*k)) + PublicPt.x();
		newPt1.setX(int(fx1));

		double fx2 = PublicPt.x() - sqrt(nLen*nLen*1.00 / (1 + k*k));
		newPt2.setX(int(fx2));

		newPt1.setY(k *fx1 + b);
		newPt2.setY(k *fx2 + b);

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


		RetPtL.setX(X1);
		RetPtL.setY(Y1);

		RetPtR.setX(X2);
		RetPtR.setY(Y2);

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
		
		bNear = DistanceToLines(m_pLineHStartRotate, pt);
		if ((bNear && !m_pLineHStartRotate->bActive) || (!bNear && m_pLineHStartRotate->bActive))
		{
			m_pLineHStartRotate->bActive = !m_pLineHStartRotate->bActive;
			bUpdate = true;
		}

		bNear = DistanceToLines(m_pLineHEndRotate, pt);
		if ((bNear && !m_pLineHEndRotate->bActive) || (!bNear && m_pLineHEndRotate->bActive))
		{
			m_pLineHEndRotate->bActive = !m_pLineHEndRotate->bActive;
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

		bNear = DistanceToLines(m_pLineVStartRotate, pt);
		if ((bNear && !m_pLineVStartRotate->bActive) || (!bNear && m_pLineVStartRotate->bActive))
		{
			m_pLineVStartRotate->bActive = !m_pLineVStartRotate->bActive;
			bUpdate = true;
		}

		bNear = DistanceToLines(m_pLineVEndRotate, pt);
		if ((bNear && !m_pLineVEndRotate->bActive) || (!bNear && m_pLineVEndRotate->bActive))
		{
			m_pLineVEndRotate->bActive = !m_pLineVEndRotate->bActive;
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

bool OperateMprLines::GetPointNearCenter(QPoint pt)
{
	bool bUpdate = false;

	MprPoint ptLT;
	ptLT.WndPt.setX(m_ptCenter->WndPt.x() - 8);
	ptLT.WndPt.setY(m_ptCenter->WndPt.y() - 8);

	MprPoint ptLB;
	ptLB.WndPt.setX(m_ptCenter->WndPt.x() - 8);
	ptLB.WndPt.setY(m_ptCenter->WndPt.y() + 8);

	MprPoint ptRT;
	ptRT.WndPt.setX(m_ptCenter->WndPt.x() + 8);
	ptRT.WndPt.setY(m_ptCenter->WndPt.y() - 8);

	MprPoint ptRB;
	ptRB.WndPt.setX(m_ptCenter->WndPt.x() + 8);
	ptRB.WndPt.setY(m_ptCenter->WndPt.y() + 8);

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
	
	bool bNearRect = (DistanceToLines(lineLeft,pt) || DistanceToLines(lineRight,pt) || DistanceToLines(lineTop,pt) || DistanceToLines(lineBottom,pt));

	delete lineLeft;
	delete lineBottom;
	delete lineRight;
	delete lineTop;

	bool bInRect = false;
	if (pt.x() > ptLT.WndPt.x() && pt.x() < ptRT.WndPt.x() && pt.y() > ptLT.WndPt.y() && pt.y() < ptLB.WndPt.y())
		bInRect = true;

	bool bNear = bNearRect || bInRect;
	if ((bNear && !m_bCenterPoint) || (!bNear && m_bCenterPoint) )
	{
		m_bCenterPoint = !m_bCenterPoint;
		bUpdate = true;
	}

	if (m_bCenterPoint)
	{
		m_pLineV->bActive = false;
		m_pLineH->bActive = false;
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
		if (m_pLineH->pt1.WndPt.y() == m_pLineH->pt2.WndPt.y())
		{
			moveLine.pt1.WndPt.setY(m_pLineH->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setY(m_pLineH->pt2.WndPt.y() + deltaPt.y());
			QPoint ptInter;
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineH->pt1.WndPt.setY(moveLine.pt1.WndPt.y());
			m_pLineH->pt2.WndPt.setY(moveLine.pt2.WndPt.y());
			m_pLineHStartRotate->pt1.WndPt.setY(m_pLineHStartRotate->pt1.WndPt.y() + deltaPt.y());
			m_pLineHStartRotate->pt2.WndPt.setY(m_pLineHStartRotate->pt2.WndPt.y() + deltaPt.y());
			m_pLineHEndRotate->pt1.WndPt.setY(m_pLineHEndRotate->pt1.WndPt.y() + deltaPt.y());
			m_pLineHEndRotate->pt2.WndPt.setY(m_pLineHEndRotate->pt2.WndPt.y() + deltaPt.y());
			m_ptCenter->WndPt = ptInter;

		}
		else if (m_pLineH->pt1.WndPt.x() == m_pLineH->pt2.WndPt.x())
		{
			moveLine.pt1.WndPt.setX(m_pLineH->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setX(m_pLineH->pt2.WndPt.x() + deltaPt.x());
			QPoint ptInter;
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineH->pt1.WndPt.setX(moveLine.pt1.WndPt.x());
			m_pLineH->pt2.WndPt.setX(moveLine.pt2.WndPt.x());
			m_pLineHStartRotate->pt1.WndPt.setX(m_pLineHStartRotate->pt1.WndPt.x() + deltaPt.x());
			m_pLineHStartRotate->pt2.WndPt.setX(m_pLineHStartRotate->pt2.WndPt.x() + deltaPt.x());
			m_pLineHEndRotate->pt1.WndPt.setX(m_pLineHEndRotate->pt1.WndPt.x() + deltaPt.x());
			m_pLineHEndRotate->pt2.WndPt.setX(m_pLineHEndRotate->pt2.WndPt.x() + deltaPt.x());
			m_ptCenter->WndPt = ptInter;
		}
		else 
		{
			moveLine.pt1.WndPt.setX(m_pLineH->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt1.WndPt.setY(m_pLineH->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setX(m_pLineH->pt2.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setY(m_pLineH->pt2.WndPt.y() + deltaPt.y());
			MprLine *pNewLineH = new MprLine;
			GetLineAcrossRc(m_rcImgOnWnd, moveLine.pt1.WndPt, moveLine.pt2.WndPt, pNewLineH->pt1.WndPt, pNewLineH->pt2.WndPt);
			QPoint ptInter;
			GetInterPoint(pNewLineH->pt1.WndPt, pNewLineH->pt2.WndPt, m_pLineV->pt1.WndPt, m_pLineV->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
			{
				delete pNewLineH;
				return;
			}
			ReCalMainAndRotateLines(pNewLineH, m_pLineH, m_pLineHStartRotate, m_pLineHEndRotate);
			m_ptCenter->WndPt = ptInter;
			delete pNewLineH;
		}	
		CalSliceLine(m_pLineH,m_pLineHSliceTop,m_pTriHStartTop,m_pTriHEndTop,m_pLineHSliceBottom,m_pTriHStartBottom,m_pTriHEndBottom);
	}
	else if (m_moveObject == VLINE)
	{
		MprLine moveLine;
		moveLine.pt1.WndPt.setX(m_pLineV->pt1.WndPt.x());
		moveLine.pt1.WndPt.setY(m_pLineV->pt1.WndPt.y());
		moveLine.pt2.WndPt.setX(m_pLineV->pt2.WndPt.x());
		moveLine.pt2.WndPt.setY(m_pLineV->pt2.WndPt.y());
		if (m_pLineV->pt1.WndPt.y() == m_pLineV->pt2.WndPt.y())
		{
			moveLine.pt1.WndPt.setY(m_pLineV->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setY(m_pLineV->pt2.WndPt.y() + deltaPt.y());
			QPoint ptInter;
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineV->pt1.WndPt.setY(m_pLineV->pt1.WndPt.y() + deltaPt.y());
			m_pLineV->pt2.WndPt.setY(m_pLineV->pt2.WndPt.y() + deltaPt.y());
			m_pLineVStartRotate->pt1.WndPt.setY(m_pLineVStartRotate->pt1.WndPt.y() + deltaPt.y());
			m_pLineVStartRotate->pt2.WndPt.setY(m_pLineVStartRotate->pt2.WndPt.y() + deltaPt.y());
			m_pLineVEndRotate->pt1.WndPt.setY(m_pLineVEndRotate->pt1.WndPt.y() + deltaPt.y());
			m_pLineVEndRotate->pt2.WndPt.setY(m_pLineVEndRotate->pt2.WndPt.y() + deltaPt.y());
			m_ptCenter->WndPt = ptInter;
		}
		else if (m_pLineV->pt1.WndPt.x() == m_pLineV->pt2.WndPt.x())
		{
			moveLine.pt1.WndPt.setX(m_pLineV->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setX(m_pLineV->pt2.WndPt.x() + deltaPt.x());
			QPoint ptInter;
			GetInterPoint(moveLine.pt1.WndPt, moveLine.pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
				return;
			m_pLineV->pt1.WndPt.setX(m_pLineV->pt1.WndPt.x() + deltaPt.x());
			m_pLineV->pt2.WndPt.setX(m_pLineV->pt2.WndPt.x() + deltaPt.x());
			m_pLineVStartRotate->pt1.WndPt.setX(m_pLineVStartRotate->pt1.WndPt.x() + deltaPt.x());
			m_pLineVStartRotate->pt2.WndPt.setX(m_pLineVStartRotate->pt2.WndPt.x() + deltaPt.x());
			m_pLineVEndRotate->pt1.WndPt.setX(m_pLineVEndRotate->pt1.WndPt.x() + deltaPt.x());
			m_pLineVEndRotate->pt2.WndPt.setX(m_pLineVEndRotate->pt2.WndPt.x() + deltaPt.x());
			m_ptCenter->WndPt = ptInter;
		}
		else
		{
			moveLine.pt1.WndPt.setX(m_pLineV->pt1.WndPt.x() + deltaPt.x());
			moveLine.pt1.WndPt.setY(m_pLineV->pt1.WndPt.y() + deltaPt.y());
			moveLine.pt2.WndPt.setX(m_pLineV->pt2.WndPt.x() + deltaPt.x());
			moveLine.pt2.WndPt.setY(m_pLineV->pt2.WndPt.y() + deltaPt.y());
			MprLine *pNewLineV = new MprLine;
			GetLineAcrossRc(m_rcImgOnWnd, moveLine.pt1.WndPt, moveLine.pt2.WndPt, pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt);
			QPoint ptInter;
			GetInterPoint(pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt, m_pLineH->pt1.WndPt, m_pLineH->pt2.WndPt, ptInter);
			if (IsPointInRc(m_rcImgOnWnd, ptInter) == false)
			{
				delete pNewLineV;
				return;
			}
			ReCalMainAndRotateLines(pNewLineV, m_pLineV, m_pLineVStartRotate, m_pLineVEndRotate);
			m_ptCenter->WndPt = ptInter;
			delete pNewLineV;
		}
		CalSliceLine(m_pLineV, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
	}	
}

void OperateMprLines::MoveCenterPt(int hPos, int vPos)
{

}

void OperateMprLines::MoveSliceTriangle(QPoint deltaPt)
{
	if (m_moveObject == HTOPSLICE)
	{
		if (m_pLineH->pt1.WndPt.y() == m_pLineH->pt2.WndPt.y())
		{
			m_fSlicePos -= deltaPt.y();
		}
		else if (m_pLineH->pt1.WndPt.x() == m_pLineH->pt2.WndPt.x())
		{
			m_fSlicePos += deltaPt.x();
		}
		else
		{
			m_fSlicePos = sqrt(deltaPt.x()*deltaPt.x() + deltaPt.y()*deltaPt.y());
		}
	}
	else if (m_moveObject == HBOTTOMSLICE)
	{
		if (m_pLineH->pt1.WndPt.y() == m_pLineH->pt2.WndPt.y())
		{
			m_fSlicePos += deltaPt.y();
		}
		else if (m_pLineH->pt1.WndPt.x() == m_pLineH->pt2.WndPt.x())
		{
			m_fSlicePos -= deltaPt.x();
		}
		else
		{
			m_fSlicePos = sqrt(deltaPt.x()*deltaPt.x() + deltaPt.y()*deltaPt.y());
		}
	}
	else if (m_moveObject == VTOPSLICE)
	{
		if (m_pLineV->pt1.WndPt.y() == m_pLineV->pt2.WndPt.y())
		{
			m_fSlicePos -= deltaPt.y();
		}
		else if (m_pLineV->pt1.WndPt.x() == m_pLineV->pt2.WndPt.x())
		{
			m_fSlicePos += deltaPt.x();
		}
		else
		{
			m_fSlicePos = sqrt(deltaPt.x()*deltaPt.x() + deltaPt.y()*deltaPt.y());
		}
	}
	else if(m_moveObject == VBOTTOMSLICE)
	{
		if (m_pLineV->pt1.WndPt.y() == m_pLineV->pt2.WndPt.y())
		{
			m_fSlicePos += deltaPt.y();
		}
		else if (m_pLineV->pt1.WndPt.x() == m_pLineV->pt2.WndPt.x())
		{
			m_fSlicePos -= deltaPt.x();
		}
		else
		{
			m_fSlicePos = sqrt(deltaPt.x()*deltaPt.x() + deltaPt.y()*deltaPt.y());
		}
	}

	if (m_fSlicePos < 0)
		m_fSlicePos = 0;
	if (m_fSlicePos > ((m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2))
		m_fSlicePos = (m_rcImgOnWnd.bottom - m_rcImgOnWnd.top) / 2;

	CalSliceLine(m_pLineH, m_pLineHSliceTop, m_pTriHStartTop, m_pTriHEndTop, m_pLineHSliceBottom, m_pTriHStartBottom, m_pTriHEndBottom);
	CalSliceLine(m_pLineV, m_pLineVSliceTop, m_pTriVStartTop, m_pTriVEndTop, m_pLineVSliceBottom, m_pTriVStartBottom, m_pTriVEndBottom);
}

void OperateMprLines::CalSliceLine(MprLine *pMainLine, MprLine *pTopSlice, MprTriAngle *pTriST, MprTriAngle *pTriET, MprLine *pBottomSlice, MprTriAngle *pTriSB, MprTriAngle *pTriEB)
{
	if (pMainLine->pt1.WndPt.y() == pMainLine->pt2.WndPt.y())
	{
		pTopSlice->pt1.WndPt.setX(pMainLine->pt1.WndPt.x());
		pTopSlice->pt1.WndPt.setY(pMainLine->pt1.WndPt.y() - m_fSlicePos);
		pTopSlice->pt2.WndPt.setX(pMainLine->pt2.WndPt.x());
		pTopSlice->pt2.WndPt.setY(pMainLine->pt2.WndPt.y() - m_fSlicePos);
		GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, true);

		pBottomSlice->pt1.WndPt.setX(pMainLine->pt1.WndPt.x());
		pBottomSlice->pt1.WndPt.setY(pMainLine->pt1.WndPt.y() + m_fSlicePos);
		pBottomSlice->pt2.WndPt.setX(pMainLine->pt2.WndPt.x());
		pBottomSlice->pt2.WndPt.setY(pMainLine->pt2.WndPt.y() + m_fSlicePos);
		GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, false);
	}
	else if (pMainLine->pt1.WndPt.x() == pMainLine->pt2.WndPt.x())
	{
		pTopSlice->pt1.WndPt.setX(pMainLine->pt1.WndPt.x() + m_fSlicePos);
		pTopSlice->pt1.WndPt.setY(pMainLine->pt1.WndPt.y());
		pTopSlice->pt2.WndPt.setX(pMainLine->pt2.WndPt.x() + m_fSlicePos);
		pTopSlice->pt2.WndPt.setY(pMainLine->pt2.WndPt.y());
		GetTrianglePoint(pTopSlice->pt1.WndPt, pTopSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriST, *pTriET, true);

		pBottomSlice->pt1.WndPt.setX(pMainLine->pt1.WndPt.x() - m_fSlicePos);
		pBottomSlice->pt1.WndPt.setY(pMainLine->pt1.WndPt.y());
		pBottomSlice->pt2.WndPt.setX(pMainLine->pt2.WndPt.x() - m_fSlicePos);
		pBottomSlice->pt2.WndPt.setY(pMainLine->pt2.WndPt.y());
		GetTrianglePoint(pBottomSlice->pt1.WndPt, pBottomSlice->pt2.WndPt, m_nTriAngleDis, m_nTriAngleWidth, m_nTriAngleWidth / 2, *pTriSB, *pTriEB, false);
	}
	else
	{

	}
}

void OperateMprLines::MoveRotateLine(QPoint pt)
{
	if (m_moveObject == HSTARTROTATE || m_moveObject == HENDROTATE)
	{
		MprLine *pNewLineH = new MprLine;
		if (m_moveObject == HSTARTROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, pNewLineH->pt1.WndPt, pNewLineH->pt2.WndPt);
		else if (m_moveObject == HENDROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, pNewLineH->pt2.WndPt, pNewLineH->pt1.WndPt);

		ReCalMainAndRotateLines(pNewLineH, m_pLineH, m_pLineHStartRotate, m_pLineHEndRotate);
		delete pNewLineH;
	}

	if (m_moveObject == VSTARTROTATE || m_moveObject == VENDROTATE)
	{
		MprLine *pNewLineV = new MprLine;
		if (m_moveObject == VSTARTROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, pNewLineV->pt1.WndPt, pNewLineV->pt2.WndPt);
		else if (m_moveObject == VENDROTATE)
			GetLineAcrossRc(m_rcImgOnWnd, pt, m_ptCenter->WndPt, pNewLineV->pt2.WndPt, pNewLineV->pt1.WndPt);

		ReCalMainAndRotateLines(pNewLineV, m_pLineV, m_pLineVStartRotate, m_pLineVEndRotate);
		delete pNewLineV;
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

bool OperateMprLines::ReCalMainAndRotateLines(MprLine *pNewLine, MprLine *pMainLine, MprLine *pStartRotate, MprLine *pEndRotate)
{
	int x1 = pNewLine->pt1.WndPt.x();
	int y1 = pNewLine->pt1.WndPt.y();
	int x2 = pNewLine->pt2.WndPt.x();
	int y2 = pNewLine->pt2.WndPt.y();
	if (y1 == y2)
	{
		int nRotateLineLength = abs(pNewLine->pt2.WndPt.x() - pNewLine->pt1.WndPt.x()) * m_fRotateLineRate;
		if (x1 < x2)
		{
			pMainLine->pt1.WndPt.setX(x1 + nRotateLineLength);
			pMainLine->pt1.WndPt.setY(y1);
			pMainLine->pt2.WndPt.setX(x2 - nRotateLineLength);
			pMainLine->pt2.WndPt.setY(y2);
			pStartRotate->pt1.WndPt.setX(x1);
			pStartRotate->pt1.WndPt.setY(y1);
			pStartRotate->pt2.WndPt.setX(x1 + nRotateLineLength);
			pStartRotate->pt2.WndPt.setY(y1);
			pEndRotate->pt1.WndPt.setX(x2 - nRotateLineLength);
			pEndRotate->pt1.WndPt.setY(y2);
			pEndRotate->pt2.WndPt.setX(x2);
			pEndRotate->pt2.WndPt.setY(y2);
		}
		else 
		{
			pMainLine->pt1.WndPt.setX(x1 - nRotateLineLength);
			pMainLine->pt1.WndPt.setY(y1);
			pMainLine->pt2.WndPt.setX(x2 + nRotateLineLength);
			pMainLine->pt2.WndPt.setY(y2);
			pStartRotate->pt1.WndPt.setX(x1);
			pStartRotate->pt1.WndPt.setY(y1);
			pStartRotate->pt2.WndPt.setX(x1 - nRotateLineLength);
			pStartRotate->pt2.WndPt.setY(y1);
			pEndRotate->pt1.WndPt.setX(x2 + nRotateLineLength);
			pEndRotate->pt1.WndPt.setY(y2);
			pEndRotate->pt2.WndPt.setX(x2);
			pEndRotate->pt2.WndPt.setY(y2);
		}
	}
	else if (x1 == x2)
	{
		int nRotateLineLength = abs(pNewLine->pt2.WndPt.y() - pNewLine->pt1.WndPt.y()) * m_fRotateLineRate;
		if (y1 < y2)
		{
			pMainLine->pt1.WndPt.setX(x1);
			pMainLine->pt1.WndPt.setY(y1 + nRotateLineLength);
			pMainLine->pt2.WndPt.setX(x2);
			pMainLine->pt2.WndPt.setY(y2 - nRotateLineLength);
			pStartRotate->pt1.WndPt.setX(x1);
			pStartRotate->pt1.WndPt.setY(y1);
			pStartRotate->pt2.WndPt.setX(x1);
			pStartRotate->pt2.WndPt.setY(y1 + nRotateLineLength);
			pEndRotate->pt1.WndPt.setX(x2);
			pEndRotate->pt1.WndPt.setY(y2 - nRotateLineLength);
			pEndRotate->pt2.WndPt.setX(x2);
			pEndRotate->pt2.WndPt.setY(y2);
		}
		else
		{
			pMainLine->pt1.WndPt.setX(x1);
			pMainLine->pt1.WndPt.setY(y1 - nRotateLineLength);
			pMainLine->pt2.WndPt.setX(x2);
			pMainLine->pt2.WndPt.setY(y2 + nRotateLineLength);
			pStartRotate->pt1.WndPt.setX(x1);
			pStartRotate->pt1.WndPt.setY(y1);
			pStartRotate->pt2.WndPt.setX(x1);
			pStartRotate->pt2.WndPt.setY(y1 - nRotateLineLength);
			pEndRotate->pt1.WndPt.setX(x2);
			pEndRotate->pt1.WndPt.setY(y2 + nRotateLineLength);
			pEndRotate->pt2.WndPt.setX(x2);
			pEndRotate->pt2.WndPt.setY(y2);
		}
	}
	else
	{
		int nRotateLineLength = sqrt(pow((pMainLine->pt2.WndPt.x() - pMainLine->pt1.WndPt.x()), 2) + pow((pMainLine->pt2.WndPt.y() - pMainLine->pt1.WndPt.y()), 2)) * m_fRotateLineRate;
		
		QPoint pTempStart, pTempEnd;
		GetInterPtByLength(pNewLine->pt1.WndPt, pNewLine->pt2.WndPt, nRotateLineLength, pTempStart);
		GetInterPtByLength(pNewLine->pt2.WndPt, pNewLine->pt1.WndPt, nRotateLineLength, pTempEnd);

		pMainLine->pt1.WndPt = pTempStart;
		pMainLine->pt2.WndPt = pTempEnd;
		pStartRotate->pt1.WndPt = pNewLine->pt1.WndPt;
		pStartRotate->pt2.WndPt = pTempStart;
		pEndRotate->pt1.WndPt = pTempEnd;
		pEndRotate->pt2.WndPt = pNewLine->pt2.WndPt;
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
	if (pt.x() > rc.left && pt.x() < rc.right && pt.y() > rc.top && pt.y() < rc.bottom)
		return true;
	return false;
}
