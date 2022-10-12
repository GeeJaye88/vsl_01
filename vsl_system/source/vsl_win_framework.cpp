////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_framework.cpp ----------
/*!
\file vsl_framework.cpp
\brief implementation of the vsl framework
\author Gareth Edwards

\note windows housekeeping and framework

*/

// ---------- housekeeping defintions ----------

	#define STRICT
	#define WIN32_LEAN_AND_MEAN


// ---- Microsoft Visual C++ include file
	#include "../../vsl_resource/vsl_resource.h"

// ----

// ---------- headers ----------
	//
	// application includes:
	//    --> "../../vsl_application/[name]/header/vsl_[name].h"
	//       --> "../../vsl_system/header/vsl_include.h"
	//       --> "../../vsl_system/header/vsl_win_structs.h"
	//
	//		& application specific stuff such as:
	//       --> "../../vsl_application/shared/header/vsl_fvf_vertex_structs.h"
	//

	//
	// framework includes:
	//    --> #include "../../vsl_application/mesh3d/header/vsl_mesh3d.h"
	//
	//    --> and init global property g_app, viz:
	//        vsl_application::Mesh3D *g_app = new vsl_application::Mesh3D();
	//
	#include "../../vsl_application/shared/header/vsl_select.h"

	//
	// framework system includes global properties & framework methods:
	//
	#include "../header/vsl_win_framework.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- WINDOWS FRAMEWORK ----------


// ---------- WinMain ----------
/*!

\brief windows main - the application's entry point
\author Gareth Edwards

\param hinstance (handle)
\param prevInstance (previous handle)
\param cmdLine
\param showCmd

\return int (exit status)

\note all window "create" parameters required accessed via the Win_Create struct

*/
int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR     lpCmdLine,
		INT       nCmdShow
	)
{

	// ---- local
		WNDCLASSEX winClass;
		MSG        uMsg;
		HRESULT    hr;
		CHAR     * winClassName = "VSL";

	// ---- initialise message
		memset(&uMsg,0,sizeof(uMsg));

	// ---- init
		winClass.lpszClassName = winClassName;
		winClass.cbSize        = sizeof(WNDCLASSEX);
		winClass.style         = CS_HREDRAW | CS_VREDRAW;
		winClass.lpfnWndProc   = Win_Process;
		winClass.hInstance     = hInstance;
		winClass.hIcon         = LoadIcon(hInstance, (LPCTSTR)IDI_ICON1);
		winClass.hIconSm       = LoadIcon(hInstance, (LPCTSTR)IDI_ICON2);
		winClass.hCursor       = LoadCursor(NULL, IDC_ARROW);
		winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
		winClass.lpszMenuName  = NULL;
		winClass.cbClsExtra    = 0;
		winClass.cbWndExtra    = 0;

	// ---- register class fails ? -  end
		if( !RegisterClassEx(&winClass) )
			return ERROR_FAIL;

	// ---- get window structs
		g_app->Fw_Get_Win_Create(&g_win_create);
		g_app->Fw_Get_Win_Engine(&g_win_engine);

	// ---- desktop shit
		RECT rect;
		HWND hdt = GetDesktopWindow();
		GetWindowRect(hdt, &rect);
		INT desktop_width = rect.right;
		INT desktop_height = rect.bottom;
		INT x_centre = desktop_width / 2;
		INT y_centre = desktop_height / 2;

	// ---- get & store system metrics
		/*
		DWORD cxBorder  = GetSystemMetrics(SM_CXBORDER);
		DWORD cyBorder  = GetSystemMetrics(SM_CYBORDER);
		DWORD cxEdge    = GetSystemMetrics(SM_CXEDGE);
		DWORD cyEdge    = GetSystemMetrics(SM_CYEDGE);
		DWORD cyCaption = GetSystemMetrics(SM_CYCAPTION);
		DWORD cyMenu    = GetSystemMetrics(SM_CYMENU);
		g_win_create->SetYBorder(cyBorder);
		g_win_create->SetYCaption(cyCaption);
		g_win_create->SetYEdge(cyEdge);
		*/

	// ---- window style, location & dimensions
		INT  win_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
		if ( g_win_create->GetDesktop() )
		{
			win_style = WS_POPUP | WS_VISIBLE;
			g_win_create->SetXY(0, 0);
			g_win_create->SetDimensions(desktop_width, desktop_height);
		}
		else
		{
			if ( g_win_create->GetCentred() )
			{
				g_win_create->SetXY(
						x_centre - g_win_create->GetWidth() / 2,
						y_centre - g_win_create->GetHeight() / 2
					);
			}
		}

	// ---- create window
		g_hWnd = CreateWindowEx( NULL,
				winClassName, 
				g_win_create->GetName().c_str(),
				win_style,
				g_win_create->GetX(), g_win_create->GetY(),
				g_win_create->GetWidth(), g_win_create->GetHeight(),
				NULL,
				NULL,
				hInstance,
				NULL
			);

	// ---- create window fails ? - end
		if( g_hWnd == NULL )
			return ERROR_FAIL;

	// ---- window housekeeping
		ShowWindow( g_hWnd, nCmdShow );
		UpdateWindow( g_hWnd );

	// ---- console ?
		BOOL console = FALSE;
		if (console)
		{
			WinConsole();
			SetConsoleTitle("VSL CONSOLE");
		}

	// ---- non-device one time application setup 
		hr = g_app->Fw_Setup();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- non-device one time setup
		hr = Vsl_Setup();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- device dependent setup
		hr = Vsl_SetupDX();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- loop, handling messages and application until WM_CLOSE, WM_DESTROY or WM_ESCAPE 
		hr = Win_Engine();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- device dependent cleanup
		hr = Vsl_CleanupDX();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- non-device dependant one time cleanup
		hr = Vsl_Cleanup();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- non-device one time application cleanup
		hr = g_app->Fw_Cleanup();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- unregister windows class
		UnregisterClass(
				winClassName,
				winClass.hInstance
			);

    return (int)uMsg.wParam;
}


// ---------- Win_Process ----------
/*!

\brief window framework - process(handle) message
\author Gareth Edwards

\return LRESULT (0)

\note this function requires additional LRESULT error (see TBD) handling.

*/
LRESULT CALLBACK Win_Process(
		HWND   hWnd,
		UINT   msg,
		WPARAM wParam,
		LPARAM lParam
	)
{

	//{
	//	std::string report = "Win_Process: " + std::to_string((UINT)msg) + "\n";
	//	OutputDebugString(report.c_str());
	//}

    static POINT ptLastMousePosit;
    static POINT ptCurrentMousePosit;
    static BOOL  bMousing;

	vsl_system::Win_Engine *win_eng;

    switch( msg )
    {
		case WM_KEYUP:
			{
				g_app->Fw_Get_Win_Engine(&win_eng);

				switch (wParam)
				{
					case VK_SHIFT:
						win_eng->SetKeyShift(0);
						break;

					default:
						break;
				}
				break;
			}
			break;

        case WM_KEYDOWN:
			{
				g_app->Fw_Get_Win_Engine(&win_eng);

				switch( wParam )
				{
					case VK_ESCAPE:
						{
							int id = MessageBox(
									NULL,
									"Do you want to exit?",
									"Confirm Exit",
									MB_ICONWARNING | MB_YESNOCANCEL | MB_DEFBUTTON2
								);
							switch (id)
							{
								case IDCANCEL:
								case IDNO:
									break;
								case IDYES:
									PostQuitMessage(0);
									break;
							}
						}
						break;
						 
					case VK_SHIFT:
						win_eng->SetKeyShift(1);
						break;

					case VK_CLEAR: // Windows legacy keypad #5
						break;

					case VK_RETURN:
						{
							if (win_eng->GetCmdLineMode())
							{
								std::string new_cmd_line = win_eng->GetCmdLine();
								if (!new_cmd_line.empty())
								{
									win_eng->SetCmdLine(new_cmd_line);
									OutputDebugString(new_cmd_line.c_str());
									OutputDebugString("\n");
									win_eng->SetCmdLine("");
								}
							}
						}
						break;

					default:
						{

							// ---- command keys
								INT cmd_key_begin = (INT)'C';
								INT cmd_key_end   = (INT)'Q';
								INT cmd_key_last  = win_eng->GetKeyDown();
								INT cmd_key_now   = (INT)wParam;

							// ---- flags
								BOOL cmd_tidy_up = FALSE;

							// ---- cmd begin
								if (cmd_key_last == cmd_key_begin && (INT)cmd_key_now == cmd_key_begin)
								{
									wParam = 0;
									win_eng->SetCmdLineMode(TRUE);
									std::string new_cmd_line = "";
									win_eng->SetCmdLine(new_cmd_line);
								}
							// ---- cmd end
								else if (cmd_key_last == cmd_key_end && (INT)cmd_key_now == cmd_key_end)
								{
									wParam = 0;
									win_eng->SetCmdLineMode(FALSE);
									std::string new_cmd_line = win_eng->GetCmdLine();
									if (!new_cmd_line.empty())
									{
										new_cmd_line.pop_back();
										win_eng->SetCmdLine(new_cmd_line);
									}
									cmd_tidy_up = TRUE;
								}

							// ---- cmd update
								if (win_eng->GetCmdLineMode() && wParam > 0)
								{
									CHAR c    = { win_eng->GetKeyShift() == 1 ? (CHAR)toupper(cmd_key_now) : (CHAR)tolower(cmd_key_now) };
									BOOL caps = (GetKeyState(VK_CAPITAL) & 0x0001) == 1;
									c         = caps ? (CHAR)toupper(cmd_key_now) : c;
									std::string new_cmd_line = win_eng->GetCmdLine();
									new_cmd_line += c;
									win_eng->SetCmdLine(new_cmd_line);
								}

							// ---- cmd send
								if (win_eng->GetCmdLineMode() || cmd_tidy_up)
								{
									OutputDebugString(win_eng->GetCmdLine().c_str());
									OutputDebugString("\n");
								}

							// ---- store
								if (wParam > 0)
									win_eng->SetKeyDown((INT)wParam);

						}
						break;
				}

				g_app->Fw_Set_Keydown(wParam);
			}
			break;

		#ifndef WM_MOUSEWHEEL
		#define WM_MOUSEWHEEL 0x020A
		#endif

		case WM_MOUSEWHEEL:
			{
				INT delta = (INT)wParam;
				INT d = delta < 0 ? -1 : 1;
				g_app->Fw_Set_MouseWheelClick(d);
			}
			break;

        case WM_LBUTTONDOWN:
        {
            ptLastMousePosit.x = ptCurrentMousePosit.x = LOWORD (lParam);
            ptLastMousePosit.y = ptCurrentMousePosit.y = HIWORD (lParam);
            bMousing = true;
        }
        break;

        case WM_LBUTTONUP:
        {
            bMousing = false;
        }
        break;

        case WM_MOUSEMOVE:
        {
            ptCurrentMousePosit.x = LOWORD (lParam);
            ptCurrentMousePosit.y = HIWORD (lParam);

            if ( bMousing )
            {
				int spinX, spinY;
				spinX = (ptCurrentMousePosit.x - ptLastMousePosit.x);
				spinY = (ptCurrentMousePosit.y - ptLastMousePosit.y);
				g_app->Fw_Set_MouseLeftButtonDownMove(spinX, spinY);
            }

            ptLastMousePosit.x = ptCurrentMousePosit.x;
            ptLastMousePosit.y = ptCurrentMousePosit.y;
        }
        break;

		case WM_EXITSIZEMOVE:
		{
			// force resize with stored param
			g_window_size = TRUE;
			wParam = g_wParam;
			lParam = g_lParam;
		}
		// ----> & on to WM_SIZE

        case WM_SIZE:
        {

			// if force resize....
			if (g_window_maximised == TRUE || g_window_size == TRUE || wParam == SIZE_MAXIMIZED)
			{

				// if the device is not NULL and the WM_SIZE message is not a
				// SIZE_MINIMIZED event, resize the device's swap buffers to match
				// the new window size.
				if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
				{
					// cleanup applications usage of graphics device
					HRESULT hr = Vsl_CleanupDX();
					if (FAILED(hr)) return 0; // TBD

					// catch maximise & unmaximise
					if (wParam == SIZE_MAXIMIZED)
						g_window_maximised = TRUE;
					else
						g_window_maximised = FALSE;

					// get y system metrics
					DWORD cyBorder  = GetSystemMetrics(SM_CYBORDER);
					DWORD cyEdge    = GetSystemMetrics(SM_CYEDGE);
					DWORD cyCaption = GetSystemMetrics(SM_CYCAPTION);
					UINT  yOffset   = (INT)((FLOAT)(cyCaption + cyBorder + cyEdge) * 1.5f);

					// store new dimensions
					g_d3dpp.BackBufferWidth = LOWORD(lParam);;
					g_d3dpp.BackBufferHeight = HIWORD(lParam);;

					// calc adjusted offset
					g_win_create->SetDimensions(g_d3dpp.BackBufferWidth, g_d3dpp.BackBufferHeight + yOffset);

					// reset device
					hr = g_pd3dDevice->Reset(&g_d3dpp);
					if (hr == D3DERR_INVALIDCALL)
					{
						MessageBox(NULL,
							"Call to Reset() failed with D3DERR_INVALIDCALL!",
							"ERROR", MB_OK | MB_ICONEXCLAMATION);
						return ERROR_FAIL; // TBD
					}

					// setup applications usage of device
					hr = Vsl_SetupDX();
					if (FAILED(hr)) return ERROR_FAIL; // TBD

				}
			}
			// catch all but the last WM_SIZE - see WM_EXITSIZEMOVE
			else
			{
				// store param
				g_wParam = wParam;
				g_lParam = lParam;
			}

			// zap g_window_size
			g_window_size = FALSE;

        }
        break;

        case WM_CLOSE:
        {
            PostQuitMessage(0); 
        }
		break;
        
        case WM_DESTROY:
        {
            PostQuitMessage(0);
        }
        break;

        default:
        {
            return DefWindowProc( hWnd, msg, wParam, lParam );
        }
        break;
    }

    return 0;
}


// ---------- Win_Engine ----------
/*!

\brief window framework engine (formally "WndPump")
\author Gareth Edwards
\return LRESULT (0)
\note all application "engine" data exposed via the Win_Engine struct

\note if there is a "message" to handle THEN translate and dispatch to Win_Process function

*/
HRESULT CALLBACK Win_Engine()
{

	// ---- frames per second
		BOOL fps_regulated = TRUE;
		UINT fps_last = 0;
		UINT fps_now = 0;
		UINT fps = g_win_engine->GetFps();

	// ---- time start in milliseconds
		FLOAT ms_start = (FLOAT)timeGetTime();

	// ---- set Win_Engine ms start & now (delta = 0)
		g_win_engine->SetMsStart(ms_start);
		g_win_engine->SetMsNow(ms_start);

	// ---- ms "since last frame" used in main loop to calculate ms delta
		FLOAT ms_now   = 0;
		FLOAT ms_last  = 0;
		FLOAT ms_delta = 0;

	// ---- interval & elapsed MUST be DOUBLE to "exactly" calculate in-second millisecond
	
	// ---- see *** below

	// ---- target ms interval
		DOUBLE d_ms_interval = 1000 / (DOUBLE)fps;

	// ---- elapsed ms
		DOUBLE d_ms_elapsed_target = d_ms_interval;
		DOUBLE d_ms_elapsed = 0;
	
	// ---- seconds
		DWORD seconds_elapsed_last = 0;
		DWORD seconds_elapsed = 0;

	// ---- message to be processed (see Win_Process function above)
		MSG msg;
		::ZeroMemory(&msg, sizeof(MSG));

	// ---- loop until completed
		while( msg.message != WM_QUIT )
		{

			// ---- IF there is a message to process THEN translate and dispatch to Win_Process function
				if ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE) )
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}

				// ---- ELSE
				else
				{

					// ---- spinlock until current time exceeds next time
						DOUBLE d_ms_get_time = (DOUBLE)timeGetTime();
						d_ms_elapsed = d_ms_get_time - ms_start;
						if (d_ms_elapsed > d_ms_elapsed_target)
						{

							// ---- invoke display method
								HRESULT hr;
								try
								{
									// ---- update Win_Engine ms now
										g_win_engine->SetMsElapsed((FLOAT)d_ms_elapsed);
										g_win_engine->SetMsNow((FLOAT)d_ms_get_time);

									// ---- update last & next, then calc delta
										ms_last  = ms_now;
										ms_now   = (FLOAT)d_ms_elapsed;
										ms_delta = ms_now - ms_last;

									// ---- update Win_Engine ms delta
										g_win_engine->SetMsDelta(ms_delta);

									// ---- seconds
										seconds_elapsed = (DWORD)(d_ms_elapsed / 1000);
										g_win_engine->SetSecondsLast(seconds_elapsed_last);
										g_win_engine->SetSecondsNow(seconds_elapsed);

									// ---- display
										hr = Vsl_Display();
										if (FAILED(hr)) throw (1);

									// ---- update seconds & reset fps if next second
										if (seconds_elapsed != seconds_elapsed_last)
										{

											// regulate ? & update Win_Engine fps regulated
											fps_regulated = fps_now + 1 < fps ? FALSE : TRUE;
											g_win_engine->SetRegulated(fps_regulated);

											// update Win_Engine fps ...
											g_win_engine->SetFpsNow(fps_now);
											#if defined(DEBUG)
											if ( fps_now != fps_last )
											{
												CHAR ods[128];
												sprintf_s(ods, 128, "WinPump: seconds %d & fps %d\n", seconds_elapsed, fps_now);
												OutputDebugString(ods);
											}
											#endif

											// update last fps & seconds, then reset fps
											seconds_elapsed_last = seconds_elapsed;
											fps_last = fps_now;
											fps_now = 0;

										}

									// ---- fps "this second" count
										fps_now++;

									// ---- update target elapsed
										#define CALC_TARGET_ELAPSED
										#if defined(CALC_TARGET_ELAPSED)

										// *** interval & elapsed MUST be DOUBLE to "exactly"
										//     calculate the in-second millisecond
										if (fps_regulated)
										{
											// calculate the # of intervals to "now"
											DOUBLE d_ms_elapsed_right_now = (DOUBLE)timeGetTime() - ms_start;
											DWORD total_ms_intervals_to_now = (DWORD)(d_ms_elapsed_right_now / d_ms_interval);

											// add an interval to get (ceiling) target value...
											d_ms_elapsed_target = (total_ms_intervals_to_now + 1) * d_ms_interval;
										}
										else
										{
											d_ms_elapsed_target = d_ms_elapsed;
										}

										#else 

										// original solution, which accumulates numerical error
										ms_elapsed_last   = ms_elapsed;
										ms_elapsed_target = ms_elapsed + ms_interval;
										
										#endif

								}

							// ---- catch, report and return unexpected error
								catch ( INT error_id )
								{
									static CHAR strError[256];
									sprintf_s(
											strError, 256,
											"VSL Framework method Win_Engine:\n"
											"    id %d, and \n"
											"    HRESULT %d.",
											error_id, hr
										);
									MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
									return ERROR_UNEXPECTED;
								}

						}

				}

		}

    return (HRESULT)msg.wParam;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- VS APPLICATION FRAMEWORK ----------


// ---------- Vsl_Cleanup ----------
/*!

\brief vsl application framework one time cleanup
\author Gareth Edwards
\return HRESULT - SUCCESS_OK, S_FAIL, ERROR_FAIL

\note Called once when an application is quit.

*/
HRESULT Vsl_Cleanup(VOID)
{

	// ---- cleanup DirectX device dependant objects
		//Vsl_CleanupDX();

	// ---- cleanup the device
		if ( g_pd3dDevice != NULL )
		{
			int ref_count = g_pd3dDevice->Release();

			if (ref_count > 0 )
			{
				static CHAR strError[256];
				sprintf_s(
						strError, 256,
						"Framework method Vsl_Cleanup:\n"
						"    The D3D device object failed to cleanup properly.\n"
						"    Release() returned a reference count of %d.",
						ref_count
					);
				MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			g_pd3dDevice = NULL;
		}

	// ---- release d3d
		SAFE_RELEASE( g_pD3D )

	return SUCCESS_OK;
}


// ---------- Vsl_CleanupDX ----------
/*!

\brief vsl application framework cleanup DX
\author Gareth Edwards
\return HRESULT - SUCCESS_OK, S_FAIL, ERROR_FAIL

\note This function is called when device dependant resources are to be released

*/
HRESULT Vsl_CleanupDX(VOID)
{

	// ---- cleanup the application
		HRESULT hr = g_app->Fw_CleanupDX(g_pd3dDevice);
		if (FAILED(hr)) return ERROR_FAIL;

    return SUCCESS_OK;
}


// ---------- Vsl_Display ----------
/*!

\brief vsl application framework display
\author Gareth Edwards
\return HRESULT - SUCCESS_OK, S_FAIL, ERROR_FAIL

\note

Can use either:
	D3DCOLOR_XRGB(60, 90, 180) or...
	D3DCOLOR_COLORVALUE(0.24f,0.35f,0.70f,1.0f)

\note this function requires additional HRESULT error handling.

*/
HRESULT Vsl_Display(VOID)
{

	// ---- local
		HRESULT hr;

	// ---- clear backbuffers
		hr = g_pd3dDevice->Clear(
				0,
				NULL,
				D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				D3DCOLOR_XRGB(
						g_win_engine->GetRed(),
						g_win_engine->GetGreen(),
						g_win_engine->GetBlue()
					),
				1.0f,
				0
			);

	// ----begin
		hr = g_pd3dDevice->BeginScene();

	// ---- display
		hr = g_app->Fw_Display(g_pd3dDevice);

	// ----end
		hr = g_pd3dDevice->EndScene();

	// ----present
		hr = g_pd3dDevice->Present( NULL, NULL, NULL, NULL );

	// ----render all panels
		//hr = Vsl_RenderPanels();

	return SUCCESS_OK;
}


// ---------- Vsl_Setup ----------
/*!

\brief vsl application framework setup resources or settings
\author Gareth Edwards
\return HRESULT - SUCCESS_OK, S_FAIL, ERROR_FAIL

This function will only be called only once during the
application's initialization phase.

Therefore, it can't contain any resources that need to be
restored every time the Direct3D device is lost or the
window is resized.

\note this function requires additional HRESULT error handling.

*/
HRESULT Vsl_Setup(VOID)
{

	// ---- local
		HRESULT hr;

	// ---- direct3d initialisation
		g_pD3D = Direct3DCreate9( D3D_SDK_VERSION );

	// ---- display mode
		D3DDISPLAYMODE d3ddm;
		hr = g_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );
		ZeroMemory( &g_d3dpp, sizeof(g_d3dpp) );

	// ---- set present parameters
		g_d3dpp.Windowed               = TRUE;
		g_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
		g_d3dpp.BackBufferFormat       = d3ddm.Format;
		g_d3dpp.EnableAutoDepthStencil = TRUE;
		g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		g_d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;
		g_d3dpp.MultiSampleType        = D3DMULTISAMPLE_NONE;
		g_d3dpp.MultiSampleQuality     = 0;

	// ---- here we go
		BOOL succeeded_create_device = FALSE;

	// ---- does this adapter mode support a 24 or 16bit depth stencil pixel format, ...?
		D3DFORMAT depth_stencil_format[2] = { D3DFMT_D24S8, D3DFMT_D16};
		for ( int dsf=0; dsf<2; dsf++ )
		{		

			// ---- set and enable depth stencil pixel format
				g_d3dpp.AutoDepthStencilFormat = depth_stencil_format[dsf];
				g_d3dpp.EnableAutoDepthStencil = true;


			// ---- anti-aliasing ?
				D3DMULTISAMPLE_TYPE aaq = (D3DMULTISAMPLE_TYPE)g_win_create->GetAaq();
				{

					// ---- test if format supports multi-sampling with value aaq
						DWORD AAQuality = 0;		
						if (SUCCEEDED(
								g_pD3D->CheckDeviceMultiSampleType(
										D3DADAPTER_DEFAULT,         //Ordinal number denoting the display adapter to query. D3DADAPTER_DEFAULT is always the primary display adapter.
										D3DDEVTYPE_HAL,             // Member of the D3DDEVTYPE enumerated type, identifying the device type.
										depth_stencil_format[dsf],	// Member of the D3DFORMAT 
										TRUE,						// windowed
										aaq,                        // D3DMULTISAMPLE_TYPE enumerated type
										&AAQuality                  // pQualityLevels 
									)
								)
							)
						{
							g_d3dpp.MultiSampleType = aaq;
							g_d3dpp.MultiSampleQuality = AAQuality - 1;
						}

					/*

					https://docs.microsoft.com/en-us/windows/win32/api/d3d9/nf-d3d9-idirect3d9-checkdevicemultisampletype

					pQualityLevels returns the number of device-specific sampling variations available
					with the given sample type. For example, if the returned value is 3, then quality
					levels 0, 1 and 2 can be used when creating resources with the given sample count.

					The meanings of these quality levels are defined by the device manufacturer and
					cannot be queried through D3D.

					For example, for a particular device different quality levels at a fixed sample
					count might refer to different spatial layouts of the sample locations or different
					methods of resolving. This can be NULL if it is not necessary to return the quality levels.

					*/
				}

			// ---- check for hardware transform & lighting
				D3DCAPS9 m_caps;	
				hr = g_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &m_caps);
				DWORD vertexProcessing = 0;
				if ( m_caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
				{
					vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;

					// ---- check for pure device
						if ( m_caps.DevCaps & D3DDEVCAPS_PUREDEVICE )
						{
							vertexProcessing |= D3DCREATE_PUREDEVICE;
						}
				}
				else
				{
					vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
				}

			// ---- create the d3d device 
				hr = g_pD3D->CreateDevice(
						D3DADAPTER_DEFAULT,
						D3DDEVTYPE_HAL,
						g_hWnd,
						vertexProcessing,
						&g_d3dpp,
						&g_pd3dDevice
					);

			// ---- failed ?
				if (SUCCEEDED(hr))
				{
					succeeded_create_device = true;
					break;
				}

		}

		// ---- catch fail
			if ( !succeeded_create_device )
			{
				// this will not be needed >>>> g_pD3D->Release();
				return ERROR_FAIL;
			}

	return SUCCESS_OK;
}


// ---------- Vsl_SetupDX ----------
/*!

\brief vsl application framework setup device dependant resources or settings
\author Gareth Edwards
\return HRESULT - SUCCESS_OK, SUCCESS_FAULT, ERROR_FAIL

This function is called when device dependant resources or
settings are to be restored after losing the DirectD device
OR the window is resized.

\note this function requires additional HRESULT error handling.

*/
HRESULT Vsl_SetupDX (VOID)
{

	// ---- local
		HRESULT hr;

	// ---- projection matrix
		D3DXMATRIX matProj;
		D3DXMatrixPerspectiveFovLH(
				&matProj,
				D3DXToRadian( 45.0f ),
				(FLOAT)g_d3dpp.BackBufferWidth /
				(FLOAT)g_d3dpp.BackBufferHeight,
				0.1f,
				100.0f
			);
		hr = g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );

	// ---- render states
		hr = g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
		hr = g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
		hr = g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, TRUE );
		hr = g_pd3dDevice->SetRenderState( D3DRS_AMBIENT,
			D3DCOLOR_COLORVALUE( 0.2f, 0.2f, 0.2f, 1.0f ) );

    // ---- texture states 
		hr = g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
		hr = g_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );

	// ---- application
		hr = g_app->Fw_SetupDX(g_pd3dDevice);
		if (FAILED(hr)) return ERROR_FAIL;

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////

// ---------- WinConsole ----------
/*!
\brief allocates a new console for the calling process

Output as per: std::cout << "fred" << std::endl;

Input as per: std::cin.get();

\param this function has no parameters
\notes
1. If the function succeeds, the return value is nonzero.
2. If the function fails, the return value is zero.
3. To get extended error information, call GetLastError.
4. Input will halt the app until CR

Remarks
=======
A process can be associated with only one console, so the AllocConsole function
fails if the calling process already has a console. A process can use the
FreeConsole function to detach itself from its current console, and then it can
call AllocConsole to create a new console. If the calling process creates a
child process, the child inherits the new console.

AllocConsole also sets up standard input, standard output, and standard error
handles for the new console. The standard input handle is a handle to the
console's input buffer, and the standard output and standard error handles are
handles to the console's screen buffer. To retrieve these handles, use the
GetStdHandle function.

This function is primarily used by graphics applications to create a console
window. Graphics applications are initialized without a console. Console
applications are normally initialized with a console, unless they are created
as detached processes (by calling the CreateProcess function with the
DETACHED_PROCESS flag).

See Also
========
CreateProcess, FreeConsole, GetStdHandle

The following functions can control & get console related information.
----------------------------------------------------------------------
GetConsoleScreenBufferInfo//Retrieve window size, screen buffer size and color attributes
SetConsoleWindowInfo//Change the size of the console window SetConsoleScreenBufferSize//Change the size of the console screen buffer
SetConsoleTextAttribute//Set the color attribute
SetConsoleTitle//Set the title of the console window
GetConsoleTitle//Get the title of the console window

*/
HRESULT WinConsole(VOID)
{

	// ---- free & then create console
		if (FreeConsole())
		{
			if (!AllocConsole()) {
				// Add some error handling here...
				// Call GetLastError() to get more info about the error.
				return ERROR_FAIL;
			}
		}
		else
		{
			return ERROR_FAIL;
		}

	// ---- std::cout, std::clog, std::cerr, std::cin
		FILE* fDummy;
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
		freopen_s(&fDummy, "CONOUT$", "w", stderr);
		freopen_s(&fDummy, "CONIN$", "r", stdin);
		std::cout.sync_with_stdio();

		std::cout.clear();
		std::clog.clear();
		std::cerr.clear();
		std::cin.clear();

	// ---- std::wcout, std::wclog, std::wcerr, std::wcin
		HANDLE hConOut = CreateFile("CONOUT$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		HANDLE hConIn = CreateFile("CONIN$", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
		SetStdHandle(STD_ERROR_HANDLE, hConOut);
		SetStdHandle(STD_INPUT_HANDLE, hConIn);
		std::wcout.clear();
		std::wclog.clear();
		std::wcerr.clear();
		std::wcin.clear();


		AttachConsole(GetCurrentProcessId());
		//freopen_s(&fDummy, "CON", "w", stdout);
		//std::cout << "This is a test info" << std::endl;

	return SUCCESS_OK;

}


////////////////////////////////////////////////////////////////////////////////
