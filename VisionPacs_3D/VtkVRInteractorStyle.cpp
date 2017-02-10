#include "stdafx.h"

#include "VtkVRInteractorStyle.h"
#include "vtkObjectFactory.h"

#include "vtkCallbackCommand.h"
#include "vtkCamera.h"
#include "vtkCellPicker.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkPiecewiseFunction.h"
#include "Vtk3DWnd.h"

vtkStandardNewMacro(VtkVRInteractorStyle);

VtkVRInteractorStyle::VtkVRInteractorStyle(void)
{
    this->View = 0;
    this->ZoomFactor = 0;

    this->NumberOfClicks = 0;
    this->ResetPixelDistance = 1;

    this->VRWindowSatPos[0] = 0;
    this->VRWindowSatPos[1] = 0;

    this->VRWindowCurPos[0] = 0;
    this->VRWindowCurPos[1] = 0;

    this->VRWindowInitial = 0;

    this->VersionStandard = false;
    this->LeftButtonInteractionStyle = PICK_INTERACTION;
}

VtkVRInteractorStyle::~VtkVRInteractorStyle()
{
}

void VtkVRInteractorStyle::OnMouseMove()
{
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    switch (this->State)
    {
    case  VTKIS_ROTATE:
        this->FindPokedRenderer(x, y);
        this->Rotate();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;
    case VTKIS_PAN:
        this->FindPokedRenderer(x, y);
        this->Pan();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;

    case  VTKIS_PICK:
        this->FindPokedRenderer(x, y);
        this->Pick();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;

    case VTKIS_DOLLY:
        this->FindPokedRenderer(x, y);
        this->Dolly();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
        break;

    case VTKIS_VRWINDOW:
        this->FindPokedRenderer(x, y);
        this->VRWindow();
        this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
    }
}

//---------------------------------------------------------------------------
void VtkVRInteractorStyle::OnLeftButtonDown()
{
    //this->View->LButtonDownMessage();

    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    this->FindPokedRenderer(x, y);

    if (this->CurrentRenderer == NULL)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);

    if (VersionStandard == true)
    {
        switch (LeftButtonInteractionStyle)
        {
        case PICK_INTERACTION:
            this->StartPick();
            this->FindPokedRenderer(x, y);
            this->Pick();
            this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
            break;
        case ROTATE_INTERACTION:
            this->StartRotate();
            break;
        }
    }
    else
    {
        switch (LeftButtonInteractionStyle)
        {
        case PICK_INTERACTION:
            this->StartPick();
            this->FindPokedRenderer(x, y);
            this->Pick();
            this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
            break;
        case ROTATE_INTERACTION:
            this->StartRotate();
            break;

        case ZOOM_INTERACTION:
            this->StartDolly();
            break;
        case PAN_INTERACTION:
            this->StartPan();
            break;
        case WL_INTERACTION:
            this->VRWindowSatPos[0] = x;
            this->VRWindowSatPos[1] = y;
            this->StartVRWindow();
            break;
        }
    }
}

void VtkVRInteractorStyle::OnLeftButtonUp()
{
    if (VersionStandard == true)
    {
        switch (LeftButtonInteractionStyle)
        {
        case ROTATE_INTERACTION:
            this->EndRotate();
            break;
        case PICK_INTERACTION:
            this->EndPick();
            break;
        }
    }
    else
    {
        switch (LeftButtonInteractionStyle)
        {
        case ROTATE_INTERACTION:
            this->EndRotate();
            break;
        case PICK_INTERACTION:
            this->EndPick();
            break;

        case ZOOM_INTERACTION:
            this->EndDolly();
            break;
        case PAN_INTERACTION:
            this->EndPan();
            break;
        case WL_INTERACTION:
            this->EndVRWindow();
            break;
        }
    }

    if (this->Interactor)
    {
        this->ReleaseFocus();
    }
}

void VtkVRInteractorStyle::OnRightButtonDown()
{
    //this->View->LButtonDownMessage();
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    this->FindPokedRenderer(x, y);
    if (this->CurrentRenderer == NULL)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    //判断是否双击
    /*
    if (m_ptempTime == NULL)
    {

    m_ptempTime = new SYSTEMTIME;
    ::GetLocalTime(m_ptempTime);

    NumberOfClicks = 1;
    }
    else
    {
    SYSTEMTIME t;
    ::GetLocalTime(&t);

    __int64 TimeDiff = (t.wHour*3600000 + t.wMinute*60000 + t.wSecond*1000 + t.wMilliseconds)
    - (m_ptempTime->wHour*3600000  + m_ptempTime->wMinute *60000 + m_ptempTime->wSecond*1000 + m_ptempTime->wMilliseconds);

    if (TimeDiff<600)
    {
    this->NumberOfClicks = 2;
    }

    *m_ptempTime = t;
    }

    int xdist = x - this->PreviousPosition[0];
    int ydist = y - this->PreviousPosition[1];

    this->PreviousPosition[0] = x;
    this->PreviousPosition[1] = y;

    int moveDistance = (int)sqrt((double)(xdist*xdist+ydist*ydist));

    if (moveDistance<this->ResetPixelDistance&&NumberOfClicks == 2)
    {
    this->View->FullWndMessage();
    this->NumberOfClicks = 0;
    return;
    }
    */

    if (VersionStandard == true)
    {
        switch (RightButtonInteractionStyle)
        {
        case ZOOM_INTERACTION:
            this->StartDolly();
            break;
        case PAN_INTERACTION:
            this->StartPan();
            break;
        }
    }
}

void VtkVRInteractorStyle::OnRightButtonUp()
{
    if (VersionStandard == true)
    {
        switch (RightButtonInteractionStyle)
        {
        case ZOOM_INTERACTION:
            this->EndDolly();
            break;
        case PAN_INTERACTION:
            this->EndPan();
            break;
        }
    }

    if (this->Interactor)
    {
        this->ReleaseFocus();
    }
}

void VtkVRInteractorStyle::OnMiddleButtonDown()
{
    //this->View->LButtonDownMessage();
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    this->FindPokedRenderer(x, y);
    if (this->CurrentRenderer == NULL)
    {
        return;
    }

    // Redefine this button to handle window/level
    this->GrabFocus(this->EventCallbackCommand);

    if (VersionStandard == true)
    {
        this->VRWindowSatPos[0] = x;
        this->VRWindowSatPos[1] = y;
        this->StartVRWindow();
    }
}

void VtkVRInteractorStyle::OnMiddleButtonUp()
{
    if (VersionStandard == true)
    {
        switch (this->State)
        {
        case VTKIS_VRWINDOW:
            this->EndVRWindow();
            break;
        }
    }
    if (this->Interactor)
    {
        this->ReleaseFocus();
    }
}

void VtkVRInteractorStyle::OnMouseWheelForward()
{
    this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
        this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == NULL)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->ReleaseFocus();
}

//----------------------------------------------------------------------------
void VtkVRInteractorStyle::OnMouseWheelBackward()
{
    this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
        this->Interactor->GetEventPosition()[1]);
    if (this->CurrentRenderer == NULL)
    {
        return;
    }

    this->GrabFocus(this->EventCallbackCommand);
    this->ReleaseFocus();
}

void VtkVRInteractorStyle::Pick()
{
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];

    double pickPos[3];

    vtkSmartPointer<vtkRenderer> pCurrentRenderer = vtkSmartPointer<vtkRenderer>::New();
    pCurrentRenderer = this->Interactor->FindPokedRenderer(x, y);

    vtkSmartPointer<vtkCellPicker> pPicker = vtkSmartPointer<vtkCellPicker>::New();
    pPicker->Pick(x, y, 0.0, pCurrentRenderer);
    vtkProp *pProp = pPicker->GetViewProp();

    /*if (pProp != NULL)
    {
        pPicker->GetPickPosition(pickPos);
        double xp = pickPos[0];
        double yp = pickPos[1];
        double zp = pickPos[2];

        View->SetWorldPick(xp, yp, zp, 0);

    }
    else
    {
        if (View->m_pActorSphere != NULL)
        {
            View->m_p3DRenderer->RemoveActor(View->m_pActorSphere);
        }
    }*/

    this->InvokeEvent(vtkCommand::PickEvent, this);
}

void VtkVRInteractorStyle::Rotate()
{
    if (this->CurrentRenderer == NULL)
    {
        return;
    }

    vtkRenderWindowInteractor *rwi = this->Interactor;

    int dx = rwi->GetEventPosition()[0] - rwi->GetLastEventPosition()[0];
    int dy = rwi->GetEventPosition()[1] - rwi->GetLastEventPosition()[1];

    int *size = this->CurrentRenderer->GetRenderWindow()->GetSize();

    double delta_elevation = -10.0 / size[1];
    double delta_azimuth = -10.0 / size[0];

    double rxf = dx * delta_azimuth * this->MotionFactor;
    double ryf = dy * delta_elevation * this->MotionFactor;

    vtkCamera *camera = this->CurrentRenderer->GetActiveCamera();
    camera->Azimuth(rxf);
    camera->Elevation(ryf);
    camera->OrthogonalizeViewUp();

    if (this->AutoAdjustCameraClippingRange)
    {
        this->CurrentRenderer->ResetCameraClippingRange();
    }

    if (rwi->GetLightFollowCamera())
    {
        this->CurrentRenderer->UpdateLightsGeometryToFollowCamera();
    }

    rwi->Render();
}

void VtkVRInteractorStyle::StartVRWindow()
{
    if (this->State != VTKIS_NONE)
    {
        return;
    }
    this->StartState(VTKIS_VRWINDOW);

    if (this->View->m_pPieceFun)
    {
        vtkSmartPointer<vtkPiecewiseFunction> pPicecFun = this->View->m_pPieceFun;
        pPicecFun->GetNodeValue(1, PicecInfo);
        this->VRWindowInitial = PicecInfo[0];
    }
}

void VtkVRInteractorStyle::EndVRWindow()
{
    if (this->State != VTKIS_VRWINDOW)
    {
        return;
    }
    this->StopState();
}

void VtkVRInteractorStyle::VRWindow()
{
    double MaxInfo[4];

    vtkRenderWindowInteractor *rwi = this->Interactor;

    this->VRWindowCurPos[0] = rwi->GetEventPosition()[0];
    this->VRWindowCurPos[1] = rwi->GetEventPosition()[1];

    if (this->View->m_pPieceFun)
    {
        this->View->m_pPieceFun->GetNodeValue(2, MaxInfo);

        int *size = this->CurrentRenderer->GetSize();

        int dy = (VRWindowCurPos[1] - VRWindowSatPos[1]) * 400 / size[1];

        double VrWindow = VRWindowInitial + dy;

        if (VRWindowInitial < -3000)
        {
            VrWindow = VrWindow - dy;
        }

        if (VRWindowInitial >= MaxInfo[0])
        {
            VrWindow = VrWindow - dy;
        }

        PicecInfo[0] = VrWindow;

        this->View->m_pPieceFun->SetNodeValue(1, PicecInfo);

        this->VRWindowSatPos[1] = this->VRWindowCurPos[1];

        VRWindowInitial = VrWindow;

        this->Interactor->Render();
    }

}
