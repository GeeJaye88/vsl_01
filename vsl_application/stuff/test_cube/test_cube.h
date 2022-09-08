////////////////////////////////////////////////////////////////////////////////

// ---------- test_cube.h ----------
/*!
\file test_cube.h
*/

#if _MSC_VER > 1000
#pragma once
#endif

public:

	// ---- application cube methods
	VOID Cube_SideSetup(LPDIRECT3DDEVICE9, FLOAT, FLOAT);
	VOID Cube_PyramidSideSetup(LPDIRECT3DDEVICE9, FLOAT, FLOAT);
	VOID Cube_Display(LPDIRECT3DDEVICE9, D3DXMATRIX, FLOAT);

private:

	// ---- quad
	LPDIRECT3DVERTEXBUFFER9	p_cube_side_vertex_buffer;
	D3DMATERIAL9			cube_side_material;
	INT                     cube_side_num_primitives;