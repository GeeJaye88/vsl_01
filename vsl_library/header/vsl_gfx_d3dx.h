////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_d3dx.h ----------
/*!
\file vsl_gfx.h
\brief vs lite Gfx D3d class interface
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif


// ---- framework
	#include "../../vsl_system/header/vsl_include.h"
	#include "../../vsl_system/header/vsl_win_structs.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_command.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---------- Gfx_D3dx class interface ----------
		class Gfx_D3dx
		{

		public:

			// ---- cdtor
				Gfx_D3dx();
				~Gfx_D3dx();

			// ---- set framework wnd struct
				VOID Set_Win_Create(vsl_system::Win_Create *wc);
				VOID Set_Gfx_Command(vsl_library::Gfx_Command *wp);

			// ---- framework
				HRESULT Setup(VOID);
				HRESULT SetupDX(LPDIRECT3DDEVICE9);
				HRESULT Projection(LPDIRECT3DDEVICE9);
				HRESULT LookAtLH(LPDIRECT3DDEVICE9);
				HRESULT Display(LPDIRECT3DDEVICE9);
				HRESULT CleanupDX(LPDIRECT3DDEVICE9);
				HRESULT Cleanup(VOID);
				HRESULT DisplayText(std::string&, D3DCOLOR& colour, RECT& rect);

		private:

			// ---- private implementation
				class Pimpl_Gfx_D3dx;
				std::unique_ptr<Pimpl_Gfx_D3dx> pimpl_gfx_d3dx;

		};

}


////////////////////////////////////////////////////////////////////////////////