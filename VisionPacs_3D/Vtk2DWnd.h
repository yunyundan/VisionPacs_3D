#ifndef VTK2DWND_H
#define VTK2DWND_H

#include <QWidget>

class CHsImage;
class ImageWnd;

namespace Ui {
class Vtk2DWnd;
}

class Vtk2DWnd : public QWidget
{
    Q_OBJECT

public:
    explicit Vtk2DWnd(QWidget *parent = 0);
    ~Vtk2DWnd();

private:
    Ui::Vtk2DWnd *ui;

	int m_nOriImgType;
	int m_nImageNum;

public:
	void InitShowImage(int nOriImgType);
	void ChangeOperate(QString operate);
	void SetImageData(void ***pImgArray, vtkSmartPointer<vtkImageData> p3Ddata, vector<CHsImage*> vOriImg);

private slots:
	void OnScrollBarMoved(int nValue);
	void RecieveImageNum(int nImgNum);

};

#endif // VTK2DWND_H
