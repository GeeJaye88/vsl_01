////////////////////////////////////////////////////////////////////////////////

// ---------- test_cube.hpp ----------
/*!
\file test_cube.hpp
*/

#if _MSC_VER > 1000
#pragma once
#endif

////////////////////////////////////////////////////////////////////////////////


// ---------- Cube_SideSetup ----------
/*!
\brief setup a single cube side
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT* - size
\param FLOAT* - move
*/
VOID Object3d_V01::Cube_SideSetup(
		LPDIRECT3DDEVICE9 device,	 // device
		FLOAT             size,      // e.g. 5
		FLOAT             move       // e.g. 1
	)
{

	// ---- init
		D3DCOLOR d = D3DCOLOR_COLORVALUE( 1, 1, 1, 0 );

		FLOAT depth = 0.5f;
		FLOAT s  = size;
		FLOAT y  = size + move;


	// ---- vertex list
		VertexNDT l[] =
		{
			// x   y   z   nx   ny  nz  d,  tu  tv
			{  s,  y,  s,  0,  -1,  0,  d,  0,  0    }, // 0
			{  s,  y, -s,  0,  -1,  0,  d,  1,  0    }, // 1
			{ -s,  y,  s,  0,  -1,  0,  d,  0,  1    }, // 2
			{ -s,  y, -s,  0,  -1,  0,  d,  1,  1    }, // 3

			{  s,  y,  s,  0,   1,  0,  d,  0,  0    }, // 4
			{  s,  y, -s,  0,   1,  0,  d,  1,  0    }, // 5
			{ -s,  y,  s,  0,   1,  0,  d,  0,  1    }, // 6
			{ -s,  y, -s,  0,   1,  0,  d,  1,  1    }, // 7

		};

	// ---- trangle vertex list
		VertexNDT cube_vertices[] =
		{
			l[0], l[1], l[2],
			l[2], l[1], l[3],
			l[6], l[5], l[4],
			l[7], l[5], l[6],
		};
		cube_side_num_primitives = 4;


	// ---- create the vertex buffer
		device->CreateVertexBuffer(
				12*sizeof(VertexNDT),
				D3DUSAGE_WRITEONLY,
				VertexNDT::FVF_Flags,
				//D3DPOOL_MANAGED, // does not have to be properly Released before calling IDirect3DDevice9::Reset
				D3DPOOL_DEFAULT,   // must be Released properly before calling IDirect3DDevice9::Reset
				&p_cube_side_vertex_buffer,
				NULL
			);


	// ---- lock and fill vertex buffer
		VOID *p_vertices = NULL;
		p_cube_side_vertex_buffer->Lock( 0, sizeof(cube_vertices), (VOID**)&p_vertices, 0 );
		std::memcpy(p_vertices, cube_vertices, sizeof(cube_vertices) );
		p_cube_side_vertex_buffer->Unlock();

}


// ---------- Cube_PyramidSideSetup ----------
/*!
\brief setup a single cube pyramid side
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT* - size
\param FLOAT* - move
*/
VOID Object3d_V01::Cube_PyramidSideSetup(
		LPDIRECT3DDEVICE9 device,	 // device
		FLOAT             size,      // e.g. 5
		FLOAT             move       // e.g. 1
	)
{

	// ---- init diffuse colour
		D3DCOLOR d = D3DCOLOR_COLORVALUE( 1, 1, 1, 0 );

	// ---- cube dimensions
		FLOAT s  = size;
		FLOAT y  = size + move;

	// ---- point of pyramid is twice size plus move
		FLOAT Y = size * 2 + move;

	// ---- unit normal to 45 degree rotation is 1 / sqrt(2)
		FLOAT nx = 0.7071f;
		FLOAT ny = 0.7071f;
		FLOAT nz = 0.7071f;


	// ---- vertex list
 		VertexNDT l[] =
		{
			// x   y   z   nx   ny  nz  d,  tu  tv

			{  s,  y,  s,  0,  -1,  0,  d,  0,  0    }, // 0
			{  s,  y, -s,  0,  -1,  0,  d,  1,  0    }, // 1
			{ -s,  y,  s,  0,  -1,  0,  d,  0,  1    }, // 2
			{ -s,  y, -s,  0,  -1,  0,  d,  1,  1    }, // 3

			{  s,  y,  s,  nx, ny,  0,  d,  0,    0  }, // 4
			{  0,  Y,  0,  nx, ny,  0,  d,  0.5f, 1  }, // 5
			{  s,  y, -s,  nx, ny,  0,  d,  1,    0  }, // 6

			{  s,  y, -s,   0, ny, -nz, d,  0,    0  }, // 7
			{  0,  Y,  0,   0, ny, -nz, d,  0.5f, 1  }, // 8
			{ -s,  y, -s,   0, ny, -nz, d,  1,    0  }, // 9

			{ -s,  y, -s, -nx, ny,  0,  d,  0,    0  }, // 10
			{  0,  Y,  0, -nx, ny,  0,  d,  0.5f, 1  }, // 11
			{ -s,  y,  s, -nx, ny,  0,  d,  1,    0  }, // 12

			{ -s,  y,  s,   0, ny,  nz, d,  0,    0  }, // 13
			{  0,  Y,  0,   0, ny,  nz, d,  0.5f, 1  }, // 14
			{  s,  y,  s,   0, ny,  nz, d,  1,    0  }, // 15

		};

	// ---- trangle vertex list
		VertexNDT cube_ertices[] =
		{
			l[ 0], l[ 1], l[ 2],
			l[ 2], l[ 1], l[ 3],
			l[ 4], l[ 5], l[ 6],
			l[ 7], l[ 8], l[ 9],
			l[10], l[11], l[12],
			l[13], l[14], l[15],
		};
		cube_side_num_primitives = 6;

	// ---- create the vertex buffer
		device->CreateVertexBuffer(
				18*sizeof(VertexNDT),
				D3DUSAGE_WRITEONLY,
				VertexN::FVF_Flags,
				//D3DPOOL_MANAGED, // does not have to be properly Released before calling IDirect3DDevice9::Reset
				D3DPOOL_DEFAULT,   // must be Released properly before calling IDirect3DDevice9::Reset
				&p_cube_side_vertex_buffer,
				NULL
			);

	// ---- lock and fill vertex buffer.
		VOID *p_vertices = NULL;
		p_cube_side_vertex_buffer->Lock( 0, sizeof(cube_ertices), (VOID**)&p_vertices, 0 );
		std::memcpy(p_vertices, cube_ertices, sizeof(cube_ertices) );
		p_cube_side_vertex_buffer->Unlock();

}


// ---------- Cube_Display ----------
/*!
\brief display cube
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param D3DXMATRIX- current world matrix
*/
VOID Object3d_V01::Cube_Display(
		LPDIRECT3DDEVICE9 device,
		D3DXMATRIX        m,
		FLOAT             dist
	)
{

	// ---- local matrices
		D3DXMATRIX matrix_world;
		D3DXMATRIX mat_zrot;
		D3DXMATRIX mat_xrot;

	// ---- xy rotation table
		FLOAT xzrt[6][2] =
		{
			{   0,   0, },
			{  90,   0, },
			{ 180,   0, },
			{ 270,   0, },
			{   0,  90, },
			{ 180,  90, },
		};

	// ---- set render states
		device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);


	// ---- 6 sides
		for (INT i = 0; i < 6; i++)
		{

			// ---- matrices
				D3DXMatrixRotationZ( &mat_zrot, D3DXToRadian(xzrt[i][1]) );
				D3DXMatrixRotationX( &mat_xrot, D3DXToRadian(xzrt[i][0]));

			// ---- multiply
				matrix_world = mat_zrot * m;
				matrix_world = mat_xrot * matrix_world;

			// ---- set this as THE transform
				device->SetTransform( D3DTS_WORLD, &matrix_world );

			// ---- display textured cube side
				device->SetMaterial(&cube_side_material);
				device->SetTexture(0, NULL);
				device->SetStreamSource(0, p_cube_side_vertex_buffer, 0, sizeof(VertexNDT));
				device->SetFVF(VertexNDT::FVF_Flags);
				device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, cube_side_num_primitives);
		}

		device->SetTransform(D3DTS_WORLD, &m);

}


////////////////////////////////////////////////////////////////////////////////