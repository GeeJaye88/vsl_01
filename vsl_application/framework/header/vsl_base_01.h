////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_base_01.h ----------
/*!
\file vsl_base_01.h
\brief interface for the Base_01 class
\author Gareth Edwards
*/

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- system
	#include "../../vsl_system/header/vsl_win_structs.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element_engine.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky_config.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky.h"
	#include "../../vsl_library/header/vsl_gfx_command.h"
	#include "../../vsl_library/header/vsl_gfx_d3dx.h"
	#include "../../vsl_library/header/vsl_gfx_log.h"

////////////////////////////////////////////////////////////////////////////////


namespace vsl_application
{

	// ---------- Base_01 class interface ----------
		class Base_01
		{

			public:

			// ---- cdtor
				Base_01(VOID);
				virtual ~Base_01();

			// ---------- FRAMEWORK ----------

			// ---- required by framework:
				virtual HRESULT Fw_Setup(VOID);
				HRESULT Fw_SetupDX(LPDIRECT3DDEVICE9);
				HRESULT Fw_Display(LPDIRECT3DDEVICE9);
				HRESULT Fw_CleanupDX(LPDIRECT3DDEVICE9);
				HRESULT Fw_Cleanup(VOID);

			// ---- required by framework: get wnd structs
				VOID Fw_Get_Win_Create(vsl_system::Win_Create **fw_win_create);
				VOID Fw_Get_Win_Engine(vsl_system::Win_Engine **fw_win_engine);

			// ---- required by framework to set Gfx_Command parameters
				VOID Fw_Set_MouseLeftButtonDownMove(INT x, INT y);
				VOID Fw_Set_MouseWheelClick(INT mouse_wheel_click_z);
				VOID Fw_Set_Keydown(WPARAM param);

			// ---------- APPLICATION ----------

			// ---- setup graphics
				virtual HRESULT Gfx_Setup_Project(VOID);
				virtual HRESULT Gfx_Setup_Configurations(VOID);
				virtual HRESULT Gfx_Setup_Coordinates(VOID);
				virtual HRESULT Gfx_Setup_Components(VOID);

			// ---- handle elements that have been bookmarked
				virtual HRESULT Gfx_Element_Bookmarks(VOID);

				VOID Gfx_Read_Project_SDL(VOID);

			// ---------- PRIVATE IMPLEMENTATION ----------

			// ---- get Win objects
				vsl_system::Win_Create *Get_Win_Create(VOID);
				vsl_system::Win_Engine *Get_Win_Engine(VOID);

			// ---- get gfx objects
				vsl_library::Gfx_Command *GetCmd(VOID);
				vsl_library::Gfx_D3dx    *GetD3D(VOID);
				vsl_library::Gfx_Log     *GetLog(VOID);

				vsl_library::Gfx_Element_Engine *GetGEE(VOID);
				vsl_library::Gfx_Kandinsky      *GetKandinsky(VOID);

			// ---------- UPDATE ----------

			// ---- update state and text overlay
				VOID Update_Gfx_Command_Param(VOID);
				VOID Update_If_AsyncKey_Pressed(VOID);
				VOID Update_On_Screen_Text(VOID);

		private:

			// ---- private implementation
				class Pimpl_Base_01;
				std::unique_ptr<Pimpl_Base_01> pimpl_base_v01;

		public:

			// ---- get private implementation
				//
				// usage: std::unique_ptr<Pimpl_Base_01> *pimp = GetPimp(VOID);
				//
				std::unique_ptr<Pimpl_Base_01> *GetPimp(VOID) { return &pimpl_base_v01; }

		};

}


////////////////////////////////////////////////////////////////////////////////



