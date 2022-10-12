////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_panel.cpp ----------
/*!
\file vsl_gfx_panel.cpp
\brief Implementation of the Gfx_Panel class
\author Gareth Edwards 
*/


#include "../header/vsl_gfx_panel.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of Gfx_Frame::Pimpl_Gfx_Frame class ----------

class Gfx_Frame::Pimpl_Gfx_Frame
{

	public:

		// ---- constructor/destructor ----
		Pimpl_Gfx_Frame() { ; }
		~Pimpl_Gfx_Frame() { ; }

		Gfx_Panel * GetPanel(std::string name)
		{
			for (std::list<Gfx_Panel>::iterator it = list_of_gfx_panels.begin(); it != list_of_gfx_panels.end(); it++)
			{
				// ---- get a pointer (this is the important bit!)
				if ((it->getName()) == name)
					return &*it;
			}
			return 0;
		}

		Gfx_Panel * AddPanel(std::string name)
		{
			list_of_gfx_panels.push_back(name);
			return &list_of_gfx_panels.back();
		}

	private:

		std::list < Gfx_Panel > list_of_gfx_panels;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- cdtor ----------

Gfx_Frame::Gfx_Frame()
{
	;
}

Gfx_Frame::~Gfx_Frame()
{
	;
}

Gfx_Panel * Gfx_Frame::AddPanel(std::string name)
{
	Gfx_Panel *fred = pimpl_gfx_frame->AddPanel(name);
	return fred;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of Gfx_Panel::Pimpl_Gfx_Panel class ----------

class Gfx_Panel::Pimpl_Gfx_Panel
{

public:

	// ---- constructor/destructor ----
		Pimpl_Gfx_Panel() { ; }
		~Pimpl_Gfx_Panel() { ; }

	private:

};


////////////////////////////////////////////////////////////////////////////////


// ---------- cdtor ----------

Gfx_Panel::Gfx_Panel(std::string name)
{
	setName(name);
}

Gfx_Panel::~Gfx_Panel()
{
	;
}


////////////////////////////////////////////////////////////////////////////////