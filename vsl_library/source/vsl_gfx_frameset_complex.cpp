////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_panel.cpp ----------
/*!
\file vsl_gfx_panel.cpp
\brief Implementation of the Gfx_Panel class
\author Gareth Edwards 
*/


#include "../header/vsl_gfx_frameset.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- Pimpl_Gfx_Frame_Style : base class & private implementation  ----------

class Gfx_Frame_Style::Pimpl_Gfx_Frame_Style
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Frame_Style() { ; }
		~Pimpl_Gfx_Frame_Style() { ; }

	private:

};

// ---------- Gfx_Frame_Style : class derived from Pimpl_Gfx_Frame_Style ----------

// ---- cdtor
Gfx_Frame_Style::Gfx_Frame_Style(std::string name) : pimpl_gfx_frame_style(new Pimpl_Gfx_Frame_Style)
	{
		// pimpl_gfx_frame_style->SetName(name);
	}
Gfx_Frame_Style::~Gfx_Frame_Style() { }


////////////////////////////////////////////////////////////////////////////////


// ---------- Pimpl_Gfx_Frameset : base class & private implementation  ----------

class Gfx_Frameset::Pimpl_Gfx_Frameset
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Frameset() { ; }
		~Pimpl_Gfx_Frameset() { ; }

	// ---- frames
		Gfx_Frame *AddFrame(std::string name)
		{
			Gfx_Frame *frame = new Gfx_Frame(name);
			if (first == NULL)
			{
				first = last = frame;
			}
			else
			{
				last->SetNext(frame);
				last = frame;
			}
			return frame;
		}
		Gfx_Frame *GetFirst() { return first; }

		HRESULT SetDimensions(UINT width, UINT height)
			{ this->width = width; this->height = height; return SUCCESS_OK; }
		UINT GetWidth()
			{ return width; }
		UINT GetHeight()
			{ return height; }

		VOID SetGfxLog(Gfx_Log *gfx_log) { this->gfx_log = gfx_log; }
		Gfx_Log *GetGfxLog() { return this->gfx_log;  }

	private:

		Gfx_Frame *first = NULL;
		Gfx_Frame *last = NULL;
		UINT width = 0, height = 0;
		vsl_library::Gfx_Log * gfx_log;

};

// ---------- Gfx_Frameset : class derived from Pimpl_Gfx_Frameset ----------

// ---- cdtor
	Gfx_Frameset::Gfx_Frameset() : pimpl_gfx_frameset(new Pimpl_Gfx_Frameset)
		{ }
	Gfx_Frameset::~Gfx_Frameset()
		{ }

// ---- frames
	Gfx_Frame * Gfx_Frameset::AddFrame(std::string name)
		{ return pimpl_gfx_frameset->AddFrame(name); }
	Gfx_Frame * Gfx_Frameset::GetFirst()
		{ return pimpl_gfx_frameset->GetFirst(); }

// ---- dimensions
	HRESULT Gfx_Frameset::SetDimensions(UINT width, UINT height)
		{ return pimpl_gfx_frameset->SetDimensions(width, height); }
	UINT Gfx_Frameset::GetWidth()
		{ return pimpl_gfx_frameset->GetWidth(); }
	UINT Gfx_Frameset::GetHeight()
		{ return pimpl_gfx_frameset->GetHeight(); }

// ---- calculate frameset
	HRESULT Gfx_Frameset::Setup()
	{
		Gfx_Log *gfx_log = pimpl_gfx_frameset->GetGfxLog();

		Gfx_Frame *frame = GetFirst();
		gfx_log->Write("Frameset :");
		while (frame != NULL)
		{
			std::string msg = "Frame : " + frame->GetName();
			gfx_log->Write(msg);
			Gfx_Panel *panel = frame->GetFirst();
			while (panel != NULL)
			{
				std::string msg = "Panel : " + panel->GetName();
				gfx_log->Write(msg);
				panel = panel->GetNext();
			}
			frame = frame->GetNext();
		}

		return SUCCESS_OK;
	}

// ---- housekeeping
	HRESULT Gfx_Frameset::SetGfxLog(Gfx_Log *gfx_log)
	{
		if (gfx_log == NULL) return ERROR_UNEXPECTED;
		pimpl_gfx_frameset->SetGfxLog(gfx_log);
		return SUCCESS_OK;
	}


////////////////////////////////////////////////////////////////////////////////


// ---------- Pimpl_Gfx_Frame : base class & private implementation  ----------

class Gfx_Frame::Pimpl_Gfx_Frame
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Frame() { ; }
		~Pimpl_Gfx_Frame() { ; }

	// ---- panels
		Gfx_Panel *AddPanel(std::string name)
		{
			Gfx_Panel *panel = new Gfx_Panel(name);
			if (first == NULL)
			{
				first = last = panel;
			}
			else
			{
				last->SetNext(panel);
				last = panel;
			}
			return panel;
		}
		Gfx_Panel *GetFirst() { return first; }

	// ---- name
		std::string GetName() { return name; }
		VOID SetName(std::string name) { this->name = name; }

	// ---- link
		Gfx_Frame * GetNext() { return next; };
		VOID SetNext(Gfx_Frame *next) { this->next = next; };

	private:

		Gfx_Panel *first = NULL;
		Gfx_Panel *last = NULL;

		Gfx_Frame *next = NULL;
		std::string name;

};

// ---------- Gfx_Frame : class derived from Pimpl_Gfx_Frame ----------

// ---- cdtor
	Gfx_Frame::Gfx_Frame(std::string name) : pimpl_gfx_frame(new Pimpl_Gfx_Frame)
	{
		pimpl_gfx_frame->SetName(name);
	}
	Gfx_Frame::~Gfx_Frame() { }

// ---- name
	std::string Gfx_Frame::GetName() { return pimpl_gfx_frame->GetName(); }
	VOID Gfx_Frame::SetName(std::string name) { pimpl_gfx_frame->SetName(name); }

// ---- link
	Gfx_Frame * Gfx_Frame::GetNext() { return pimpl_gfx_frame->GetNext(); }
	VOID Gfx_Frame::SetNext(Gfx_Frame *next) { pimpl_gfx_frame->SetNext(next); }

// ---- panels
	Gfx_Panel * Gfx_Frame::AddPanel(std::string name)
		{ return pimpl_gfx_frame->AddPanel(name); }
	Gfx_Panel * Gfx_Frame::GetFirst()
		{ return pimpl_gfx_frame->GetFirst(); }


////////////////////////////////////////////////////////////////////////////////


// ---------- Pimpl_Gfx_Panel : base class & private implementation  ----------

class Gfx_Panel::Pimpl_Gfx_Panel
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Panel() { ; }
		~Pimpl_Gfx_Panel() { ; }

	// ---- name
		std::string GetName() { return name; }
		VOID SetName(std::string name) { this->name = name; }

	// ---- link
		Gfx_Panel * GetNext() { return next; };
		VOID SetNext(Gfx_Panel *next) { this->next = next; };

	private:

		Gfx_Panel *next = NULL;
		std::string name;

};

// ---------- Gfx_Panel : class derived from Pimpl_Gfx_Panel ----------

// ---- cdtor
	Gfx_Panel::Gfx_Panel(std::string name) : pimpl_gfx_panel(new Pimpl_Gfx_Panel)
	{
		pimpl_gfx_panel->SetName(name);
	}
	Gfx_Panel::~Gfx_Panel() { }

// ---- name
	std::string Gfx_Panel::GetName() { return pimpl_gfx_panel->GetName(); }
	VOID Gfx_Panel::SetName(std::string name) { pimpl_gfx_panel->SetName(name); }

// ---- link
	Gfx_Panel * Gfx_Panel::GetNext() { return pimpl_gfx_panel->GetNext(); }
	VOID Gfx_Panel::SetNext(Gfx_Panel *next) { pimpl_gfx_panel->SetNext(next); }



////////////////////////////////////////////////////////////////////////////////