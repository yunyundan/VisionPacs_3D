#ifndef IMAGEWND_H
#define IMAGEWND_H

#include <QWidget>


class CHsImage;
class CHmyMprMaker;
class WorkZone;
class OperateMprLines;
class Hmy3DVector;
class HmyPlane3D;
class HmyLine3D;
class HmyImageData3D;
class CResliceControl;

namespace Ui {
class ImageWnd;
}

class ImageWnd : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWnd(QWidget *parent = 0);
    ~ImageWnd();

private:
    Ui::ImageWnd *ui;

	//聚焦标记
	bool m_bFocused; 

	//计算图像在窗口内显示的rc
	RECT CalDisplayRect(CHsImage*pImg);//RECT DlgRc,

	int m_nInteractionState;//鼠标操作的状态

	//鼠标隐藏计数
	QPoint m_PrePoint;//移动、缩放、WC等,需要鼠标移动控制效果

	HSRECT m_fImgRc;//精确的图像区域

	//确定窗口图像类型
	int m_nImgWndType;

	//普通MPR切割类
	CHmyMprMaker *m_pMprMaker;

	//本窗口图像总数
	int m_nImgNum;

	//初始化角落信息
	void RefreshCornorInfoWidget();
	bool m_bInitCorInfo;
	
	//角落信息处理
	typedef struct _ROWITEM
	{
		QString sValue;//填充值之后
		QString sType;
		_ROWITEM()
		{
			sValue = "";
			sType = "Normal";
		}
	}ROWITEM;

	typedef struct _QEDITITEM
	{
		QString sName;
		QString sPos;
		QLineEdit *qEdit;
		QLabel *qPreLabel;
		QLabel *qNextLabel;
		_QEDITITEM()
		{
			sName = "";
			sPos = "";
			qEdit = NULL;
			qPreLabel = NULL;
			qNextLabel = NULL;
		}
	}QEDITITEM;

	vector<QEDITITEM> m_vCornorEdit;
	int ArrangeCorinfo(CORNORINFO corInfo,map<int,ROWITEM> &mapRow);
	void InitNormalCorInfo();
	void InitPosCorInfo();
	void ReSetPosCorInfoPos(QSize deltaSize);
	void RefreshPosCorInfo();

	//标签集合，方便调整位置
	map<QLabel*,QString> m_mapInfoLabel;
	map<QLineEdit *, QString> m_mapInfoEdit;

	//序数
	int m_nCurImgIndex;

	//图像
	HmyLine3D *m_pLine1;
	HmyLine3D *m_pLine2;
	CHsImage *m_pImg;

	//操作线
	OperateMprLines *m_pOperateLines;

	//切割面管理器
	CResliceControl *m_pResliceControl;

protected:
	QPixmap *m_pPixmap;
	QImage *m_pQImage;

	void paintEvent(QPaintEvent *event);
	void resizeEvent(QResizeEvent* size);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	
public:
	
	unsigned int m_nLeftButtonInteractionStyle;
	unsigned int m_nRightButtonInteractionStyle;

public:
	//设置本窗体是否为聚焦窗口
	void SetFocuse(bool bFocused){ m_bFocused = bFocused; } 

	//获得本窗口是否为聚焦窗口
	bool IsFocused(){ return m_bFocused; }	

	//判断本窗口内是否有图像显示
	bool IsImgEmpty();

	//设置本窗口内显示图像
	int SetImage(CHsImage *pImg);

	void ConvertCoord(long *x1, long *y1, long *x2, long *y2, bool bFromHwdToImg);

	//设置操作
	void SetOperate(QString operate);

	//窗口左边转图像坐标
	POINT ConvertWndToImg(RECT ImgRcOnWnd, long nImgW, long nImgH, QPoint &pt);

	long m_nCurW, m_nCurC;

	void ***m_p3DArray;
	vtkSmartPointer<vtkImageData> m_p3DImgData;
	vector<CHsImage*> m_vOriImg;

	//初始化参数
	void InitImageDataPara(void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, vector<CHsImage*> vOriImg, HmyImageData3D *pHmyImgData);
	void SetResliceControl(CResliceControl *pResliceContrl);
	void SetImageWndType(QString sWndName);

	//初次显示图像
	int FirstCalAndShowImage();

	//根据叙述正向位置的Line参数
	void SetOrthogonalIndex(int nIndex);

	//获得图像
	int CalcAndShowImg(QString sWndName, int nSliceNum);
	void CalcAndShowImg();

	//获得操作线
	OperateMprLines *GetOperateLine() { return m_pOperateLines; }

	//获得图像
	CHsImage *GetImage() { return m_pImg; }

	//设置MPR模式
	void SetMprMode(QString sModeName);

	//MPR切面方程
	HmyImageData3D *m_pHmyImageData;

	//计算此直线在本图像坐标系上的空间方向
	void GetLinesDirection(QPoint pt1, QPoint pt2, HmyLine3D &line);
	
signals:
	void SendImageNum(int);
	void ImageIndexChange(int);
	void ImageThickChange(double);

private slots:
	void OnEditTextChanged(const QString &sText);
	void OnEditFinished();
	void OnMprLinesShow(bool isShow);
	
	
};

#endif // IMAGEWND_H
