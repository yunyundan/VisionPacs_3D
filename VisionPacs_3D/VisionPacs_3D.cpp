#include "stdafx.h"
#include "VisionPacs_3D.h"
#include "HsFile.h"
#include "HsImage.h"


VisionPacs_3D::VisionPacs_3D(QWidget *parent)
	: QMainWindow(parent),
	ui(new Ui::VisionPacs_3D)
	, m_sOnShowSeriesUID("")
	, m_iPeriodNum(1)
	, m_iCurPeriod(1)
	, m_pWaitDlg(NULL)
	, m_sFilePath("")
	, m_sSeriesUID("")
{
	ui->setupUi(this);
	setWindowState(Qt::WindowMaximized);
	//设置窗体标题栏隐藏
	setWindowFlags(Qt::FramelessWindowHint);
	//可获取鼠标跟踪效果
	setMouseTracking(true);
}

VisionPacs_3D::~VisionPacs_3D()	
{
	delete ui;

	for (int i=0; i<m_vAllDcmFile.size(); i++)
	{
		delete m_vAllDcmFile[i];
	}
	m_vAllDcmFile.clear();

	for (int i=0; i<m_vAllImage.size();i++)
	{
		delete m_vAllImage[i];
	}
	m_vAllImage.clear();

	if (m_pWaitDlg)
	{
		delete m_pWaitDlg;
		m_pWaitDlg = NULL;
	}
}

void VisionPacs_3D::on_actionOpen_File_triggered()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open Dicom"), ".", tr("Dicom File(*.dcm)"));

	CHsFile *hf = new CHsFile;

	hf->Hs_LoadFile(path.toLatin1().data());

	pHsElement pPixEle = hf->Hs_FindFirstEle(NULL, TAG_PIXEL_DATA, true);//找主图

	if (pPixEle == NULL)
		return;

	CHsImage *pImg = new CHsImage;
	int nFrame = hf->Hs_GetFrameCount(pPixEle);
	pImg->m_ImgInfo.nFrame = nFrame;
	hf->Hs_GetImageInfo(pPixEle, pImg->m_ImgInfo, 0);
	pImg->Hs_InitImgState();
	pImg->SetDs(hf);
	ui->Workzone->showImg(pImg);
}

void VisionPacs_3D::on_actionTest_triggered()
{
	QString sFilePath = "E:\\ctdata\\DJ201310B\\DJ20131022B0256";
	QString sSeriesUID = "1.2.392.200036.9116.2.5.1.37.2420749095.1382489636.301658";

	m_sFilePath = sFilePath;
	m_sSeriesUID = sSeriesUID;

	ShowWaitDlg();
}


void VisionPacs_3D::StartProcessImage()
{
	//开始处理图像
	vector<string> vImagePathList;
	ExtractImage(m_sFilePath, m_sSeriesUID, vImagePathList);

	int nRet = ReadAllImageInfo(vImagePathList);
	if (nRet != Ret_Success)
		return;

	m_iPeriodNum = m_mPriodList.size();
	ChoosePeriod(0);

	ui->Workzone->ProcessImageData();
}

void VisionPacs_3D::ShowWaitDlg()
{
	//弹出进度条窗口
	if (m_pWaitDlg == NULL)
	{
		m_pWaitDlg = new WaitDlg(this);
		m_pWaitDlg->setModal(true);
		m_pWaitDlg->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
		QObject::connect(this, SIGNAL(SetWaitProgress(int)), m_pWaitDlg, SLOT(SetProgress(int)));
		QObject::connect(ui->Workzone, SIGNAL(SetWaitProgress(int)), m_pWaitDlg, SLOT(SetProgress(int)));
		QObject::connect(m_pWaitDlg, SIGNAL(StartProcess()), this, SLOT(StartProcessImage()), Qt::QueuedConnection);		
		m_pWaitDlg->show();
	}
}

void VisionPacs_3D::ExtractImage(QString sFilePath, QString sSeriesUID, vector<string> &vImagePathList)
{
	emit SetWaitProgress(0);

	if (m_sOnShowSeriesUID.compare(sSeriesUID) == 0)
		return;
	else
		ui->Workzone->ClearImg(true);

	vImagePathList.clear();
	bool bSeriesIniGo = false;

	//获得对应UID文件列表，分两种方式
	bSeriesIniGo = ReadImgListFromIni(sFilePath,sSeriesUID,vImagePathList);

	bool b = true;
	if (bSeriesIniGo == false)
	{
		gdcm::Directory d;
		d.Load(sFilePath.toLatin1().data(), true);
		const gdcm::Directory::FilenamesType ftAllFilenames = d.GetFilenames();
		const size_t nfiles = ftAllFilenames.size();

		//此过滤器摘出选定SeriesUID图像
		gdcm::SmartPointer<gdcm::Scanner> spSeries = new gdcm::Scanner;
		const gdcm::Tag t1(0x0020, 0x000e);
		spSeries->AddTag(t1);

		b = spSeries->Scan(ftAllFilenames);
		vImagePathList = spSeries->GetAllFilenamesFromTagToValue(t1, sSeriesUID.toLatin1().data());//指定序列图像路径
	}
	emit SetWaitProgress(10);

	//过滤图像SOP类型和类型
	gdcm::SmartPointer<gdcm::Scanner> spFilterScanner = new gdcm::Scanner;//此过滤器为图像类型过滤器
	const gdcm::Tag t2(0x0002, 0x0002);//SOP class
	const gdcm::Tag t3(0x0008, 0x0008);//Image Type;

	spFilterScanner->AddTag(t2);
	spFilterScanner->AddTag(t3);
	b = spFilterScanner->Scan(vImagePathList);

	gdcm::Scanner::ValuesType  vtSopClass = spFilterScanner->GetValues(t2);
	gdcm::Scanner::ValuesType  vtImageType = spFilterScanner->GetValues(t3);
	if (vtSopClass.size() != 1)
	{
		set<string>::iterator set_iter;
		for (set_iter = vtSopClass.begin(); set_iter != vtSopClass.end(); set_iter++)
		{
			string sSopClass = *set_iter;
			if (sSopClass.compare("1.2.840.10008.5.1.4.1.1.66") != 0)
			{
				gdcm::Directory::FilenamesType ftTemp = spFilterScanner->GetAllFilenamesFromTagToValue(t2, sSopClass.c_str());
				vImagePathList = ftTemp;
			}
		}
	}
	if (vtImageType.size() != 1)
	{
		set<string>::iterator set_iter;
		int iFileNum = 0;
		for (set_iter = vtImageType.begin(); set_iter != vtImageType.end(); set_iter++)
		{
			string sImageType = *set_iter;
			gdcm::Directory::FilenamesType ftTemp = spFilterScanner->GetAllFilenamesFromTagToValue(t2, sImageType.c_str());
			int iTempNum = ftTemp.size();
			if (iTempNum > iFileNum)
			{
				vImagePathList = ftTemp;
				iFileNum = iTempNum;
			}
		}
	}
	emit SetWaitProgress(15);
	return;
}

bool VisionPacs_3D::ReadImgListFromIni(QString sFilePath, QString sSeriesUID, vector<string> &vImagePathList)
{
	QString sIniPath = QString("%1\\seriesinfo\\%2.ini").arg(sFilePath).arg(sSeriesUID);
	QFile fSeriesIni(sIniPath);

	if (fSeriesIni.exists() != true)
		return false;

	HANDLE hf = ::CreateFile(sIniPath.toStdWString().data(), GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0);

	if (hf == INVALID_HANDLE_VALUE || hf == NULL)
		return false;

	//读全部字符
	DWORD wSize = ::GetFileSize(hf, NULL);
	if (wSize <= 0)
	{
		::CloseHandle(hf);
		return false;
	}

	char *pInfo = new char[wSize + 100];
	memset(pInfo, 0, wSize);

	DWORD wRead = 0;
	::SetFilePointer(hf, 0, NULL, FILE_BEGIN);

	::ReadFile(hf, pInfo, wSize, &wRead, NULL);

	::CloseHandle(hf);

	//截取需要字符
	QVector<int> iPosV;//"filename="的位置

	int i = 0;
	while (i + 8 < wSize)
	{
		if (pInfo[i] == 'f'	&&
			pInfo[i + 1] == 'i' &&
			pInfo[i + 2] == 'l'	&&
			pInfo[i + 3] == 'e'	&&
			pInfo[i + 4] == 'n'	&&
			pInfo[i + 5] == 'a'	&&
			pInfo[i + 6] == 'm'	&&
			pInfo[i + 7] == 'e' &&
			pInfo[i + 8] == '=')
		{
			iPosV.push_back(i + 9);
		}

		i++;
	}

	int nUID = int(iPosV.size());
	for (int s = 0; s < nUID; s++)
	{
		char ch[100]; memset(ch, 0, 100);

		int pos = iPosV[s];
		for (int c = 0; c < 100; c++)
		{
			if (pos + c >= wSize)
			{
				if (int(strlen(ch)) > 0)
				{
					string fi;
					fi = sFilePath.toStdString();
					fi += "\\";
					fi += ch;
					vImagePathList.push_back(fi);
				}
				break;
			}


			ch[c] = pInfo[pos + c];

			if (ch[c] == '\r' || ch[c] == '\n' || ch[c] == 's')
			{
				ch[c] = '\0';
				if (int(strlen(ch)) > 0)
				{
					string fi;
					fi = sFilePath.toStdString();
					fi += "\\";
					fi += ch;
					vImagePathList.push_back(fi);
				}

				break;
			}
		}
	}
	delete []pInfo;
	return true;
}

bool VisionPacs_3D::ReadAllImageInfo(vector<string> vImagePathList)
{
	for (int i=0; i<m_vAllDcmFile.size();i++)
		delete m_vAllDcmFile[i];
	m_vAllDcmFile.clear();

	for (int i=0; i<m_vAllImage.size();i++)
		delete m_vAllImage[i];
	m_vAllImage.clear();

	m_mPriodList.clear();

	int nRet = Ret_Success;
	double iStep = 20.00 / vImagePathList.size();
	for (int i=0; i<vImagePathList.size();i++)
	{
		CHsFile *pDs = new CHsFile;
		nRet = pDs->Hs_LoadFile(vImagePathList[i].c_str(),false);
		if (nRet != 0)
		{
			QMessageBox::about(this, "错误","载入有误");
			delete pDs;
			return nRet;
		}

		pHsElement pPixEle = pDs->Hs_FindFirstEle(NULL, TAG_PIXEL_DATA, true);
		if (pPixEle == NULL)
		{
			QMessageBox::about(this, "错误", "未发现图像数据");
			delete pDs;
			return Ret_NoPixelFound;
		}

		QString sModality = "";
		pDs->Hs_GetStringValueA(TAG_MODALITY, sModality, true, 0);
		if (sModality.compare("CT") != 0 && sModality.compare("MR") != 0)
		{
			QMessageBox::about(this, "错误", "不支持此类型图像");
			delete pDs;
			return Ret_UnSupportPara;
		}

		int nFrame = pDs->Hs_GetFrameCount(pPixEle);
		if (nFrame <= 0)
		{
			QMessageBox::about(this, "错误", "文件有误");
			delete pDs;
			return Ret_NoPixelFound;
		}
		m_vAllDcmFile.push_back(pDs);
		
		emit SetWaitProgress(15+i*iStep);

		for (int i = 0; i < nFrame; i++)
		{
			CHsImage *pImg = new CHsImage;
			pDs->Hs_GetImageInfo(pPixEle, pImg->m_ImgInfo, i);
			pImg->SetDs(pDs);
			pImg->m_bMpr = true;
			m_vAllImage.push_back(pImg);

			if (pImg->m_ImgInfo.sScanOptions.indexOf("HELICAL") != -1)
			{
				vector<CHsImage *> &v = m_mPriodList[pImg->m_ImgInfo.iAcquisitionNum].imgV;
				v.push_back(pImg);
				m_mPriodList[pImg->m_ImgInfo.iAcquisitionNum].nFrameID = i;
			}
			else
			{
				vector<CHsImage *> &v = m_mPriodList[1].imgV;
				v.push_back(pImg);
				m_mPriodList[1].nFrameID = i;
			}
		}
	}

	return nRet;
}

bool VisionPacs_3D::ChoosePeriod(int iPeriod)
{
	PRIODLIST::const_iterator iter = m_mPriodList.begin();
	int iMiniPriod = (*iter).first;

	m_iCurPeriod = iMiniPriod + iPeriod;

	ui->Workzone->ClearImg(true);

	ui->Workzone->SetImageVector(m_mPriodList[m_iCurPeriod].imgV);

	return true;
}
