////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_d3dx.cpp ----------
/*!
\file vsl_gfx_d3dx.cpp
\brief Implementation of the Gfx_D3dx class
\author Gareth Edwards 
*/


#include "../header/vsl_gfx_d3dx.h"


static vsl_library::Gfx_Command gfx_command;


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


// ---------- Private Implementation of Gfx_D3dx::Pimpl_Gfx_D3dx class ----------

class Gfx_D3dx::Pimpl_Gfx_D3dx
{

public:

	// ---- cdtor
		Pimpl_Gfx_D3dx() { ; }
		~Pimpl_Gfx_D3dx() { ; }

	// ---- system
		vsl_system::Win_Create   *fw_win_create;
		vsl_library::Gfx_Command *gfx_command;

	// ---- scene
		D3DLIGHT9  sun_light;
		D3DLIGHT9  back_light;
		LPD3DXFONT font;

	// ---- lookat: convert mouse to lookat view rotation and look distance from at 
		FLOAT Get_LookAt_Distance()
			{ return lookat_distance; }
		VOID  Set_LookAt_Distance(FLOAT lookat_distance)
			{ this->lookat_distance = lookat_distance; }

		FLOAT Get_LookAt_XAxisRotation()
			{ return lookat_x_axis_rotation; }
		FLOAT Get_LookAt_YAxisRotation()
			{ return lookat_y_axis_rotation; }
		VOID  Set_LookAt_XAxisRotation(FLOAT x_axis_rotation)
			{ lookat_x_axis_rotation = x_axis_rotation; }
		VOID  Set_LookAt_YAxisRotation(FLOAT y_axis_rotation)
			{ lookat_y_axis_rotation = y_axis_rotation; }

	private:

		// ---- lookat: convert mouse to lookat view rotation and look distance from at 
		//
		//  note: convert - 
		//      gfx_command.mouse_wheel_click_z to look location distance from at location  
		//      gfx_command.GetMouseLeftButtonDownMoveX to lookat vector Y axis rotation  
		//      gfx_command.GetMouseLeftButtonDownMoveY to lookat vector X axis rotation 
		//
		FLOAT lookat_distance  = 0;
		FLOAT lookat_x_axis_rotation = 0;
		FLOAT lookat_y_axis_rotation = 0;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- cdtor ----------

Gfx_D3dx::Gfx_D3dx() : pimpl_gfx_d3dx(new Pimpl_Gfx_D3dx)
{
	;
}

Gfx_D3dx::~Gfx_D3dx()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- set framework wnd struct ----------


// ---------- Set_Win_Create ----------
/*!
\brief framework set pointer to wnd create struct
\author Gareth Edwards
\param Win_Create *wc
*/
VOID Gfx_D3dx::Set_Win_Create(vsl_system::Win_Create *win_cr8)
{
	pimpl_gfx_d3dx->fw_win_create = win_cr8;
}

// ---------- Set_Gfx_Command ----------
/*!
\brief framework set pointer to wnd process struct
\author Gareth Edwards
\param Gfx_Command *gfx_com
*/
VOID Gfx_D3dx::Set_Gfx_Command(vsl_library::Gfx_Command *gfx_com)
{
	pimpl_gfx_d3dx->gfx_command = gfx_com;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Setup ----------
/*!
\brief setup scene
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
\note
*/
HRESULT Gfx_D3dx::Setup(VOID)
{
 
	// ---- lambda
	auto init_light = [](D3DLIGHT9 *l)
	{
		l->Type = D3DLIGHT_DIRECTIONAL;
		l->Direction = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		D3DCOLORVALUE full = { 1, 1, 1, 1 };
		l->Diffuse = full;
		l->Specular = full;
	};

	// ---- directional sun light
	init_light(&pimpl_gfx_d3dx->sun_light);

	// ---- directional back light
	init_light(&pimpl_gfx_d3dx->back_light);

	// ---- coloured back light
	D3DCOLORVALUE diffuse = { 0, 0.5f, 0.5f, 0 };
	pimpl_gfx_d3dx->back_light.Diffuse = diffuse;
	D3DCOLORVALUE specular = { 0.3f, 0.3f, 0.3f, 0 };
	pimpl_gfx_d3dx->back_light.Specular = specular;

	return SUCCESS_OK;
}


// ---------- SetupDX ----------
/*!
\brief setup scene dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::SetupDX(LPDIRECT3DDEVICE9 device)
{

	// ---- turn lighting ON
		device->SetRenderState( D3DRS_LIGHTING , TRUE);

	// ---- ambient light
		device->SetRenderState( D3DRS_AMBIENT,D3DCOLOR_COLORVALUE( 0.1, 0.1, 0.1, 1.0));

	// ---- sun light
		device->SetLight( 0, &pimpl_gfx_d3dx->sun_light );
		device->LightEnable( 0, TRUE);

	// ---- back light
		device->SetLight( 1, &pimpl_gfx_d3dx->back_light );
		device->LightEnable( 1, TRUE );

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
				&pimpl_gfx_d3dx->font
			);

	return SUCCESS_OK;
}


// ---------- SetupViewport ----------
/*!
\brief setup the viewport parameters
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::SetupViewport(LPDIRECT3DDEVICE9 device)
{

	// ---- get client adjusted viewrect
		UINT width = pimpl_gfx_d3dx->fw_win_create->GetWidth();
		UINT height = pimpl_gfx_d3dx->fw_win_create->GetHeight();


	// ---- viewport
		UINT left   = 0;
		UINT top    = 0;
		UINT right  = width;
		UINT bottom = height;

	// ---- set viewport
		D3DVIEWPORT9 fvp;
		fvp.X      = (DWORD)(left   + 0.5);
		fvp.Y      = (DWORD)(top    + 0.5);
		fvp.Width  = (DWORD)(right  - left + 0.5);
		fvp.Height = (DWORD)(bottom - top  + 0.5);
		fvp.MinZ   =  0;
		fvp.MaxZ   =  1;
		device->SetViewport(&fvp);

	return SUCCESS_OK;
}


// ---------- SetupProjection ----------
/*!
\brief setup a left-handed perspective projection matrix based on a vertical field of view
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::SetupProjection(LPDIRECT3DDEVICE9 device)
{

	// ---- local
		HRESULT hr;
		FLOAT fovy = D3DXToRadian(40.0f);
		FLOAT aspect = (FLOAT)pimpl_gfx_d3dx->fw_win_create->GetWidth() /
			(FLOAT)pimpl_gfx_d3dx->fw_win_create->GetHeight();
		FLOAT zn = 0.1f;
		FLOAT zf = 100.0f;

	// ---- projection matrix
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH(
				&matProj,
				fovy,   // vertical field of view in radians
				aspect, // aspect ration, defined as view width divided by height
				zn,     // z value of near view-plane
				zf      // z value of far view-plane
			);
		hr = device->SetTransform(D3DTS_PROJECTION, &matProj);

	return SUCCESS_OK;
}


// ---------- SetupLookAtLH ----------
/*!
\brief setup lookat axis aligned colinear with negative (left handed) z axis
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::SetupLookAtLH(LPDIRECT3DDEVICE9 device)
{

	// ---- Setup_Lookat_Axis
		/*
		    note: default is lookat axis aligned colinear with negative (left handed) z axis
		*/
		auto Setup_Lookat_Axis = [](FLOAT xang, FLOAT yang, FLOAT dist, D3DXVECTOR3 v[])
		{
			// ---- init
				FLOAT rad = 3.1415296499f / 180.0f;
				FLOAT xrad = xang * rad;
				FLOAT yrad = yang * rad;
				FLOAT y=0, z=0;

			// ---- rotate each around x then y axis 
			for (UINT i = 0; i < 3; i++)
			{
				// ---- init
					switch (i)
					{
						/* eye */ case 0: y = 0; z = dist; break;
						/* at  */ case 1: y = 0; z = dist + 1; break;
						/* up  */ case 2: y = 1; z = dist + 1; break;
					}

				// rotate about x axis
					FLOAT yy = y * cos(xrad) - z * sin(xrad);
					FLOAT zr = y * sin(xrad) + z * cos(xrad);

				// rotate about y axis (no x)
					v[i].x = zr * sin(yrad);// + x * cos(yrad);
					v[i].y = yy;
					v[i].z = zr * cos(yrad);// - x * sin(yrad);
			}
		};

	// ---- update camera "view" by accumulated mouse move and wheel click
		vsl_library::Gfx_Command *gfx_com = pimpl_gfx_d3dx->gfx_command;
		pimpl_gfx_d3dx->Set_LookAt_Distance(-gfx_com->GetMouseWheelClick());
		pimpl_gfx_d3dx->Set_LookAt_XAxisRotation(gfx_com->GetMouseLeftButtonDownMoveX()/2);
		pimpl_gfx_d3dx->Set_LookAt_YAxisRotation(gfx_com->GetMouseLeftButtonDownMoveY()/2);

	// ---- rotate negative camera "view" to match user experience
		D3DXVECTOR3 v[3];
		Setup_Lookat_Axis(
				-pimpl_gfx_d3dx->Get_LookAt_YAxisRotation(),
				-pimpl_gfx_d3dx->Get_LookAt_XAxisRotation(),
				-pimpl_gfx_d3dx->Get_LookAt_Distance(),
				v
			);

	// ---- init eye, at & up, then set view transform
		D3DXMATRIX  matrix_view;
		D3DXVECTOR3 eye(v[0].x, v[0].y, v[0].z); // defines the eye point
		D3DXVECTOR3 at( v[1].x, v[1].y, v[1].z); // defines the look-at target.
		D3DXVECTOR3 up( v[2].x, v[2].y, v[2].z); // defines the world's "up"
		D3DXMatrixLookAtLH(&matrix_view, &eye, &at, &up);
		device->SetTransform(D3DTS_VIEW, &matrix_view);

	return SUCCESS_OK;
}


// ---------- Display ----------
/*!
\brief display
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::Display(LPDIRECT3DDEVICE9 device)
{

	// ---- ambient light
		device->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_COLORVALUE(0.3f, 0.3f, 0.3f, 1.0));

	// ---- sun light
		pimpl_gfx_d3dx->sun_light.Direction = D3DXVECTOR3(-1, 0, 1);
		device->SetLight(0, &pimpl_gfx_d3dx->sun_light);

	// ---- back light
		pimpl_gfx_d3dx->back_light.Direction = D3DXVECTOR3(1, 1, 1);
		device->SetLight(1, &pimpl_gfx_d3dx->back_light);

	return SUCCESS_OK;
}


// ---------- CleanupDX ----------
/*!
\brief cleanup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::CleanupDX (LPDIRECT3DDEVICE9 device)
{

	// ---- invalidate the font object
		if (pimpl_gfx_d3dx->font != NULL )
		{
			int ref_count = pimpl_gfx_d3dx->font->Release();

			if (ref_count > 0 )
			{
				static CHAR strError[256];
				sprintf_s ( strError, 256,
					"The font object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					ref_count);
				MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			pimpl_gfx_d3dx->font = NULL;

		}

	return SUCCESS_OK;
}


// ---------- Cleanup ----------
/*!
\brief cleanup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::Cleanup(VOID)
{
	
	return SUCCESS_OK;
}


// ---------- DisplayText ----------
/*!
\brief display text
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_D3dx::DisplayText(
	std::string& text,
	D3DCOLOR& colour,
	RECT& rect)
{
	pimpl_gfx_d3dx->font->DrawText(
			NULL,
			(LPCSTR)text.c_str(),
			-1,
			&rect,
			0,
			colour
		);

	return SUCCESS_OK;
}
