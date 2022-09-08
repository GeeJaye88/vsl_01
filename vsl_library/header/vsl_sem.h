////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_sem.h ----------
/*!
\file vsl_sem.h
\brief structs, enumerations and macros
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- VS_VertexFormat ----
/*!
\brief vertex formats
\author Gareth Edwards
*/
enum VS_VertexFormat
{
	VS_XYZ      = 0x002, // 2
	VS_XYZRHW   = 0x004, // 4
	VS_NORMAL   = 0x010, // 16
	VS_DIFFUSE  = 0x040, // 64
	VS_SPECULAR = 0x080, // 128
	VS_TEX1     = 0x100  // 256
};


// ---- d3d macros ----

// Initialises a color with the supplied red, green, blue, and 
// alpha values which must be in the range 0 through 255.
#define VS_D3DCOLOR_ARGB(a,r,g,b) \
	((D3DCOLOR)((((a)& 0xff) << 24) | (((r)& 0xff) << 16) | (((g)& 0xff) << 8) | ((b)& 0xff)))


// Initialises a color with the supplied red, green, blue, and 
// alpha values which must be in the range 0 through 255.
#define VS_D3DCOLOR_RGBA(r,g,b,a) VS_D3DCOLOR_ARGB(a,r,g,b)


// Initialises a color with the supplied red, green, blue, and 
// alpha FLOATing-point values which must be in the range 0 through 1.0.
#define VS_D3DCOLOR_COLORVALUE(r,g,b,a) \
	VS_D3DCOLOR_RGBA((DWORD)((r)*255.f), (DWORD)((g)*255.f), (DWORD)((b)*255.f), (DWORD)((a)*255.f))


////////////////////////////////////////////////////////////////////////////////
