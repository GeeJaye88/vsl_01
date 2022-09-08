////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_template.h ----------
/*!
\file vsl_template.h
\brief interface for the Template class
\author Gareth Edwards
*/

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- system include
	#include "../../vsl_system/header/vsl_include.h"
	#include "../../vsl_system/header/vsl_win_structs.h"

// ---- application include
	#include "../../vsl_application/shared/header/vsl_fvf_vertex_structs.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- Mesh3D class interface ----------

namespace vsl_application
{

	class Template
	{

	public:

		// ---- cdtor
			Template();
			virtual ~Template();


		 // ---- system methods
			HRESULT Fw_Setup();
			HRESULT Fw_SetupDX(LPDIRECT3DDEVICE9);
			HRESULT Fw_Display(LPDIRECT3DDEVICE9);
			HRESULT Fw_CleanupDX(LPDIRECT3DDEVICE9);
			HRESULT Fw_Cleanup();
		  
		// ---- system struct methods
			VOID Fw_Get_Win_Create(vsl_system::Win_Create **wc);
			VOID Fw_Get_Win_Engine(vsl_system::Win_Engine **wc);

		// ---- system notify event methods
			VOID Fw_Set_MouseLeftButtonDownMove(INT, INT);
			VOID Fw_Set_MouseWheelClick(INT);
			VOID Fw_Set_Keydown(WPARAM param);


		private:

		// ---- system structs
			vsl_system::Win_Create fw_win_create;
			vsl_system::Win_Engine fw_win_engine;


		public:

		// ---- application cube methods
			VOID Cube_SideSetup(LPDIRECT3DDEVICE9, FLOAT, FLOAT);
			VOID Cube_PyramidSideSetup(LPDIRECT3DDEVICE9, FLOAT, FLOAT);
			VOID Cube_Display(LPDIRECT3DDEVICE9, D3DXMATRIX, FLOAT);

		// ---- application display methods
			VOID  Display_Text();
			VOID  Display_SetDefaults();
			VOID  Display_AsyncKeyState();


		private:

		// ---- application properties

		// ---- gui
			FLOAT  x_rotate = 0;
			FLOAT  y_rotate = 0;
			FLOAT  z_translate = 0;
			DWORD  key_just_pressed = 0;
			DWORD  key_last_pressed = 0;
			DOUBLE y_turntable;

		// ---- quad
			LPDIRECT3DVERTEXBUFFER9	p_cube_side_vertex_buffer;
			D3DMATERIAL9			cube_side_material;
			INT                     cube_side_num_primitives;

		// ---- scene
			D3DLIGHT9 sun_light;
			D3DLIGHT9 back_light;

		// ---- display text font
			LPD3DXFONT font;

	};

}

////////////////////////////////////////////////////////////////////////////////



