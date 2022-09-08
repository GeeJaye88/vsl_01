////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_command.cpp ----------
/*!
\file vsl_gfx_command.cpp
\brief Implementation of the Gfx_Command class
\author Gareth Edwards
*/


#include "../header/vsl_gfx_command.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


// ---------- Private Implementation of Gfx_Command::Pimpl_Gfx_Command class ----------
class Gfx_Command::Pimpl_Gfx_Command
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Command() {}
		~Pimpl_Gfx_Command() { ; }

	// ---- mouse
		FLOAT  mouse_lbd_move_x = 0;
		FLOAT  mouse_lbd_move_y = 0;
		FLOAT  mouse_wheel_click = 0;
		FLOAT  mouse_wheel_click_default = 0;

	// ---- key
		DWORD key_just_pressed = 0;
		DWORD key_last_pressed = 0;

	// ---- key toggle states
		BOOL key_t_toggle = FALSE;

	// ---- key state values	
		FLOAT key_t_value = 0;

	private:

};


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------

Gfx_Command::Gfx_Command() : pimpl_gfx_command(new Pimpl_Gfx_Command)
{
	;
}

Gfx_Command::~Gfx_Command()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- get ----------

DWORD Gfx_Command::GetKeyJustPressed(VOID)
	{ return pimpl_gfx_command->key_just_pressed; }

DWORD Gfx_Command::GetKeyLastPressed(VOID)
	{ return pimpl_gfx_command->key_last_pressed; }

FLOAT Gfx_Command::GetMouseLeftButtonDownMoveX(VOID)
	{ return pimpl_gfx_command->mouse_lbd_move_x; }

FLOAT Gfx_Command::GetMouseLeftButtonDownMoveY(VOID)
	{ return pimpl_gfx_command->mouse_lbd_move_y; }

FLOAT Gfx_Command::GetMouseWheelClick(VOID)
	{ return pimpl_gfx_command->mouse_wheel_click; }

HRESULT Gfx_Command::GetToggleValue(CHAR key, FLOAT *value)
	{
		CHAR KEY = toupper(key);
		switch (KEY)
		{
			case 'T': *value = pimpl_gfx_command->key_t_value;
			default: return SUCCESS_FAULT;
		}
		return SUCCESS_OK;
	}


// ---------- set methods ----------

VOID Gfx_Command::SetKeyJustPressed(DWORD key_just_pressed)
	{ pimpl_gfx_command->key_just_pressed = key_just_pressed; }

VOID Gfx_Command::SetKeyLastPressed(DWORD key_last_pressed)
	{ pimpl_gfx_command->key_last_pressed = key_last_pressed; }

VOID Gfx_Command::SetMouseLeftButtonDownMove(FLOAT x, FLOAT y)
	{ 
		pimpl_gfx_command->mouse_lbd_move_x -= x;
		pimpl_gfx_command->mouse_lbd_move_y -= y;
	}

VOID Gfx_Command::SetMouseWheelClick(FLOAT mouse_wheel_click)
	{ pimpl_gfx_command->mouse_wheel_click = mouse_wheel_click; }

HRESULT Gfx_Command::SetToggleValue(CHAR key, FLOAT value)
	{
		CHAR KEY = toupper(key);
		switch (KEY)
		{
			case 'T':
				{
				pimpl_gfx_command->key_t_value; break;
				}
			default: return SUCCESS_FAULT;
		}
		return SUCCESS_OK;
	}

HRESULT Gfx_Command::SetToggle(CHAR key)
	{
		CHAR KEY = toupper(key);
		switch (KEY)
		{
			case 'T': pimpl_gfx_command->key_t_toggle = pimpl_gfx_command->key_t_toggle ? FALSE : TRUE; break;
			default: return SUCCESS_FAULT;
		}
		return SUCCESS_OK;
	}

VOID Gfx_Command::SetDefaultMouseWheelClick(FLOAT wheel_click)
	{ pimpl_gfx_command->mouse_wheel_click_default = pimpl_gfx_command->mouse_wheel_click = wheel_click; }


// ---------- big picture ----------

VOID Gfx_Command::Reset(VOID)
	{
		pimpl_gfx_command->mouse_lbd_move_x = 0;
		pimpl_gfx_command->mouse_lbd_move_y = 0;
		pimpl_gfx_command->mouse_wheel_click = pimpl_gfx_command->mouse_wheel_click_default;
		pimpl_gfx_command->key_t_value = 0;
	}

VOID Gfx_Command::Update(FLOAT time_ms_delta)
	{
		if (pimpl_gfx_command->key_t_toggle)
			pimpl_gfx_command->key_t_value += (FLOAT)(time_ms_delta / 20);
	}


////////////////////////////////////////////////////////////////////////////////