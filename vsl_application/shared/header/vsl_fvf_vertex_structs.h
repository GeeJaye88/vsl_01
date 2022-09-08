////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_fvf_vertex_structs.h ----------
/*!
\file vsl_fvf_vertex_structs.h
\brief fvf vertex utility structs
\author Gareth Edwards
*/

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---------- structs for FVF - Flexible Vertex Formats ----------

namespace vsl_application
{

	struct VertexN
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL
		};
	};

	struct VertexNT
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		FLOAT tu, tv;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
		};

	};

	struct VertexNDT
	{
		FLOAT x, y, z;
		FLOAT nx, ny, nz;
		DWORD diffuse;
		FLOAT tu, tv;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1
		};
	};

}


////////////////////////////////////////////////////////////////////////////////


// ---------- struct for a FVF Vertex - Flexible Vertex Format ----------

namespace vsl_application
{

	struct Vertex
	{

		// ---- properties
			FLOAT x, y, z;


		// --- enumerators
			enum FVF
			{
				FVF_Flags = D3DFVF_XYZ
			};


		// ---- cdtor housekeeping stuff
			Vertex::Vertex() { x = 0; y = 0; z = 0; };
			Vertex::Vertex(int n) { new Vertex[n]; }
			Vertex::Vertex(FLOAT xn, FLOAT yn, FLOAT zn)
			{
				x = xn,
				y = yn,
				z = zn;
			}


		// ---- math stuff

			VOID DivideBy(FLOAT d)
			{
				this->x /= d;
				this->y /= d;
				this->z /= d;
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
				if ( len < 0.00001f )
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

			VOID CrossProduct(Vertex *a, Vertex *b)
			{
				this->x = a->y * b->z - a->z * b->y;
				this->y = a->z * b->x - a->x * b->z;
				this->z = a->x * b->y - a->y * b->x;
			};

			FLOAT DotProduct(Vertex &v)
			{
				return (x*v.x) + (y*v.y) + (z*v.z);
			}

			VOID Zero() { x = 0; y = 0; z = 0; }


		// ---- overloaded operators

			Vertex operator=(const Vertex &vec)
			{
				return Vertex(x = vec.x, y = vec.y, z = vec.z);
			}

			Vertex operator+(const Vertex &vec)
			{
				return Vertex(x + vec.x, y + vec.y, z + vec.z);
			}

			Vertex operator-(const Vertex &vec)
			{
				return Vertex(x - vec.x, y - vec.y, z - vec.z);
			}

			Vertex operator+=(const Vertex &vec)
			{
				return Vertex(x = x + vec.x, y = y + vec.y, z = z + vec.z);
			}

	};

}


////////////////////////////////////////////////////////////////////////////////


namespace vsl_application
{

	struct MyRectangle
	{
		int width, height;

	public:
		VOID set_values(int x, int y)
		{
			width = x;
			height = y;
		}
		int area() { return width * height; }
	};

}


////////////////////////////////////////////////////////////////////////////////

