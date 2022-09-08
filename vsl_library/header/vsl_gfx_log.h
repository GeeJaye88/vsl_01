////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_log.h ----------
/*!
\file vsl_gfx_log.h
\brief vs lite Gfx_Log class interface
\author Gareth Edwards
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
		class Gfx_Log
		{

			public:

			// ---- cdtor
				Gfx_Log();
				~Gfx_Log();

			// ---- set
				VOID SetShowDate(BOOL show_date);
				VOID SetShowTime(BOOL show_time);
				VOID SetShowLineNumber(BOOL show_line_number);
				VOID SetShowSimple(BOOL show_simple);

			// ---- write 
				HRESULT Write(VOID);
				HRESULT Write(CHAR *message);
				HRESULT Write(std::string message);

			// ---- banner 
				HRESULT WriteBanner(std::string banner_message);
				HRESULT WriteBannerLine();
				HRESULT WriteBannerMessage(std::string banner_message);


		private:

			// ---- private implementation
				class Pimpl_Gfx_Log;
				std::unique_ptr<Pimpl_Gfx_Log> pimpl_gfx_log;

		};

}


////////////////////////////////////////////////////////////////////////////////