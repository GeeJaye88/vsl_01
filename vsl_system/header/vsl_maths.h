////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_maths.h ----------
/*!
\file vsl_include.h
\brief vsl maths library
\author Gareth Edwards
*/  

#pragma once

// ---- vs system
#include "../../vsl_system/header/vsl_include.h"


////////////////////////////////////////////////////////////////////////////////

namespace vsl_system {

////////////////////////////////////////////////////////////////////////////////


	// ---- Vsl_Vector2 ----
	/*!
	\brief 2D vector primitive
	\author Gareth Edwards
	*/
	struct Vsl_Vector2
	{

	public:

		FLOAT x;
		FLOAT y;

		Vsl_Vector2::Vsl_Vector2()
		{
			x = y = 0;
		}

		Vsl_Vector2::Vsl_Vector2(FLOAT vx, FLOAT vy)
		{
			x = vx;
			y = vy;
		}
	};


////////////////////////////////////////////////////////////////////////////////


	// ---- Vsl_Vector3 ----
	/*!
	\brief 3D vector primitive
	\author Gareth Edwards
	*/
	struct Vsl_Vector3
	{

	public:

		FLOAT x;
		FLOAT y;
		FLOAT z;

		Vsl_Vector3::Vsl_Vector3()
		{
			x = y = z = 0;
		}

		Vsl_Vector3::Vsl_Vector3(FLOAT xn, FLOAT yn, FLOAT zn)
		{
			x = xn,
				y = yn,
				z = zn;
		}

		FLOAT Length()
		{
			FLOAT len = (FLOAT)sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
			if (len < 0.00001f)
			{
				return 0;
			}
			return len;
		}

		FLOAT Normalise()
		{
			FLOAT len = (FLOAT)sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
			if (len < 0.00001f)
			{
				return 0;
			}
			{
				this->x /= len;
				this->y /= len;
				this->z /= len;
			}
			return len;
		}

		VOID CrossProduct(Vsl_Vector3 *a, Vsl_Vector3 *b)
		{
			this->x = a->y * b->z - a->z * b->y;
			this->y = a->z * b->x - a->x * b->z;
			this->z = a->x * b->y - a->y * b->x;
		};

		FLOAT DotProduct(Vsl_Vector3 &v)
		{
			return (x*v.x) + (y*v.y) + (z*v.z);
		}

		FLOAT ScalarProduct(Vsl_Vector3 &v)
		{
			return (x*v.x) + (y*v.y) + (z*v.z);
		}

		Vsl_Vector3 operator=(const Vsl_Vector3 &vec)
		{
			return Vsl_Vector3(x = vec.x, y = vec.y, z = vec.z);
		}

		Vsl_Vector3 operator+(const Vsl_Vector3 &vec)
		{
			return Vsl_Vector3(x + vec.x, y + vec.y, z + vec.z);
		}

		Vsl_Vector3 operator+=(const Vsl_Vector3 &vec)
		{
			return Vsl_Vector3(x += vec.x, y += vec.y, z += vec.z);
		}

		Vsl_Vector3 operator-(const Vsl_Vector3 &vec)
		{
			return Vsl_Vector3(x - vec.x, y - vec.y, z - vec.z);
		}

		void operator*=(const FLOAT &value)
		{
			x *= value; y *= value; z *= value;
		}

		void operator/=(const FLOAT &value)
		{
			x /= value; y /= value; z /= value;
		}
	};


////////////////////////////////////////////////////////////////////////////////


	// ---- Vsl_Vector3D ----
	/*!
	\brief primitive type
	\author Gareth Edwards
	*/
	//struct Vsl_Vector3D
	//{

	//public:

	//	double x;
	//	double y;
	//	double z;

	//	Vsl_Vector3D::Vsl_Vector3D()
	//	{
	//		x = y = z = 0;
	//	}

	//	Vsl_Vector3D::Vsl_Vector3D(double xn, double yn, double zn)
	//	{
	//		x = xn,
	//			y = yn,
	//			z = zn;
	//	}

	//};


////////////////////////////////////////////////////////////////////////////////

} // namespace vsl_system

////////////////////////////////////////////////////////////////////////////////



