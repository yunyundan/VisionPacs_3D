#pragma once
//////////////////////////////////////////////////////////////////////////
//µ¥Î»:mm
namespace v3D{
	class Vector3D
	{
	private:
		double _x;
		double _y;
		double _z;

	public:
		Vector3D(double x = 0.0, double y = 0.0, double z = 0.0):_x(x),_y(y), _z(z){}
		Vector3D(int x, int y, int z):_x(double(x)), _y(double(y)), _z(double(z)){}
		Vector3D(POINT pt, int z):_x(double(pt.x)), _y(double(pt.y)), _z(double(z)){}

		Vector3D(const Vector3D &rhs){*this = rhs;}
		Vector3D & operator=(const Vector3D &rhs){_x = rhs._x; _y = rhs._y; _z = rhs._z; return *this;}
		double x() const {return _x;}
		double y() const {return _y;}
		double z() const {return _z;}

		void SetValue(double x, double y, double z)
		{
			_x = x; _y = y; _z = z;
		}

		Vector3D operator-(const Vector3D & rhs) const 
		{
			return Vector3D(_x - rhs.x(), _y - rhs.y(), _z - rhs.z());
		}

		Vector3D operator+(const Vector3D&rhs) const
		{
			return Vector3D(_x + rhs.x(), _y + rhs.y(), _z + rhs.z());
		}

		Vector3D Scale(double r)
		{
			return Vector3D(_x*r, _y*r, _z*r);
		}

		Vector3D& Normalize()
		{
			double det = sqrt(_x*_x + _y*_y + _z*_z);
			if( fabs(det) < 0.000001)
				det = 1.0;
			_x /= det;
			_y /= det;
			_z /= det;
			return *this;
		}

		Vector3D &ReversX()
		{
			_x *= -1;
			return *this;
		}

		Vector3D &ReversY()
		{
			_y *= -1;
			return *this;
		}

		Vector3D &ReversZ()
		{
			_z *= -1;
			return *this;
		}

		Vector3D & Revers()
		{
			ReversX();
			ReversY();
			return ReversZ();
		}
	};

	Vector3D exProd(const Vector3D &lhs, const Vector3D &rhs);
	double inProd(const Vector3D &lhs, const Vector3D &rhs);

	double Determinant(const Vector3D & v1, const Vector3D & v2, const Vector3D &v3);
	int VectorDirect(const Vector3D &l);
	bool NormalVector(const Vector3D & M1, const Vector3D & rowvector, const Vector3D &colvector
						, double cosangle, double sinangle, Vector3D &nor);
	Vector3D Get3DPoint(const Vector3D & M1, const Vector3D & normalv, double len);
	bool	 PointInPlane(const Vector3D &pt, const Vector3D &M1,const Vector3D &n, double);
	Vector3D Get2DPointInPlane(const Vector3D &M1, const Vector3D &M2, const Vector3D & rowvector
				, const Vector3D &);
	bool IsPlaneParallel(const Vector3D &nor1, const Vector3D &nor2);
}
