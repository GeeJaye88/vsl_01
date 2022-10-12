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

		
	// --- "forward" declaration
		class Gfx_Panel;


	// ---------- Gfx_Frame class interface ----------
		class Gfx_Frame
		{

		public:

			// ---- cdtor
				Gfx_Frame();
				~Gfx_Frame();

				Gfx_Panel * AddPanel(std::string name);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Frame;
				Pimpl_Gfx_Frame *pimpl_gfx_frame;

		};


	// ---------- Gfx_Panel class interface ----------
		class Gfx_Panel
		{

		public:

			// ---- cdtor
				Gfx_Panel(std::string name);
				~Gfx_Panel();

				Gfx_F_Rectangle  panel_proportional;
				Gfx_UI_Rectangle panel;
				Gfx_UI_Rectangle margin;
				Gfx_UI_Rectangle border;
				Gfx_UI_Rectangle padding;

		private:
			std::string name;

		public:

			//Gfx_Panel() : name(0) {}
			//Gfx_Panel(int n) : name(n) {}

			std::string getName() const { return name; }
			void setName(std::string name) { this->name = name; }


		private:

			// ---- private implementation
				class Pimpl_Gfx_Panel;
				Pimpl_Gfx_Panel *pimpl_gfx_panel;

		};

}


////////////////////////////////////////////////////////////////////////////////