#ifndef WORKZONE_H
#define WORKZONE_H

#include <QWidget>

class CHsImage;
class Vtk2DWnd;

namespace Ui {
class WorkZone;
}

class WorkZone : public QWidget
{
    Q_OBJECT

public:
    explicit WorkZone(QWidget *parent = 0);
    ~WorkZone();

private:
    Ui::WorkZone *ui;

	QVector<Vtk2DWnd*> m_v2DWnd;	//图像窗口队列
	vector<CHsImage*> m_vImage;		//显示图像

	int m_nOriImageType;			//原始图像类型
	int m_nDataX, m_nDataY, m_nDataZ;

	int m_nOldSlicePos;	//判断层厚是否发生变化

	bool m_bOblique;

public:
	void ClearImg(bool bIncludeVR);

	void SetImageVector(vector<CHsImage*> vImage){ m_vImage = vImage; }

	void ProcessImageData();

	void InitDisplayWnd();

	void ReRenderVrWnd();

	void VrOperteChange(QString sOperateName);
	void ImgOperteChange(QString sOperateName);

	void InitVolumePropertyWidget(ctkVTKVolumePropertyWidget *pCtkVPwidget);

	vtkSmartPointer<vtkImageData> m_pImageData;
	void ***m_pImageArray;

private:
	void InitSignalsAndSlots();

private slots:
	void Btn_VrOrientationClick();
	void CB_VrModeChanged(QString sModeName);
	void Btn_SetMprState(int nBtnID);
	void OnMprLinesInfo(MprLinesInfo info);
	void CB_MprModeChanged(QString sModeName);
	void OnEditSTChange(double dSliceThick);
	void OnScrollBarChange(QString sWndName, int nValue);
signals:
	void SetWaitProgress(int);
	void MprLinesShowChange(bool);
	void ActiveMprOblique(bool);
};

#endif // WORKZONE_H
