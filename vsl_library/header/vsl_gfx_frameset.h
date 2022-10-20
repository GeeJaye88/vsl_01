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

	// ---------- Gfx Box & Spacing Properties ----------

	// ---- FLOAT LTRB relative to parent frame (e.g., 0.1,0.1,0.5,0.5)
		struct Gfx_Box { FLOAT left = 0, top = 0, right = 1, bottom = 1; };

	// ---- UINT LTRB spacing bounded by box in screen ordinates
		enum Gfx_Spacing_Type
			{
				MARGIN,
				BORDER,
				PADDING,
			};
		struct Gfx_Spacing { UINT left = 0, top = 0, right = 0, bottom = 0; };
		struct Gfx_Content { UINT left = 1, top = 2, right = 3, bottom = 4; };


////////////////////////////////////////////////////////////////////////////////
			
		
	// --- "forward" declarations
		class Gfx_Frame;
		class Gfx_Panel;

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


////////////////////////////////////////////////////////////////////////////////


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

			// ---- link
				Gfx_Frame * AddChildFrame(std::string name);
				Gfx_Frame * GetFirstChildFrame();

				VOID SetBox(Gfx_Box *box);
				VOID SetSpacing(
						Gfx_Spacing_Type type,
						Gfx_Spacing *spacing
					);
				VOID GetContent(Gfx_Content *content);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Frame;
				std::unique_ptr<Pimpl_Gfx_Frame> pimpl_gfx_frame;

		};


}


////////////////////////////////////////////////////////////////////////////////