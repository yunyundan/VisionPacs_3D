#include "StdAfx.h"
#include "Vector3d.h"
//#include "vector2d.h"

#define TOLERANCE		0.0000001
namespace v3D{

	Vector3D exProd(const Vector3D &lhs, const Vector3D &rhs)
	{
		double x = lhs.y() * rhs.z() - rhs.y() * lhs.z();
		double y = rhs.x() * lhs.z() - lhs.x() * rhs.z();
		double z = lhs.x() * rhs.y() - rhs.x() * lhs.y();
		return Vector3D(x, y, z);
	}

	double inProd(const Vector3D &lhs, const Vector3D &rhs)
	{
		return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
	}

	//************************************
	// Method:    NormalDirect
	// FullName:  v3D::NormalDirect
	// Access:    public 
	// Returns:   int 0 - x, 1 - y, 2 - z
	// Qualifier: 计算已知行向量和列向量的平面法向量的近似平行坐标轴
	// Parameter: const Vector3D & lhs
	// Parameter: const Vector3D & rhs
	//************************************
	int VectorDirect(const Vector3D &nn/*, const Vector3D &rhs*/)
	{
		//Vector3D nn = exProd(lhs, rhs);
		int ret = 0;
		double dMax = nn.x();
		if(fabs(dMax) -  fabs(nn.y())< TOLERANCE)
		{
			dMax = nn.y();
			ret = 1;
		}

		if(fabs(dMax) - fabs(nn.z()) < TOLERANCE)
			ret = 2;
		
		return ret;
	}


	//************************************
	// Method:    Get3DPoint
	// FullName:  v3D::Get3DPoint
	// Access:    public 
	// Returns:   v3D::Vector3D
	// Qualifier: 已知一点(M1)和行列向量的平面，求和行向量成夹角的单位向量
	// Parameter: const Vector3D & M1
	// Parameter: const Vector3D & rowvector
	// Parameter: const Vector3D & colvector
	// Parameter: double cosangle
	// Parameter: double sinangle
	//************************************
	bool  NormalVector(const Vector3D & M1, const Vector3D & rowvector, const Vector3D &colvector
						, double cosangle, double sinangle, Vector3D &nor)
	{
		Vector3D rep = Vector3D(cosangle, sinangle, 0.0);
		Vector3D n = exProd(rowvector, colvector);
		//系数行列式
		double delta = Determinant(rowvector, colvector, n);
		if (fabs(delta) < TOLERANCE)
			return false;

		double x = Determinant(Vector3D(cosangle, rowvector.y(), rowvector.z())
							, Vector3D(sinangle, colvector.y(), colvector.z())
							, Vector3D(0.0, n.y(), n.z()));
		double y = Determinant(Vector3D(rowvector.x(), cosangle, rowvector.z())
							, Vector3D(colvector.x(), sinangle, colvector.z())
							, Vector3D(n.x(), 0.0, n.z()));
		double z = Determinant(Vector3D(rowvector.x(), rowvector.y(), cosangle)
							, Vector3D(colvector.x(), colvector.y(), sinangle)
							, Vector3D(n.x(), n.y(), 0.0));

		nor.SetValue(x/delta, y/delta, z/delta);
		return true;
	}

	//************************************
	// Method:    Determinant
	// FullName:  v3D::Determinant
	// Access:    public 
	// Returns:   double
	// Qualifier: 计算3阶行列式
	// Parameter: const Vector3D & v1
	// Parameter: const Vector3D & v2
	// Parameter: const Vector3D & v3
	//************************************
	double Determinant(const Vector3D & v1, const Vector3D & v2, const Vector3D &v3)
	{
		return v1.x()*v2.y()*v3.z()+v2.x()*v3.y()*v1.z()
			+ v3.x()*v1.y()*v2.z() - v3.x()*v2.y()*v1.z()
			- v2.x()*v1.y()*v3.z()-v1.x()*v3.y()*v2.z();
	}

	//************************************
	// Method:    Get3DPoint
	// FullName:  v3D::Get3DPoint
	// Access:    public 
	// Returns:   v3D::Vector3D
	// Qualifier: 已知直线一点和方向向量，求距离该点长度为len的一点(已知点,M1, 作为起点)
	// Parameter: const Vector3D & M1
	// Parameter: const Vector3D & normalv
	// Parameter: double len
	//************************************
	Vector3D Get3DPoint(const Vector3D & M1, const Vector3D & normalv, double len)
	{
		return Vector3D(len*normalv.x() + M1.x(), len*normalv.y()+M1.y()
						,len*normalv.z() + M1.z());
	}

	//************************************
	// Method:    PointInPlane
	// FullName:  v3D::PointInPlane
	// Access:    public 
	// Returns:   bool
	// Qualifier: 判断已知点是否在已知平面内,M1为平面内的点
	// Parameter: const Vector3D & pt
	// Parameter: const Vector3D & M1
	// Parameter: const Vector3D & rowvector
	// Parameter: const Vector3D & colvector
	//************************************
	bool PointInPlane(const Vector3D &pt, const Vector3D &M1,const Vector3D &n
					, double surface_thick)
	{
		//Vector3D n = exProd(rowvector, colvector);
		double d = inProd(pt - M1, n);
		if(fabs(inProd(pt - M1, n)) < surface_thick)
			return true;

		return false;
	}
	//************************************
	// Method:    Get2DPointInPlane
	// FullName:  v3D::Get2DPointInPlane
	// Access:    public 
	// Returns:   POINT
	// Qualifier: M1为平面的左上点
	// Parameter: const Vector3D & M1
	// Parameter: const Vector3D & M2
	// Parameter: const Vector3D & rowvector
	//************************************
	Vector3D Get2DPointInPlane(const Vector3D &M1, const Vector3D &M2, const Vector3D & rowvector
							, const Vector3D & planNOr)
	{
		Vector3D lenVec = M2 - M1;
		double len2 = lenVec.x()*lenVec.x() + lenVec.y()*lenVec.y()+ lenVec.z()*lenVec.z();
		double pttoplane = fabs(inProd(M2, planNOr) - inProd(M1, planNOr));
		
		double t = sqrt(len2);
		if (t < TOLERANCE)
			t = 1.0;
		double cosangle = (rowvector.x()*lenVec.x() + rowvector.y() * lenVec.y() 
					+ rowvector.z()*lenVec.z())/t;

		double x = t*cosangle;

//		double y;
// 		if(pttoplane < TOLERANCE)
// 			y = sqrt(t - x*x);
// 		else
		double y = sqrt(fabs(len2 - x*x - pttoplane*pttoplane));

		return Vector3D(x, y);
	}

	bool IsPlaneParallel(const Vector3D &nor1, const Vector3D &nor2)
	{
		Vector3D n = exProd(nor1, nor2);

		if(fabs(n.x()) < 0.2 && fabs(n.y()) < 0.2 && fabs(n.z()) < 0.2) //没有使用TOLERANCE因为设备提供的值精确度不够
			return true;

		return false;
	}
}
