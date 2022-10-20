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


// ---------- Pimpl_Gfx_Frameset : base class & private implementation  ----------

class Gfx_Frameset::Pimpl_Gfx_Frameset
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Frameset() { ; }
		~Pimpl_Gfx_Frameset() { ; }

	// ---- link
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

	// ---- stuff
		HRESULT SetDimensions(UINT width, UINT height)
			{ this->width = width; this->height = height; return SUCCESS_OK; }
		UINT GetWidth()
			{ return width; }
		UINT GetHeight()
			{ return height; }

		VOID SetGfxLog(Gfx_Log *gfx_log) { this->gfx_log = gfx_log; }
		Gfx_Log *GetGfxLog() { return this->gfx_log;  }

	private:

	// ---- link
		Gfx_Frame *first = NULL;
		Gfx_Frame *last = NULL;

	// ---- stuff
		UINT width = 0, height = 0;
		vsl_library::Gfx_Log * gfx_log;

};

// ---------- Gfx_Frameset : class derived from Pimpl_Gfx_Frameset ----------

// ---- cdtor
	Gfx_Frameset::Gfx_Frameset() : pimpl_gfx_frameset(new Pimpl_Gfx_Frameset)
		{ }
	Gfx_Frameset::~Gfx_Frameset()
		{ }

// ---- link
	Gfx_Frame * Gfx_Frameset::AddFrame(std::string name)
		{ return pimpl_gfx_frameset->AddFrame(name); }
	Gfx_Frame * Gfx_Frameset::GetFirst()
		{ return pimpl_gfx_frameset->GetFirst(); }

// ---- stuff: dimensions
	HRESULT Gfx_Frameset::SetDimensions(UINT width, UINT height)
		{ return pimpl_gfx_frameset->SetDimensions(width, height); }
	UINT Gfx_Frameset::GetWidth()
		{ return pimpl_gfx_frameset->GetWidth(); }
	UINT Gfx_Frameset::GetHeight()
		{ return pimpl_gfx_frameset->GetHeight(); }

// ---- stuff: calculate frameset
	HRESULT Gfx_Frameset::Setup()
	{
		Gfx_Log *gfx_log = pimpl_gfx_frameset->GetGfxLog();
		Gfx_Frame *frame = GetFirst();
		gfx_log->Write("Frameset :");
		while (frame != NULL)
		{
			std::string msg = "Frame : " + frame->GetName();
			gfx_log->Write(msg);
			Gfx_Frame *child_frame = frame->GetFirstChildFrame();
			while (child_frame != NULL)
			{
				std::string msg = "Child Frame : " + child_frame->GetName();
				gfx_log->Write(msg);
				child_frame = child_frame->GetNext();
			}
			frame = frame->GetNext();
		}
		return SUCCESS_OK;
	}

// ---- stuff: log & report
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

	// ---- link
		Gfx_Frame *AddChildFrame(std::string name)
		{
			Gfx_Frame *child_frame = new Gfx_Frame(name);
			if (first_child == NULL)
			{
				first_child = last_child = child_frame;
			}
			else
			{
				last_child->SetNext(child_frame);
				last_child = child_frame;
			}
			return child_frame;
		}
		Gfx_Frame * GetNext() { return next; };
		VOID SetNext(Gfx_Frame *next) { this->next = next; };
		Gfx_Frame *GetFirstChildFrame() { return first_child; }

	// ---- stuff
		std::string GetName() { return name; }
		VOID SetName(std::string name) { this->name = name; }

		VOID SetBox(Gfx_Box *box)
			{ this->box = *(box); }
		VOID SetSpacing(Gfx_Spacing_Type type, Gfx_Spacing *spacing)
			{
				switch (type)
				{
					case MARGIN: this->margin = *(spacing); break;
					case BORDER: this->border = *(spacing); break;
					case PADDING: this->padding = *(spacing); break;
					default:
						break;
				}
			}
		VOID GetContent(Gfx_Content *content)
		{
			*(content) = this->content;
		}

	private:

	// ---- link
		Gfx_Frame *first_child = NULL;
		Gfx_Frame *last_child = NULL;

	// ---- stuff
		Gfx_Frame *next = NULL;
		std::string name;

	// ---- FLOAT LTRB relative to parent frame (e.g., 0.1,0.1,0.5,0.5)
		Gfx_Box  box = { 0, 0, 1, 1 };

	// ---- UINT LTRB bounded by box in screen ordinates
		Gfx_Spacing margin  = { 0, 0, 0, 0 };
		Gfx_Spacing border  = { 0, 0, 0, 0 };
		Gfx_Spacing padding = { 0, 0, 0, 0 };

	// ---- UINT LTRB bounded by spacing in screen ordinates
		Gfx_Content content;
		float fred = 0;

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

// ---- link: child frames
	Gfx_Frame * Gfx_Frame::AddChildFrame(std::string name)
		{ return pimpl_gfx_frame->AddChildFrame(name); }
	Gfx_Frame * Gfx_Frame::GetFirstChildFrame()
		{ return pimpl_gfx_frame->GetFirstChildFrame(); }

// ---- set stuff
	VOID Gfx_Frame::SetBox(Gfx_Box *box)
		{ pimpl_gfx_frame->SetBox(box); }
	VOID Gfx_Frame::SetSpacing(Gfx_Spacing_Type type, Gfx_Spacing *spacing)
		{ pimpl_gfx_frame->SetSpacing(type, spacing); }

// ---- get stuff
	VOID Gfx_Frame::GetContent(Gfx_Content *content)
		{ pimpl_gfx_frame->GetContent(content); }


////////////////////////////////////////////////////////////////////////////////