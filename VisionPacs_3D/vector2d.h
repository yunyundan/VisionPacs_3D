#ifndef _VECTOR2D_H
#define _VECTOR2D_H
#pragma once

#include <set>
#include <assert.h>
#include <limits>

#define USEDPIE			3.1415926
#define TOLERANCE		0.0000001

class Transform
{
public:
	//************************************
	// Method:    Reverse
	// FullName:  Transform<T>::Reverse
	// Access:    public static 
	// Returns:   void
	// Qualifier: 圆点对称
	// Parameter: T & t1
	// Parameter: T & t2
	//************************************
	template<class T>
	static void Reverse(T &t1, T &t2)
	{
		t1 *= -1 ;t2 *= -1;
	}

	//************************************
	// Method:    Clock90
	// FullName:  Transform<T>::Clock90
	// Access:    public static 
	// Returns:   void
	// Qualifier: 顺时针旋转
	// Parameter: T & t1
	// Parameter: T & t2
	//************************************
	template<class T>
	static void Clock90(T &t1, T &t2)
	{
		T tmp = t1; t1 = -t2; t2 = tmp;
	}

	//************************************
	// Method:    AntiClock90
	// FullName:  Transform<T>::AntiClock90
	// Access:    public static 
	// Returns:   void
	// Qualifier: 逆时针旋转90
	// Parameter: T & t1
	// Parameter: T & t2
	//************************************
	template<class T>
	static void AntiClock90(T &t1, T &t2)
	{
		T tmp = t1; t1 = t2; t2 = -tmp;
	}

	//************************************
	// Method:    Scale
	// FullName:  Transform<T>::Scale
	// Access:    public static 
	// Returns:   void
	// Qualifier: 按比例放大
	// Parameter: T n
	// Parameter: T & t1 [out] 
	// Parameter: T & t2 [out]
	//************************************
	template<class T>
	static void Scale(T n, T &t1, T &t2)
	{
		t1 *= n; t2 *= n;
	}

	//************************************
	// Method:    Flip
	// FullName:  Transform<T>::Flip
	// Access:    public static 
	// Returns:   void
	// Qualifier: 垂直镜像
	// Parameter: T & x
	// Parameter: T & y
	//************************************
	template<class T>
	static void Flip(T &x, T &y)
	{
		y *= -1;
	}

	//************************************
	// Method:    Mirror
	// FullName:  Transform<T>::Mirror
	// Access:    public static 
	// Returns:   void
	// Qualifier: 水平镜像
	// Parameter: T & x
	// Parameter: T & y
	//************************************
	template<class T>
	static void Mirror(T &x, T &y)
	{
		x *= -1;
	}

	//************************************
	// Method:    inProd
	// FullName:  Transform<T>::inProd
	// Access:    public static 
	// Returns:   T
	// Qualifier: 内积
	// Parameter: T x1
	// Parameter: T y1
	// Parameter: T x2
	// Parameter: T y2
	//************************************
	template<class T>
	static T inProd(T x1, T y1, T x2, T y2)
	{
		return (x1*x2+y1*y2);
	}
	//************************************
	// Method:    exProd
	// FullName:  Transform<T>::exProd
	// Access:    public static 
	// Returns:   T
	// Qualifier: x1,y1 to x2,y2 外积
	// Parameter: T x1
	// Parameter: T y1
	// Parameter: T x2
	// Parameter: T y2
	//************************************
	template<class T>
	static T exProd(T x1, T y1, T x2, T y2)
	{
		return (x1*y2 - x2*y1);
	}
	
	//************************************
	// Method:    calcTriangleArea
	// FullName:  Transform<T>::calcTriangleArea
	// Access:    public static 
	// Returns:   double
	// Qualifier: 3定点面积公式
	// Parameter: T & pt1
	// Parameter: T & pt2
	// Parameter: T & pt3
	//************************************
	template<class T>
	static double calcTriangleArea(T &pt1, T &pt2, T &pt3)
	{
		return 0.5*static_cast<double>(pt1.x*pt2.y+pt3.x*pt1.y + pt2.x*pt3.y - pt3.x*pt2.y - pt1.x*pt3.y-pt2.x*pt1.y);
	}

	//************************************
	// Method:    calcLength
	// FullName:  Transform<T>::calcLength
	// Access:    public static 
	// Returns:   double
	// Qualifier: 两点距离
	// Parameter: T & pt1
	// Parameter: T & pt2
	//************************************
	template<class T>
	static double calcLength(T &pt1, T &pt2)
	{
		return sqrt(static_cast<double>((pt1.x-pt2.x)*(pt1.x-pt2.x)+(pt1.y-pt2.y)*(pt1.y-pt2.y)));
	}

	//************************************
	// Method:    calcSlope
	// FullName:  Transform<T>::calcSlope
	// Access:    public static 
	// Returns:   bool	- true ,the v is valid;false, the v isn't valid and the angle is (2k+1)/2 *90
	// Qualifier:
	// Parameter: T & p1			[in] the first pt
	// Parameter: T & p2			[in] the second pt
	// Parameter: double & v		[out] the tan value
	//************************************
	template<class T>
	static bool calcSlope(T &p1, T &p2, double &v)
	{
		if(fabs(p1.x - p2.x) < TOLERANCE)
		{
			v = 0.0;
			return false;
		}

		v = 1.0*(p1.y - p2.y)/(p1.x - p2.x);
		return true;						//ok, the real value
	}

	//************************************
	// Method:    calcPt
	// FullName:  Transform<T>::calcPt
	// Access:    public static 
	// Returns:   void
	// Qualifier: 计算相对某个向量固定距离的点坐标(点为到向量终点一定距离的点), T may be AnnPoint or Point
	// Parameter: T & vector1			[in] the vector
	// Parameter: double angle			the pt's angel to vector
	// Parameter: double alen			the vector length
	// Parameter: double blen			the vec2's distance to the vector
	// Parameter: T & vec2				[out] the vector
	//************************************
	template<class T>
	static void calcVector(T &vector1, double angle, double blen, T &vec2)
	{
		AnnPoint p1;
		double alen = calcLength(vector1, p1);
		vec2.x = (vector1.y*blen*sin(angle) + vector1.x*blen*cos(angle))/alen;
		vec2.y = (-vector1.x*blen*sin(angle) + vector1.y*blen*cos(angle))/alen;
	}

	//************************************
	// Method:    calcPtToVector
	// FullName:  Transform<T>::calcPtToVector
	// Access:    public static 
	// Returns:   void
	// Qualifier: 功能同上
	// Parameter: T & vector1
	// Parameter: double anglesin		点和向量夹角的正弦
	// Parameter: double anglecos		。。。		  余弦
	// Parameter: double alen
	// Parameter: double blen
	// Parameter: T & pt
	//************************************
	template<class T>
	static void calcPtToVector(T &vector1, double anglesin, double anglecos
					, double alen, double blen, T &pt)
	{
		pt.x = (vector1.y*blen*anglesin + vector1.x*blen*anglecos)/alen;
		pt.y = (-vector1.x*blen*anglesin + vector1.y*blen*anglecos)/alen;
	}

	template<class T>
	static void calcOrgthVector(T &vec1, double len2, bool bright, T &vec2)
	{
		if(fabs(vec1.y) < TOLERANCE)
		{
			vec2.x = 0;
			vec2.y = len2;
			double z = exProd(vec1.x, vec1.y, vec2.x, vec2.y);
			if(bright ^ (z > TOLERANCE))	//当同为真或同为假的时候返回
				vec2.y *= -1.0;
		}
		else
		{
			double v = (sqrt(vec1.x*vec1.x + vec1.y*vec1.y));
			assert(fabs(v) > TOLERANCE);
			assert(fabs(vec1.y) > TOLERANCE);

			vec2.x  = vec1.y*len2 / v;
			vec2.y = -vec1.x*vec2.x/vec1.y;
			double z = exProd(vec1.x, vec1.y, vec2.x, vec2.y);
			if(bright ^ (z > TOLERANCE))
			{
				vec2.x *= -1.0;
				vec2.y *= -1.0;
			}
			
		}
	}

	template<class T>
	static bool IsRectCross(T &p1, T &p2, T &p11, T &p12)
	{
		T tp1.x = min(p1.x, p2.x);
		tp1.y = min(p1.y, p2.y);
		T tp2.x = max(p1.x, p2.x);
		tp2.y = max(p1.y, p2.y);

		T tp11.x = min(p11.x, p12.x);
		tp11.y = min(p11.y, p12.y);
		T tp12.x = max(p11.x, p12.x);
		tp12.y = max(p11.y, p12.y);

		if(tp2.x - tp11.x > TOLERANCE
			&& tp12.x - tp1.x > TOLERANCE
			&& tp2.y - tp11.y >TOLERANCE
			&& tp12.y - tp1.y > TOLERANCE)
			return true;

		return false
	}
};

struct PosInfo
{
	double value;
	int pos;
	bool bR;
	PosInfo():value(0.0), pos(0), bR(false){}
	friend bool operator > (const PosInfo& l, const PosInfo& r)
	{
		if(fabs(r.value) - fabs(l.value) > 0.0000006)
			return true;

		return false;
	}
};

////////////////////////////////////////////////////////////////////////////
///--   --
///|r1 c1|
///|r2 c2|
///--   --
class PosMatrix
{
public:
	PosMatrix()
		:_x1(1), _y1(0),_x2(0), _y2(1)
		,_st(""), _sb("")
		,_sl(""), _sr("")
	{

	}
	void Flip()
	{
		Transform::Flip(_x1, _y1);
		Transform::Flip(_x2, _y2);
		QString s = _st;
		_st = _sb;
		_sb = s;
	}
	void Mirror()
	{
		Transform::Mirror(_x1, _y1);
		Transform::Mirror(_x2, _y2);
		QString s = _sl;
		_sl = _sr;
		_sr = s;
	}
	void Clock90()
	{
		Transform::Clock90(_x1, _y1);
		Transform::Clock90(_x2, _y2);

		QString s = _sl;
		_sl = _sb;
		_sb = _sr;
		_sr = _st;
		_st = s;
	}
	void AntiClock90()
	{
		Transform::AntiClock90(_x1, _y1);
		Transform::AntiClock90(_x2, _y2);
		QString s = _sl;
		_sl = _st;
		_st = _sr;
		_sr = _sb;
		_sb = s;
	}

	///Get the times of rotation from primitive image, clock's
	int GetRotateN(int r, int c, int &r1, int &c1)
	{
		///dot product
		int p = _x1*r + _y1 *c;
		///ver
		if(p == 0)
		{
			if(_y1 < 0)
			{
				r1 = r; c1 = c;
				return -1;
			}
			else
			{
				r1 = -r; c1 = c;
				return 1;
			}
		}
		///hor
		if(p == -1)
		{
			r1 = c; c1 = -r;
			return 2;
		}

		r1 = 0; c1 = 1;
		return 0;
	}


	int IsMirrorOrFlip(int r, int c)
	{
		int p = _x2*r + _y2 *c;
		assert(p != 0);
		if(p == 1)
			return 0;	//noting
		if(_x2 == 0)
			return 2;	//flip	

		return 1;		//mirror
	}

	void SetOrientPos(double *r, double *c)
	{
		PosInfo p[3];
		for (int i = 0; i < 3; i++)
		{
			p[i].value = r[i];
			p[i].pos = i+1;
			p[i].bR = r[i] < -0.0000001 ? true:false;
		}

		MaxOrder(p, 3);
		SetInfo(p[0], p[1], _sl, _sr);
		
		for (int i = 0; i < 3; i ++)
		{
			p[i].value = c[i];
			p[i].pos = i+1;
			p[i].bR = c[i] < -0.0000001 ? true:false;
		}

		MaxOrder(p, 3);		
		SetInfo(p[0], p[1], _st, _sb);
	}
	void SetOriginStrPos(const QString &r, const QString &b)
	{
		_sr = r;
		_sb = b;
		_sr.trimmed();
		_sb.trimmed();
	}

	QString l() const {return _sl;}
	QString r() const{return _sr;}
	QString t() const{return _st;}
	QString b() const{return _sb;}
	void Restore()
	{
		_x1 = 1; _y1 = 0; _x2= 0; _y2= 1;
		_sl=_sb = _sr = _st = "";
	}

	int _x1, _y1;
	int _x2, _y2;
private:
	void SetInfo(const PosInfo & p1, const PosInfo &p2, QString &s1, QString &s2)
	{
		switch(p1.pos)
		{
		case 1:
			if(p1.bR) {s1 = "L"; s2 = "R";}
			else	  {s1 = "R"; s2 = "L";}
			break;
		case 2:
			if(p1.bR) {s1 = "P"; s2 = "A";}
			else	  {s1 = "A"; s2 = "P";}
			break;
		case 3:
			if(p1.bR) {s1 = "H"; s2 = "F";}
			else	  {s1 = "F"; s2 = "H";}
			break;
		}

		if(fabs(p2.value) > 0.0000001)
		{
			switch(p2.pos)
			{
			case 1:
				if(p2.bR) {s1 += "L"; s2 += "R";}
				else	  {s1 += "R"; s2 += "L";}
				break;
			case 2:
				if(p2.bR) {s1 += "P"; s2 += "A";}
				else	  {s1 += "A"; s2 += "P";}
				break;
			case 3:
				if(p2.bR) {s1 += "H"; s2 += "F";}
				else	  {s1 += "F"; s2 += "H";}
			    break;
			}
		}
	}

	PosMatrix(const PosMatrix &rhs)
	{
		_x1 = rhs._x1; _x2 = rhs._x2; _y1 = rhs._y1; _y2 = rhs._y2;
		_sb = rhs._sb; _sl = rhs._sl; _sr = rhs._sr; _st = rhs._st;
	}

	PosMatrix &operator=(const PosMatrix &rhs)
	{
		_x1 = rhs._x1; _x2 = rhs._x2; _y1 = rhs._y1; _y2 = rhs._y2;
		_sb = rhs._sb; _sl = rhs._sl; _sr = rhs._sr; _st = rhs._st;
		return *this;
	}

	void MaxOrder(PosInfo *pInfo, int num)
	{
		for (int i = 1; i < num; i ++)
		{
			for (int j = 0; j < i; j++)
			{
				if(pInfo[j] > pInfo[i])
				{
					PosInfo pos = pInfo[i];
					pInfo[i] = pInfo[j];
					pInfo[j] = pos;
				}
			}
		}
	}
	
	QString _st, _sb, _sl, _sr;
};
#endif