#include "stdafx.h"
#include "Hmy3DMath.h"

Hmy3DVector::Hmy3DVector(double x /*= 0.0*/, double y /*= 0.0*/, double z /*= 0.0*/)
{
	_x = x;
	_y = y;
	_z = z;
}

Hmy3DVector::Hmy3DVector(int x, int y, int z)
{
	_x = x * 1.00;
	_y = y * 1.00;
	_z = z * 1.00;
}

Hmy3DVector::~Hmy3DVector()
{
}

void Hmy3DVector::SetValue(double x, double y, double z)
{
	_x = x; _y = y; _z = z;
}

Hmy3DVector Hmy3DVector::Add(Hmy3DVector v)
{
	return(Hmy3DVector(_x + v.x(), _y + v.y(), _z + v.z()));
}

Hmy3DVector Hmy3DVector::Sub(Hmy3DVector v)
{
	return(Hmy3DVector(_x - v.x(), _y - v.y(), _z - v.z()));
}

Hmy3DVector Hmy3DVector::Multiply(Hmy3DVector v)
{
	return(Hmy3DVector(_x * v.x(), _y * v.y(), _z * v.z()));
}

Hmy3DVector Hmy3DVector::Scale(double r)
{
	return(Hmy3DVector(_x*r, _y*r, _z*r));
}

double Hmy3DVector::Dot(Hmy3DVector v)
{
	return _x*v.x() + _y*v.y() + _z*v.z();
}

Hmy3DVector Hmy3DVector::Cross(Hmy3DVector v)
{
	Hmy3DVector result;
	result.SetX(_y*v.z() - _z*v.y());
	result.SetY(_z*v.x() - _x*v.z());
	result.SetZ(_x*v.y() - _y*v.x());
	return result;
}

double Hmy3DVector::GetLengthSq()
{
	return _x * _x + _y * _y + _z * _z;
}

double Hmy3DVector::GetLength()
{
	return sqrt(GetLengthSq());
}

Hmy3DVector& Hmy3DVector::Normalize()
{
	double det = sqrt(_x*_x + _y*_y + _z*_z);
	if (fabs(det) < 0.000001)
		det = 1.0;
	_x /= det;
	_y /= det;
	_z /= det;
	return *this;
}

double Hmy3DVector::AngleTo(Hmy3DVector v)
{
	double theta = Dot(v) / (GetLength()*v.GetLength());
	double arc = (theta < -1) ? -1 : ((theta > 1) ? 1 : theta);
	return acos(arc);
}

double Hmy3DVector::DistanceTo(Hmy3DVector v)
{
	double dx = _x - v.x();
	double dy = _y - v.y();
	double dz = _z - v.z();

	double dis = dx * dx + dy * dy + dz * dz; 
	return sqrt(dis);
}

bool HmyLine3D::IntersectLine(HmyLine3D line, Hmy3DVector &Intersect)
{
	Hmy3DVector da = _end.Sub(_start);
	Hmy3DVector db = line.end().Sub(line.start());
	Hmy3DVector dc = line.start().Sub(_start);

	Hmy3DVector daCrossDb = da.Cross(db);
	Hmy3DVector dcCrossDb = dc.Cross(db);
	Hmy3DVector dcCrossDa = dc.Cross(da);

	int nCoolanar = floor(abs(dc.Dot(daCrossDb)));
	if (nCoolanar != 0)
		return false;

	double s = daCrossDb.Dot(daCrossDb) / daCrossDb.GetLengthSq();
	double t = dcCrossDa.Dot(daCrossDb) / daCrossDb.GetLengthSq();

	if (!((s >= 0 && s <= 1) || (t >= 0 && t <= 1)))
		return false;

	Hmy3DVector intersection = _start.Add(da.Scale(s));
	double distanceTest = (intersection.Sub(line.start())).GetLengthSq() + (intersection.Sub(line.end())).GetLengthSq();
	Hmy3DVector distance = line.end().Sub(line.start());
	double dLineDistance = distance.x()*distance.x() + distance.y()*distance.y() + distance.z()*distance.z();
	if (distanceTest <= dLineDistance) {
		Intersect = intersection;
		return true;
	}
	return false;
}

void HmyPlane3D::SetFromNormalAndCoplanarPoint(Hmy3DVector normal, Hmy3DVector point)
{
	_normal = normal;
	_constant = -point.Dot(_normal);
}

int HmyPlane3D::IntersectPlane(HmyPlane3D plane, Hmy3DVector &origin, Hmy3DVector &direction)
{
	direction = _normal.Cross(plane.normal());

	if (direction.GetLength() < 1e-10)
	{
		if (abs(_constant - plane.constant()) < 1e-5)
			return Plane_Same;
		return Plane_Parallel;
	}

	double h1 = _constant;
	double h2 = plane.constant();
	double n1dotn2 = _normal.Dot(plane.normal());

	double c1 = -(h1 - h2 * n1dotn2) / (1 - n1dotn2 * n1dotn2);
	double c2 = -(h2 - h1 * n1dotn2) / (1 - n1dotn2 * n1dotn2);

	origin = _normal.Scale(c1).Add(plane.normal().Scale(c2));
	return Plane_Cross;	
}

void HmyImageData3D::CalBounds()
{
	if (_spacing[0] > _spacing[1] ||
		_spacing[2] > _spacing[3] ||
		_spacing[4] > _spacing[5])
	{
		_bounds[0] = 1.0;
		_bounds[1] = -1.0;
		_bounds[2] = 1.0;
		_bounds[3] = -1.0;
		_bounds[4] = 1.0;
		_bounds[5] = -1.0;
	}

	int swapXBounds = (_spacing[0] < 0);  // 1 if true, 0 if false
	int swapYBounds = (_spacing[1] < 0);  // 1 if true, 0 if false
	int swapZBounds = (_spacing[2] < 0);  // 1 if true, 0 if false

	_bounds[0] = _origin.x() + (_extent[0 + swapXBounds] * _spacing[0]);
	_bounds[2] = _origin.y() + (_extent[2 + swapYBounds] * _spacing[1]);
	_bounds[4] = _origin.z() + (_extent[4 + swapZBounds] * _spacing[2]);

	_bounds[1] = _origin.x() + (_extent[1 - swapXBounds] * _spacing[0]);
	_bounds[3] = _origin.y() + (_extent[3 - swapYBounds] * _spacing[1]);
	_bounds[5] = _origin.z() + (_extent[5 - swapZBounds] * _spacing[2]);
}

void HmyImageData3D::CalSoloBounds()
{
	if (_spacing[0] > _spacing[1] ||
		_spacing[2] > _spacing[3] ||
		_spacing[4] > _spacing[5])
	{
		_solobounds[0] = 1.0;
		_solobounds[1] = -1.0;
		_solobounds[2] = 1.0;
		_solobounds[3] = -1.0;
		_solobounds[4] = 1.0;
		_solobounds[5] = -1.0;
	}

	int swapXBounds = (_spacing[0] < 0);  // 1 if true, 0 if false
	int swapYBounds = (_spacing[1] < 0);  // 1 if true, 0 if false
	int swapZBounds = (_spacing[2] < 0);  // 1 if true, 0 if false

	_solobounds[0] = _extent[0 + swapXBounds] * _spacing[0];
	_solobounds[2] = _extent[2 + swapYBounds] * _spacing[1];
	_solobounds[4] = _extent[4 + swapZBounds] * _spacing[2];

	_solobounds[1] = _extent[1 - swapXBounds] * _spacing[0];
	_solobounds[3] = _extent[3 - swapYBounds] * _spacing[1];
	_solobounds[5] = _extent[5 - swapZBounds] * _spacing[2];
}

void HmyImageData3D::CoverImagePosToWorldPos(Hmy3DVector imagePos, Hmy3DVector &worldPos)
{
	Hmy3DVector x = _xCosin.Scale(imagePos.x()*_spacing[0]);
	Hmy3DVector y = _yCosin.Scale(imagePos.y()*_spacing[1]);
	Hmy3DVector z = _zCosin.Scale(imagePos.z()*_spacing[2]);
	worldPos = ((x.Add(y)).Add(z)).Add(_origin);

}

void HmyImageData3D::CoverWorldPosToImagePos(Hmy3DVector worldPos, Hmy3DVector &imagePos)
{
	Hmy3DVector point = worldPos.Sub(_origin);
	imagePos.SetX(_xCosin.Dot(point) / _spacing[0]);
	imagePos.SetY(_yCosin.Dot(point) / _spacing[1]);
	imagePos.SetZ(_zCosin.Dot(point) / _spacing[2]);
}

// Matrix Inversion (adapted from Richard Carling in "Graphics Gems,"
// Academic Press, 1990).
HmyMatrix4x4 HmyMatrix4x4::Invert()
{
	// inverse( original_matrix, inverse_matrix )
	// calculate the inverse of a 4x4 matrix
	//
	//     -1
	//     A  = ___1__ adjoint A
	//         det A
	//

	// calculate the 4x4 determinent
	// if the determinent is zero,
	// then the inverse matrix is not unique.

	double det = Determinant();
	if (det == 0.0)
	{
		HmyMatrix4x4 a;
		return a;
	}

	// calculate the adjoint matrix
	HmyMatrix4x4 outElements = Adjoint();

	// scale the adjoint matrix to get the inverse
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			outElements.SetElement(i,j,outElements.GetElement(i,j)/det);
		}		
	}

	return outElements;
}

//----------------------------------------------------------------------------
double HmyMatrix4x4::Determinant()
{
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	// assign to individual variable names to aid selecting
	//  correct elements

	a1 = Element[0][0];  b1 = Element[0][1];  c1 = Element[0][2];  d1 = Element[0][3];
	a2 = Element[1][0];  b2 = Element[1][1];  c2 = Element[1][2];  d2 = Element[1][3];
	a3 = Element[2][0];  b3 = Element[2][1];  c3 = Element[2][2];  d3 = Element[2][3];
	a4 = Element[3][0];  b4 = Element[3][1];  c4 = Element[3][2];  d4 = Element[3][3];

	return a1 * Determinant3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4)
		- b1 * Determinant3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4)
		+ c1 * Determinant3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4)
		- d1 * Determinant3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4);
}

//----------------------------------------------------------------------------
HmyMatrix4x4 HmyMatrix4x4::Adjoint()
{
	//
	//   adjoint( original_matrix, inverse_matrix )
	//
	//     calculate the adjoint of a 4x4 matrix
	//
	//      Let  a   denote the minor determinant of matrix A obtained by
	//           ij
	//
	//      deleting the ith row and jth column from A.
	//
	//                    i+j
	//     Let  b   = (-1)    a
	//          ij            ji
	//
	//    The matrix B = (b  ) is the adjoint of A
	//                     ij
	//
	double a1, a2, a3, a4, b1, b2, b3, b4, c1, c2, c3, c4, d1, d2, d3, d4;

	// assign to individual variable names to aid
	// selecting correct values

	a1 = Element[0][0];  b1 = Element[0][1];  c1 = Element[0][2];  d1 = Element[0][3];
	a2 = Element[1][0];  b2 = Element[1][1];  c2 = Element[1][2];  d2 = Element[1][3];
	a3 = Element[2][0];  b3 = Element[2][1];  c3 = Element[2][2];  d3 = Element[2][3];
	a4 = Element[3][0];  b4 = Element[3][1];  c4 = Element[3][2];  d4 = Element[3][3];

	// row column labeling reversed since we transpose rows & columns
	HmyMatrix4x4 outElem;
	outElem.SetElement(0, 0, Determinant3x3(b2, b3, b4, c2, c3, c4, d2, d3, d4));
	outElem.SetElement(1, 0, Determinant3x3(a2, a3, a4, c2, c3, c4, d2, d3, d4));
	outElem.SetElement(2, 0, Determinant3x3(a2, a3, a4, b2, b3, b4, d2, d3, d4));
	outElem.SetElement(3, 0, Determinant3x3(a2, a3, a4, b2, b3, b4, c2, c3, c4));

	outElem.SetElement(0, 1, Determinant3x3(b1, b3, b4, c1, c3, c4, d1, d3, d4));
	outElem.SetElement(1, 1, Determinant3x3(a1, a3, a4, c1, c3, c4, d1, d3, d4));
	outElem.SetElement(2, 1, Determinant3x3(a1, a3, a4, b1, b3, b4, d1, d3, d4));
	outElem.SetElement(3, 1, Determinant3x3(a1, a3, a4, b1, b3, b4, c1, c3, c4));

	outElem.SetElement(0, 2, Determinant3x3(b1, b2, b4, c1, c2, c4, d1, d2, d4));
	outElem.SetElement(1, 2, Determinant3x3(a1, a2, a4, c1, c2, c4, d1, d2, d4));
	outElem.SetElement(2, 2, Determinant3x3(a1, a2, a4, b1, b2, b4, d1, d2, d4));
	outElem.SetElement(3, 2, Determinant3x3(a1, a2, a4, b1, b2, b4, c1, c2, c4));

	outElem.SetElement(0, 3, Determinant3x3(b1, b2, b3, c1, c2, c3, d1, d2, d3));
	outElem.SetElement(1, 3, Determinant3x3(a1, a2, a3, c1, c2, c3, d1, d2, d3));
	outElem.SetElement(2, 3, Determinant3x3(a1, a2, a3, b1, b2, b3, d1, d2, d3));
	outElem.SetElement(3, 3, Determinant3x3(a1, a2, a3, b1, b2, b3, c1, c2, c3));

	return outElem;
}

double HmyMatrix4x4::Determinant3x3(double a1, double a2, double a3, double b1, double b2, double b3, double c1, double c2, double c3)
{
	return (a1 * Determinant2x2(b2, b3, c2, c3)
		- b1 * Determinant2x2(a2, a3, c2, c3)
		+ c1 * Determinant2x2(a2, a3, b2, b3));
}

double HmyMatrix4x4::Determinant2x2(double a, double b, double c, double d)
{
	return (a * d - b * c);
}

HmyMatrix4x4 HmyMatrix4x4::MultiplyMatrix(HmyMatrix4x4 m)
{
	HmyMatrix4x4 result;
	for (int r = 0; r < 4; r++)
	{
		for (int i = 0; i < 4; i++)
		{
			double a = 0;
			for (int j = 0; j < 4; j++)
			{
				a += Element[r][j] * m.GetElement(j, i);
			}
			result.SetElement(r, i, a);
		}
	}
	return result;
}
