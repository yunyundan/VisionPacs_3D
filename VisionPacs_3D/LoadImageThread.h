#pragma once

#include <QThread>

class CHsImage;

class LoadImageThread : public QThread
{
	Q_OBJECT
private:
	CHsImage *m_pLoadImage;
	int m_nImageIndex;
	vtkSmartPointer<vtkImageData> m_pVtkData;
	int m_nOriImageType;

protected:

	void run();

public:
	LoadImageThread(QObject *parent);
	~LoadImageThread();

	void SetThreadPara(CHsImage *pImage, int nIndex, vtkSmartPointer<vtkImageData> m_pImageData, int nOriImageType);
};
