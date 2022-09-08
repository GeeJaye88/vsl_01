////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_framework.h ----------
/*!
\file vsl_framework.h
\brief vsl system declarations
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- MACROS ----

	#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
	#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
	#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// ---- GLOBALS ----

	HWND                     g_hWnd = NULL;
	LPDIRECT3D9              g_pD3D = NULL;
	LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
	D3DPRESENT_PARAMETERS    g_d3dpp;
	BOOL                     g_window_size = FALSE;
	WPARAM                   g_wParam;
	LPARAM                   g_lParam;
	vsl_system::Win_Create	*g_win_create;
	vsl_system::Win_Engine	*g_win_engine;


// ---- WINDOWS FRAMEWORK ----

	INT WINAPI WinMain(
			HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPSTR     lpCmdLine,
			INT       nCmdShow
		);

	LRESULT CALLBACK Win_Process(
			HWND   hWnd,
			UINT   msg,
			WPARAM wParam,
			LPARAM lParam
		);

	HRESULT CALLBACK Win_Engine();

	HRESULT WinConsole(VOID);

// ---- APPLICATION FRAMEWORK ----

	HRESULT  Vsl_Cleanup(VOID);
	HRESULT  Vsl_CleanupDX(VOID);
	HRESULT  Vsl_Display(VOID);
	HRESULT  Vsl_Setup(VOID);
	HRESULT  Vsl_SetupDX(VOID);


////////////////////////////////////////////////////////////////////////////////
