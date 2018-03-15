#pragma once
class Hmy3DVector
{
private:
	double _x;
	double _y;
	double _z;

public:
	Hmy3DVector(double x = 0.0, double y = 0.0, double z = 0.0);
	Hmy3DVector(int x, int y, int z);
	~Hmy3DVector();

	double x() { return _x; }
	double y() { return _y; }
	double z() { return _z; }

	void SetX(double x) { _x = x; }
	void SetY(double y) { _y = y; }
	void SetZ(double z) { _z = z; }
	void SetValue(double x, double y, double z);

	Hmy3DVector& operator=(const Hmy3DVector &vector)
	{
		_x = vector._x;
		_y = vector._y;
		_z = vector._z;
		return *this;
	}

	Hmy3DVector Add(Hmy3DVector v);
	Hmy3DVector Sub(Hmy3DVector v);
	Hmy3DVector Multiply(Hmy3DVector v);
	Hmy3DVector Scale(double r);
	double Dot(Hmy3DVector v);
	Hmy3DVector Cross(Hmy3DVector v);
	double GetLengthSq();
	double GetLength();
	Hmy3DVector& Normalize();
	double AngleTo(Hmy3DVector v);
	double DistanceTo(Hmy3DVector v);
};

class HmyLine3D
{
private:
	Hmy3DVector _start;
	Hmy3DVector _end;
public:
	HmyLine3D(Hmy3DVector start = NULL, Hmy3DVector end = NULL)
	{
		_start = start;
		_end = end;
	}
	Hmy3DVector start() const { return _start; }
	Hmy3DVector end() const { return _end; }

	void Set(Hmy3DVector start, Hmy3DVector end)
	{
		_start = start;
		_end = end;
	}

	void SetValue(int nIndex, Hmy3DVector value)
	{
		if (nIndex == 0)
			_start = value;
		else
			_end = value;
	}

	Hmy3DVector GetLinesVector()
	{
		return (_end.Sub(_start)).Normalize();
	}

	bool IntersectLine(HmyLine3D line, Hmy3DVector &Intersect);

};

class HmyPlane3D
{
private:
	Hmy3DVector _normal;
	Hmy3DVector _origin;
	double _constant;

public:
	HmyPlane3D(Hmy3DVector normal = NULL,double origin = NULL,double constant = 0.00)
	{
		_normal = normal;
		_constant = constant;
	}

	Hmy3DVector normal() const { return _normal; }
	void GetNormal(double normal[3]) { normal[0] = _normal.x(); normal[1] = _normal.y(); normal[2] = _normal.z(); }
	double constant() const { return _constant; }

	void SetNormal(Hmy3DVector normal) { _normal = normal; }
	void SetNormal(double normal[3]) { _normal.SetValue(normal[0], normal[1], normal[2]); }
	void SetConstant(double constant) { _constant = constant; }

	void SetFromNormalAndCoplanarPoint(Hmy3DVector normal, Hmy3DVector point);
	int IntersectPlane(HmyPlane3D plane, Hmy3DVector &origin, Hmy3DVector &direction);

};

class HmyImageData3D
{
private:
	Hmy3DVector _origin;
	Hmy3DVector _xCosin;
	Hmy3DVector _yCosin;
	Hmy3DVector _zCosin;

	double _spacing[3];
	int _extent[6];
	double _bounds[6];
	double _solobounds[6];

	void CalBounds();
	void CalSoloBounds();
public:
	HmyImageData3D()
	{
		_origin = NULL;
		_xCosin = NULL;
		_yCosin = NULL;
		_zCosin = NULL;
			
	}

	HmyImageData3D(Hmy3DVector origin, Hmy3DVector xCosin, Hmy3DVector yCosin, Hmy3DVector zCosin)
	{
		_origin = origin;
		_xCosin = xCosin;
		_yCosin = yCosin;
		_zCosin = zCosin;
	}

	Hmy3DVector Origin() const { return _origin; }
	Hmy3DVector XCosin() const { return _xCosin; }
	Hmy3DVector YCosin() const { return _yCosin; }
	Hmy3DVector ZCosin() const { return _zCosin; }

	
	double XSpacing() const { return _spacing[0]; }
	double YSpacing() const { return _spacing[1]; }
	double ZSpacing() const { return _spacing[2]; }

	void GetExtent(int extent[6]) 
	{
		extent[0] = _extent[0]; extent[1] = _extent[1];
		extent[2] = _extent[2]; extent[3] = _extent[3];
		extent[4] = _extent[4]; extent[5] = _extent[5];
	}

	void GetBounds(double bounds[6])
	{
		CalBounds();
		bounds[0] = _bounds[0]; bounds[1] = _bounds[1];
		bounds[2] = _bounds[2]; bounds[3] = _bounds[3];
		bounds[4] = _bounds[4]; bounds[5] = _bounds[5];
	}

	void GetSoloBounds(double bounds[6])
	{
		CalSoloBounds();
		bounds[0] = _solobounds[0]; bounds[1] = _solobounds[1];
		bounds[2] = _solobounds[2]; bounds[3] = _solobounds[3];
		bounds[4] = _solobounds[4]; bounds[5] = _solobounds[5];
	}

	void GetCenter(double center[3])
	{
		CalBounds();
		for (int i = 0; i < 3; i++)
		{
			center[i] = (_bounds[2 * i + 1] + _bounds[2 * i]) / 2.0;
		}
	}

	void GetSoloCenter(double center[3])
	{
		CalSoloBounds();
		for (int i = 0; i < 3; i++)
		{
			center[i] = (_solobounds[2 * i + 1] + _solobounds[2 * i]) / 2.0;
		}
	}

	void SetOrigin(Hmy3DVector origin) { _origin = origin; }
	void SetCosin(Hmy3DVector xCosin, Hmy3DVector yCosin, Hmy3DVector zCosin)
	{
		_xCosin = xCosin; _yCosin = yCosin; _zCosin = zCosin;
	}
	void SetXCosin(Hmy3DVector xCosin) { _xCosin = xCosin; }
	void SetYCosin(Hmy3DVector yCosin) { _yCosin = yCosin; }
	void SetZCosin(Hmy3DVector zCosin) { _zCosin = zCosin; }
	void SetSpacing(double xSpacing, double ySpacing, double zSpacing)
	{
		_spacing[0] = xSpacing; _spacing[1] = ySpacing; _spacing[2] = zSpacing;
	}
	void SetXSpacing(double xSpacing) { _spacing[0] = xSpacing; }
	void SetYSpacing(double ySpacing) { _spacing[1] = ySpacing; }
	void SetZSpacing(double zSpacing) { _spacing[2] = zSpacing; }
	void SetExtent(int nDataX, int nDataY, int nDataZ)
	{
		_extent[0] = 0; _extent[1] = nDataX - 1;
		_extent[2] = 0; _extent[3] = nDataY - 1;
		_extent[4] = 0; _extent[5] = nDataZ - 1;
	}

	void CoverImagePosToWorldPos(Hmy3DVector imagePos, Hmy3DVector &worldPos);
	void CoverWorldPosToImagePos(Hmy3DVector worldPos, Hmy3DVector &imagePos);

};

class HmyMatrix4x4
{
private:
	double Element[4][4];

public:
	HmyMatrix4x4()
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j= 0; j<4; j++)
			{
				Element[i][j] = 0.0;
			}			
		}
	};

	HmyMatrix4x4& operator=(const HmyMatrix4x4 &matrix)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Element[i][j] = matrix.GetElement(i, j);
			}
		}
		return *this;
	}

	double GetElement(int i, int j) const
	{
		return this->Element[i][j];
	}

	void SetElement(int i, int j, double value)
	{
		if (this->Element[i][j] != value)
		{
			this->Element[i][j] = value;
		}
	}

	void DeepCopy(double elem[16])
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				Element[i][j] = elem[i*4+j];
			}
		}
	}

	HmyMatrix4x4 Invert();
	double Determinant();
	HmyMatrix4x4 Adjoint();
	double Determinant3x3(double a1, double a2, double a3,
		double b1, double b2, double b3,
		double c1, double c2, double c3);
	double Determinant2x2(double a, double b, double c, double d);
	HmyMatrix4x4 MultiplyMatrix(HmyMatrix4x4 m);
};

