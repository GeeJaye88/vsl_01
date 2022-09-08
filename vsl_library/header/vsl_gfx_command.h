////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_command.h ----------
/*!
\file vsl_gfx_command.h
\brief vs lite Gfx_Command class interface
\author Gareth Edwards

\note example usage:

application methods required by framework:

	VOID Fw_Set_MouseLeftButtonDownMove(INT, INT);

		-> invokes ->SetMouseLeftButtonDownMove((FLOAT)x, (FLOAT)y);

	VOID Fw_Set_MouseWheelClick(INT);
		-> Invokes ->GetMouseWheelClick();
			mouse_wheel_click += (FLOAT)d;
			->SetMouseWheelClick(mouse_wheel_click);

	VOID Fw_Set_Keydown(WPARAM param);

		-> invokes ->SetToggle((CHAR)param);

*/  

#if _MSC_VER > 1000
#pragma once
#endif


// ---- framework
	#include "../../vsl_system/header/vsl_include.h"

////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---------- Gfx_Log class interface ----------
		class Gfx_Command
		{

		public:

			// ---- cdtor
				Gfx_Command();
				~Gfx_Command();

			// ---- get
				DWORD GetKeyJustPressed(VOID);
				DWORD GetKeyLastPressed(VOID);
				FLOAT GetMouseLeftButtonDownMoveX(VOID);
				FLOAT GetMouseLeftButtonDownMoveY(VOID);
				FLOAT GetMouseWheelClick(VOID);

			// ---- set
				VOID  SetKeyJustPressed(DWORD key_just_pressed);
				VOID  SetKeyLastPressed(DWORD key_last_pressed);
				VOID  SetMouseLeftButtonDownMove(FLOAT x, FLOAT y);
				VOID  SetMouseWheelClick(FLOAT mouse_wheel_click_z);
				VOID  SetDefaultMouseWheelClick(FLOAT wheel_click);

			// ---- toggle
				HRESULT GetToggleValue(CHAR key, FLOAT *value);
				HRESULT SetToggleValue(CHAR key, FLOAT value);
				HRESULT SetToggle(CHAR key);

			// ---- big picture
				VOID Reset(VOID);
				VOID Update(FLOAT time_ms_delta);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Command;
				std::unique_ptr<Pimpl_Gfx_Command> pimpl_gfx_command;

		};

}


////////////////////////////////////////////////////////////////////////////////