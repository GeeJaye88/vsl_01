/////////////////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_lsystem.h ----------
/*!
\file vsl_lsystem.h
\brief interface for the LSystem class
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


// ---------- struct for position vectors ----------

namespace vsl_application
{
	struct LVertex
	{
		D3DXVECTOR3 pos;

		enum FVF
		{
			FVF_Flags = D3DFVF_XYZ
		};
	};
}


/////////////////////////////////////////////////////////////////////////////////////////////


// ---------- LSystem class ----------

namespace vsl_application
{

	class LSystem
	{

	public:

		// ---- cdtor
			LSystem();
			virtual ~LSystem();


		// ---- system methods
			HRESULT Fw_Setup();
			HRESULT Fw_SetupDX(LPDIRECT3DDEVICE9 device);
			HRESULT Fw_Display(LPDIRECT3DDEVICE9 device);
			HRESULT Fw_CleanupDX(LPDIRECT3DDEVICE9 device);
			HRESULT Fw_Cleanup();

		// ---- system struct methods
			VOID Fw_Get_Win_Create(vsl_system::Win_Create **wc);
			VOID Fw_Get_Win_Engine(vsl_system::Win_Engine **wc);

		// ---- system notify event methods
			VOID Fw_Set_MouseLeftButtonDownMove(INT, INT);
			VOID Fw_Set_MouseWheelClick(INT);
			VOID Fw_Set_Keydown(WPARAM param);


		// ---- lsystem methods
			VOID Lsystem_ReadTextfile(CHAR *filename);
			VOID Lsystem_Iterate(INT counter);
			VOID Lsystem_DrawTree();
			VOID Lsystem_Fog(LPDIRECT3DDEVICE9 device);
			VOID Lsystem_SelectVariable(INT var);


	private:

		// ---- system structs
			vsl_system::Win_Create fw_win_create;
			vsl_system::Win_Engine fw_win_engine;

	public:


		// ---- application display methods
			VOID  Display_Text();
			VOID  Display_SetDefaults();
			VOID  Display_AsyncKeyState();


		// ---- gui
			FLOAT  x_rotate = 0;
			FLOAT  y_rotate = 0;
			FLOAT  z_translate = 0;
			DWORD  key_just_pressed = 0;
			DWORD  key_last_pressed = 0;
			DOUBLE y_turntable;


		// ---- materials
			D3DMATERIAL9 lsystem_material;


		// ---- font
			LPD3DXFONT font;               // display text font
			std::vector<LVertex> verts;    // list of vertices 
			LVertex v;                     // instance of vertex struct


		// ---- variable for Tree type display
			std::string Tree;


			// ---- lsystem variables for text file parameters and rules
			std::string axiom;
			std::string rule;
			std::string rule_1;
			std::string rule_2;
			std::string big_string;
			std::string string_next;


			// ---- display
			BOOL  instructions_on;        // toggle instruction
			BOOL  text_on;                // toggle text
			BOOL  turntable_on;           // toggle turntable
			BOOL  fog_on;                 // toggle fog


		// ---- process
			INT   current_angle;          // L-system parameters
			FLOAT current_length;
			INT   iteration_depth;
			INT   variable_increment;     // variable increment 
			INT   variable_selected;      // active variable select
			FLOAT camera_y_offset;        // camera y-axis offset

	};

}


/////////////////////////////////////////////////////////////////////////////////////////////
