#ifndef VTK3DWND_H
#define VTK3DWND_H

#include <QWidget>
#include "stdafx.h"
#include "VtkVRInteractorStyle.h"

namespace Ui {
class Vtk3DWnd;
}

class Vtk3DWnd : public QWidget
{
    Q_OBJECT

public:
    explicit Vtk3DWnd(QWidget *parent = 0);
    ~Vtk3DWnd();

private:
    Ui::Vtk3DWnd *ui;

    void resizeEvent(QResizeEvent *);

    vtkSmartPointer<vtkRenderWindowInteractor>	 m_pIrener;

    vtkSmartPointer<vtkVolume>					 m_pVolume;	//volume掌管映射器和属性对象
//    vtkSmartPointer<vtkSmartVolumeMapper>		 m_pSmartMapper;
    vtkSmartPointer<vtkSlicerGPURayCastVolumeMapper> m_pSlicerMapper;
    vtkSmartPointer<vtkOrientationMarkerWidget>  m_pMarkerWidget;
    vtkSmartPointer<vtkVolumeProperty>			 m_pVolProperty;//设置属性指针
    vtkSmartPointer<vtkAnnotatedCubeActor>       m_pCubeAcor;
    vtkSmartPointer<vtkImageResample>            m_pImgResample;

	vtkSmartPointer<VtkVRInteractorStyle>		 m_pStyle;

public:

    vtkSmartPointer<vtkRenderer>				 m_p3DRenderer;
    vtkSmartPointer<vtkPiecewiseFunction>	 	 m_pPieceFun;
    vtkSmartPointer<vtkColorTransferFunction>	 m_pClrTrans;

    void Setup3DVR();//设置共享属性，方位方块等
    void Set3DVRmode(QString sMode);//透明度等设置
    void SetupCamera(QString sOrientationName);//设置观看角度

	void SetImageDate(vtkSmartPointer<vtkImageData> pImageData);

	void ReRender();

    double ReductionFactor;
    double FrameRate;

public:
	void ChangeOperate(QString sOperate);

};

#endif // VTK3DWND_H
