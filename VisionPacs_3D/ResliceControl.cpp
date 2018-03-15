#include "stdafx.h"
#include "ResliceControl.h"
#include "Hmy3dMath.h"

CResliceControl::CResliceControl()
{
	xAxis = Hmy3DVector(1.0, 0.0, 0.0);
	yAxis = Hmy3DVector(0.0, 1.0, 0.0);
	zAxis = Hmy3DVector(0.0, 0.0, 1.0);

	m_vecImageCenter = Hmy3DVector(0.0, 0.0, 0.0);
	m_vecPreImageCenter = Hmy3DVector(0.0, 0.0, 0.0);
	m_dThickness[0] = m_dThickness[1] = m_dThickness[2] = 0.0;
	m_extent[0] = m_extent[1] = m_extent[2] = 0;

	for (int i = 0; i < 3; i++)
	{
		HmyPlane3D *cutPlane = new HmyPlane3D();
		m_cutPlanes.push_back(cutPlane);
	}

	m_cutPlanes[0]->SetNormal(Hmy3DVector(1, 0, 0));
	m_cutPlanes[1]->SetNormal(Hmy3DVector(0, -1, 0));
	m_cutPlanes[2]->SetNormal(Hmy3DVector(0, 0, 1));
}


CResliceControl::~CResliceControl()
{
	for (int i=0; i<m_cutPlanes.size(); i++)
	{
		delete m_cutPlanes[i];
	}
	m_cutPlanes.clear();

}

void CResliceControl::CalBoundPlanes()
{
	Hmy3DVector topLeft = m_ImageData->Origin();
	Hmy3DVector btX = m_ImageData->XCosin().Scale(m_extent[1] * m_ImageData->XSpacing());
	Hmy3DVector btY = m_ImageData->YCosin().Scale(m_extent[3] * m_ImageData->YSpacing());
	Hmy3DVector btZ = m_ImageData->ZCosin().Scale(m_extent[5] * m_ImageData->ZSpacing());
	Hmy3DVector bottomRight = topLeft.Add(btX.Add(btY).Add(btZ));

	HmyPlane3D planeTop;
	planeTop.SetFromNormalAndCoplanarPoint(m_ImageData->XCosin().Cross(m_ImageData->YCosin()), topLeft);
	m_boundPlanes.push_back(planeTop);

	HmyPlane3D planeBottom;
	planeBottom.SetFromNormalAndCoplanarPoint(m_ImageData->XCosin().Cross(m_ImageData->YCosin()), bottomRight);
	m_boundPlanes.push_back(planeBottom);

	HmyPlane3D planeLeft;
	planeLeft.SetFromNormalAndCoplanarPoint(m_ImageData->YCosin().Cross(m_ImageData->ZCosin()), topLeft);
	m_boundPlanes.push_back(planeLeft);

	HmyPlane3D planeRight;
	planeRight.SetFromNormalAndCoplanarPoint(m_ImageData->YCosin().Cross(m_ImageData->ZCosin()), bottomRight);
	m_boundPlanes.push_back(planeRight);

	HmyPlane3D planeBack;
	planeBack.SetFromNormalAndCoplanarPoint(m_ImageData->ZCosin().Cross(m_ImageData->XCosin()), topLeft);
	m_boundPlanes.push_back(planeBack);

	HmyPlane3D planeFront;
	planeFront.SetFromNormalAndCoplanarPoint(m_ImageData->ZCosin().Cross(m_ImageData->XCosin()), bottomRight);
	m_boundPlanes.push_back(planeFront);
}

void CResliceControl::ComputeAxes()
{
	vector<Hmy3DVector> normals;

	for (int i = 0; i < 3; i++)
	{
		Hmy3DVector normal = GetPlane(i)->normal();
		normals.push_back(normal);
	}

	// The axes are the intersections of the plane normals.
	zAxis = normals[0].Cross(normals[1]);
	xAxis = normals[1].Cross(normals[2]);
	yAxis = normals[2].Cross(normals[0]);
}

void CResliceControl::BuildCursorGeometry()
{
	ComputeAxes();

	double bounds[6];
	m_ImageData->GetSoloBounds(bounds);

}

void CResliceControl::SetImageData(HmyImageData3D *imageData)
{
	m_ImageData = imageData;
	m_ImageData->GetExtent(m_extent);

	double center[3];
	m_ImageData->GetSoloCenter(center);
	m_vecImageCenter.SetX(center[0]);
	m_vecImageCenter.SetY(center[1]);
	m_vecImageCenter.SetZ(center[2]);

}

Hmy3DVector CResliceControl::GetAxis(int i)
{
	if (i == 0)
	{
		return xAxis;
	}
	else if (i == 1)
	{
		return yAxis;
	}
	else
	{
		return zAxis;
	}
}

void CResliceControl::SetThickness(double thick[3])
{
	m_dThickness[0] = thick[0];
	m_dThickness[1] = thick[1];
	m_dThickness[1] = thick[1];
}

HmyPlane3D *CResliceControl::GetPlane(int i)
{
	return m_cutPlanes[i];
}

void CResliceControl::SetCenter(double center[3])
{
	if ((m_vecImageCenter.x() != center[0]) 
		|| (m_vecImageCenter.y() != center[1]) 
		|| (m_vecImageCenter.z() != center[2]))
	{

		// Ensure that the center of the cursor lies within the image bounds.
		if (center[0] < 0 || center[0] > m_extent[0] ||
			center[1] < 0 || center[1] > m_extent[1] ||
			center[2] < 0 || center[2] > m_extent[2])
		{
			return;
		}
		
		m_vecPreImageCenter = m_vecImageCenter;
		m_vecImageCenter.SetValue(center[0], center[1], center[2]);
	}
}

void CResliceControl::SetCenter(Hmy3DVector center)
{
	m_vecPreImageCenter = m_vecImageCenter;
	m_vecImageCenter = center;
}
