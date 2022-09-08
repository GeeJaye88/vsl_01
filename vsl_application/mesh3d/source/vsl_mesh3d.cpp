////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_mesh3d.cpp ----------
/*!
\file vsl_mesh3d.cpp
\brief implementation of the Mesh3D class
\author Gareth Edwards

\note

   INTERACTION
   ===========
   
   Using # key display:
	
   # 1 - pyramidal or simple box
     2 - recursive teapots
     3 - mesh with sine wave
     4 - mesh with multiple sine wave
     5 - mesh with sine wave & derived normals
     6 - mesh with multiple Gerstner waves

   # 7 - wireframe / solid
	
   # 8 - mesh surface normals

   Use the mouse left click to drag object rotation.

   Use the mouse wheel to +/- object distance.


   SETUP
   =====

   See "../header/vsl_demo3d.h" for the:
	
   1. Enumeration of CUBE_OBJECT, TEAPOT_OBJECT, and MESH_OBJECT, and
      the property BYTE object_displayed set to MESH_OBJECT.

   2. Enumeration of  SINGLE_SINE, MULTIPLE_SINE, PARAMETRIC_SINE, and
	  GERSTNER_WAVE, and the property UINT mesh_type set to SINGLE_SINE.

   3. The property BOOL mesh_display_normals, set to FALSE.
	
   4. The property BOOL mesh_display_solid set TRUE.

   5. See below for selecting to create a pyramidal or single sided cube.
		#define SIMPLE_CUBE
		#if defined(SIMPLE_CUBE)
			CubeSideSetup(device, 4, 1);
		#else
			CubePyramidSideSetup(device, 4, 1);
		#endif

   6. See below for the selection of surface normal calculation:
		#if defined(DEBUG)
			MeshCalculateGoodEnoughNormals(device);
		#else
			MeshCalculateCorrectNormals(device);
		#endif

   7. See method SetKeyJustPressed for Keyboard options.

   8. See method DisplayText for displayed Text.
   5
*/


// ---------- include Mesh3D header ----------
#include "../header/vsl_mesh3d.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_application;


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
Mesh3D::Mesh3D()
{

	// ---- system windows config
		fw_win_create.SetName("VS Lite Mesh 3D Demo");
		fw_win_create.SetDesktop(TRUE);
		fw_win_engine.SetFps(60);

	// ---- set default display values
		Display_SetDefaults();

	// ---- input
		key_just_pressed = 0 ;
		key_last_pressed = key_just_pressed;

}


// ---------- destructor ----------
/*!
\brief destructor
\author Gareth Edwards
*/
Mesh3D::~Mesh3D ()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- system methods ----------


// ---------- Fw_Setup ----------
/*!
\brief system setup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Mesh3D::Fw_Setup()
{

	// ---- lambda
		auto init_material = [](D3DMATERIAL9 *d)
		{
			D3DCOLORVALUE zero = { 0, 0, 0, 0 };
			D3DCOLORVALUE full = { 1, 1, 1, 0 };
			d->Ambient  = zero;
			d->Emissive = zero;
			d->Diffuse  = full;
			d->Specular = full;
			d->Power = 40;
		};


    // ---- cube material
		ZeroMemory( &cube_side_material, sizeof(D3DMATERIAL9) ); 
		init_material(&cube_side_material);


    // ---- teapot material
		ZeroMemory( &teapot_material, sizeof(D3DMATERIAL9) );
		init_material(&teapot_material);
		teapot_material.Diffuse.r = 0;


	// ---- mesh material
		ZeroMemory(&mesh_material, sizeof(D3DMATERIAL9));
		init_material(&mesh_material);
		mesh_material.Specular.r = 0.5f;
		mesh_material.Specular.g = 0.5f;
		mesh_material.Specular.b = 0.5f;
		mesh_material.Power      = 60;


	// ---- mesh normal material
		ZeroMemory(&mesh_normal_material, sizeof(D3DMATERIAL9));
		init_material(&mesh_normal_material);
		D3DCOLORVALUE full = { 0.3f, 0.6f, 0.9f, 1 };
		mesh_normal_material.Emissive = full;


	// ---- lambda
		auto init_light = [](D3DLIGHT9 *l)
		{
			l->Type = D3DLIGHT_DIRECTIONAL;
			l->Direction  = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
			D3DCOLORVALUE full = { 1, 1, 1, 1 };
			l->Diffuse  = full;
			l->Specular = full;
		};


    // ---- directional sun light
		init_light(&sun_light);
		sun_light.Direction = D3DXVECTOR3(0, -1, -1);


    // ---- directional back light
		init_light(&back_light);
		back_light.Type = D3DLIGHT_DIRECTIONAL;
		back_light.Direction  = D3DXVECTOR3( 0,  1,  1 );
		D3DCOLORVALUE specular = { 0.3f, 0.3f, 0.3f, 0 };
		back_light.Specular = specular;


	// ---- mesh dimensions
		INT rows = 256;
		INT cols = 256;
		mesh_vertex_columns = cols;
		mesh_vertex_rows    = rows;
		mesh_cel_columns    = cols - 1;
		mesh_cel_rows       = rows - 1;


	// ---- allocate calculation buffers
		mesh_num_calc_vertices = mesh_vertex_rows * mesh_vertex_columns;
		mesh_calc_vertices = new Vertex[mesh_num_calc_vertices];
		mesh_calc_normals = new Vertex[mesh_num_calc_vertices];
		mesh_calc_points = new Vertex[mesh_num_calc_vertices];

	// ---- allocate displayed normals
		mesh_num_normals = (mesh_vertex_rows) * (mesh_vertex_columns);
		mesh_normal_vertices = new Vertex[mesh_num_normals * 2];

	return SUCCESS_OK;
}


// ---------- Fw_SetupDX ----------
/*!
\brief system setup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Mesh3D::Fw_SetupDX(LPDIRECT3DDEVICE9 device)
{

	// ---- turn lighting ON
		device->SetRenderState( D3DRS_LIGHTING , TRUE);


	// ---- ambient light
		device->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE( 0.1, 0.1, 0.1, 1.0));


	// ---- sun light
		device->SetLight( 0, &sun_light );
		device->LightEnable( 0, TRUE);


	// ---- back light
		device->SetLight( 1, &back_light );
		device->LightEnable( 1, FALSE );


    // ---- create a texture object
		D3DXCreateTextureFromFile( device, "vsl_application/mesh3d/image/water20800.jpg", &p_mesh_texture );
		device->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		device->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );


	// ---- shading mode
		device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD); // default D3DSHADE_GOURAUD / PHONG


    // ---- create a pyramidal or single sided cube
		#define SIMPLE_CUBE
		#if defined(SIMPLE_CUBE)
			CubeSideSetup(device, 4, 1);
		#else
			CubePyramidSideSetup(device, 4, 1);
		#endif


	// ---- create teapot objects
		TeapotsSetup(device);


	// ---- create a mesh object
		FLOAT param[] = { -10, -10, 10, 10, 0, 0, 1, 1 };
		MeshSetup(device, param);


	// ---- create an instance of the font object for function DisplayText
		D3DXCreateFont(
				device,
				20, 0, FW_NORMAL, 0,
				FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,
				TEXT("Arial"), // was Arial
				&font
			);

	return SUCCESS_OK;
}


// ---------- Fw_Display ----------
/*!
\brief system display
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Mesh3D::Fw_Display(LPDIRECT3DDEVICE9 device)
{

	// ---- update if key pressed
		Display_AsyncKeyState();


    // ---- local matrices
		D3DXMATRIX matrix_view;
		D3DXMATRIX matrix_rotation;
		D3DXMATRIX matrix_translation;
		D3DXMATRIX matrix_world;


	// ---- increment turntable
		y_turntable += (DOUBLE)(fw_win_engine.GetMsDelta() / 20);


	// ---- view matrix
	    D3DXMatrixIdentity( &matrix_view );
		device->SetTransform( D3DTS_VIEW, &matrix_view);


	// ---- rotation matrix
		D3DXMatrixRotationYawPitchRoll(
				&matrix_rotation,
				D3DXToRadian(x_rotate),
				D3DXToRadian(y_rotate),
				0
			);


	// --- display text
		Display_Text();


	// ---- display cube ?
		if ( object_displayed == CUBE_OBJECT )
		{

			// ---- set render states
				device->SetRenderState(D3DRS_FILLMODE, mesh_display_solid ? D3DFILL_SOLID : D3DFILL_WIREFRAME);
				device->SetRenderState(D3DRS_CULLMODE, mesh_display_solid ? D3DCULL_CW : D3DCULL_NONE);
				device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.3f, 0.3f, 0.3f, 1.0));


			// ---- sun light
				sun_light.Direction = D3DXVECTOR3(-1, -1, 1);
				device->SetLight(0, &sun_light);


			// ---- back light
				back_light.Type = D3DLIGHT_DIRECTIONAL;
				back_light.Direction = D3DXVECTOR3(1, 0, 0);
				device->SetLight(1, &back_light);


			// ---- locate & turntable
				D3DXMATRIX mat_spin;
				D3DXMatrixTranslation(&matrix_translation, 0, 0, z_translate);
				D3DXMatrixRotationY(&mat_spin, D3DXToRadian( (FLOAT)y_turntable * cube_speed));
				matrix_world = matrix_rotation * matrix_translation;
				matrix_world = mat_spin        * matrix_world;


			// ---- display cube
				CubeDisplay(device, matrix_world, z_translate);

		}

	// ---- display teapots ?
		else if ( object_displayed == TEAPOT_OBJECT)
		{

			// ---- set render states
				device->SetRenderState(D3DRS_FILLMODE, mesh_display_solid ? D3DFILL_SOLID : D3DFILL_WIREFRAME);
				device->SetRenderState(D3DRS_CULLMODE, mesh_display_solid ? D3DCULL_CCW : D3DCULL_NONE);
				device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.1f, 0.1f, 0.1f, 1.0));


			// ---- sun light
				sun_light.Direction = D3DXVECTOR3(-1, -1, 1);
				device->SetLight(0, &sun_light);


			// ---- back light
				back_light.Type = D3DLIGHT_DIRECTIONAL;
				back_light.Direction = D3DXVECTOR3(1, 0, 0);
				device->SetLight(1, &back_light);


			// ---- locate & turntable
				D3DXMATRIX mat_scale, mat_spin;
				D3DXMatrixTranslation( &matrix_translation, 0, 2, z_translate * 0.66f);
				D3DXMatrixRotationY  ( &mat_spin,           D3DXToRadian( (FLOAT)y_turntable * teapot_speed));
				matrix_world = matrix_rotation * matrix_translation;
				matrix_world = mat_spin * matrix_world;


			// ---- display teapot
				device->SetMaterial( &teapot_material );
				device->SetTexture( 0, 0 );
				device->SetTransform(D3DTS_WORLD, &matrix_world);
				//p_teapot_mesh[0]->DrawSubset(0);


			// ---- recursively display teapots
				TeapotsDisplay(device, matrix_world, 6, 0);

		}

	// ---- display mesh (note: y_turntable is phase_shift) ?
		else if ( object_displayed == MESH_OBJECT )
		{
			MeshDisplay(device, &matrix_rotation, (FLOAT)y_turntable);
		}



	return SUCCESS_OK;
}


// ---------- Fw_Cleanup dx ----------
/*!
\brief system cleanup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Mesh3D::Fw_CleanupDX (LPDIRECT3DDEVICE9 device)
{

    // ---- invalidate the texture object
		if ( p_mesh_texture != NULL )
		{
			int new_ref_count = p_mesh_texture->Release();

			if ( new_ref_count > 0 )
			{
				static char std_error[256];
				sprintf_s ( std_error, 256,
					"The texture object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					new_ref_count );
				MessageBox( NULL, std_error, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			p_mesh_texture = NULL;
		}


    // ---- invalidate the vertex buffer object
		if ( p_cube_side_vertex_buffer != NULL )
		{
			int new_ref_count = p_cube_side_vertex_buffer->Release();

			if ( new_ref_count > 0 )
			{
				static char std_error[256];
				sprintf_s ( std_error, 256,
					"The vertex buffer object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					new_ref_count );
				MessageBox( NULL, std_error, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			p_cube_side_vertex_buffer = NULL;
		}


    // ---- invalidate the teapot objects
		for ( int i=0; i<5; i++ )
		{
			if ( p_teapot_mesh[i] != NULL )
			{
				int new_ref_count = p_teapot_mesh[i]->Release();

				if( new_ref_count > 0 )
				{
					static char std_error[256];
					sprintf_s ( std_error, 256,
						"The teapot mesh object failed to cleanup properly.\n"
						"Release() returned a reference count of %d",
						new_ref_count );
					MessageBox( NULL, std_error, "ERROR", MB_OK | MB_ICONEXCLAMATION );
				}

				p_teapot_mesh[i] = NULL;
			}
		}


	// ---- invalidate the mesh object
		if ( p_mesh != NULL )
		{
			int new_ref_count = p_mesh->Release();

			if ( new_ref_count > 0 )
			{
				static char std_error[256];
				sprintf_s ( std_error, 256,
					"The plane mesh object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					new_ref_count );
				MessageBox( NULL, std_error, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			p_mesh = NULL;

		}


	// ---- invalidate the font object
		if ( font != NULL )
		{
			int nNewRefCount = font->Release();

			if ( nNewRefCount > 0 )
			{
				static char strError[256];
				sprintf_s ( strError, 256,
					"The font object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					nNewRefCount );
				MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			font = NULL;

		}

	return SUCCESS_OK;
}


// ---------- Fw_Cleanup ----------
/*!
\brief system cleanup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Mesh3D::Fw_Cleanup()
{

	// ---- calculation buffers
		delete [] mesh_calc_vertices;
		delete [] mesh_calc_normals;
		delete [] mesh_calc_points;

	// ---- displayed normals
		delete [] mesh_normal_vertices;

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- system get methods ----------


// ---------- Fw_Get_Win_Create ----------
/*!
\brief system get pointer to wnd create struct
\author Gareth Edwards
\param Win_Create **wc
*/
VOID Mesh3D::Fw_Get_Win_Create(vsl_system::Win_Create **wc)
	{ *wc = &fw_win_create; }


// ---------- Fw_Get_Win_Engine ----------
/*!
\brief system get pointer to wnd engine struct
\author Gareth Edwards
\param Win_Engine **wc
*/
VOID Mesh3D::Fw_Get_Win_Engine(vsl_system::Win_Engine **wc)
	{ *wc = &fw_win_engine; }


////////////////////////////////////////////////////////////////////////////////


// ---------- system notify event methods ----------


// ---------- Fw_Set_MouseLeftButtonDownMove ----------
/*!
\brief system notify mouse left button down move event
\author Gareth Edwards
\param FLOAT x
\param FLOAT y
*/
VOID Mesh3D::Fw_Set_MouseLeftButtonDownMove(INT x, INT y)
{
	fw_win_engine.SetMouseLeftButtonDownMove(x,y);
	x_rotate -= x;
	y_rotate -= y;
}


// ---------- Fw_Set_MouseWheelClick ----------
/*!
\brief system notify mouse wheel click event
\author Gareth Edwards
\param intd d (+/- 1)
*/
VOID Mesh3D::Fw_Set_MouseWheelClick(INT d)
{
	fw_win_engine.SetMouseWheelClickDelta(d);
	z_translate += (FLOAT)d;
}


// ---------- Fw_Set_Keydown ----------
/*!
\brief system notify keyboard event
\author Gareth Edwards
\param WPARAM (parameter)
*/
VOID Mesh3D::Fw_Set_Keydown(WPARAM param)
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- cube specific methods ----------


// ---------- CubeSideSetup ----------
/*!
\brief setup a single cube side
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT* - size
\param FLOAT* - move
*/
VOID Mesh3D::CubeSideSetup(
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


// ---------- CubePyramidSideSetup ----------
/*!
\brief setup a single cube pyramid side
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT* - size
\param FLOAT* - move
*/
VOID Mesh3D::CubePyramidSideSetup(
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


// ---------- CubeDisplay ----------
/*!
\brief display cube
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param D3DXMATRIX- current world matrix
*/
VOID Mesh3D::CubeDisplay(
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
				device->SetTexture(0, p_mesh_texture);
				device->SetStreamSource(0, p_cube_side_vertex_buffer, 0, sizeof(VertexNDT));
				device->SetFVF(VertexNDT::FVF_Flags);
				device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, cube_side_num_primitives);
		}

}


////////////////////////////////////////////////////////////////////////////////


// ---------- TeapotsSetup ----------
/*!
\brief teapot
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
*/
VOID Mesh3D::TeapotsSetup(
		LPDIRECT3DDEVICE9 device
	)
{
	
	// ---- local D3DX
		HRESULT hr;


	// ---- create the mesh
			hr = D3DXLoadMeshFromX(
				"vsl_application/mesh3d/model/teapot.x",
				//D3DXMESH_SYSTEMMEM, // does not have to be properly Released before calling IDirect3DDevice9::Reset
				//D3DXMESH_MANAGED,   // ditto
				//D3DXMESH_WRITEONLY, // ditto
				D3DXMESH_DYNAMIC,     // must be Released properly before calling IDirect3DDevice9::Reset
				device,
				NULL, NULL, NULL, NULL,
				&p_teapot_mesh[0]
			);
			if (FAILED(hr))
			{
				::MessageBox(0, "Error : TeapotsSetup - D3DXCreateMeshFVF() - FAILED", 0, 0);
				return;
			}


	// ---- get mesh parameters
		DWORD fvf                  = p_teapot_mesh[0]->GetFVF();
		DWORD num_faces            = p_teapot_mesh[0]->GetNumFaces();
		DWORD num_vertices         = p_teapot_mesh[0]->GetNumVertices();
		DWORD num_bytes_per_vertex = p_teapot_mesh[0]->GetNumBytesPerVertex();
		DWORD options              = p_teapot_mesh[0]->GetOptions();


	// ---- create 4 smaller teapot objects
		for ( int i=1; i<=4; i++ )
		{

			// ---- load a teapot
				D3DXLoadMeshFromX(
						"vsl_application/mesh3d/model//teapot.x",
						D3DXMESH_DYNAMIC,
						device,
						NULL, NULL, NULL, NULL,
						&p_teapot_mesh[i]
					);


			// ---- lock and update the teapot mesh vertex buffer
				VertexN *pVertex = NULL;
				HRESULT hr = p_teapot_mesh[i]->LockVertexBuffer(0, (VOID**) &pVertex);


			// ---- scale
				FLOAT scalar = 1 / (FLOAT)(2*i);
				for (DWORD v=0; v<num_vertices; v++)
				{
					pVertex->x *= scalar;
					pVertex->y *= scalar;
					pVertex->z *= scalar;
					pVertex ++;
				}


			// ---- unlock vertex buffer
				hr = p_teapot_mesh[i]->UnlockVertexBuffer();
		}

}


// ---------- TeapotsDisplay ----------
/*!
\brief teapot 
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param D3DXMATRIX - current world matrix
\param FLOAT - distance translated from parent
\param int - level of recursion
*/
VOID Mesh3D::TeapotsDisplay(
		LPDIRECT3DDEVICE9 device,
		D3DXMATRIX        m,
		FLOAT             dist,
		INT               level
	)
{

	// ---- stop recursion....
		if ( level == 4 ) return;


	// ---- local matrices
		D3DXMATRIX matrix_world;
		D3DXMATRIX mat_spin;
		D3DXMATRIX matrix_rotation;
		D3DXMATRIX matrix_translation;


	// ---- 4 teapots
		for ( INT t=0; t<5; t++ )
		{

			// ---- @ 90 degrees
				FLOAT angle = (FLOAT)t / 4 * 360;

			// ---- matrices
				D3DXMatrixRotationY   ( &mat_spin, D3DXToRadian((FLOAT)y_turntable*teapot_speed));
				D3DXMatrixTranslation ( &matrix_translation, dist, 0, 0 );
				D3DXMatrixRotationY   ( &matrix_rotation, D3DXToRadian(angle) );

			// ---- multiply
				matrix_world = matrix_rotation * m;
				matrix_world = matrix_translation * matrix_world;
				matrix_world = mat_spin * matrix_world;

			// ---- set this as THE transform
				device->SetTransform( D3DTS_WORLD, &matrix_world );

			// ---- draw - NOTE swap level for t
				device->SetMaterial( &teapot_material );
				p_teapot_mesh[level]->DrawSubset(0);

			// ---- ...and another four teapots
				TeapotsDisplay(device, matrix_world, dist * 0.5f, level+1);

			/*
				
			// ---- output indented text to match level of iteration
				char *indent = NULL;
				indent = "";
				switch (level)
				{
					case 1: indent = " "; break;
					case 2: indent = "  "; break;
					case 3: indent = "   "; break;
					case 4: indent = "    "; break;
				}

				if ( indent == NULL ) indent = "";

				char ods[128];
				sprintf(ods," %s %d - %d \n", indent, level, t);
				OutputDebugString(ods);

			*/

		}

}


////////////////////////////////////////////////////////////////////////////////


// ---------- MESH FRAMEWORK ---------


// ---------- MeshSetup ----------
/*!
\brief setup a row/column mesh in the xz plane
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param int - row start in height field
\param int - column start in height field
\param FLOAT* - mesh parameters array - see below

Mesh parameters array must contain eight values.

These are:

	0: x minimum\n
	1: z \n
	2: x maximum\n
	3: z \n
	4: u minimum\n
	5: v \n
	6: u maximum\n
	7: v \n

*/
VOID Mesh3D::MeshSetup(
		LPDIRECT3DDEVICE9	device, // device
		FLOAT              *param   // mesh parameters
	)
{

	// ---- store mesh parameters
		for (UINT i = 0; i < 8; i++)
		{
			mesh_param[i] = param[i];
		}
		

	// ---- initialise mesh properties
		mesh_vertex_format = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
		mesh_vertex_size   = 0;
		mesh_vertices      = mesh_vertex_rows * mesh_vertex_columns;
		mesh_faces         = mesh_cel_columns * mesh_cel_rows * 2;


	// ---- local
		HRESULT		hr;
		DWORD*		p_index_data;
		DWORD*		p_attribute_buffer;


	// ---- create the mesh
		hr  = D3DXCreateMeshFVF(
				mesh_faces,
				mesh_vertices,
				D3DXMESH_32BIT, //0,
				mesh_vertex_format,
				device,
				&p_mesh
			);
			if (FAILED(hr))
			{
				::MessageBox(0, "Error : MeshSetup - D3DXCreateMeshFVF() - FAILED", 0, 0);
				return;
			}


	// ---- INDEX BUFFER ----
		

	// ---- lock and fill index buffer
		UINT col, row;
		hr = p_mesh->LockIndexBuffer(0, (VOID**) &p_index_data);

		// ---- for each cel of the mesh
			UINT ro, ol, oh;
			for (row=0; row < mesh_cel_rows; row++)
			{
				ro = row * mesh_vertex_columns;
				for (col=0; col < mesh_cel_columns; col++)
				{

					ol = ro + col;
					oh = ro + col + mesh_vertex_columns;
					*(p_index_data+0) = ol;
					*(p_index_data+1) = ol+1;
					*(p_index_data+2) = oh;
					*(p_index_data+3) = oh;
					*(p_index_data+4) = ol+1;
					*(p_index_data+5) = oh+1;
					p_index_data += 6;


				}
			}

	// ---- unlock
		hr = p_mesh->UnlockIndexBuffer();


	// ---- ATTRIBUTE BUFFER ----


	// ---- lock and fill attribute buffer
		hr = p_mesh->LockAttributeBuffer(0, &p_attribute_buffer);
		for(UINT f=0; f< mesh_faces; f++)
			p_attribute_buffer[f]= 0;
		hr = p_mesh->UnlockAttributeBuffer();


	// ---- initialise
		MeshInitialise(device);

}


// ---------- MeshInitialise ----------
/*!
\brief invoked by MeshInitialise to fully initialise mesh geometry
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
*/
VOID Mesh3D::MeshInitialise(
		LPDIRECT3DDEVICE9	device
	)
{

	// ---- local
		HRESULT		hr;
		VertexNT*	p_vertex_data;


	// ---- lock and fill vertex buffer.
		hr = p_mesh->LockVertexBuffer(0, (VOID**) &p_vertex_data);


	// ---- vertex index
		INT v_index = 0;


	// ---- dimensions
		FLOAT x_width = mesh_param[2] - mesh_param[0];
		FLOAT z_depth = mesh_param[3] - mesh_param[1];
		FLOAT x_min   = mesh_param[0];
		FLOAT z_min   = mesh_param[1];


	// ---- mapping
		FLOAT u_width = mesh_param[6] - mesh_param[4];
		FLOAT v_depth = mesh_param[7] - mesh_param[5];
		FLOAT u_min   = mesh_param[4];
		FLOAT v_min   = mesh_param[5];


	// ---- calculation buffers
		Vertex *p_calc_vertices = mesh_calc_vertices;
		Vertex *p_calc_normals = mesh_calc_normals;


	// ---- init mesh

	// ---- rows
		for (DWORD row=0; row<= mesh_cel_rows; row++)
		{

			//---- y & v ordinates
				FLOAT rmu = FLOAT(row)/ FLOAT(mesh_cel_rows);
				FLOAT z   = rmu * z_depth + z_min;
				FLOAT v   = (1-rmu) * v_depth + v_min;

			// ---- columns
				for (DWORD col=0; col<= mesh_cel_columns; col++)
				{

					// ---- vertex
						FLOAT cmu = FLOAT(col)/ FLOAT(mesh_cel_columns);
						p_vertex_data[v_index].x  = (1-cmu) * x_width + x_min;
						p_vertex_data[v_index].y  = 0.0;
						p_vertex_data[v_index].z  = z;

					// ---- normal
						p_vertex_data[v_index].nx = 0;
						p_vertex_data[v_index].ny = 1;
						p_vertex_data[v_index].nz = 0;

					// ---- texture
						p_vertex_data[v_index].tu = cmu * u_width + u_min;
						p_vertex_data[v_index].tv = v;

					// ---- calculation buffers
						p_calc_vertices[v_index].x = p_vertex_data[v_index].x;
						p_calc_vertices[v_index].y = p_vertex_data[v_index].y;
						p_calc_vertices[v_index].z = p_vertex_data[v_index].z;
						p_calc_normals[v_index].x = 0;
						p_calc_normals[v_index].y = 1;
						p_calc_normals[v_index].z = 0;

					// ---- increment vertex index
						v_index++;
				}
		}


	// ---- unlock
		hr = p_mesh->UnlockVertexBuffer();


	// ---- zero display normals
		mesh_num_normals = (mesh_vertex_rows) * (mesh_vertex_columns);
		mesh_normal_vertices = new Vertex[mesh_num_normals * 2];
		Vertex *vertex = mesh_normal_vertices;
		memset(mesh_normal_vertices, 0, mesh_num_normals * 2 * sizeof(Vertex));

}


// ---------- MeshDisplay ----------
/*!
\brief display mesh
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT - wave phase shift
\return bool (TRUE if ok)
*/
VOID Mesh3D::MeshDisplay(
		LPDIRECT3DDEVICE9 device,
		D3DXMATRIX *rotation,
		FLOAT phase_shift
	)
{

	// ---- local matrices
		D3DXMATRIX matrix_translation;
		D3DXMATRIX matrix_world;


	// ---- set render states
		device->SetRenderState(D3DRS_FILLMODE,  mesh_display_solid ? D3DFILL_SOLID  : D3DFILL_WIREFRAME);
		device->SetRenderState(D3DRS_CULLMODE,  mesh_display_solid ? D3DCULL_CCW    : D3DCULL_NONE);
		//device->SetRenderState(D3DRS_SHADEMODE, mesh_display_solid ? D3DSHADE_PHONG : D3DSHADE_FLAT);
		device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.3f, 0.3f, 0.3f, 1.0));

	// ---- sun light
		sun_light.Direction = D3DXVECTOR3(0, -1, -1);
		device->SetLight(0, &sun_light);


	// ---- back light
		back_light.Type = D3DLIGHT_DIRECTIONAL;
		back_light.Direction = D3DXVECTOR3(0, 1, 1);
		device->SetLight(1, &back_light);


	// ---- locate mesh
		D3DXMatrixTranslation( &matrix_translation, 0, 0.0f , z_translate);
		matrix_world = *rotation * matrix_translation;
		device->SetTransform( D3DTS_WORLD, &matrix_world );


	// ---- update mesh
		switch ( mesh_type )
		{
			case SINGLE_SINE:
				MeshSineWaveOriginal(phase_shift);
				break;

			case MULTIPLE_SINE:
				MeshSineWaveMultiple(phase_shift);
				break;

			case PARAMETRIC_SINE:
				MeshSineWaveMultipleNew(phase_shift * 0.5f);
				break;

			case GERSTNER_WAVE:
				MeshGerstner(phase_shift * 0.375f);
				break;

			default:
				break;
		}


	// ---- not parametric, then calculate "good enough" normals ?
		if ( mesh_type != PARAMETRIC_SINE )
		{
			#define GOOD_ENOUGH
			#if defined(GOOD_ENOUGH)
			MeshCalculateGoodEnoughNormals();
			#else
			MeshCalculateCorrectNormals();
			#endif
		}


	// ---- display mesh
		device->SetMaterial(&mesh_material);
		device->SetTexture(0, p_mesh_texture);
		p_mesh->DrawSubset(0);


	// ---- display mesh normals
		if ( mesh_display_normals )
		{
			MeshCalculateDisplayedNormals();
			//device->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
			device->SetMaterial(&mesh_normal_material);
			device->SetTexture(0, 0);
			device->SetFVF(Vertex::FVF_Flags);
			device->DrawPrimitiveUP(D3DPT_LINELIST, mesh_num_normals, mesh_normal_vertices, sizeof(Vertex));
		}

}


////////////////////////////////////////////////////////////////////////////////


// ---------- MESH NORMALS ---------


// ---------- MeshCalculateGoodEnoughNormals ----------
/*!
\brief calculate "good enough" surface normals from mesh geometry
\author Gareth Edwards

\note The code has been left incomplete for speed - it is good enough!
	  However the calculation of the cross product should use unit normals!!!

\note This code is a single waterfall of computation which invokes no other
	  functions and executes very quickly in either DEBUG or RELEASE mode.

	  To generate "correct" surface normals requires a more complex maths
	  solution, one which uses Vertex functions and operator overloads.
	  These are inimical to fast execution in DEBUG mode. Hence there are
	  two MeshCalculate[?]Normals functions.

*/
VOID Mesh3D::MeshCalculateGoodEnoughNormals()
{

	// ---- buffer
		INT cols       = mesh_cel_columns + 1;
		INT rows       = mesh_cel_rows + 1;
		INT row_stride = cols;
		INT extent     = cols * rows;
		INT cel_count  = 0;
		FLOAT fcc      = 0;


	// ---- tables
		INT  ro[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // row offset
		BOOL gs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // grid stage


	//---- for each cel...
		INT   last_row_offset, this_row_offset, next_row_offset;
		INT   lro, tro, nro;
		FLOAT x1, y1, z1;
		FLOAT x2, y2, z2;
		FLOAT x3, y3, z3;
		FLOAT v1x, v1y, v1z;
		FLOAT v2x, v2y, v2z;
		FLOAT xd, yd, zd;
		FLOAT len;
		for (INT row = 0; row < rows; row++)
		{
				last_row_offset = (row - 1) * row_stride;
				this_row_offset =  row      * row_stride;
				next_row_offset = (row + 1) * row_stride;

				for (INT col = 0; col < cols; col++)
				{

					// ---- row offsets
						lro = last_row_offset + col;
						tro = this_row_offset + col;
						nro = next_row_offset + col;

					// ---- cel offset grid list
						ro[0] = lro-1; ro[1] = lro; ro[2] = lro+1;
						ro[7] = tro-1;              ro[3] = tro+1;
						ro[6] = nro-1; ro[5] = nro; ro[4] = nro+1;
						ro[8] = ro[0];

					// ---- set grid state (gs) flag 'within'
						for (INT i=0; i<9; i++)
						{
							gs[i] = ro[i] >= 0 && ro[i] < extent ? true : false;
						}

					// ---- set grid state (gs) flag if not first/last row & column
						if ( col == 0      ) gs[0] = gs[6] = gs[7] = gs[8] = 0;
						if ( col == cols-1 ) gs[2] = gs[3] = gs[4] = 0;
						if ( row == 0      ) gs[0] = gs[1] = gs[2] = 0;
						if ( row == rows-1 ) gs[4] = gs[5] = gs[6] = 0;

					// ---- zero normal
						mesh_calc_normals[tro].x = 0;
						mesh_calc_normals[tro].y = 0;
						mesh_calc_normals[tro].z = 0;

					// ---- accumulate 'legal' grid normals
						cel_count = 0;
						x2 = mesh_calc_vertices[tro].x;
						y2 = mesh_calc_vertices[tro].y;
						z2 = mesh_calc_vertices[tro].z;
						for (INT i=0; i<8; i++)
						{
								if ( gs[i] && gs[i+1] )
								{

									// ---- calc cross product
										x1 = mesh_calc_vertices[ro[i]].x;
										y1 = mesh_calc_vertices[ro[i]].y;
										z1 = mesh_calc_vertices[ro[i]].z;
										x3 = mesh_calc_vertices[ro[i+1]].x;
										y3 = mesh_calc_vertices[ro[i+1]].y;
										z3 = mesh_calc_vertices[ro[i+1]].z;
										v1x = x1 - x2;
										v1y = y1 - y2;
										v1z = z1 - z2;
										v2x = x3 - x2;
										v2y = y3 - y2;
										v2z = z3 - z2;
										xd = v1y*v2z - v1z*v2y;
										yd = v1z*v2x - v1x*v2z;
										zd = v1x*v2y - v1y*v2x;

									// ---- normalise
										len = (FLOAT)sqrt(xd*xd + yd*yd + zd*zd);
										mesh_calc_normals[tro].x += xd / len;
										mesh_calc_normals[tro].y += yd / len;
										mesh_calc_normals[tro].z += zd / len;

									// ---- incr cel counter
										cel_count++;

								}
						}

						// ---- average normal ( nb - check for error ? )
							fcc = (FLOAT)cel_count;
							mesh_calc_normals[tro].x /= fcc;
							mesh_calc_normals[tro].y /= fcc;
							mesh_calc_normals[tro].z /= fcc;

				}

		}


	// ---- lock and fill vertex buffer normals
		HRESULT		hr;
		VertexNT *p_vertex_data;
		hr = p_mesh->LockVertexBuffer(0, (VOID**)&p_vertex_data);
		DWORD v_index = 0;
		for (DWORD v = 0; v < mesh_vertices; v++)
		{
			p_vertex_data->nx = mesh_calc_normals[v_index].x;
			p_vertex_data->ny = mesh_calc_normals[v_index].y;
			p_vertex_data->nz = mesh_calc_normals[v_index].z;
			p_vertex_data++;
			v_index++;
		}
		hr = p_mesh->UnlockVertexBuffer();

}


// ---------- MeshCalculateCorrectNormals ----------
/*!
\brief calculate "correct" surface normals from mesh geometry
\author Gareth Edwards
\note To generate "correct" surface normals requires a more complex maths
	  solution than is found in the preceeding "good enough" function.

	  This function uses Vertex functions and operator overloads.

	  These are inimical to fast execution in DEBUG mode. Hence there are
	  two MeshCalculate[?]Normals functions.

*/
VOID Mesh3D::MeshCalculateCorrectNormals()
{

	// ---- buffer
		INT cols       = mesh_cel_columns + 1;
		INT rows       = mesh_cel_rows + 1;
		INT row_stride = cols;
		INT extent     = cols * rows;
		INT cel_count  = 0;


	// ---- tables
		INT  ro[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // row offset
		BOOL gs[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // grid stage


	//---- row column variables
		INT   last_row_offset, this_row_offset, next_row_offset;
		INT   lro, tro, nro;


	// ---- Vertex variables
		Vertex n2;
		Vertex v1d, v3d, vcp;
		Vertex v1, v2, v3;


	//---- for each cel...
		for (INT row = 0; row < rows; row++)
		{
				last_row_offset = (row - 1) * row_stride;
				this_row_offset =  row      * row_stride;
				next_row_offset = (row + 1) * row_stride;

				for (INT col = 0; col < cols; col++)
				{

					// ---- row offsets
						lro = last_row_offset + col;
						tro = this_row_offset + col;
						nro = next_row_offset + col;

					// ---- cel offset grid list
						ro[0] = lro-1; ro[1] = lro; ro[2] = lro+1;
						ro[7] = tro-1;              ro[3] = tro+1;
						ro[6] = nro-1; ro[5] = nro; ro[4] = nro+1;
						ro[8] = ro[0];

					// ---- set grid state (gs) flag 'within'
						for (INT i=0; i<9; i++)
						{
							gs[i] = ro[i] >= 0 && ro[i] < extent ? true : false;
						}

					// ---- set grid state (gs) flag if not first/last row & column
						if ( col == 0      ) gs[0] = gs[6] = gs[7] = gs[8] = 0;
						if ( col == cols-1 ) gs[2] = gs[3] = gs[4] = 0;
						if ( row == 0      ) gs[0] = gs[1] = gs[2] = 0;
						if ( row == rows-1 ) gs[4] = gs[5] = gs[6] = 0;

					// ---- zero normal
						n2.Zero();

					// ---- accumulate 'legal' grid normals
						cel_count = 0;
						v2 = mesh_calc_vertices[tro];
						for (INT i=0; i<8; i++)
						{
								if ( gs[i] && gs[i+1] )
								{

									// ---- vectors
										v1 = mesh_calc_vertices[ ro[i]  ];
										v3 = mesh_calc_vertices[ ro[i+1]];

									// ---- cross product
										v1d = v1 - v2;
										v3d = v3 - v2;
										v1d.Normalise();
										v3d.Normalise();
										vcp.CrossProduct(&v1d, &v3d);
										vcp.Normalise();

									// ---- add
										n2 += vcp;
										cel_count++;

								}
						}

					// ---- average normal
						n2.DivideBy((FLOAT)cel_count);


					// ---- store
						mesh_calc_normals[tro].x = n2.x;
						mesh_calc_normals[tro].y = n2.y;
						mesh_calc_normals[tro].z = n2.z;

				}

		}


	// ---- lock and fill vertex buffer normals
		HRESULT		hr;
		VertexNT *p_vertex_data;
		hr = p_mesh->LockVertexBuffer(0, (VOID**)&p_vertex_data);
		INT v_index = 0;
		for (DWORD v = 0; v < mesh_vertices; v++)
		{
			p_vertex_data->nx = mesh_calc_normals[v_index].x;
			p_vertex_data->ny = mesh_calc_normals[v_index].y;
			p_vertex_data->nz = mesh_calc_normals[v_index].z;
			p_vertex_data++;
			v_index++;
		}
		hr = p_mesh->UnlockVertexBuffer();

}


// ---------- MeshCalculateDisplayedNormals ----------
/*!
\brief calculate displayed surface normals from mesh
       surface vertices and mesh surface normal vertices
\author Gareth Edwards
*/
VOID Mesh3D::MeshCalculateDisplayedNormals()
{
	FLOAT scalar = 0.25f;
	INT v_index = 0;
	Vertex *vertex = mesh_normal_vertices;
	for (INT v = 0; v < mesh_num_normals; v++)
	{
		vertex->x = mesh_calc_vertices[v_index].x;
		vertex->y = mesh_calc_vertices[v_index].y;
		vertex->z = mesh_calc_vertices[v_index].z;
		vertex++;
		vertex->x = mesh_calc_vertices[v_index].x + (mesh_calc_normals[v_index].x * scalar);
		vertex->y = mesh_calc_vertices[v_index].y + (mesh_calc_normals[v_index].y * scalar);
		vertex->z = mesh_calc_vertices[v_index].z + (mesh_calc_normals[v_index].z * scalar);
		vertex++;
		v_index++;
	}
}


////////////////////////////////////////////////////////////////////////////////


// ---------- MESH VARIANTS ----------


// ---------- MeshSineWaveOriginal ----------
/*!
\brief update mesh with sine wave
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT - wave phase shift
\return bool (TRUE if ok)

\note requires surface normals to be re-calculated

*/
VOID Mesh3D::MeshSineWaveOriginal(
		FLOAT phase_shift
	)
{

	// ---- sine wave parameters
		FLOAT x_emitter = 5;
		FLOAT z_emitter = 5;
		FLOAT amplitude = 0.5f;
		FLOAT period    = 90;
		FLOAT vertShift = 0;

	// ---- calc for each vertex
		Vertex *p_calc_vertices = mesh_calc_vertices;
		int v_index = 0;
		for (DWORD v = 0; v < mesh_vertices; v++)
		{
			FLOAT xd = p_calc_vertices[v_index].x - x_emitter;
			FLOAT zd = p_calc_vertices[v_index].z - z_emitter;
			FLOAT d = (FLOAT)sqrt(xd*xd + zd * zd);
			p_calc_vertices[v_index].y = amplitude * (FLOAT)sin((period*d + phase_shift) * FLOAT(0.01745329252f)) + vertShift;
			v_index++;
		}

	// ---- copy into vertex buffer.
		VertexNT*	p_vertex_data;
		HRESULT hr = p_mesh->LockVertexBuffer(0, (VOID**) &p_vertex_data);
		v_index = 0;
		for (DWORD v=0; v < mesh_vertices; v++)
		{
			p_vertex_data->x = p_calc_vertices[v_index].x;
			p_vertex_data->y = p_calc_vertices[v_index].y;
			p_vertex_data->z = p_calc_vertices[v_index].z;
			p_vertex_data++;
			v_index++;
		}
		hr = p_mesh->UnlockVertexBuffer();

}


// ---------- MeshSineWaveMultiple ----------
/*!

\brief update mesh with multiple sine waves
\author Gareth Edwards
\param FLOAT - wave phase shift
\return bool (TRUE if ok)

\note requires surface normals to be re-calculated

*/
VOID Mesh3D::MeshSineWaveMultiple(
		FLOAT phase_shift
	)
{

	// ---- emitter
		struct emitter
		{
			FLOAT x         = 0;
			FLOAT z         = 0;
			FLOAT amplitude = 0.5f;
			FLOAT period    = 90;
			FLOAT speed     = 1;
			BOOL  status    = true;
		};

	// ---- emitters
		emitter emit[3] =
		{
			{  500,  500, 0.500f, 30, 2, true  },
			{  200, 1000, 0.500f, 60, 1, true  },
			{ -500, 1000, 0.500f, 90, 2, true  }
		};
		int num_emitter = sizeof(emit) / sizeof(emitter);

	// ---- calc for each vertex
		FLOAT xd=0, zd=0, d=0;
		Vertex *p_calc_vertices = mesh_calc_vertices;
		INT v_index = 0;
		for (DWORD v = 0; v < mesh_vertices; v++)
		{

			// ---- for each emitter
				p_calc_vertices[v_index].y = 0;
				for (INT e = 0; e < num_emitter; e++)
				{
					if ( emit[e].status )
					{
						xd = p_calc_vertices[v_index].x - emit[e].x;
						zd = p_calc_vertices[v_index].z - emit[e].z;
						d = (FLOAT)sqrt(xd*xd + zd * zd);
						p_calc_vertices[v_index].y +=
							emit[e].amplitude *
							(FLOAT)sin((emit[e].period*d + phase_shift * emit[e].speed) * FLOAT(0.01745329252f));
					}
				}
				v_index++;
		}


	// ---- copy into vertex buffer.
		VertexNT*	p_vertex_data;
		HRESULT hr = p_mesh->LockVertexBuffer(0, (VOID**) &p_vertex_data);
		v_index = 0;
		for (DWORD v=0; v < mesh_vertices; v++)
		{
			p_vertex_data->x = p_calc_vertices[v_index].x;
			p_vertex_data->y = p_calc_vertices[v_index].y;
			p_vertex_data->z = p_calc_vertices[v_index].z;
			p_vertex_data++;
			v_index++;
		}
		hr = p_mesh->UnlockVertexBuffer();

}


// ---------- MeshSineWaveMultipleNew ----------
/*!
\brief calculate multiple sine waves & derived normals
\author Gareth Edwards
\param FLOAT - wave phase shift

\note requires surface normals to be re-calculated

*/
VOID Mesh3D::MeshSineWaveMultipleNew(
		FLOAT phase_shift
	)
{

	// ---- waves
		mesh_sine_wave sine_wave_list[3] =
		{
			{ -500, -500, 0.225f,  90, phase_shift * 6,  1  },
			{ -500,    0, 0.015f, 200, phase_shift * 6,  1  },
			{ -500, -200, 0.010f, 600, phase_shift * 6,  0  }
		};


	// ---- init
		Vertex *p_calc_vertices = mesh_calc_vertices;
		Vertex *p_calc_normals = mesh_calc_normals;


	// ---- init mesh
		INT v_index = 0;
		for (DWORD v = 0; v < mesh_vertices; v++)
		{
			p_calc_vertices[v_index].y = 0;
			p_calc_normals[v_index].x = 0;
			p_calc_normals[v_index].y = 0;
			p_calc_normals[v_index].z = 0;
			v_index++;
		}

	// ---- calc mesh
		for (UINT i = 0; i < 3; i++)
		{
			if (sine_wave_list[i].status)
			{
				MeshSineWaveMultipleNewCalc(&sine_wave_list[i]);
			}
		}


	// ---- calc normal for each vertex
		v_index = 0;
		for (UINT v = 0; v < mesh_vertices; v++)
		{
			FLOAT len = (FLOAT)sqrt(
				p_calc_normals[v_index].x * p_calc_normals[v_index].x +
				p_calc_normals[v_index].y * p_calc_normals[v_index].y +
				p_calc_normals[v_index].z * p_calc_normals[v_index].z
			);
			p_calc_normals[v_index].x /= len;
			p_calc_normals[v_index].y /= len;
			p_calc_normals[v_index].z /= len;
			v_index++;
		}


	// ---- lock and fill vertex buffer
		HRESULT		hr;
		VertexNT *p_vertex_data;
		hr = p_mesh->LockVertexBuffer(0, (VOID**)&p_vertex_data);


	// ---- copy mesh
		v_index = 0;
		for (UINT v = 0; v < mesh_vertices; v++)
		{
			p_vertex_data->x  = p_calc_vertices[v_index].x;
			p_vertex_data->y  = p_calc_vertices[v_index].y;
			p_vertex_data->z  = p_calc_vertices[v_index].z;
			p_vertex_data->nx = p_calc_normals[v_index].x;
			p_vertex_data->ny = p_calc_normals[v_index].y;
			p_vertex_data->nz = p_calc_normals[v_index].z;
			p_vertex_data++;
			v_index++;
		}

	// ---- unlock
		hr = p_mesh->UnlockVertexBuffer();

}


// ---------- MeshSineWaveMultipleNewCalc ----------
/*!
\brief calculate a (one) sine wave & derived normals
\author Gareth Edwards

\note 1

	Sine curve is parameterized as (x,sin(x)).

	A unit-length tangent is (1,cos(x))/sqrt(1+(cos(x))^2) = (tx,ty).

	A unit-length normal is (ty,-tx).

\note 2

	We have a scaled sine function, t sin x.

	First differentiate it with respect to x so we get:

		D(t sin x) = t D(sin x) = t cos x

	From analytic geometry we know that slope of a tangent of
	a function (derivative) and it's direction angle a satisfy
	the relation: tan a = k.

	So we have: tan a = t cos x

	Solving for a: a = arctan(t cos x)

	Now we have the direction angle which unambigiously
	defines an unit length tangent vector s:

		s = i cos a + j sin a

	Knowing the tangent it's easy to get the normal:

		n = -i sin a + j cos a

	So the normal parametrizes as (-sin a, cos a) where

		a = arctan(t cos x)

*/
VOID Mesh3D::MeshSineWaveMultipleNewCalc(
		mesh_sine_wave *sine_wave
	)
{

	// ---- sine wave
		FLOAT sine_wave_x_emitter  = sine_wave->x_emitter;
		FLOAT sine_wave_z_emitter  = sine_wave->z_emitter;
		FLOAT sine_wave_amplitude  = sine_wave->amplitude;
		FLOAT sine_wave_period     = sine_wave->period;
		FLOAT sine_wave_phase_shit = sine_wave->phase_shift;
		FLOAT to_radian            = FLOAT(0.01745329252f);


	// ---- for each vertex
		Vertex *p_calc_vertices = mesh_calc_vertices;
		Vertex *p_calc_normals = mesh_calc_normals;
		DWORD v_index = 0;
		for (DWORD v=0; v< mesh_vertices; v++)
		{

			// ---- emitter distance x
				FLOAT xd = sine_wave_x_emitter - p_calc_vertices[v_index].x;
				FLOAT zd = sine_wave_z_emitter - p_calc_vertices[v_index].z;
				FLOAT x  = (FLOAT)sqrt(xd*xd+zd*zd);

			// ---- 2D:

			// ---- calculate y = f(x)
				p_calc_vertices[v_index].y += sine_wave_amplitude *
					(FLOAT)sin( (sine_wave_period*x + sine_wave_phase_shit) * to_radian );

			// ---- tangent angle = f'(x) 
				FLOAT a  = (FLOAT)atan(p_calc_vertices[v_index].y);

			// ---- tangent vector (tx, ty) to normal (ty, -tx)
				FLOAT tx = (FLOAT)sin(a); // ty
				FLOAT ty = (FLOAT)cos(a); // tx

			// ---- 3D:

			// ---- y rotate normal (tx, 0) to emitter
				FLOAT b  = (FLOAT)atan2(xd / x, zd / x) * to_radian;
				FLOAT rx = tx *  (FLOAT)cos(b);
				FLOAT rz = tx * -(FLOAT)sin(b);

			// ---- set normal
				p_calc_normals[v_index].x += rx;
				p_calc_normals[v_index].y += ty;
				p_calc_normals[v_index].z += rz;


			// ---- increment
				v_index++;

		}

}


// ---------- MeshGerstner ----------
/*!

\brief update mesh with multiple Gerstner waves
\author Gareth Edwards
\param FLOAT - wave phase shift
\return bool (TRUE if ok)

\note requires surface normals to be re-calculated

*/

VOID Mesh3D::MeshGerstner(
		FLOAT phase_shift
	)
{

	// ---- init direction
		GerstnerVec2 dir_list[3] =
		{
			{  0.5f,  0.5f },
			{  0.5f,  0.0f },
			{  0.5f,  0.2f }
		};


	// ---- init direction, wave amplitude & wave length
		GerstnerWave wave_list[3] =
			{
				{ dir_list[0], 0.225f, 1.0000f },
				{ dir_list[1], 0.015f, 0.6666f },
				{ dir_list[2], 0.010f, 0.6666f }
				//{ dir_list[0], 0.225f, 1.00f },
				//{ dir_list[1], 0.015f, 0.50f },
				//{ dir_list[2], 0.010f, 0.75f }
			};


	// ---- init cb_list wave steepness, speed & waves
		GerstnerWaves waves;
		waves.num_waves = sizeof(wave_list) / sizeof(GerstnerWave);
		waves.wave_speed = phase_shift/20;
		waves.wave_steepness = 2.0f;


	// ---- lock vertex buffer.
		Vertex *p_calc_vertices = mesh_calc_vertices;


	// ---- re-init regular flat mesh
		FLOAT x_width = mesh_param[2] - mesh_param[0];
		FLOAT z_depth = mesh_param[3] - mesh_param[1];
		FLOAT x_min   = mesh_param[0];
		FLOAT z_min   = mesh_param[1];
		DWORD v_index = 0;
		for (UINT row=0; row<= mesh_cel_rows; row++)
		{
				FLOAT rmu = FLOAT(row)/ FLOAT(mesh_cel_rows);
				FLOAT z   = rmu * z_depth + z_min;
				for (UINT col=0; col<= mesh_cel_columns; col++)
				{
						FLOAT cmu = FLOAT(col)/ FLOAT(mesh_cel_columns);
						p_calc_vertices[v_index].x  = (1-cmu) * x_width + x_min;
						p_calc_vertices[v_index].y  = 0.0;
						p_calc_vertices[v_index].z  = z;
						v_index++;
				}
		}


	// ---- re-calculate
		GerstnerVec3 position = { 0, 0, 0 };
		GerstnerVec3 displaced = { 0, 0, 0 };
		v_index = 0;
		for (UINT v=0; v< mesh_vertices; v++)
		{
			position  = GerstnerVec3(p_calc_vertices[v_index].x, p_calc_vertices[v_index].y, p_calc_vertices[v_index].z);
			displaced = CalcGerstnerWaveOffset(waves, wave_list, position, 1);
			p_calc_vertices[v_index].x = displaced.x;
			p_calc_vertices[v_index].y = displaced.y;
			p_calc_vertices[v_index].z = displaced.z;
			v_index++;
		}


	// ---- copy into vertex buffer.
		VertexNT*	p_vertex_data;
		HRESULT hr = p_mesh->LockVertexBuffer(0, (VOID**) &p_vertex_data);
		v_index = 0;
		for (UINT v=0; v < mesh_vertices; v++)
		{
			p_vertex_data->x = p_calc_vertices[v_index].x;
			p_vertex_data->y = p_calc_vertices[v_index].y;
			p_vertex_data->z = p_calc_vertices[v_index].z;
			p_vertex_data++;
			v_index++;
		}
		hr = p_mesh->UnlockVertexBuffer();

}


Mesh3D::GerstnerVec3 Mesh3D::CalcGerstnerWaveOffset(
		GerstnerWaves &waves,
		GerstnerWave  wave_list[],
		GerstnerVec3 location,
		FLOAT time
	)
{

	// ---- local
		UINT num_waves = waves.num_waves;
		FLOAT fnw = (FLOAT)num_waves;

	// ---- displace
		GerstnerVec3 sum = GerstnerVec3(location.x, 0, location.z);
		for (UINT i = 0; i < num_waves; i++)
		{
			const GerstnerWave &wave = wave_list[i];
			FLOAT wi = 2 / wave.wave_length;
			FLOAT rad = ( (FLOAT)Mesh3D::dot(
					wave.dir,
						GerstnerVec2(location.x, location.z) ) +
							time * waves.wave_speed) * wi;
			FLOAT sine = (FLOAT)sin(rad);
			FLOAT cosine = (FLOAT)cos(rad);
			sum.y += sine * wave.amplitude;
			sum.x += wave.dir.x * cosine * waves.wave_steepness / ( wi * (FLOAT)fnw);
			sum.z += wave.dir.y * cosine * waves.wave_steepness / ( wi * (FLOAT)fnw);
		}

	return sum;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- APPLICATION STUFF --------


// ---------- Display_Text --------
/*!
\brief  display instructions and variables text onscreen
\author Gareth Edwards
*/
VOID Mesh3D::Display_Text()
{

	// ---- font
		D3DCOLOR fontColor = D3DCOLOR_XRGB(255,255,255);

	// ---- position text
		RECT rct;
		rct.left   = 20;
		rct.right  = 700;
		rct.top    = 20;
		rct.bottom = rct.top+20;

	// ---- local string variable for text
		std::string text;

	// ---- local char variable for variables and rules
		font->DrawText(NULL, (LPCSTR)fw_win_create.GetName().c_str(), -1, &rct, 0, fontColor);
		rct.top += 40; rct.bottom += 40;

	// ---- title
		switch (object_displayed)
		{
			case CUBE_OBJECT:
				text = "Cubic VOID formed from six pyramids";
				break;
			case TEAPOT_OBJECT:
				text = "Recursively located, rotated & scaled teapots";
				break;
			case MESH_OBJECT:
				{
					text = "Surface generated from ";
					switch (mesh_type)
					{
						case SINGLE_SINE:
							text += "a single Sine wave";
							break;
						case MULTIPLE_SINE:
							text += "three Sine waves";
							break;
						case PARAMETRIC_SINE:
							text += "three Sine waves with parametric normals";
							break;
						case GERSTNER_WAVE:
							text += "three Gerstner wave functions";
							break;
					}
				}
		}
		font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);


	// ---- fps
		rct.top = fw_win_create.GetHeight() - 80;
		rct.bottom = rct.top + 20;
		font->DrawText(NULL, (LPCSTR)fw_win_engine.GetFpsReport().c_str(), -1, &rct, 0, fontColor);

}


// ---------- Display_SetDefaults ----------
/*!
\brief set default
\author Gareth Edwards
*/
VOID Mesh3D::Display_SetDefaults()
{

	// ---- system variables
	x_rotate = 0;
	y_rotate = -30;
	z_translate = 25;
	y_turntable = 0;

}


// ---------- UpdateIfKeyPressed ----------
/*!
\brie update app if key async pressed
\author Gareth Edwards
*/
VOID Mesh3D::Display_AsyncKeyState()
{

	// ---- store
		key_last_pressed = key_just_pressed;


	// ---- key pressed ?
		if ( GetAsyncKeyState(VK_LEFT)  & 0x8000f )
			key_just_pressed = VK_LEFT;
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			key_just_pressed = VK_RIGHT;
		else if ( GetAsyncKeyState('1') & 0x8000f )
			key_just_pressed = 1;
		else if ( GetAsyncKeyState('2') & 0x8000f )
			key_just_pressed = 2;
		else if ( GetAsyncKeyState('3') & 0x8000f )
			key_just_pressed = 3;
		else if ( GetAsyncKeyState('4') & 0x8000f )
			key_just_pressed = 4;
		else if ( GetAsyncKeyState('5') & 0x8000f )
			key_just_pressed = 5;
		else if ( GetAsyncKeyState('6') & 0x8000f )
			key_just_pressed = 6;
		else if ( GetAsyncKeyState('7') & 0x8000f )
			key_just_pressed = 7;
		else if ( GetAsyncKeyState('8') & 0x8000f )
			key_just_pressed = 8;
		else if ( GetAsyncKeyState('X') & 0x8000f )
			key_just_pressed = 'X';
		else
			key_just_pressed = 0;


	// ---- not pressed ?
		if ( key_just_pressed == 0 )
			return;


	// ---- select action (if any)
		switch ( key_just_pressed )
		{
			case 1:
				object_displayed = CUBE_OBJECT;
				break;
			case 2:
				object_displayed = TEAPOT_OBJECT;
				break;
			case 3:
				object_displayed = MESH_OBJECT;
				mesh_type = SINGLE_SINE;
				break;
			case 4:
				object_displayed = MESH_OBJECT;
				mesh_type = MULTIPLE_SINE;
				break;
			case 5:
				object_displayed = MESH_OBJECT;
				mesh_type = PARAMETRIC_SINE;
				break;
			case 6:
				object_displayed = MESH_OBJECT;
				mesh_type = GERSTNER_WAVE;
				break;
			case 7:
				mesh_display_solid = mesh_display_solid ? FALSE : TRUE;
				Sleep(250);
				break;
			case 8:
				mesh_display_normals = mesh_display_normals ? FALSE : TRUE;
				Sleep(250);
				break;
			case 'X':
				Display_SetDefaults();
				break;
			default:
				break;
		}


	// ---- report
		#if DEBUG
		CHAR ods[128];
		sprintf_s(ods, 128, " +-> key just pressed is %d \n", key_just_pressed);
		OutputDebugString(ods);
		#endif
}


////////////////////////////////////////////////////////////////////////////////