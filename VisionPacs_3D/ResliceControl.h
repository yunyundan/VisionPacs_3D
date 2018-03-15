#pragma once
#include "Hmy3dMath.h"

class HmyImageData3D;
class HmyPlane3D;
class Hmy3DVector;

class CResliceControl
{
public:
	CResliceControl();
	~CResliceControl();

private:
	HmyImageData3D *m_ImageData;

	int  m_extent[6];

	Hmy3DVector m_vecPreImageCenter;
	Hmy3DVector m_vecImageCenter;

	Hmy3DVector xAxis;
	Hmy3DVector yAxis;
	Hmy3DVector zAxis;	

	double m_dThickness[3];

	vector<HmyPlane3D > m_boundPlanes;
	vector<HmyPlane3D *> m_cutPlanes;

private:
	void CalBoundPlanes();
	void ComputeAxes();
	void BuildCursorGeometry();	
public:
	void SetImageData(HmyImageData3D *imageData);

	Hmy3DVector GetAxis(int i);

	void SetThickness(double thick[3]);

	HmyPlane3D *GetPlane(int i);

	void SetCenter(double center[3]);
	void SetCenter(Hmy3DVector center);

	Hmy3DVector GetCenter() { return m_vecImageCenter; }
	Hmy3DVector GetPreCenter() { return m_vecPreImageCenter; }
};

