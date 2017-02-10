#pragma once
#ifndef VtkVRInteratorStyle_h
#define VtkVRInteratorStyle_h

#include "vtkVersion.h"
#include "vtkInteractorStyleImage.h"
#include "VtkSelfDefineInteractorStyleExport.h"

class vtkRenderWindowInteractor;
class vtkRenderer;
class vtkPiecewiseFunction;
class Vtk3DWnd;

#define  VTKIS_VRWINDOW     2024

class VTKSELFDEFINEINTERACTORSTYLE_EXPORT VtkVRInteractorStyle : public vtkInteractorStyleImage
{
public:
    static VtkVRInteractorStyle *New();
    vtkTypeMacro(VtkVRInteractorStyle, vtkInteractorStyleImage);

    virtual void OnMouseMove();
    virtual void OnLeftButtonDown();
    virtual void OnLeftButtonUp();
    virtual void OnMiddleButtonDown();
    virtual void OnMiddleButtonUp();
    virtual void OnRightButtonDown();
    virtual void OnRightButtonUp();
    virtual void OnMouseWheelForward();
    virtual void OnMouseWheelBackward();

    virtual void Pick();
    virtual void Rotate();

    void VRWindow();
    void StartVRWindow();
    void EndVRWindow();

    void SetView(Vtk3DWnd *view)
    {
            this->View = view;
    }

    vtkGetObjectMacro(View, Vtk3DWnd);

    vtkSetMacro(ZoomFactor, double);
    vtkGetMacro(ZoomFactor, double);

    vtkSetMacro(LeftButtonInteractionStyle, int);
    vtkSetMacro(RightButtonInteractionStyle, int);
    vtkSetMacro(VersionStandard, bool);

protected:
    VtkVRInteractorStyle();
    ~VtkVRInteractorStyle();

private:
    VtkVRInteractorStyle(const VtkVRInteractorStyle&) VTK_DELETE_FUNCTION;
    void operator=(const VtkVRInteractorStyle&)VTK_DELETE_FUNCTION;

    double  ZoomFactor;

    int NumberOfClicks;
    int ResetPixelDistance;
    int PreviousPosition[2];

    double VRWindowSatPos[2];
    double VRWindowCurPos[2];
    double VRWindowInitial;
    double PicecInfo[4];

    Vtk3DWnd *View;
    bool VersionStandard;
    int	 LeftButtonInteractionStyle;
    int	 RightButtonInteractionStyle;

public:
	enum InteractionStyleIds {
		PICK_INTERACTION,
		ROTATE_INTERACTION,
		ZOOM_INTERACTION,
		PAN_INTERACTION,
		WL_INTERACTION
	};
};

#endif // VTKVRINTERACTORSTYLE_H
