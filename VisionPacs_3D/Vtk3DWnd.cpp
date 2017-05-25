#include "stdafx.h"
#include "Vtk3DWnd.h"
#include "ui_Vtk3DWnd.h"
#include "VtkHeader.h"
#include "HsFile.h"
#include "AppConfig.h"

Vtk3DWnd::Vtk3DWnd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Vtk3DWnd)
{
    ui->setupUi(this);


    m_p3DRenderer = vtkSmartPointer<vtkRenderer>::New();
    ui->VtkWidget->GetRenderWindow()->AddRenderer(m_p3DRenderer);
    ui->VtkWidget->GetRenderWindow()->SetGlobalWarningDisplay(0);
	m_p3DRenderer->ResetCamera();

    // set up interactor
    m_pIrener = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    m_pIrener->SetRenderWindow(ui->VtkWidget->GetRenderWindow());

    m_pStyle = vtkSmartPointer<VtkVRInteractorStyle>::New();
	m_pStyle->SetView(this);
	m_pStyle->SetLeftButtonInteractionStyle(VtkVRInteractorStyle::PICK_INTERACTION);
	m_pStyle->SetRightButtonInteractionStyle(VtkVRInteractorStyle::ZOOM_INTERACTION);
	m_pStyle->SetVersionStandard(true);
	m_pIrener->SetInteractorStyle(m_pStyle);

    //指针初始化
    m_pVolProperty = vtkSmartPointer<vtkVolumeProperty>::New();

	//m_pSmartMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    m_pSlicerMapper = vtkSmartPointer<vtkSlicerGPURayCastVolumeMapper>::New();

    m_pVolume = vtkSmartPointer<vtkVolume>::New();

    m_pCubeAcor = vtkSmartPointer<vtkAnnotatedCubeActor>::New();

    m_pMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();

    m_pPieceFun = vtkSmartPointer<vtkPiecewiseFunction>::New();

    m_pClrTrans = vtkSmartPointer<vtkColorTransferFunction>::New();

    m_pImgResample = vtkSmartPointer<vtkImageResample>::New();

    FrameRate = 0.8;
    ReductionFactor = 0.6;

	ui->VtkWidget->GetRenderWindow()->Render();	
}

Vtk3DWnd::~Vtk3DWnd()
{
    delete ui;
}

void Vtk3DWnd::resizeEvent(QResizeEvent *)
{
	QRect rcWin = rect();
	ui->VtkWidget->GetRenderWindow()->SetSize(rcWin.width(), rcWin.height());
}

void Vtk3DWnd::Setup3DVR()
{
    m_pIrener->SetDesiredUpdateRate(FrameRate);

    //m_pImgResample->SetInputConnection(m_pChange->GetOutputPort());
    m_pImgResample->SetAxisMagnificationFactor(0, ReductionFactor);
    m_pImgResample->SetAxisMagnificationFactor(1, ReductionFactor);
    m_pImgResample->SetAxisMagnificationFactor(2, ReductionFactor);

    //光线投射映射器渲染数据
    m_pSlicerMapper->SetInputConnection(m_pImgResample->GetOutputPort());//m_pSmartMapper

    ////体渲染方向块
    m_pCubeAcor->SetFaceTextScale(0.3);
    m_pCubeAcor->GetCubeProperty()->SetColor(0.77, 0.81, 0.78);
    m_pCubeAcor->GetTextEdgesProperty()->SetLineWidth(1);
    m_pCubeAcor->GetTextEdgesProperty()->SetDiffuse(1);
    m_pCubeAcor->GetTextEdgesProperty()->SetAmbient(1);
    m_pCubeAcor->GetTextEdgesProperty()->SetColor(0.28, 0.28, 0.28);

    m_pCubeAcor->SetXPlusFaceText("L");
    m_pCubeAcor->SetXMinusFaceText("R");
    m_pCubeAcor->SetYPlusFaceText("A");
    m_pCubeAcor->SetYMinusFaceText("P");
    m_pCubeAcor->SetZPlusFaceText("I");
    m_pCubeAcor->SetZMinusFaceText("S");
    m_pCubeAcor->SetXFaceTextRotation(180);
    m_pCubeAcor->SetYFaceTextRotation(180);
    m_pCubeAcor->SetZFaceTextRotation(270);

    // 改变方块字体颜色
    m_pCubeAcor->GetXPlusFaceProperty()->SetColor(0, 1, 0);
    m_pCubeAcor->GetXPlusFaceProperty()->SetInterpolationToFlat();
    m_pCubeAcor->GetXMinusFaceProperty()->SetColor(0, 1, 0);
    m_pCubeAcor->GetXMinusFaceProperty()->SetInterpolationToFlat();
    m_pCubeAcor->GetYPlusFaceProperty()->SetColor(0, 0, 1);
    m_pCubeAcor->GetYPlusFaceProperty()->SetInterpolationToFlat();
    m_pCubeAcor->GetYMinusFaceProperty()->SetColor(0, 0, 1);
    m_pCubeAcor->GetYMinusFaceProperty()->SetInterpolationToFlat();
    m_pCubeAcor->GetZPlusFaceProperty()->SetColor(1, 0, 0);
    m_pCubeAcor->GetZPlusFaceProperty()->SetInterpolationToFlat();
    m_pCubeAcor->GetZMinusFaceProperty()->SetColor(1, 0, 0);
    m_pCubeAcor->GetZMinusFaceProperty()->SetInterpolationToFlat();

	m_pMarkerWidget->SetViewport(0.85, 0, 1, 0.15);
	m_pMarkerWidget->SetOutlineColor(0, 0, 0);
	m_pMarkerWidget->SetOrientationMarker(m_pCubeAcor);
	m_pMarkerWidget->SetInteractor(m_pIrener);
	m_pMarkerWidget->SetCurrentRenderer(m_p3DRenderer);
	m_pMarkerWidget->SetEnabled(1);
	m_pMarkerWidget->InteractiveOff();

    //设置属性，其中包括两个映射
    m_pVolProperty->SetColor(m_pClrTrans);
    m_pVolProperty->SetScalarOpacity(m_pPieceFun);

    m_pVolume->SetProperty(m_pVolProperty);
    m_pVolume->SetMapper(m_pSlicerMapper);//m_pSmartMapper

    //volume掌管映射器和属性对象
    m_p3DRenderer->AddVolume(m_pVolume);
}

void Vtk3DWnd::Set3DVRmode(QString sMode)
{
	if (sMode.compare("Bone") == 0)
	{
		// 创建标量到不透明度的转移函数
		m_pPieceFun->RemoveAllPoints();
		m_pClrTrans->RemoveAllPoints();

		m_pPieceFun->AddPoint(-2048, 0, 0.5, 0.0);
		m_pPieceFun->AddPoint(130, 0, 0.5, 0.0);
		m_pPieceFun->AddPoint(680, 1, 0.5, 0.0);
		m_pPieceFun->AddPoint(3071, 1, 0.5, 0.0);

		//创建标量到颜色的转移函数

		m_pClrTrans->AddRGBPoint(-2048, 1, 1, 1);
		m_pClrTrans->AddRGBPoint(-140, 1, 0.54, 0.27);
		m_pClrTrans->AddRGBPoint(2, 0.96, 0.48, 0.16);
		m_pClrTrans->AddRGBPoint(99, 0.95, 0.19, 0.25);
		m_pClrTrans->AddRGBPoint(230, 1, 0.71, 0.25);
		m_pClrTrans->AddRGBPoint(330, 1, 0.96, 0.85);
		m_pClrTrans->AddRGBPoint(515, 1, 1, 1);
		m_pClrTrans->AddRGBPoint(3071, 1, 1, 1);


		m_pSlicerMapper->SetBlendModeToComposite();//m_pSmartMapper
		m_pVolProperty->ShadeOn();
		m_pVolProperty->SetInterpolationTypeToLinear();
		m_pVolProperty->SetAmbient(0.3);
		m_pVolProperty->SetDiffuse(0.6);
		m_pVolProperty->SetSpecular(0.5);
		m_pVolProperty->SetSpecularPower(40.0);
		m_pVolProperty->SetScalarOpacityUnitDistance(0.8919);
	}
	else if (sMode.compare("Heart") == 0)
	{
		m_pPieceFun->RemoveAllPoints();
		m_pClrTrans->RemoveAllPoints();

		m_pPieceFun->AddPoint(-2048, 0, 0.5, 0.0);
		m_pPieceFun->AddPoint(-61.16, 0, 0.5, 0.15);
		m_pPieceFun->AddPoint(488, 1, 0.0, 0.0);
		m_pPieceFun->AddPoint(1063, 0, 0.5, 0.0);

		m_pClrTrans->AddRGBPoint(-2048, 1, 1, 1);
		m_pClrTrans->AddRGBPoint(-140, 1, 0.54, 0.27);
		m_pClrTrans->AddRGBPoint(2, 0.96, 0.48, 0.16);
		m_pClrTrans->AddRGBPoint(69, .90, .23, .20);
		m_pClrTrans->AddRGBPoint(238, .93, .93, .7);
		m_pClrTrans->AddRGBPoint(238, .90, .78, .42);
		m_pClrTrans->AddRGBPoint(275, 1, 1, 1);

		m_pSlicerMapper->SetBlendModeToComposite();
		m_pVolProperty->ShadeOn();
		m_pVolProperty->SetAmbient(0.3);
		m_pVolProperty->SetDiffuse(0.6);
		m_pVolProperty->SetSpecular(0.5);
		m_pVolProperty->SetSpecularPower(40.0);
		m_pVolProperty->SetScalarOpacityUnitDistance(0.8919);
	}
	else if (sMode.compare("Kidney") == 0)
	{
		m_pPieceFun->RemoveAllPoints();
		m_pClrTrans->RemoveAllPoints();

		m_pPieceFun->AddPoint(-2048, 0, 0.5, 0.0);
		m_pPieceFun->AddPoint(96, 0, .2, .0);
		m_pPieceFun->AddPoint(684, 1, .5, 0.0);
		m_pPieceFun->AddPoint(3071, 0, 0.5, 0.0);

		m_pClrTrans->AddRGBPoint(-2048, 1, 1, 1);
		m_pClrTrans->AddRGBPoint(-140, 1, 0.54, 0.27);
		m_pClrTrans->AddRGBPoint(2, 0.96, 0.48, 0.16);
		m_pClrTrans->AddRGBPoint(30, .41, .52, .51);
		m_pClrTrans->AddRGBPoint(110, 1, 1, 1);
		m_pClrTrans->AddRGBPoint(120, .95, .19, .2);
		m_pClrTrans->AddRGBPoint(200, 1, 0.71, 0.25);
		m_pClrTrans->AddRGBPoint(250, 1, 1, 1);
		m_pClrTrans->AddRGBPoint(330, 0.98, 0.89, 0.78);
		m_pClrTrans->AddRGBPoint(1770, 1, 1, 1);

		m_pSlicerMapper->SetBlendModeToComposite();
		m_pVolProperty->ShadeOn();
		m_pVolProperty->SetAmbient(0.3);
		m_pVolProperty->SetDiffuse(0.6);
		m_pVolProperty->SetSpecular(0.5);
		m_pVolProperty->SetSpecularPower(40.0);
		m_pVolProperty->SetScalarOpacityUnitDistance(0.8919);
	}   
	else if (sMode.compare("MIP") == 0)
	{
		m_pPieceFun->RemoveAllPoints();
		m_pClrTrans->RemoveAllPoints();

		double range[2];
		m_pImgResample->GetOutput()->GetScalarRange(range);

		m_pPieceFun->AddPoint(range[0], 0.0, 0.5, 0.0);
		m_pPieceFun->AddPoint(range[1] / 2, 1.0, 0.5, 0.0);
		m_pClrTrans->AddRGBSegment(0.0, 1.0, 1.0, 1.0, 255.0, 1.0, 1.0, 1.0);
		m_pSlicerMapper->SetBlendModeToMaximumIntensity();
		m_pVolProperty->SetIndependentComponents(TRUE);
		m_pVolProperty->ShadeOn();
		m_pVolProperty->SetAmbient(0.3);
		m_pVolProperty->SetDiffuse(0.6);
		m_pVolProperty->SetSpecular(0.5);
		m_pVolProperty->SetSpecularPower(40.0);
		m_pVolProperty->SetScalarOpacityUnitDistance(0.8919);
		m_pVolProperty->SetInterpolationTypeToLinear();
	}
}

void Vtk3DWnd::SetupCamera(QString sOrientationName)
{
	if (sOrientationName.compare("Orientation_I") == 0)
	{
		m_p3DRenderer->GetActiveCamera()->SetPosition(0, 0, 1);
		m_p3DRenderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	}
	else if (sOrientationName.compare("Orientation_S") == 0)
	{
		m_p3DRenderer->GetActiveCamera()->SetPosition(0, 0, -1);
		m_p3DRenderer->GetActiveCamera()->SetViewUp(0, 1, 0);
	}
	else if (sOrientationName.compare("Orientation_A") == 0)
	{
		m_p3DRenderer->GetActiveCamera()->SetPosition(0, 1, 0);
		m_p3DRenderer->GetActiveCamera()->SetViewUp(0, 0, -1);
	}
	else if (sOrientationName.compare("Orientation_P") == 0)
	{
		m_p3DRenderer->GetActiveCamera()->SetPosition(0, -1, 0);
		m_p3DRenderer->GetActiveCamera()->SetViewUp(0, 0, -1);
	}
	else if (sOrientationName.compare("Orientation_L") == 0 )
	{
		m_p3DRenderer->GetActiveCamera()->SetPosition(1, 0, 0);
		m_p3DRenderer->GetActiveCamera()->SetViewUp(0, 0, -1);
	}
	else if (sOrientationName.compare("Orientation_R") == 0)
	{
		m_p3DRenderer->GetActiveCamera()->SetPosition(-1, 0, 0);
		m_p3DRenderer->GetActiveCamera()->SetViewUp(0, 0, -1);
	}

	m_p3DRenderer->GetActiveCamera()->SetFocalPoint(0, 0, 0);
	m_p3DRenderer->GetActiveCamera()->ComputeViewPlaneNormal();

	//double bons[6];
 //	m_pImgResample->GetOutput()->GetBounds(bons);

	//double xs = (bons[1] - bons[0]);
	//double ys = (bons[3] - bons[2]);
	//double zs = (bons[5] - bons[4]);

	//double bnds_x = xs / 2.0;
	//double bnds_y = ys / 2.0;
	//double bnds_z = zs / 2.0;

	//double temp = bnds_x > bnds_y ? bnds_x : bnds_y;
	//double ParaScale = temp > bnds_z ? temp : bnds_z;

	//Update渲染
	m_p3DRenderer->ResetCamera();
	//m_p3DRenderer->GetActiveCamera()->ParallelProjectionOn();
	//m_p3DRenderer->GetActiveCamera()->SetParallelScale(ParaScale*2/3);
}

void Vtk3DWnd::SetImageDate(vtkSmartPointer<vtkImageData> pImageData)
{
	m_pImgResample->SetInputData(pImageData);
	m_pImgResample->Update();

	Setup3DVR();
	Set3DVRmode("Bone");	

	SetupCamera("Orientation_A");

	SetupCornorInfo();

	m_pIrener->Initialize();
	ui->VtkWidget->GetRenderWindow()->Render();	
}

void Vtk3DWnd::SetupCornorInfo()
{
	if (m_pSourceDs == NULL)
		return;

	int nRet = 0;

	MODINFO modInfo;
	pHsElement pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_MODALITY, false);
	if (pEle)
	{
		int nRet;
		QString sModality = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		bool b = CAppConfig::GetInfoSet(sModality, modInfo);
	}

	QFont ft;
	ft.setPointSize(modInfo.nSize);
	ft.setFamily(modInfo.sFaceName);	
	if (1)//左上角
	{
		QString sLtValue = "";
		pHsElement pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_MANUFACTURER, false);
		if (pEle)
		{
			sLtValue = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}
		QLabel *LTlabel1 = new QLabel(ui->VtkWidget);
		LTlabel1->setFont(ft);
		LTlabel1->setAlignment(Qt::AlignLeft);
		LTlabel1->setText(sLtValue);
		LTlabel1->adjustSize();
		QRect rcLable = LTlabel1->geometry();
		rcLable = QRect(2,2, rcLable.width(), rcLable.height());
		LTlabel1->setGeometry(rcLable);
		LTlabel1->show();

		pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_STUDY_ID, false);
		if (pEle)
		{
			sLtValue = "Ex:"+ m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}

		QLabel *LTlabel2 = new QLabel(ui->VtkWidget);
		LTlabel2->setFont(ft);
		LTlabel2->setAlignment(Qt::AlignLeft);
		LTlabel2->setText(sLtValue);
		LTlabel2->adjustSize();
		rcLable = LTlabel2->geometry();
		rcLable = QRect(2, 2+rcLable.height(), rcLable.width(), rcLable.height());
		LTlabel2->setGeometry(rcLable);
		LTlabel2->show();
	}

	if (1)
	{
		QString sLtValue = "";
		pHsElement pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_INSTITUTION_NAME, false);
		if (pEle)
		{
			sLtValue = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}

		QLabel *RTlabel1 = new QLabel(ui->VtkWidget);
		RTlabel1->setFont(ft);
		RTlabel1->setAlignment(Qt::AlignLeft);
		RTlabel1->setText(sLtValue);
		RTlabel1->adjustSize();
		QRect rcLable = RTlabel1->geometry();
		rcLable = QRect(rect().right() - rcLable.width() - 2, 2, rcLable.width(), rcLable.height());
		RTlabel1->setGeometry(rcLable);
		RTlabel1->show();

		pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_PATIENT_NAME, false);
		if (pEle)
		{
			sLtValue = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}

		QLabel *RTlabel2 = new QLabel(ui->VtkWidget);
		RTlabel2->setFont(ft);
		RTlabel2->setAlignment(Qt::AlignLeft);
		RTlabel2->setText(sLtValue);
		RTlabel2->adjustSize();
		rcLable = RTlabel2->geometry();
		rcLable = QRect(rect().right() - rcLable.width() - 2, 2 + rcLable.height(), rcLable.width(), rcLable.height());
		RTlabel2->setGeometry(rcLable);
		RTlabel2->show();

		pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_PATIENT_SEX, false);
		if (pEle)
		{
			sLtValue = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}

		QLabel *RTlabel3 = new QLabel(ui->VtkWidget);
		RTlabel3->setFont(ft);
		RTlabel3->setAlignment(Qt::AlignLeft);
		RTlabel3->setText(sLtValue);
		RTlabel3->adjustSize();
		rcLable = RTlabel3->geometry();
		rcLable = QRect(rect().right() - rcLable.width() - 2, 2 + rcLable.height()*2, rcLable.width(), rcLable.height());
		RTlabel3->setGeometry(rcLable);
		RTlabel3->show();
		
		pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_PATIENT_AGE, false);
		if (pEle)
		{
			sLtValue = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}

		QLabel *RTlabel4 = new QLabel(ui->VtkWidget);
		RTlabel4->setFont(ft);
		RTlabel4->setAlignment(Qt::AlignLeft);
		RTlabel4->setText(sLtValue);
		RTlabel4->adjustSize();
		rcLable = RTlabel4->geometry();
		rcLable = QRect(rect().right() - rcLable.width() - 2, 2 + rcLable.height() * 3, rcLable.width(), rcLable.height());
		RTlabel4->setGeometry(rcLable);
		RTlabel4->show();
	}


	if (1)
	{
		QString sLtValue = "";
		pHsElement pEle = m_pSourceDs->Hs_FindFirstEle(NULL, TAG_MODALITY, false);
		if (pEle)
		{
			sLtValue = m_pSourceDs->Hs_GetConvertValue(pEle, 0, nRet);
		}

		QLabel *RBlabel1 = new QLabel(ui->VtkWidget);
		RBlabel1->setFont(ft);
		RBlabel1->setAlignment(Qt::AlignLeft);
		RBlabel1->setText(sLtValue);
		RBlabel1->adjustSize();
		QRect rcLable = RBlabel1->geometry();
		rcLable = QRect(rect().right() - rcLable.width() - 2, rect().bottom()-rcLable.height()*2, rcLable.width(), rcLable.height());
		RBlabel1->setGeometry(rcLable);
		RBlabel1->show();

		QLabel *RBlabel2 = new QLabel(ui->VtkWidget);
		RBlabel2->setFont(ft);
		RBlabel2->setAlignment(Qt::AlignLeft);
		RBlabel2->setText("VR");
		RBlabel2->adjustSize();
		rcLable = RBlabel2->geometry();
		rcLable = QRect(rect().right() - rcLable.width() - 2, rect().bottom() - rcLable.height(), rcLable.width(), rcLable.height());
		RBlabel2->setGeometry(rcLable);
		RBlabel2->show();
	}
}

void Vtk3DWnd::ReRender()
{
	ui->VtkWidget->GetRenderWindow()->Render();
}

void Vtk3DWnd::ConnectVolumeToWidget(ctkVTKVolumePropertyWidget *pCtkVPwidget)
{
	pCtkVPwidget->setVolumeProperty(m_pVolProperty);
}

void Vtk3DWnd::ChangeOperate(QString sOperate)
{
	if (sOperate.compare("VR_location") == 0)
	{
		m_pStyle->SetLeftButtonInteractionStyle(VtkVRInteractorStyle::PICK_INTERACTION);
	}
	else if (sOperate.compare("VR_rotate") == 0)
	{
		m_pStyle->SetLeftButtonInteractionStyle(VtkVRInteractorStyle::ROTATE_INTERACTION);
	}
	else if (sOperate.compare("VR_zoom") == 0)
	{
		m_pStyle->SetRightButtonInteractionStyle(VtkVRInteractorStyle::ZOOM_INTERACTION);
	}
	else if (sOperate.compare("VR_pan") == 0)
	{
		m_pStyle->SetRightButtonInteractionStyle(VtkVRInteractorStyle::PAN_INTERACTION);
	}
}
