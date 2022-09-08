////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_object3d_v01.h ----------
/*!
\file vsl_object3d_v01.h
\brief interface for the Object3d_V01 class
\author Gareth Edwards
*/

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- system
	#include "../../vsl_system/header/vsl_interface.h"

// ---- library
	#include "../../vsl_library/header/vsl_Gfx_Element_Engine.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky_config.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_application
{

	// ---------- Object3d_V01 class interface ----------
		class Object3d_V01
		{

			public:

			// ---- cdtor
				Object3d_V01(VOID);
				virtual ~Object3d_V01();

			// ---------- FRAMEWORK ----------

			// ---- framework
				virtual HRESULT FW_Report(VOID);
				virtual HRESULT FW_Setup(VOID);
				HRESULT FW_SetupDX(LPDIRECT3DDEVICE9);
				HRESULT FW_Display(LPDIRECT3DDEVICE9);
				HRESULT FW_CleanupDX(LPDIRECT3DDEVICE9);
				HRESULT FW_Cleanup(VOID);

			// ---- framework get wnd struct
				VOID FW_GetWndCreate(vsl_system::WndCreate **wc);
				VOID FW_GetWndContext(vsl_system::WndContext **wc);

			// ---- framework notify events
				VOID FW_MouseLeftButtonDownMove(INT, INT);
				VOID FW_MouseWheelClick(INT);
				VOID FW_Keydown(WPARAM param);
				VOID FW_Update(VOID);


			// ---------- GFX PROJECT ----------

			// ---- gfx project display setup
				HRESULT Gfx_Project_Setup(VOID);

			// ---- gfx project pre display transform
				static HRESULT Gfx_Project_Transform(
					vsl_library::Gfx_Element *element,
					LPD3DXMATRIXSTACK matrix_stack);


			// ---------- SCENE ----------

			// ---- d3d scene framework
				HRESULT D3D_Scene_Setup(VOID);
				HRESULT D3D_Scene_SetupDX(LPDIRECT3DDEVICE9);
				HRESULT D3D_Scene_Projection(LPDIRECT3DDEVICE9);
				HRESULT D3D_Scene_LookAtLH(LPDIRECT3DDEVICE9);
				HRESULT D3D_Scene_Display(LPDIRECT3DDEVICE9);
				HRESULT D3D_Scene_CleanupDX(LPDIRECT3DDEVICE9);


			// ---------- UPDATE ----------

			// ---- update state and text overlay
				VOID  Update_If_Key_Pressed(VOID);
				VOID  Update_Displayed_Text(VOID);

			// ---- graphics
				vsl_library::Gfx_Element_Engine *gfx_element_engine;

		private:

			// ---- private implementation
				class Pimpl_Object3d_V01;
				std::unique_ptr<Pimpl_Object3d_V01> pimpl_object3d_V01;

		public:

			// ---- get example (works for derived Pimpl_Object3d_V01!)
				std::unique_ptr<Pimpl_Object3d_V01> *GetPimp(VOID) { return &pimpl_object3d_V01; }
				// 	std::unique_ptr<Pimpl_Object3d_V01> *pimp = GetPimp(VOID);

		};

}


////////////////////////////////////////////////////////////////////////////////



