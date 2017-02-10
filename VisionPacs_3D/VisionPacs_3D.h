#ifndef VISIONPACS_3D_H
#define VISIONPACS_3D_H

#include <QtWidgets/QMainWindow>
#include "ui_VisionPacs_3D.h"
#include "WaitDlg.h"

class CHsFile;
class CHsImage;

//多期MAP
typedef struct _DuoQiVector
{
	vector<CHsImage *> imgV;
	int nInstanceID;
	int nFrameID;
	bool bLoaded;
	_DuoQiVector()
	{
		nInstanceID = nFrameID = 0;
		bLoaded = false;
	}
}DuoQiVector;

typedef map<int, DuoQiVector> PRIODLIST;

class VisionPacs_3D : public QMainWindow
{
	Q_OBJECT

public:
	VisionPacs_3D(QWidget *parent = 0);
	~VisionPacs_3D();

private:
	Ui::VisionPacs_3D *ui;

	//文件序列信息
	QString m_sFilePath;
	QString m_sSeriesUID;

	//窗口中正在显示的序列UID
	QString m_sOnShowSeriesUID;

	//序列中所有文件信息，图像信息
	QVector<CHsFile*>  m_vAllDcmFile;
	QVector<CHsImage*> m_vAllImage;

	//多期MAP
	PRIODLIST m_mPriodList;
	int m_iPeriodNum;

	int m_iCurPeriod;

	//显示进度窗口
	WaitDlg *m_pWaitDlg;
	void ShowWaitDlg();

signals:
	void SetWaitProgress(int);

private slots:
	void on_actionOpen_File_triggered();
	void on_actionTest_triggered();
	void StartProcessImage();

public:
	//在指定文件夹中筛选出指定seriesUID的图像并排序
	void ExtractImage(QString sFilePath, QString sSeriesUID, vector<string> &vImagePathList);
	bool ReadImgListFromIni(QString sFilePath, QString sSeriesUID, vector<string> &vImagePathList);

	//读取图像信息
	bool ReadAllImageInfo(vector<string> vImagePathList);

	//选择特定的分期进行处理（无分期也要处理）
	bool ChoosePeriod(int iPeriod);

};

#endif // VISIONPACS_3D_H
