////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_win_structs.h ----------
/*!
\file vsl_win_structs.h
\brief vsl_win_structs structs & classes
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system include
	#include "../../vsl_system/header/vsl_include.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- Win_Create struct ----------
/*!
\brief all window "create" parameters required accessed via the Win_Create struct
\author Gareth Edwards
*/

namespace vsl_system
{

	class Win_Create
	{

		public:

		// ---- cdtor
			Win_Create(VOID);
			~Win_Create();

		// ---- WinMain
			VOID SetAaq(UINT aaq);
			VOID SetCentred(BOOL centred);
			VOID SetDesktop(BOOL desktop);
			VOID SetName(std::string name);
			VOID SetXY(UINT x, UINT y);
			VOID SetDimensions(UINT width, UINT height);
			VOID SetYCaption(UINT y);
			VOID SetYBorder(UINT y);
			VOID SetYEdge(UINT y);

			UINT GetAaq(VOID);
			BOOL GetCentred(VOID);
			BOOL GetDesktop(VOID);
			std::string GetName(VOID);
			UINT GetX(VOID); UINT GetY(VOID);
			UINT GetWidth(VOID); UINT GetHeight(VOID);
			UINT GetYCaption(VOID);
			UINT GetYBorder(VOID);
			UINT GetYEdge(VOID);

		private:

			class Pimpl_Win_Create; Pimpl_Win_Create  *pimpl_win_create;

	};
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Win_Engine struct ----------
/*!
\brief all application "engine" data exposed via the Win_Engine struct
\author Gareth Edwards
*/

namespace vsl_system
{

	class Win_Engine
	{

		public:

		// ---- cdtor
			Win_Engine(VOID);
			~Win_Engine();

		// ---- display
			VOID  SetFps(UINT fps);
			VOID  SetFpsNow(UINT fps_now);
			VOID  SetColour(UINT r, UINT g, UINT b);

			UINT  GetFps(VOID);
			UINT  GetFpsNow(VOID);
			std::string GetFpsReport(VOID);
			UINT  GetRed(VOID), GetGreen(VOID), GetBlue(VOID);

		// ---- time
			VOID  SetMsStart(FLOAT ms_start);
			VOID  SetMsDelta(FLOAT ms_delta);
			VOID  SetMsElapsed(FLOAT ms_elapsed);
			VOID  SetMsNow(FLOAT ms_delta);
			VOID  SetSecondsLast(DWORD seconds_last);
			VOID  SetSecondsNow(DWORD seconds_now);

			FLOAT GetMsStart(VOID);
			FLOAT GetMsDelta(VOID);
			FLOAT GetMsElapsed(VOID);
			FLOAT GetMsNow(VOID);
			DWORD GetSecondsLast(VOID);
			DWORD GetSecondsNow(VOID);

			VOID  SetRegulated(BOOL regulated);
			BOOL  GetRegulated(VOID);

		// ---- device

			/*
				note: these are non vsl_library legacy methods,
				used by applications NOT based on vsl_application
			*/
			VOID  SetMouseLeftButtonDownMove(INT mouse_move_x, INT mouse_move_y);
			VOID  SetMouseWheelClickDelta(INT wheel_click_delta);
			// not used!
			INT   GetMouseLeftButtonDownX(VOID);
			INT   GetMouseLeftButtonDownY(VOID);
			INT   GetMouseWheelClickDelta(VOID);

			/*
				note: used in windows framework Win_Process function
				to acquire text input
			*/
			VOID  SetKeyDown(INT key_down);
			VOID  SetKeyShift(INT key_shift);
			INT   GetKeyDown(VOID);
			INT   GetKeyShift(VOID);

			/*
				note: command line interface
			*/
			VOID  SetCmdLineMode(BOOL mode);
			BOOL  GetCmdLineMode(VOID);

			std::string GetCmdLine();
			VOID SetCmdLine(std::string line);

		private:

			class Pimpl_Win_Engine; Pimpl_Win_Engine *pimpl_win_engine;

	};
}


////////////////////////////////////////////////////////////////////////////////


