#include "stdafx.h"
#include "ImageWnd.h"
#include "ui_ImageWnd.h"

#include "HsImage.h"


extern RECT GetShowRcByImgSize(RECT rc, double ImgWidth, double ImgHeight);

ImageWnd::ImageWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWnd)
	, m_bFocused(false)
	, m_pImg(NULL)
	, m_pQImage(NULL)
	, m_pPixmap(NULL)
	, m_nLeftButtonInteractionStyle(LOCTION_INTERACTION)
	, m_nRightButtonInteractionStyle(WINDOW_LEVEL_INTERACTION)
{
    ui->setupUi(this);
}

ImageWnd::~ImageWnd()
{
    delete ui;

	if (m_pPixmap)
		delete m_pPixmap;

	//if (m_pImg)
	//{
	//	delete m_pImg;
	//	m_pImg = NULL;
	//}

	if (m_pQImage)
	{
		delete m_pQImage;
		m_pQImage = NULL;
	}
}

void ImageWnd::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	if (m_pPixmap == NULL)
		m_pPixmap = new QPixmap(size());
	m_pPixmap->fill(Qt::black);
	painter.drawPixmap(QPoint(0, 0), *m_pPixmap);

	if (m_pImg != NULL)
	{
		RECT ImgRc = m_pImg->GetWndRc();
		QImage qImg;
		m_pImg->Hs_QtDrawImg(qImg, ImgRc);

		QRect rc(ImgRc.left, ImgRc.top, ImgRc.right - ImgRc.left, ImgRc.bottom - ImgRc.top);
		painter.drawImage(rc, qImg);
	}

}

void ImageWnd::mousePressEvent(QMouseEvent *event)
{
	if (IsImgEmpty() == true)
		return;

	if (event->button() == Qt::LeftButton)
	{
		switch (m_nLeftButtonInteractionStyle)
		{
		case LOCTION_INTERACTION:
			m_nInteractionState = IMGSTSTEM_LOCTION;
			break;
		case BROWSER_INTERACTIOM:
			m_PrePoint = event->pos();
			m_nInteractionState = IMGSTSTEM_BROWSER;
			break;
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		switch (m_nRightButtonInteractionStyle)
		{
		case WINDOW_LEVEL_INTERACTION:
		{
			m_PrePoint = event->pos();
			m_nInteractionState = IMGSTSTEM_WL;
			break;
		}

		case ZOOM_INTERACTION:
		{
			m_PrePoint = event->pos();
			m_pImg->Hs_Size(NULL, 500, 0, HSIZE_RESAMPLE);
			RECT ImgRc = m_pImg->GetWndRc();
			m_fImgRc.left = ImgRc.left;
			m_fImgRc.top = ImgRc.top;
			m_fImgRc.right = ImgRc.right;
			m_fImgRc.bottom = ImgRc.bottom;
			m_nInteractionState = IMGSTSTEM_ZOOM;
			break;
		}
		case PAN_INTERACTION:
		{
			RECT ImgRc = m_pImg->GetWndRc();
			m_PrePoint.setX(event->pos().x() - ImgRc.left);
			m_PrePoint.setY(event->pos().y() - ImgRc.top);

			m_pImg->Hs_Size(NULL, 500, 0, HSIZE_RESAMPLE);
			m_nInteractionState = IMGSTSTEM_PAN;
			break;
		}
		}
	}

}

void ImageWnd::mouseMoveEvent(QMouseEvent *event)
{
	if (IsImgEmpty() == true)
		return;

	QPoint pt;
	pt = event->pos();

	bool bNeedRefresh = false;

	if (m_nInteractionState == IMGSTSTEM_LOCTION)
	{
		POINT ImgPt;
		ImgPt = this->ConvertWndToImg(m_pImg->GetWndRc(), m_pImg->m_ImgState.nCurOriPixCol, m_pImg->m_ImgState.nCurOriPixRow, pt);

		int nNewRow = m_pImg->m_ImgState.nCurOriPixRow;

		bNeedRefresh = true;

	}
	else if (m_nInteractionState == IMGSTSTEM_WL)
	{
		long w = pt.x() - m_PrePoint.x();
		long c = pt.y() - m_PrePoint.y();

		m_pImg->Hs_WinLevel(w, c, true, &m_nCurW, &m_nCurC);

		//Invalidate(FALSE);
		//UpdateWindow();
		bNeedRefresh = true;

		m_PrePoint.setX(pt.x());
		m_PrePoint.setY(pt.y());
	}

	else if (m_nInteractionState == IMGSTSTEM_BROWSER)
	{

	}

	else if (m_nInteractionState == IMGSTSTEM_ZOOM)
	{
		int nMvY = pt.y() - m_PrePoint.y();//只关心y值变化

		double fZoom = nMvY*1.00 / (200 - 0) + 1;

		QRect rc = rect();
		RECT rcWnd;
		rcWnd.top = rc.top(); rcWnd.left = rc.left(); rcWnd.bottom = rc.bottom(); rcWnd.right = rc.right();

		double x = (rcWnd.left + rcWnd.right)*1.00 / 2;//窗口中心点
		double y = (rcWnd.top + rcWnd.bottom)*1.00 / 2;

		//以x,y点为中心放大公式:ImgRc.left-x = (ImgRc.left-x)*(nMvY*1.00/100 + 1);

		double w = m_fImgRc.right - m_fImgRc.left;

		m_fImgRc.left = (m_fImgRc.left - x)*fZoom + x;
		m_fImgRc.right = (m_fImgRc.right - x)*fZoom + x;
		m_fImgRc.top = (m_fImgRc.top - y)*fZoom + y;

		SIZE sz = m_pImg->Hs_GetImgSize();
		m_fImgRc.bottom = (m_fImgRc.right - m_fImgRc.left)*sz.cy*1.00 / sz.cx + m_fImgRc.top;


		//看是不是太大，或太小
		double WndSize = (m_fImgRc.bottom - m_fImgRc.top)*(m_fImgRc.right - m_fImgRc.left)*1.00;
		double BmpSize = sz.cx*sz.cy*1.00;

		double v = WndSize / BmpSize;

		if (v > 0.01 && v < 200.00)
		{
			RECT ImgRc;
			ImgRc.left = min(int(m_fImgRc.left), int(m_fImgRc.right));
			ImgRc.top = min(int(m_fImgRc.top), int(m_fImgRc.bottom));
			ImgRc.right = max(int(m_fImgRc.left), int(m_fImgRc.right));
			ImgRc.bottom = max(int(m_fImgRc.top), int(m_fImgRc.bottom));
			m_pImg->SetWndRc(ImgRc);
		}

		bNeedRefresh = true;

		m_PrePoint.setX(pt.x());
		m_PrePoint.setY(pt.y());
	}

	else if (m_nInteractionState == IMGSTSTEM_PAN)
	{
		RECT ImgRc = m_pImg->GetWndRc();
		int w = ImgRc.right - ImgRc.left;
		int h = ImgRc.bottom - ImgRc.top;

		ImgRc.left = pt.x() - m_PrePoint.x();
		ImgRc.top = pt.y() - m_PrePoint.y();

		ImgRc.right = ImgRc.left + w;
		ImgRc.bottom = ImgRc.top + h;

		m_pImg->SetWndRc(ImgRc);

		bNeedRefresh = true;
	}

	if (bNeedRefresh == true)
	{
		update();
	}
}

void ImageWnd::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		switch (m_nLeftButtonInteractionStyle)
		{
		case LOCTION_INTERACTION:
		 {
			this->m_nInteractionState = IMGSTSTEM_LOCTION;
			break;
		 }
		case BROWSER_INTERACTIOM:
			this->m_nInteractionState = IMGSTSTEM_LOCTION;
			break;
		}
	}
	else if (event->button() == Qt::RightButton)
	{
		switch (m_nRightButtonInteractionStyle)
		{
		case WINDOW_LEVEL_INTERACTION:
			this->m_nInteractionState = IMGSTSTEM_LOCTION;
			break;
		case ZOOM_INTERACTION:
		{
			this->m_nInteractionState = IMGSTSTEM_LOCTION;

			QRect rc = rect();
			RECT rcWnd;
			rcWnd.left = rc.left() + 1; rcWnd.top = rc.top() + 1; rcWnd.right = rc.right() - 1; rcWnd.bottom = rc.bottom() - 1;

			//记下图像实际显示区域与居中显示区域的比值-----此比值用于窗口尺寸发生变化时适当缩放图像
			SIZE CurOriSize = m_pImg->Hs_GetImgSize(false);
			RECT CenterRc = GetShowRcByImgSize(rcWnd, CurOriSize.cx, CurOriSize.cy);

			RECT ImgRc = m_pImg->GetWndRc();
			double fZoomX = (ImgRc.right - ImgRc.left)*1.00 / (CenterRc.right - CenterRc.left);

			m_pImg->m_ImgState.fZoomX = fZoomX;

			break;
		}

		case PAN_INTERACTION:
		{
			this->m_nInteractionState = IMGSTSTEM_LOCTION;

			QPoint pt;
			pt = event->pos();

			RECT ImgRc = m_pImg->GetWndRc();
			int w = ImgRc.right - ImgRc.left;
			int h = ImgRc.bottom - ImgRc.top;

			ImgRc.left = pt.x() - m_PrePoint.x();
			ImgRc.top = pt.y() - m_PrePoint.y();

			ImgRc.right = ImgRc.left + w;
			ImgRc.bottom = ImgRc.top + h;
			m_pImg->SetWndRc(ImgRc);

			QRect rc = rect();
			RECT WndRc;
			WndRc.left = rc.left() + 1; WndRc.top = rc.top() + 1; WndRc.right = rc.right() - 1; WndRc.bottom = rc.bottom() - 1;

			HSPOINT fCenterPt;
			fCenterPt.x = ((WndRc.left + WndRc.right) / 2 - ImgRc.left)*1.00 / w;
			fCenterPt.y = ((WndRc.top + WndRc.bottom) / 2 - ImgRc.top)*1.00 / h;

			m_pImg->m_ImgState.fCenterPt = fCenterPt;

			break;
		}
		}
	}
}

bool ImageWnd::IsImgEmpty()
{
	if (m_pImg == NULL)
		return true;

	return m_pImg->Hs_IsEmpty();
	return true;
}

int ImageWnd::SetImage(CHsImage *pImg)
{
	int nRet = Ret_Success;

	if (pImg)
	{
		m_pImg = pImg;

		if (m_pImg->Hs_IsEmpty())
		{
			m_pImg->Hs_Reload(0);
		}
		RECT rc = CalDisplayRect(m_pImg);
		m_pImg->SetWndRc(rc);
	}

	update();
	return Ret_Success;
}


RECT ImageWnd::CalDisplayRect(CHsImage*pImg)
{
	RECT DisplayRc = { 0, 0, 0, 0 };
	if (pImg == NULL)
		return DisplayRc;

	RECT DlgRc;
	QRect qDlgrc = rect();		
	DlgRc.left = qDlgrc.left() + 1;		DlgRc.right = qDlgrc.right() -1 ;		DlgRc.top = qDlgrc.top() + 1;		DlgRc.bottom = qDlgrc.bottom() - 1;

	SIZE ImgSize = pImg->Hs_GetImgSize(false);//获取当前图像的原始像素矩阵长宽

	//1.先确定图像显示大小
	RECT CenterRc = GetShowRcByImgSize(DlgRc, ImgSize.cx, ImgSize.cy);

	int nNewW = (LONG)((CenterRc.right - CenterRc.left) * (pImg->m_ImgState.fZoomX));
	int nNewH = (LONG)((CenterRc.bottom - CenterRc.top) * (pImg->m_ImgState.fZoomX));

	//2.确定图像位置
	if (pImg->m_ImgState.fCenterPt.x == -100.00)//乳腺靠左
	{
		DisplayRc.left = DlgRc.left;
		DisplayRc.right = DlgRc.left + nNewW;

		//pImg->m_ImgState.fCenterPt.x = ( (DlgRc.left + DlgRc.right)/2 - DisplayRc.left )*1.00/nNewW;
		DisplayRc.top = (DlgRc.top + DlgRc.bottom) / 2 - nNewH*pImg->m_ImgState.fCenterPt.y;
		DisplayRc.bottom = DisplayRc.top + nNewH;

	}
	else if (pImg->m_ImgState.fCenterPt.x == 100.00)//乳腺靠右
	{
		DisplayRc.right = DlgRc.right;
		DisplayRc.left = DlgRc.right - nNewW;

		//pImg->m_ImgState.fCenterPt.x = ( (DlgRc.left + DlgRc.right)/2 - DisplayRc.left )*1.00/nNewW;
		DisplayRc.top = (DlgRc.top + DlgRc.bottom) / 2 - nNewH*pImg->m_ImgState.fCenterPt.y;
		DisplayRc.bottom = DisplayRc.top + nNewH;
	}
	else
	{
		DisplayRc.left = (DlgRc.left + DlgRc.right) / 2 - nNewW*pImg->m_ImgState.fCenterPt.x;
		DisplayRc.top = (DlgRc.top + DlgRc.bottom) / 2 - nNewH*pImg->m_ImgState.fCenterPt.y;

		DisplayRc.bottom = DisplayRc.top + nNewH;
		DisplayRc.right = DisplayRc.left + nNewW;
	}


	m_pImg->SetWndRc(DisplayRc);//有点绕--因为下面的ConvertCoord需要图像在窗口中的显示区域

	RECT rcCom;//ImgRc与rcWnd的交集
	if (::IntersectRect(&rcCom, &DisplayRc, &DlgRc) == TRUE)
	{
		RECT rcImg = rcCom;
		ConvertCoord(&rcImg.left, &rcImg.top, &rcImg.right, &rcImg.bottom, TRUE);//rcCom是用户看到的图像区域--以屏幕坐标为单位的,这是转换到m_pImg像素坐标上

		m_pImg->Hs_Size(&rcImg, rcCom.right - rcCom.left, rcCom.bottom - rcCom.top, HSIZE_RESAMPLE, true);

	}


	return DisplayRc;
}

void ImageWnd::ConvertCoord(long *x1, long *y1, long *x2, long *y2, bool bFromHwdToImg)
{
	if (IsImgEmpty())
		return;

	RECT ImgRc = m_pImg->GetWndRc();

	//	AtlTrace("\r\n%d:	%.2f	%.2f	%.2f	%.2f",bFromHwdToImg,*x1, *y1, *x2, *y2);
	SIZE ImgSz = m_pImg->Hs_GetImgSize(false);
	if (bFromHwdToImg)//窗口->图像
	{
		if (x1)
			*x1 = (*x1 - ImgRc.left)*ImgSz.cx / (ImgRc.right - ImgRc.left);
		if (x2)
			*x2 = (*x2 - ImgRc.left)*ImgSz.cx / (ImgRc.right - ImgRc.left);

		if (y1)
			*y1 = (*y1 - ImgRc.top)*ImgSz.cy / (ImgRc.bottom - ImgRc.top);
		if (y2)
			*y2 = (*y2 - ImgRc.top)*ImgSz.cy / (ImgRc.bottom - ImgRc.top);
	}
	else
	{
		if (x1)
			*x1 = (*x1)*(ImgRc.right - ImgRc.left) / ImgSz.cx + ImgRc.left;
		if (x2)
			*x2 = (*x2)*(ImgRc.right - ImgRc.left) / ImgSz.cx + ImgRc.left;

		if (y1)
			*y1 = (*y1)*(ImgRc.bottom - ImgRc.top) / ImgSz.cy + ImgRc.top;
		if (x2)
			*y2 = (*y2)*(ImgRc.bottom - ImgRc.top) / ImgSz.cy + ImgRc.top;
	}
}

void ImageWnd::setOperate(QString operate)
{
	if (operate.compare("Img_location") == 0)
		m_nLeftButtonInteractionStyle = LOCTION_INTERACTION;
	else if (operate.compare("Img_browser") == 0)
		m_nLeftButtonInteractionStyle = BROWSER_INTERACTIOM;
	else if (operate.compare("Img_wl") == 0)
		m_nRightButtonInteractionStyle = WINDOW_LEVEL_INTERACTION;
	else if (operate.compare("Img_zoom") == 0)
		m_nRightButtonInteractionStyle = ZOOM_INTERACTION;
	else if (operate.compare("Img_pan") == 0)
		m_nRightButtonInteractionStyle = PAN_INTERACTION;		
}

POINT ImageWnd::ConvertWndToImg(RECT ImgRcOnWnd, long nImgW, long nImgH, QPoint &pt)
{
	POINT ImgPt = { 0, 0 };
	if (ImgRcOnWnd.left == ImgRcOnWnd.right || ImgRcOnWnd.top == ImgRcOnWnd.bottom)
		return ImgPt;

	double fx = nImgW*1.00 / (ImgRcOnWnd.right - ImgRcOnWnd.left);
	ImgPt.x = fx * (pt.x() - ImgRcOnWnd.left);

	double fy = nImgH*1.00 / (ImgRcOnWnd.bottom - ImgRcOnWnd.top);
	ImgPt.y = fy * (pt.y() - ImgRcOnWnd.top);

	return ImgPt;
}

