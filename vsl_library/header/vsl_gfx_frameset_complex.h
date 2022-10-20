////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_panel.h ----------
/*!
\file vsl_gfx_panel.h
\brief vs lite Gfx Panel class interface
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system include
	#include "../../vsl_system/header/vsl_include.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_log.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---------- Gfx Panel Properties----------

		struct Gfx_F_Rectangle
		{
			FLOAT left;
			FLOAT top;
			FLOAT right;
			FLOAT bottom;
		};

		struct Gfx_UI_Rectangle
		{
			UINT left;
			UINT top;
			UINT right;
			UINT bottom;
		};

		
	// --- "forward" declarations
		class Gfx_Frame;
		class Gfx_Panel;


	// ---------- Gfx_Frame_Style class interface ----------
		class Gfx_Frame_Style
		{

		public:

			// ---- cdtor
				Gfx_Frame_Style(std::string name);
				~Gfx_Frame_Style();


				Gfx_F_Rectangle  panel = { 0, 0, 1, 1 };

				Gfx_UI_Rectangle margin = { 0, 0, 0, 0 };
				Gfx_UI_Rectangle border = { 0, 0, 0, 0 };
				Gfx_UI_Rectangle padding = { 0, 0, 0, 0 };
				Gfx_UI_Rectangle content = { 0, 0, 0, 0 };

		private:

			// ---- private implementation
				class Pimpl_Gfx_Frame_Style;
				std::unique_ptr<Pimpl_Gfx_Frame_Style> pimpl_gfx_frame_style;

		};

	// ---------- Gfx_Frameset class interface ----------
		class Gfx_Frameset
		{

		public:

			// ---- cdtor
				Gfx_Frameset();
				~Gfx_Frameset();

			// ---- frames
				Gfx_Frame *AddFrame(std::string name);
				Gfx_Frame *GetFirst();

			// ---- dimensions
				HRESULT SetDimensions(UINT width, UINT height);
				UINT GetWidth();
				UINT GetHeight();

			// ---- calculate frameset
				HRESULT Setup();

			// ---- housekeeeping
				HRESULT SetGfxLog(Gfx_Log *gfx_log);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Frameset;
				std::unique_ptr<Pimpl_Gfx_Frameset> pimpl_gfx_frameset;

		};


	// ---------- Gfx_Frame class interface ----------
		class Gfx_Frame
		{

		public:

			// ---- cdtor
				Gfx_Frame(std::string name);
				~Gfx_Frame();

				std::string GetName();
				void SetName(std::string name);

			// ---- links
				Gfx_Frame * GetNext();
				VOID SetNext(Gfx_Frame *next);

			// ---- panels
				Gfx_Panel * AddPanel(std::string name);
				Gfx_Panel * GetFirst();

		private:

			// ---- private implementation
				class Pimpl_Gfx_Frame;
				std::unique_ptr<Pimpl_Gfx_Frame> pimpl_gfx_frame;

		};


	// ---------- Gfx_Panel class interface ----------
		class Gfx_Panel
		{

		public:

			// ---- cdtor
				Gfx_Panel(std::string name);
				~Gfx_Panel();

				Gfx_F_Rectangle  panel   = { 0, 0, 1, 1};

				Gfx_UI_Rectangle margin  = { 0, 0, 0, 0 };
				Gfx_UI_Rectangle border  = { 0, 0, 0, 0 };
				Gfx_UI_Rectangle padding = { 0, 0, 0, 0 };
				Gfx_UI_Rectangle content = { 0, 0, 0, 0 };


				std::string GetName();
				void SetName(std::string name);

			// ---- links
				Gfx_Panel * GetNext();
				VOID SetNext(Gfx_Panel *next);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Panel;
				std::unique_ptr<Pimpl_Gfx_Panel> pimpl_gfx_panel;

		};

}


////////////////////////////////////////////////////////////////////////////////