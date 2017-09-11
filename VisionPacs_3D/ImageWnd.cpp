#include "stdafx.h"
#include "ImageWnd.h"
#include "ui_ImageWnd.h"

#include "HsImage.h"
#include "HsNormalMprMaker.h"
#include "WorkZone.h"
#include "OperateMprLines.h"

#include "AppConfig.h"

extern RECT GetShowRcByImgSize(RECT rc, double ImgWidth, double ImgHeight);

ImageWnd::ImageWnd(QWidget *parent):
    QWidget(parent)
	,ui(new Ui::ImageWnd)
	, m_bFocused(false)
	, m_pImg(NULL)
	, m_pQImage(NULL)
	, m_pPixmap(NULL)
	, m_nLeftButtonInteractionStyle(LOCTION_INTERACTION)
	, m_nRightButtonInteractionStyle(WINDOW_LEVEL_INTERACTION)
	,m_nImgWndType(ORIIMG_AXIAL)
	,m_pNormalMaker(NULL)
	,m_nImgNum(0)
	,m_bInitCorInfo(false)
	,m_pOperateLines(NULL)
{
    ui->setupUi(this);
	setMouseTracking(true);
}

ImageWnd::~ImageWnd()
{
    delete ui;

	if (m_pPixmap)
		delete m_pPixmap;

	if (m_pImg)
	{
		delete m_pImg;
		m_pImg = NULL;
	}

	if (m_pQImage)
	{
		delete m_pQImage;
		m_pQImage = NULL;
	}

	if (m_pNormalMaker)
	{
		delete m_pNormalMaker;
		m_pNormalMaker = NULL;
	}

	if (m_pOperateLines)
	{
		delete m_pOperateLines;
		m_pOperateLines = NULL;
	}
}

void ImageWnd::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QSize s = size();
	if (m_pPixmap )
		delete m_pPixmap;
	m_pPixmap = new QPixmap(size());
	m_pPixmap->fill(Qt::black);
	painter.drawPixmap(QPoint(0, 0), *m_pPixmap);

	if (m_pImg != NULL)
	{
		RECT DlgRc;
		QRect qDlgrc = rect();
		DlgRc.left = qDlgrc.left();		
		DlgRc.right = qDlgrc.right();		
		DlgRc.top = qDlgrc.top();		
		DlgRc.bottom = qDlgrc.bottom();

		RECT ImgRc = m_pImg->GetWndRc();

		if (m_pImg->Hs_IsWholeImgSized() == true)
		{
			QImage qImg;
			m_pImg->Hs_QtDrawImg(qImg, ImgRc);

			QRect rc(ImgRc.left, ImgRc.top, ImgRc.right - ImgRc.left, ImgRc.bottom - ImgRc.top);
			painter.drawImage(rc, qImg);
		}
		else
		{
			RECT rcCom;
			if (IntersectRect(&rcCom, &ImgRc, &DlgRc) == TRUE)
			{
				QImage qImg;
				m_pImg->Hs_QtDrawImg(qImg, rcCom);

				QRect rc(rcCom.left, rcCom.top, rcCom.right - rcCom.left, rcCom.bottom - rcCom.top);
				painter.drawImage(rc, qImg);
			}
		}
	

		//MPR定位线
		if (m_pOperateLines)
		{
			m_pOperateLines->OnMprLinesPaint(&painter);
		}
	}
}

void ImageWnd::resizeEvent(QResizeEvent* size)
{
	if (m_pImg)
	{
		QSize deltaSize = size->size() - size->oldSize();
		m_pImg->SetWndRc(CalDisplayRect(m_pImg));
		ReSetPosCorInfo(deltaSize);
	}
	update();
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

		if (m_pOperateLines && m_pOperateLines->IsMprLineShow())
		{
			m_pOperateLines->OnMprLinesMousePress(event);
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

		bNeedRefresh = false;
	}
	else if (m_nInteractionState == IMGSTSTEM_WL)
	{
		long w = (pt.x() - m_PrePoint.x())/2;
		long c = (pt.y() - m_PrePoint.y())/2;

		m_pImg->Hs_WinLevel(w, c, true, &m_nCurW, &m_nCurC);

		bNeedRefresh = true;

		m_PrePoint.setX(pt.x());
		m_PrePoint.setY(pt.y());
	}
	else if (m_nInteractionState == IMGSTSTEM_BROWSER)
	{
		int n = (pt.y() - m_PrePoint.y()) / 6;//6个像素换一幅图像

		if (n != 0)
		{
			emit ImageIndexChange(n);
			m_PrePoint.setX(pt.x());
			m_PrePoint.setY(pt.y());
		}
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
			RECT rcCom;//ImgRc与rcWnd的交集
			if (::IntersectRect(&rcCom, &ImgRc, &rcWnd) == TRUE)
			{
				RECT rcImg = rcCom;
				ConvertCoord(&rcImg.left, &rcImg.top, &rcImg.right, &rcImg.bottom, TRUE);//rcCom是用户看到的图像区域--以屏幕坐标为单位的,这是转换到m_pImg像素坐标上

				m_pImg->Hs_Size(&rcImg, rcCom.right - rcCom.left, rcCom.bottom - rcCom.top, HSIZE_RESAMPLE, true);
			}
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

	if (m_pOperateLines && m_pOperateLines->IsMprLineShow())
	{
		bNeedRefresh = (m_pOperateLines->OnMprLinesMouseMove(event) || bNeedRefresh);
	}

	if (bNeedRefresh == true)
	{
		update();
		RefreshCornorInfoWidget();
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

		if (m_pOperateLines && m_pOperateLines->IsMprLineShow())
		{
			m_pOperateLines->OnMprLinesMouseRelease(event);
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
			m_pImg->SetWndRc(CalDisplayRect(m_pImg));
			update();
			RefreshCornorInfoWidget();
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

			update();
			RefreshCornorInfoWidget();
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

		RefreshCornorInfoWidget();
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

void ImageWnd::SetOperate(QString operate)
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

void ImageWnd::GetImgNumAndWndType(QString sWndName, int nOriImgType)
{
	if (sWndName.compare("Axial_Wnd") == 0)
		m_nImgWndType = ORIIMG_AXIAL;
	else if (sWndName.compare("Coronal_Wnd") == 0)
		m_nImgWndType = ORIIMG_CORONAL;
	else
		m_nImgWndType = ORIIMG_SAGITTAL;

	if (m_nImgWndType == nOriImgType)
	{
		m_nImgNum = m_vOriImg.size();
	}
	else
	{
		if (nOriImgType == ORIIMG_AXIAL)
		{
			if (sWndName.compare("Coronal_Wnd") == 0)
				m_nImgNum = m_vOriImg[0]->m_ImgInfo.nOriRows;
			else
				m_nImgNum = m_vOriImg[0]->m_ImgInfo.nOriCols;
		}
		else if (nOriImgType == ORIIMG_SAGITTAL)
		{
			if (sWndName.compare("Coronal_Wnd") == 0)
				m_nImgNum = m_vOriImg[0]->m_ImgInfo.nOriCols;
			else
				m_nImgNum = m_vOriImg[0]->m_ImgInfo.nOriRows;
		}
		else
		{
			if (sWndName.compare("Axial_Wnd") == 0)
				m_nImgNum = m_vOriImg[0]->m_ImgInfo.nOriCols;
			else
				m_nImgNum = m_vOriImg[0]->m_ImgInfo.nOriRows;
		}
	}

	emit SendImageNum(m_nImgNum);
}

int ImageWnd::CalcAndShowNormalImg(QString sWndName, int nOriImgType, int iImgIndex,int iSlice)
{
	if (m_pImg)
		m_pImg->Hs_FreeMem();


	if (m_pImg == NULL)
	{
		m_pImg = new CHsImage;
		m_pImg->m_ImgInfo = m_vOriImg[0]->m_ImgInfo;//先复制源图基本信息
		m_pImg->SetDs(m_vOriImg[0]->GetDs());
		m_pImg->Hs_GetCornerInfo(true);
		m_pImg->SetBelongWnd(this);
	}

	if (m_pNormalMaker == NULL)
	{
		m_pNormalMaker = new CHsNormalMprMaker();
		m_pNormalMaker->InitParaAndData(m_vOriImg, m_p3DArray, m_p3DImgData, m_nImgWndType, nOriImgType);
	}
	
	m_pNormalMaker->GetShowImage(m_pImg, iImgIndex, iSlice);

	if (m_pOperateLines == NULL)
	{
		m_pOperateLines = new OperateMprLines(this, m_nImgWndType);
		connect(m_pOperateLines, SIGNAL(MprLinesInfoOutput(MprLinesInfo)), this->parent()->parent(), SLOT(OnMprLinesInfo(MprLinesInfo)));
	}

	SetImage(m_pImg);

	return 0;
}

void ImageWnd::SetMprMode(QString sModeName)
{
	if (m_pNormalMaker == NULL)
		return;

	if (sModeName.compare("MIP") == 0)
		m_pNormalMaker->m_nMprMode = 0;
	else if (sModeName.compare("MAP") == 0)
		m_pNormalMaker->m_nMprMode = 1;

	SetImage(m_pImg);
}

void ImageWnd::RefreshCornorInfoWidget()
{
	m_pImg->Hs_GetCornerInfo(false);

	if (m_bInitCorInfo == false)
	{
		InitNormalCorInfo();
		InitPosCorInfo();
		m_bInitCorInfo = true;
	}
	else
	{
		for (int i =0; i<m_vCornorEdit.size(); i++)
		{
			if (m_vCornorEdit[i].sName == "ww")
				m_vCornorEdit[i].qEdit->setText(QString::number(m_pImg->m_ImgState.CurWc.x));
			else if (m_vCornorEdit[i].sName == "wc")
				m_vCornorEdit[i].qEdit->setText(QString::number(m_pImg->m_ImgState.CurWc.y));
			else if (m_vCornorEdit[i].sName == "zoomfactor")
				m_vCornorEdit[i].qEdit->setText(QString::number(m_pImg->m_ImgState.fZoomX,'f',2));
			else if (m_vCornorEdit[i].sName == "slicethick")
				m_vCornorEdit[i].qEdit->setText(QString::number(m_pImg->m_fCurSliceThick, 'f', 2));
			else if (m_vCornorEdit[i].sName == "imageindex")
			{
				m_vCornorEdit[i].qPreLabel->setText(QString("Index:%1").arg(m_nCurImgIndex));
				m_vCornorEdit[i].qPreLabel->adjustSize();
			}			
		}
	}
}

void ImageWnd::InitNormalCorInfo()
{
	MODINFO modInfo = m_pImg->m_CornorInfo;
	QFont ft;
	ft.setPointSize(modInfo.nSize);
	ft.setFamily(modInfo.sFaceName);
	int nCorNum = modInfo.coInfoV.size();
	for (int i = 0; i < nCorNum; i++)
	{
		CORNORINFO corInfo = modInfo.coInfoV[i];
		map<int, ROWITEM> mapRow;
		ArrangeCorinfo(corInfo, mapRow);
		map <int, ROWITEM>::iterator  iter;
		for (iter = mapRow.begin(); iter != mapRow.end(); iter++)
		{
			if (iter->second.sType.compare("Normal") == 0)
			{
				QLabel *label = new QLabel(this);
				label->setFont(ft);
				label->setAlignment(Qt::AlignLeft);
				label->setText(iter->second.sValue);
				label->adjustSize();
				QRect rcLable = label->geometry();
				QRect rcLocation;
				if (corInfo.sPos.compare("LT") == 0)
					rcLocation = QRect(2, 2 + rcLable.height()*(iter->first - 1), rcLable.width(), rcLable.height());
				else if (corInfo.sPos.compare("RT") == 0)
					rcLocation = QRect(rect().right() - rcLable.width() - 2, 2 + rcLable.height()*(iter->first - 1), rcLable.width(), rcLable.height());
				else if (corInfo.sPos.compare("LB") == 0)
					rcLocation = QRect(2, rect().bottom() - rcLable.height()*(mapRow.size() - iter->first + 1) - 2, rcLable.width(), rcLable.height());
				else if (corInfo.sPos.compare("RB") == 0)
					rcLocation = QRect(rect().right() - rcLable.width() - 2, rect().bottom() - rcLable.height()*(mapRow.size() - iter->first + 1) - 2, rcLable.width(), rcLable.height());
				label->setGeometry(rcLocation);
				label->show();
				m_mapInfoLabel[label] = corInfo.sPos;
			}
			else
			{
				if (iter->second.sType.compare("ww/wc") == 0)
				{
					int iPos = iter->second.sValue.indexOf("/");
					QString ww = iter->second.sValue.mid(3, iPos - 3).trimmed();
					QString wc = iter->second.sValue.mid(iPos + 1, iter->second.sValue.length() - iPos - 1).trimmed();
					//窗宽窗位前缀Label
					QLabel *wcLabel = new QLabel(this);
					wcLabel->setFont(ft);
					wcLabel->setAlignment(Qt::AlignLeft);
					wcLabel->setText("WC:");
					wcLabel->adjustSize();
					QRect rcWcLable = wcLabel->geometry();
					rcWcLable = QRect(2, rect().bottom() - rcWcLable.height()*(mapRow.size() - iter->first + 1) - 2, rcWcLable.width(), rcWcLable.height());
					wcLabel->setGeometry(rcWcLable);
					wcLabel->show();
					m_mapInfoLabel[wcLabel] = corInfo.sPos;
					//创建窗宽LineEdit，为了更好地控制控件长度，先用edit得到长度
					QLabel *tLabel = new QLabel(this);
					tLabel->setFont(ft);
					tLabel->setAlignment(Qt::AlignLeft);
					tLabel->setText(ww);
					tLabel->adjustSize();
					QRect tRC = tLabel->geometry();
					QLineEdit *qWwLineEdit = new QLineEdit(this);
					qWwLineEdit->setObjectName("ww");
					qWwLineEdit->setFont(ft);
					qWwLineEdit->setAlignment(Qt::AlignLeft);
					qWwLineEdit->setValidator(new QIntValidator(qWwLineEdit));
					qWwLineEdit->setText(ww);
					qWwLineEdit->setGeometry(QRect(rcWcLable.right(), rect().bottom() - rcWcLable.height()*(mapRow.size() - iter->first + 1) - 2, tRC.width() + 4, tRC.height()));
					qWwLineEdit->show();
					m_mapInfoEdit[qWwLineEdit] = corInfo.sPos;
					connect(qWwLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(OnEditTextChanged(const QString &)));
					connect(qWwLineEdit, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
					QEDITITEM wwitem;
					wwitem.sName = "ww";
					wwitem.qEdit = qWwLineEdit;
					//窗宽窗位分隔符Label
					QLabel *wcSplite = new QLabel(this);
					wcSplite->setFont(ft);
					wcSplite->setAlignment(Qt::AlignLeft);
					wcSplite->setText("/");
					wcSplite->adjustSize();
					QRect rcSplite = wcSplite->geometry();
					rcSplite = QRect(qWwLineEdit->geometry().right(), rect().bottom() - rcSplite.height()*(mapRow.size() - iter->first + 1) - 2, rcSplite.width(), rcSplite.height());
					wcSplite->setGeometry(rcSplite);
					wcSplite->show();
					wwitem.qPreLabel = wcSplite;
					m_vCornorEdit.push_back(wwitem);
					m_mapInfoLabel[wcSplite] = corInfo.sPos;
					//创建窗位LineEdit，为了更好地控制控件长度，先用edit得到长度
					tLabel->setText(wc);
					tLabel->adjustSize();
					tRC = tLabel->geometry();
					delete tLabel;
					QLineEdit *qWcLineEdit = new QLineEdit(this);
					qWcLineEdit->setObjectName("wc");
					qWcLineEdit->setFont(ft);
					qWcLineEdit->setAlignment(Qt::AlignLeft);
					qWcLineEdit->setValidator(new QIntValidator(qWcLineEdit));
					qWcLineEdit->setText(wc);
					qWcLineEdit->setGeometry(QRect(rcSplite.right() + 2, rect().bottom() - rcSplite.height()*(mapRow.size() - iter->first + 1) - 2, tRC.width() + 4, tRC.height()));
					qWcLineEdit->show();
					m_mapInfoEdit[qWcLineEdit] = corInfo.sPos;
					connect(qWcLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(OnEditTextChanged(const QString &)));
					connect(qWcLineEdit, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
					QEDITITEM wcitem;
					wcitem.sName = "wc";
					wcitem.qEdit = qWcLineEdit;
					m_vCornorEdit.push_back(wcitem);
				}
			}

			if (iter->second.sType.compare("slicethick") == 0)
			{
				//创建层厚LineEdit，为了更好地控制控件长度，先用edit得到长度
				QString sSt = QString::number(m_pImg->m_fCurSliceThick, 'f', 2);
				QLabel *tLabel = new QLabel(this);
				tLabel->setFont(ft);
				tLabel->setAlignment(Qt::AlignLeft);
				tLabel->setText(sSt);
				tLabel->adjustSize();
				QRect tRC = tLabel->geometry();
				delete tLabel;
				QLineEdit *qStLineEdit = new QLineEdit(this);
				qStLineEdit->setObjectName("slicethick");
				qStLineEdit->setFont(ft);
				qStLineEdit->setAlignment(Qt::AlignLeft);
				QDoubleValidator *pDoubleValidator = new QDoubleValidator(qStLineEdit);
				pDoubleValidator->setRange(0, 50);
				pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
				pDoubleValidator->setDecimals(2);
				qStLineEdit->setValidator(pDoubleValidator);
				qStLineEdit->setText(sSt);
				qStLineEdit->setGeometry(QRect(2, rect().bottom() - tRC.height()*(mapRow.size() - iter->first + 1) - 2, tRC.width() + 4, tRC.height()));
				qStLineEdit->show();
				m_mapInfoEdit[qStLineEdit] = corInfo.sPos;
				QObject::connect(qStLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(OnEditTextChanged(const QString &)));
				QObject::connect(qStLineEdit, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
				//层厚单位Label
				QLabel *stUnit = new QLabel(this);
				stUnit->setFont(ft);
				stUnit->setAlignment(Qt::AlignLeft);
				stUnit->setText("mm");
				stUnit->adjustSize();
				QRect rcStUnit = stUnit->geometry();
				rcStUnit = QRect(qStLineEdit->geometry().right(), rect().bottom() - rcStUnit.height()*(mapRow.size() - iter->first + 1) - 2, rcStUnit.width(), rcStUnit.height());
				stUnit->setGeometry(rcStUnit);
				stUnit->show();
				m_mapInfoLabel[stUnit] = corInfo.sPos;
				QEDITITEM stitem;
				stitem.sName = "slicethick";
				stitem.qEdit = qStLineEdit;
				stitem.qPreLabel = stUnit;
				m_vCornorEdit.push_back(stitem);
			}

			if (iter->second.sType.compare("zoomfactor") == 0)
			{
				QString sZf = QString::number(m_pImg->m_ImgState.fZoomX, 'f', 2);
				QLabel *tLabel = new QLabel(this);
				tLabel->setFont(ft);
				tLabel->setAlignment(Qt::AlignLeft);
				tLabel->setText(sZf);
				tLabel->adjustSize();
				QRect tRC = tLabel->geometry();
				delete tLabel;
				QLineEdit *qZfLineEdit = new QLineEdit(this);
				qZfLineEdit->setObjectName("zoomfactor");
				qZfLineEdit->setFont(ft);
				qZfLineEdit->setAlignment(Qt::AlignLeft);
				QDoubleValidator *pDoubleValidator = new QDoubleValidator(qZfLineEdit);
				pDoubleValidator->setRange(0, 100);
				pDoubleValidator->setNotation(QDoubleValidator::StandardNotation);
				pDoubleValidator->setDecimals(2);
				qZfLineEdit->setValidator(pDoubleValidator);
				qZfLineEdit->setText(sZf);
				qZfLineEdit->setGeometry(QRect(rect().right() - tRC.width() - 4, rect().bottom() - tRC.height()*(mapRow.size() - iter->first + 1) - 2, tRC.width() + 4, tRC.height()));
				qZfLineEdit->show();
				m_mapInfoEdit[qZfLineEdit] = corInfo.sPos;
				QObject::connect(qZfLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(OnEditTextChanged(const QString &)));
				QObject::connect(qZfLineEdit, SIGNAL(editingFinished()), this, SLOT(OnEditFinished()));
				QEDITITEM zfitem;
				zfitem.sName = "zoomfactor";
				zfitem.qEdit = qZfLineEdit;
				m_vCornorEdit.push_back(zfitem);
			}

			if (iter->second.sType.compare("wndtype") == 0)
			{
				QString sZf = "";
				if (m_nImgWndType == ORIIMG_AXIAL)
					sZf = "Axial";
				else if (m_nImgWndType == ORIIMG_SAGITTAL)
					sZf = "Sagittal";
				else if (m_nImgWndType == ORIIMG_CORONAL)
					sZf = "Coronal";
				QLabel *zfLabel = new QLabel(this);
				zfLabel->setFont(ft);
				zfLabel->setAlignment(Qt::AlignLeft);
				zfLabel->setText(sZf);
				zfLabel->adjustSize();
				QRect zfRC = zfLabel->geometry();
				zfLabel->setGeometry(QRect(rect().right() - zfRC.width(), rect().bottom() - zfRC.height()*(mapRow.size() - iter->first + 1) - 2, zfRC.width(), zfRC.height()));
				zfLabel->show();
				m_mapInfoLabel[zfLabel] = corInfo.sPos;
			}


			if (iter->second.sType.compare("imageindex") == 0)
			{
				QString sImgIndex = QString("Index:%1").arg(m_nCurImgIndex);
				QLabel *indexLabel = new QLabel(this);
				indexLabel->setFont(ft);
				indexLabel->setAlignment(Qt::AlignLeft);
				indexLabel->setText(sImgIndex);
				indexLabel->adjustSize();
				QRect indexRC = indexLabel->geometry();
				indexLabel->setGeometry(2, 2 + indexRC.height()*(iter->first - 1), indexRC.width(), indexRC.height());
				indexLabel->show();
				m_mapInfoLabel[indexLabel] = corInfo.sPos;
				QEDITITEM zfitem;
				zfitem.sName = "imageindex";
				zfitem.qPreLabel = indexLabel;
				m_vCornorEdit.push_back(zfitem);
			}
		}
	}
}

void ImageWnd::InitPosCorInfo()
{
	if (m_pImg->m_ImgInfo.ImgLocPara.bValide == true)
	{
		m_pImg->Hs_GetPatientPos(m_pImg->m_ImgInfo.ImgLocPara.fFirstColCosX,
			m_pImg->m_ImgInfo.ImgLocPara.fFirstColCosY,
			m_pImg->m_ImgInfo.ImgLocPara.fFirstColCosZ,
			m_pImg->m_ImgState.sTopPatientPos,
			m_pImg->m_ImgState.sBottomPatientPos);

		m_pImg->Hs_GetPatientPos(m_pImg->m_ImgInfo.ImgLocPara.fFirstRowCosX,
			m_pImg->m_ImgInfo.ImgLocPara.fFirstRowCosY,
			m_pImg->m_ImgInfo.ImgLocPara.fFirstRowCosZ,
			m_pImg->m_ImgState.sLeftPatientPos,
			m_pImg->m_ImgState.sRightPatientPos);
	}

	MODINFO modInfo = m_pImg->m_CornorInfo;
	QFont ft;
	ft.setPointSize(modInfo.nSize);
	ft.setFamily(modInfo.sFaceName);

	QLabel *tLabel = new QLabel(this);
	tLabel->setFont(ft);
	tLabel->setAlignment(Qt::AlignLeft);
	tLabel->setText(m_pImg->m_ImgState.sTopPatientPos);
	tLabel->adjustSize();
	QRect tRC = tLabel->geometry();
	tLabel->setGeometry((rect().right() - tRC.width())/2, 2, tRC.width(), tRC.height());
	tLabel->show();
	m_mapInfoLabel[tLabel] = "TC";

	QLabel *rLabel = new QLabel(this);
	rLabel->setFont(ft);
	rLabel->setAlignment(Qt::AlignLeft);
	rLabel->setText(m_pImg->m_ImgState.sRightPatientPos);
	rLabel->adjustSize();
	tRC = rLabel->geometry();
	rLabel->setGeometry(rect().right()-2-tRC.width(), (rect().height()-tRC.height())/2, tRC.width(), tRC.height());
	rLabel->show();
	m_mapInfoLabel[rLabel] = "RC";

	QLabel *bLabel = new QLabel(this);
	bLabel->setFont(ft);
	bLabel->setAlignment(Qt::AlignLeft);
	bLabel->setText(m_pImg->m_ImgState.sBottomPatientPos);
	bLabel->adjustSize();
	tRC = bLabel->geometry();
	bLabel->setGeometry((rect().right() - tRC.width()) / 2, rect().bottom()-tRC.height()-2 , tRC.width(), tRC.height());
	bLabel->show();
	m_mapInfoLabel[bLabel] = "BC";

	QLabel *lLabel = new QLabel(this);
	lLabel->setFont(ft);
	lLabel->setAlignment(Qt::AlignLeft);
	lLabel->setText(m_pImg->m_ImgState.sLeftPatientPos);
	lLabel->adjustSize();
	tRC = lLabel->geometry();
	lLabel->setGeometry( 2, (rect().height() - tRC.height()) / 2, tRC.width(), tRC.height());
	lLabel->show();
	m_mapInfoLabel[bLabel] = "LC";
}

void ImageWnd::ReSetPosCorInfo(QSize deltaSize)
{
	map <QLabel*, QString>::iterator  labeIter;
	for (labeIter = m_mapInfoLabel.begin(); labeIter != m_mapInfoLabel.end(); labeIter++)
	{
		if (labeIter->second.compare("RT") == 0 && deltaSize.width() != 0)
		{
			QRect oldRc = labeIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setLeft(oldRc.left() + deltaSize.width());
			newRc.setWidth(oldRc.width());
			labeIter->first->setGeometry(newRc);
		}
		else if (labeIter->second.compare("LB") == 0 && deltaSize.height() != 0)
		{
			QRect oldRc = labeIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setTop(oldRc.top() + deltaSize.height());
			newRc.setHeight(oldRc.height());
			labeIter->first->setGeometry(newRc);
		}
		else if (labeIter->second.compare("RB") == 0 && (deltaSize.height() != 0 || deltaSize.width() != 0))
		{
			QRect oldRc = labeIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setLeft(oldRc.left() + deltaSize.width());
			newRc.setWidth(oldRc.width());
			newRc.setTop(oldRc.top() + deltaSize.height());
			newRc.setHeight(oldRc.height());
			labeIter->first->setGeometry(newRc);
		}

		if (labeIter->second.compare("TC") == 0 && deltaSize.width() != 0)
		{
			QRect oldRc = labeIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setLeft(oldRc.left() + deltaSize.width());
			newRc.setWidth(oldRc.width());
			labeIter->first->setGeometry(newRc);
		}
		else if (labeIter->second.compare("LC") == 0 && deltaSize.height() != 0)
		{
			QRect oldRc = labeIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setTop(oldRc.top() + deltaSize.height());
			newRc.setHeight(oldRc.height());
			labeIter->first->setGeometry(newRc);
		}
		else if ((labeIter->second.compare("BC") == 0 || labeIter->second.compare("RC") == 0) && (deltaSize.height() != 0 || deltaSize.width() != 0))
		{
			QRect oldRc = labeIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setLeft(oldRc.left() + deltaSize.width());
			newRc.setWidth(oldRc.width());
			newRc.setTop(oldRc.top() + deltaSize.height());
			newRc.setHeight(oldRc.height());
			labeIter->first->setGeometry(newRc);
		}
	}

	map <QLineEdit*, QString>::iterator  editIter;
	for (editIter = m_mapInfoEdit.begin(); editIter != m_mapInfoEdit.end(); editIter++)
	{
		if (editIter->second.compare("RT") == 0 && deltaSize.width() != 0)
		{
			QRect oldRc = editIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setLeft(oldRc.left() + deltaSize.width());
			newRc.setWidth(oldRc.width());
			editIter->first->setGeometry(newRc);
		}
		else if (editIter->second.compare("LB") == 0 && deltaSize.height() != 0)
		{
			QRect oldRc = editIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setTop(oldRc.top() + deltaSize.height());
			newRc.setHeight(oldRc.height());
			editIter->first->setGeometry(newRc);
		}
		else if (editIter->second.compare("RB") == 0 && (deltaSize.height() != 0 || deltaSize.width() != 0))
		{
			QRect oldRc = editIter->first->geometry();
			QRect newRc = oldRc;
			newRc.setLeft(oldRc.left() + deltaSize.width());
			newRc.setWidth(oldRc.width());
			newRc.setTop(oldRc.top() + deltaSize.height());
			newRc.setHeight(oldRc.height());
			editIter->first->setGeometry(newRc);
		}
	}


}

int ImageWnd::ArrangeCorinfo(CORNORINFO corInfo, map<int, ROWITEM> &mapRow)
{
	int itemNum = corInfo.infoV.size();
	for (int i=0; i<itemNum; i++)
	{
		INFOITEM item = corInfo.infoV[i];
		if (mapRow.find(item.iRow) == mapRow.end())
		{
			mapRow[item.iRow].sValue = item.sValue;
			if (item.sTag.compare("0x00281051") == 0 || item.sTag.compare("0x00281050") == 0)
				mapRow[item.iRow].sType = "ww/wc";
			else if (item.sTag.compare("-1") == 0)
				mapRow[item.iRow].sType = "slicethick";
			else if (item.sTag.compare("-2") == 0)
				mapRow[item.iRow].sType = "imageindex";
			else if (item.sTag.compare("-5") == 0)
				mapRow[item.iRow].sType = "zoomfactor";
			else if (item.sTag.compare("-6") == 0)
				mapRow[item.iRow].sType = "wndtype";
		}
		else
		{
			mapRow[item.iRow].sValue += item.sValue;
		}
	}
	return 0;
}

void ImageWnd::OnEditTextChanged(const QString &sText)
{
	MODINFO modInfo = m_pImg->m_CornorInfo;
	QFont ft;
	ft.setPointSize(modInfo.nSize);
	ft.setFamily(modInfo.sFaceName);
	QLineEdit *edit = (QLineEdit *)sender();
	QString sName = edit->objectName();
	if (sName.compare("ww")==0)
	{
		QEDITITEM ww;
		QEDITITEM wc;
		for (int i=0; i<m_vCornorEdit.size(); i++)
		{
			if (m_vCornorEdit[i].sName.compare("ww") == 0)
				ww = m_vCornorEdit[i];
			else if (m_vCornorEdit[i].sName.compare("wc") == 0)
				wc = m_vCornorEdit[i];
		}
		QRect rcEdit = edit->geometry();
		QLabel *tLabel = new QLabel(this);
		tLabel->setFont(ft);
		tLabel->setAlignment(Qt::AlignLeft);
		tLabel->setText(sText);
		tLabel->adjustSize();
		QRect tRc = tLabel->geometry();
		rcEdit.setWidth(tRc.width() + 4);
		edit->setGeometry(rcEdit);
		delete tLabel;
		QRect rcLabel = ww.qPreLabel->geometry();
		ww.qPreLabel->setGeometry(QRect(rcEdit.right(), rcLabel.top(), rcLabel.width(), rcLabel.height()));
		rcLabel = ww.qPreLabel->geometry();
		rcEdit = wc.qEdit->geometry();
		wc.qEdit->setGeometry(QRect(rcLabel.right(), rcEdit.top(), rcEdit.width(), rcEdit.height()));
	}
	else if (sName.compare("slicethick") == 0)
	{
		for (int i = 0; i < m_vCornorEdit.size(); i++)
		{
			if (m_vCornorEdit[i].sName.compare(sName) == 0)
			{
				QEDITITEM st = m_vCornorEdit[i];
				QRect rcEdit = edit->geometry();
				QLabel *tLabel = new QLabel(this);
				tLabel->setFont(ft);
				tLabel->setAlignment(Qt::AlignLeft);
				tLabel->setText(sText);
				tLabel->adjustSize();
				QRect tRc = tLabel->geometry();
				rcEdit.setWidth(tRc.width() + 4);
				edit->setGeometry(rcEdit);
				delete tLabel;
				QRect rcLabel = st.qPreLabel->geometry();
				st.qPreLabel->setGeometry(QRect(rcEdit.right(), rcLabel.top(),rcLabel.width(),rcLabel.height()));
				break;
			}
		}
	}
	else if(sName.compare("wc") == 0)
	{
		QRect rc = edit->geometry();
		QLabel *tLabel = new QLabel(this);
		tLabel->setFont(ft);
		tLabel->setAlignment(Qt::AlignLeft);
		tLabel->setText(sText);
		tLabel->adjustSize();
		QRect tRc = tLabel->geometry();
		rc.setWidth(tRc.width()+4);
		edit->setGeometry(rc);
		delete tLabel;	
	}
	else if (sName.compare("zoomfactor") == 0)
	{
		QRect rc = edit->geometry();
		QLabel *tLabel = new QLabel(this);
		tLabel->setFont(ft);
		tLabel->setAlignment(Qt::AlignLeft);
		tLabel->setText(sText);
		tLabel->adjustSize();
		QRect tRc = tLabel->geometry();
		QRect curRc = QRect(rect().right() - tRc.width() - 4, rc.top(), tRc.width() + 4, rc.height());
		edit->setGeometry(curRc);
		delete tLabel;
	}	
}

void ImageWnd::OnEditFinished()
{
	QLineEdit *edit = (QLineEdit *)sender();
	QString sName = edit->objectName();
	if (sName.compare("ww") == 0)
	{
		int nWw = edit->text().toInt();
		m_pImg->Hs_WinLevel(nWw, m_pImg->m_ImgState.CurWc.y, false);
	}
	else if(sName.compare("wc") == 0)
	{
		int nWc = edit->text().toInt();
		m_pImg->Hs_WinLevel(m_pImg->m_ImgState.CurWc.x, nWc, false);
	}
	else if (sName.compare("zoomfactor") == 0)
	{
		double fZf = edit->text().toDouble();
		m_pImg->m_ImgState.fZoomX = fZf;
		m_pImg->SetWndRc(CalDisplayRect(m_pImg));
	}
	else if (sName.compare("slicethick") == 0)
	{
		double fSliceThick = edit->text().toDouble();
		emit ImageThickChange(fSliceThick);
	}
	edit->clearFocus();
	update();
}

void ImageWnd::OnMprLinesShow(bool isShow)
{
	if (m_pOperateLines)
	{
		m_pOperateLines->SetMprLineShow(isShow);
	}
	update();
}




