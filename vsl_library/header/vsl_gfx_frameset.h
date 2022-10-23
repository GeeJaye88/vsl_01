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

		
	// --- "forward" declaration
		class Gfx_Frame;

	// ---------- Gfx_Frameset class interface ----------
		class Gfx_Frameset
		{

		public:

			// ---- cdtor
				Gfx_Frameset();
				~Gfx_Frameset();

			// ---- links
				Gfx_Frame *AddFrame(std::string name);
				Gfx_Frame *GetFirst();

			// ---- dimensions
				HRESULT SetDimensions(UINT width, UINT height);
				UINT    GetWidth();
				UINT    GetHeight();

			// ---- calculate
				HRESULT Setup();

			// ---- log & report
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

			struct Rectangle
				{
					FLOAT left = 0, top = 0, right = 0, bottom = 0;
				};

			enum TYPE
				{
					OUTSIDE,    // proportional
					MARGIN,     // pixels
					BORDER,     // pixels
					PADDING,    // pixels
					INSIDE      // proportional
				};

			// ---- cdtor
				Gfx_Frame(std::string name);
				~Gfx_Frame();

			// ---- general
				std::string GetName();
				VOID        SetName(std::string name);
				VOID        SetDimensions(UINT width, UINT height);
				UINT        GetWidth();
				UINT        GetHeight();

			// ---- links
				Gfx_Frame * GetParent();
				VOID        SetParent(Gfx_Frame *parent);
				Gfx_Frame * GetNext();
				VOID        SetNext(Gfx_Frame *next);

			// ---- child
				Gfx_Frame * AddChild(std::string name);
				Gfx_Frame * GetFirst();

				VOID SetFrameRect(
						Gfx_Frame::TYPE type,
						Gfx_Frame::Rectangle *rect
					);
				VOID GetFrameRect(
						Gfx_Frame::TYPE type,
						Gfx_Frame::Rectangle *rect
					);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Frame;
				std::unique_ptr<Pimpl_Gfx_Frame> pimpl_gfx_frame;

		};


}


////////////////////////////////////////////////////////////////////////////////