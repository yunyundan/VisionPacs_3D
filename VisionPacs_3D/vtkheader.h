#ifndef VTKHEADER_H
#define VTKHEADER_H

#ifndef INITIAL_OPENGL
#define INITIAL_OPENGL
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL)
VTK_MODULE_INIT(vtkRenderingContextOpenGL)
VTK_MODULE_INIT(vtkRenderingFreeType)
#endif

#include "QVTKWidget.h"
#include "vtkAlgorithm.h"
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkColorTransferFunction.h"
#include "vtkPiecewiseFunction.h"
#include "vtkVolumeProperty.h"
#include "vtkNew.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRendererCollection.h"
#include "vtkDICOMImageReader.h"
#include "vtkSmartVolumeMapper.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkAnnotatedCubeActor.h"
#include "vtkImageResample.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "Slicer\vtkSlicerGPURayCastVolumeMapper.h"
#include "vtkImageData.h"
#include "vtkCornerAnnotation.h"
#include "vtkTextProperty.h"


#endif // VTKHEADER_H


//GDCM Header
#include "gdcmSorter.h"
#include "gdcmIPPSorter.h"
#include "gdcmScanner.h"
#include "gdcmDataSet.h"
#include "gdcmAttribute.h"
#include "gdcmTesting.h"
#include "gdcmDecoder.h"

//CTK Header
#include "CTK/ctkTransferFunction.h"
#include "CTK/ctkVTKColorTransferFunction.h"
#include "CTK/ctkTransferFunctionView.h"
#include "CTK/ctkTransferFunctionGradientItem.h"
#include "CTK/ctkTransferFunctionControlPointsItem.h"
#include "CTK/ctkVTKVolumePropertyWidget.h"
