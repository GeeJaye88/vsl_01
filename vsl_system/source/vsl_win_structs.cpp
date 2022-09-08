////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_win_structs.cpp ----------
/*!
\file vsl_win_structs.cpp
\brief Implementation of the Win_Create & Win_Engine structs
\author Gareth Edwards 
*/


#include "../../vsl_system/header/vsl_win_structs.h"


using namespace vsl_system;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of Pimpl_Win_Create class ----------

class Win_Create::Pimpl_Win_Create
{

public:

	// ---- cdtor ----
		Pimpl_Win_Create(VOID)
			{ ; };
		~Pimpl_Win_Create()
			{ ; };

	// ---- wnd create
		UINT aaq = 0;
		BOOL centred = FALSE;
		BOOL desktop = FALSE;
		std::string name = "None";
		UINT x = 0, y = 0;
		UINT width = 800, height = 600;
		UINT sm_y_caption = 0, sm_y_border = 0, sm_y_edge = 0;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Win_Create ----------

// ---- WinMain->Vsl_Setup

Win_Create::Win_Create(VOID)
	{ pimpl_win_create = new Pimpl_Win_Create(); }

Win_Create::~Win_Create()
	{ delete pimpl_win_create; pimpl_win_create = NULL; }

UINT Win_Create::GetAaq(VOID)
	{ return pimpl_win_create->aaq; }

VOID Win_Create::SetAaq(UINT aaq)
	{
		//
		// Defines the levels of full - scene multisampling that the device can apply.
		// aaq = 1 then note D3DMULTISAMPLE_NONMASKABLE
		// aaq must be level 0 or between levels 2 and 16
		//
		// see usage in WinMain->Vsl_Setup
		//
		// only using regularly tested level #'s below...
		//
		switch (aaq)
		{
			case 0: // NONE
			case 2:
			case 4:
			case 8:
				pimpl_win_create->aaq = aaq;
				break;
			default:
				// warn TBD
				break;
		}
	}

VOID Win_Create::SetCentred(BOOL centred)
	{ pimpl_win_create->centred = centred; }

VOID Win_Create::SetDesktop(BOOL desktop)
	{ pimpl_win_create->desktop = desktop; }

VOID Win_Create::SetName(std::string name)
	{ pimpl_win_create->name = name; }

VOID Win_Create::SetXY(UINT x, UINT y)
	{pimpl_win_create->x = x; pimpl_win_create->y = y; }

VOID Win_Create::SetDimensions(UINT width, UINT height)
	{ pimpl_win_create->width = width; pimpl_win_create->height = height; }

VOID Win_Create::SetYBorder(UINT y)
	{ pimpl_win_create->sm_y_border = y; }

VOID Win_Create::SetYCaption(UINT y)
	{ pimpl_win_create->sm_y_caption = y; }

VOID Win_Create::SetYEdge(UINT y)
	{ pimpl_win_create->sm_y_edge = y; }

BOOL Win_Create::GetCentred(VOID)
	{ return pimpl_win_create->centred; }

BOOL Win_Create::GetDesktop(VOID)
	{ return pimpl_win_create->desktop; }

std::string Win_Create::GetName(VOID)
	{ std::string something(pimpl_win_create->name); return something; }

UINT Win_Create::GetX(VOID)
	{ return pimpl_win_create->x; }

UINT Win_Create::GetY(VOID)
	{ return pimpl_win_create->y; }

UINT Win_Create::GetWidth(VOID)
	{ return pimpl_win_create->width; }

UINT Win_Create::GetHeight(VOID)
	{ return pimpl_win_create->height; }

UINT Win_Create::GetYBorder(VOID)
	{ return pimpl_win_create->sm_y_border; }

UINT Win_Create::GetYCaption(VOID)
	{ return pimpl_win_create->sm_y_caption; }

UINT Win_Create::GetYEdge(VOID)
	{ return pimpl_win_create->sm_y_edge; }



////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of Pimpl_Win_Engine class ----------

class Win_Engine::Pimpl_Win_Engine
{

public:

	// ---- cdtor
		Pimpl_Win_Engine(VOID)
			{ ; };
		~Pimpl_Win_Engine()
			{ ; };

	// ---- display
		UINT fps = 60;
		UINT fps_now = fps;
		UINT red = 60, green = 90, blue = 180;

	// ---- time
		FLOAT ms_start = 0;
		FLOAT ms_delta = 0;
		FLOAT ms_elapsed = 0;
		FLOAT ms_now = 0;
		DWORD seconds_last = 0;
		DWORD seconds_now = 0;

	// ---- input
		INT   mouse_move_x = 0;
		INT   mouse_move_y = 0;
		INT   mouse_wheel_delta = 0;
		INT   keydown = 0;
		BOOL  regulated = TRUE;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- Win_Engine ----------

Win_Engine::Win_Engine(VOID)
	{ pimpl_win_engine = new Pimpl_Win_Engine(); }

Win_Engine::~Win_Engine()
	{ delete pimpl_win_engine; pimpl_win_engine = NULL; }


// ---- fps

VOID Win_Engine::SetFps(UINT fps)
	{ pimpl_win_engine->fps = fps; }

VOID Win_Engine::SetFpsNow(UINT fps_now)
	{ pimpl_win_engine->fps_now = fps_now; }

VOID Win_Engine::SetColour(UINT r, UINT g, UINT b)
	{	pimpl_win_engine->red = r;
		pimpl_win_engine->green = g;
		pimpl_win_engine->blue = b; }

VOID Win_Engine::SetRegulated(BOOL regulated)
	{ pimpl_win_engine->regulated = regulated; }

UINT Win_Engine::GetFps(VOID)
	{ return pimpl_win_engine->fps; }

UINT Win_Engine::GetFpsNow(VOID)
	{ return pimpl_win_engine->fps_now; }

std::string Win_Engine::GetFpsReport(VOID)
{
	std::string report;
	report = std::to_string(GetFpsNow());
	report += " FPS ";
	if ( GetRegulated() )
	{
		report += "[ Regulated ]";
	}
	else
	{
		report += "[ NOT Regulated ";
		report += std::to_string(GetFps());
		report += " FPS ]";
	}
	return report;
}

BOOL Win_Engine::GetRegulated(VOID)
	{ return pimpl_win_engine->regulated; }


// ---- display

UINT Win_Engine::GetRed(VOID)
	{ return pimpl_win_engine->red; }

UINT Win_Engine::GetGreen(VOID)
	{ return pimpl_win_engine->green; }

UINT Win_Engine::GetBlue(VOID)
	{ return pimpl_win_engine->blue; }


// ---- time

VOID Win_Engine::SetMsStart(FLOAT ms_start)
	{ pimpl_win_engine->ms_start = ms_start; }

VOID Win_Engine::SetMsDelta(FLOAT ms_delta)
	{ pimpl_win_engine->ms_delta = ms_delta; }

VOID Win_Engine::SetMsElapsed(FLOAT ms_elapsed)
	{ pimpl_win_engine->ms_elapsed = ms_elapsed; }

VOID Win_Engine::SetMsNow(FLOAT ms_now)
	{ pimpl_win_engine->ms_now = ms_now; }

VOID  Win_Engine::SetSecondsLast(DWORD seconds_last)
	{ pimpl_win_engine->seconds_last = seconds_last; }

VOID  Win_Engine::SetSecondsNow(DWORD seconds_now)
	{ pimpl_win_engine->seconds_now = seconds_now; }


FLOAT Win_Engine::GetMsStart(VOID)
	{ return pimpl_win_engine->ms_start; }

FLOAT Win_Engine::GetMsDelta(VOID)
	{ return pimpl_win_engine->ms_delta; }

FLOAT Win_Engine::GetMsElapsed(VOID)
	{ return pimpl_win_engine->ms_elapsed; }

FLOAT Win_Engine::GetMsNow(VOID)
	{ return pimpl_win_engine->ms_now; }

DWORD Win_Engine::GetSecondsLast(VOID)
	{ return pimpl_win_engine->seconds_last; }

DWORD Win_Engine::GetSecondsNow(VOID)
	{ return pimpl_win_engine->seconds_now; }


/*
	note: these are non vsl_library legcacy methods

	      vsl_library uses Gfx_Command struct
*/

VOID Win_Engine::SetMouseLeftButtonDownMove(INT mouse_move_x, INT mouse_move_y)
	{	pimpl_win_engine->mouse_move_x = mouse_move_x;
		pimpl_win_engine->mouse_move_y = mouse_move_y; }

VOID Win_Engine::SetMouseWheelClickDelta(INT mouse_wheel_delta)
	{ pimpl_win_engine->mouse_wheel_delta = mouse_wheel_delta; }

VOID Win_Engine::SetKeydown(INT keydown)
	{ pimpl_win_engine->keydown = keydown; }

INT  Win_Engine::GetMouseLeftButtonDownX(VOID)
	{ return pimpl_win_engine->mouse_move_x; }

INT  Win_Engine::GetMouseLeftButtonDownY(VOID)
	{ return pimpl_win_engine->mouse_move_y; }

INT  Win_Engine::GetMouseWheelClickDelta(VOID)
	{ return pimpl_win_engine->mouse_wheel_delta; }

INT  Win_Engine::GetKeydown(VOID)
	{ return pimpl_win_engine->keydown; }


////////////////////////////////////////////////////////////////////////////////
