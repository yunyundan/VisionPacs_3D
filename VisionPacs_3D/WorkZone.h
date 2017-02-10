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

public:
	void showImg(CHsImage *pImg);

	void ClearImg(bool bIncludeVR);

	void SetImageVector(vector<CHsImage*> vImage){ m_vImage = vImage; }

	void ProcessImageData();

	void InitDisplayWnd();

	vtkSmartPointer<vtkImageData> m_pImageData;

	void ReRenderVrWnd();

private slots:
	void Btn_ImgOperateClick();
	void Btn_VrOperateClick();
	void Btn_VrOrientationClick();

signals:
	void SetWaitProgress(int);
};

#endif // WORKZONE_H
