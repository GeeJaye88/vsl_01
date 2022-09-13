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

			// ---------- APPLICATION ----------

			// ---- setup graphics
				HRESULT Gfx_Setup_Project(VOID) override;
				HRESULT Gfx_Setup_Configurations(VOID) override;
				HRESULT Gfx_Setup_Coordinates(VOID) override;
				HRESULT Gfx_Setup_Components(VOID) override;

			// ---- handle elements that have been bookmarked
				HRESULT Gfx_Element_Bookmarks(VOID) override;

		};

}

////////////////////////////////////////////////////////////////////////////////



