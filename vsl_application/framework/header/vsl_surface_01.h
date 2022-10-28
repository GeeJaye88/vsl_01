////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_surface_01 ----------
/*!
\file vsl_surface_01.h
\brief interface for Surface_01 class derived from Base_01 class
\author Gareth Edwards

\note surface creation

*/

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- system
	#include "../../vsl_system/header/vsl_win_structs.h"

// ---- library
	#include "../../vsl_library/header/vsl_Gfx_Element_Engine.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky_interface.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky.h"

// ---- application
	#include "../../vsl_application/framework/header/vsl_base_01.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_application
{

	// ---- derived class
		class Surface_01 : public Base_01
		{
			public:

			// ---------- OVERIDE ----------


			// ---------- APPLICATION ----------

			// ---- invoked by Fw_Display - setup
				HRESULT Gfx_Setup_Project(VOID) override;

			// ---- invoked by Fw_Setup - setup specific elements
				HRESULT Gfx_Setup_Configurations(VOID) override;
				HRESULT Gfx_Setup_Coordinates(VOID) override;
				HRESULT Gfx_Setup_Components(VOID) override;

			// ---- invoked by Fw_Display
				HRESULT Gfx_Setup_Bookmarks(VOID) override;


			// ---------- UPDATE ----------

				// ---- update on input and add text overlay
				VOID Update_If_AsyncKey_Pressed(VOID) override;
				VOID Update_On_Screen_Text(VOID) override;


			// ---------- NEW ---------- 

			// ---- setup
				HRESULT Setup_Project(VOID);

			// ---- display
				HRESULT Surface_01::Display_Project(vsl_library::Gfx_Frame *frame, UINT level);

				// ---- properties
				BOOL text_mode = FALSE;
		};

}

////////////////////////////////////////////////////////////////////////////////



