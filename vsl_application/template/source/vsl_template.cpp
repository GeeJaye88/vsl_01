////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_template.cpp ----------
/*!
\file vsl_template.cpp
\brief implementation of the Template class
\author Gareth Edwards
*/


// ---------- include Mesh3D header ----------
#include "../header/vsl_template.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_application;


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
Template::Template()
{

	// ---- system windows config
		fw_win_create.SetName("VS Lite Template");
		fw_win_create.SetDesktop(TRUE);

	// ---- default
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
Template::~Template()
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
HRESULT Template::Fw_Setup()
{

	// ---- lambda
		auto init_material = [](D3DMATERIAL9 *d)
		{
			D3DCOLORVALUE zero = { 0, 0, 0, 0 };
			D3DCOLORVALUE full = { 1, 1, 1, 1 };
			d->Ambient  = zero;
			d->Emissive = zero;
			d->Diffuse  = full;
			d->Specular = full;
			d->Power = 40;
		};


    // ---- cube material
		ZeroMemory( &cube_side_material, sizeof(D3DMATERIAL9) );
		init_material(&cube_side_material);


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
		D3DCOLORVALUE diffuse = { 0, 0.2f, 0.5f, 0 };
		back_light.Diffuse = diffuse;
		D3DCOLORVALUE specular = { 0.3f, 0.3f, 0.3f, 0 };
		back_light.Specular = specular;

	return SUCCESS_OK;
}


// ---------- Fw_SetupDX ----------
/*!
\brief system setup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Template::Fw_SetupDX(LPDIRECT3DDEVICE9 device)
{

	// ---- turn lighting ON
		device->SetRenderState( D3DRS_LIGHTING , TRUE);


	// ---- ambient light
		device->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE( 0.1, 0.1, 0.1, 1.0));


	// ---- sun light
		device->SetLight( 0, &sun_light );
		device->LightEnable( 0, TRUE );


	// ---- back light
		device->SetLight( 1, &back_light );
		device->LightEnable( 1, TRUE );


    // ---- create a pyramidal or single sided cube
		//#define SIMPLE_CUBE
		#if defined(SIMPLE_CUBE)
			Cube_SideSetup(device, 4, 1);
		#else
			Cube_PyramidSideSetup(device, 4, 1);
		#endif


	// ---- create an instance of the font object for function DisplayText
		D3DXCreateFont(
				device,
				20, 0, FW_NORMAL, 0,
				FALSE,
				DEFAULT_CHARSET,
				OUT_DEFAULT_PRECIS,
				DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_DONTCARE,
				TEXT("Arial"),
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
HRESULT Template::Fw_Display(LPDIRECT3DDEVICE9 device)
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


	// ---- display cube
		{

			// ---- set render states
				device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
				device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.3f, 0.3f, 0.3f, 1.0));


			// ---- sun light
				sun_light.Direction = D3DXVECTOR3(-1, -1, 1);
				device->SetLight(0, &sun_light);


			// ---- back light
				back_light.Type = D3DLIGHT_DIRECTIONAL;
				back_light.Direction = D3DXVECTOR3(1, 0, 0);
				device->LightEnable(1, TRUE);
				device->SetLight(1, &back_light);


			// ---- locate & turntable
				D3DXMATRIX mat_spin;
				D3DXMatrixTranslation(&matrix_translation, 0, 0, z_translate);
				D3DXMatrixRotationY(&mat_spin, D3DXToRadian( (FLOAT)y_turntable) );
				matrix_world = matrix_rotation * matrix_translation;
				matrix_world = mat_spin        * matrix_world;


			// ---- display cube
				Cube_Display(device, matrix_world, z_translate);

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
HRESULT Template::Fw_CleanupDX (LPDIRECT3DDEVICE9 device)
{

    // ---- invalidate the vertex buffer object
		if ( p_cube_side_vertex_buffer != NULL )
		{
			int ref_count = p_cube_side_vertex_buffer->Release();

			if (ref_count > 0 )
			{
				static char std_error[256];
				sprintf_s ( std_error, 256,
					"The vertex buffer object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					ref_count);
				MessageBox( NULL, std_error, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			p_cube_side_vertex_buffer = NULL;
		}


	// ---- invalidate the font object
		if ( font != NULL )
		{
			int ref_count = font->Release();

			if (ref_count > 0 )
			{
				static char strError[256];
				sprintf_s ( strError, 256,
					"The font object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					ref_count);
				MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			font = NULL;

		}

	return SUCCESS_OK;
}


// ---------- Cleanup ----------
/*!
\brief system cleanup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Template::Fw_Cleanup()
{
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
VOID Template::Fw_Get_Win_Create(vsl_system::Win_Create **wc)
	{ *wc = &fw_win_create; }


// ---------- Fw_Get_Win_Engine ----------
/*!
\brief system get pointer to wnd engine struct
\author Gareth Edwards
\param Win_Engine **wc
*/
VOID Template::Fw_Get_Win_Engine(vsl_system::Win_Engine **wc)
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
VOID Template::Fw_Set_MouseLeftButtonDownMove(INT x, INT y)
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
VOID Template::Fw_Set_MouseWheelClick(INT d)
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
VOID Template::Fw_Set_Keydown(WPARAM param)
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- cube specific methods ----------


// ---------- Cube_SideSetup ----------
/*!
\brief setup a single cube side
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\param FLOAT* - size
\param FLOAT* - move
*/
VOID Template::Cube_SideSetup(
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
VOID Template::Cube_PyramidSideSetup(
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
VOID Template::Cube_Display(
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
				device->SetTexture(0, NULL);
				device->SetStreamSource(0, p_cube_side_vertex_buffer, 0, sizeof(VertexNDT));
				device->SetFVF(VertexNDT::FVF_Flags);
				device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, cube_side_num_primitives);
		}

}

////////////////////////////////////////////////////////////////////////////////




// ---------- Display_Text --------
/*!
\brief display instructions and variables
\author Gareth Edwards
*/
VOID Template::Display_Text()
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
		//rct.top += 40; rct.bottom += 40;

	// ---- fps
		rct.top = fw_win_create.GetHeight() - 80;
		rct.bottom = rct.top + 20;
		font->DrawText(NULL, (LPCSTR)fw_win_engine.GetFpsReport().c_str(), -1, &rct, 0, fontColor);

}


// ---------- Display_SetDefault ----------
/*!
\brief set display defaults
\author Gareth Edwards
*/
VOID Template::Display_SetDefaults()
{

	// ---- system variables
		x_rotate = 0;
		y_rotate = -30;
		z_translate = 25;
		y_turntable = 0;

}


// ---------- Display_AsyncKeyState ----------
/*!
\brie handle display update if key async pressed
\author Gareth Edwards
*/
VOID Template::Display_AsyncKeyState()
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
		if (key_just_pressed == 0 )
			return;


	// ---- select action (if any)
		switch (key_just_pressed )
		{
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				Sleep(250);
				break;
			case 8:
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