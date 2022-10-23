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

	// ---- links
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
				frame->SetDimensions(this->width, this->height);
				return frame;
			}
		Gfx_Frame *GetFirst()
			{
				return first;
			}

	// ---- dimensions
		HRESULT SetDimensions(UINT width, UINT height)
			{
				this->width = width;
				this->height = height;
				return SUCCESS_OK;
			}
		UINT GetWidth()
			{
				return width;
			}
		UINT GetHeight()
			{
				return height;
			}

	// ---- calculate

		VOID Calculate(Gfx_Frame *frame)
		{
			auto to_pixel = [](FLOAT prop, FLOAT dimension) -> FLOAT
				{
					FLOAT legal_prop = prop < 0 ? 0 : (prop > 1 ? 1: prop);
					return floorf(legal_prop * dimension + 0.5f);
				};

			auto to_prop = [](FLOAT pixel, FLOAT dimension) -> FLOAT
				{
					FLOAT legal_pixel = pixel < 0 ? 0 : (pixel > dimension ? dimension : pixel);
					return legal_pixel / dimension;
				};

			// ---- parent frame
				Gfx_Frame *parent = frame->GetParent();

			// ---- get/calculate dimensions of this frame
				FLOAT float_width;
				FLOAT float_height;
				if (parent == NULL)
				{
					// ---- no parent - use frameset pixel dimensions
						float_width  = (FLOAT)this->GetWidth(); 
						float_height = (FLOAT)this->GetHeight();
				}
				else
				{
					// ---- use parent frame pixel dimensions
						float_width  = (FLOAT)parent->GetWidth();
						float_height = (FLOAT)parent->GetHeight();

					// ---- convert parent frame proportional inside dimensions to pixels
						Gfx_Frame::Rectangle parent_inside;
						parent->GetFrameRect(Gfx_Frame::TYPE::INSIDE, &parent_inside);
						FLOAT left   = to_pixel(parent_inside.left,   float_width);
						FLOAT top    = to_pixel(parent_inside.top,    float_height);
						FLOAT right  = to_pixel(parent_inside.right,  float_width);
						FLOAT bottom = to_pixel(parent_inside.bottom, float_height);

					// ---- set frame pixel dimensions
						float_width  = right - left;
						float_height = bottom - top;
						frame->SetDimensions((UINT)(float_width), (UINT)(float_height));
				}


			// ---- get outside frame proportions & convert to inside pixels
				Gfx_Frame::Rectangle outside;
				Gfx_Frame::Rectangle inside;
				frame->GetFrameRect(Gfx_Frame::TYPE::OUTSIDE, &outside);
				inside.left   = to_pixel(outside.left,   float_width);
				inside.top    = to_pixel(outside.top,    float_height);
				inside.right  = to_pixel(outside.right,  float_width);
				inside.bottom = to_pixel(outside.bottom, float_height);

			// ---- add +/- spacingn to inside pixels
				Gfx_Frame::Rectangle r[3];
				frame->GetFrameRect(Gfx_Frame::TYPE::MARGIN,  &r[0]);
				frame->GetFrameRect(Gfx_Frame::TYPE::BORDER,  &r[1]);
				frame->GetFrameRect(Gfx_Frame::TYPE::PADDING, &r[2]);
				for (UINT i = 0; i < 3; i++)
				{
					inside.left   += r[i].left;
					inside.top    += r[i].top;
					inside.right  -= r[i].right;
					inside.bottom -= r[i].bottom;
				}

			// ---- convert inside pixels to inside proportions
				inside.left   = to_prop(inside.left,   float_width);
				inside.top    = to_prop(inside.top,    float_height);
				inside.right  = to_prop(inside.right,  float_width);
				inside.bottom = to_prop(inside.bottom, float_height);

			// ---- store inside proportions
				frame->SetFrameRect(Gfx_Frame::TYPE::INSIDE, &inside);

				float fred = 2;

		}

		HRESULT Setup(Gfx_Frame *frame, UINT level)
			{
				while (frame != NULL)
				{
					Calculate(frame);
					std::string msg = std::string(4*level, ' ') + std::to_string(level) + ": " + frame->GetName();
					gfx_log->Write(msg);
					if (Gfx_Frame *child = frame->GetFirst())
					{
						Setup(child, level+1);
					}
					frame = frame->GetNext();
				}
				return SUCCESS_OK;
			}

		VOID SetGfxLog(Gfx_Log *gfx_log)
			{
				this->gfx_log = gfx_log;
			}
		Gfx_Log *GetGfxLog()
			{
				return this->gfx_log;
			}

	private:

	// ---- links
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

// ---- links
	Gfx_Frame * Gfx_Frameset::AddFrame(std::string name)
		{
			Gfx_Frame *frame = pimpl_gfx_frameset->AddFrame(name);
			frame->SetParent(NULL);
			return frame;
		}
	Gfx_Frame * Gfx_Frameset::GetFirst()
		{
			return pimpl_gfx_frameset->GetFirst();
		}

// ---- set & get dimensions
	HRESULT Gfx_Frameset::SetDimensions(UINT width, UINT height)
		{
			return pimpl_gfx_frameset->SetDimensions(width, height);
		}
	UINT Gfx_Frameset::GetWidth()
		{
			return pimpl_gfx_frameset->GetWidth();
		}
	UINT Gfx_Frameset::GetHeight()
		{
			return pimpl_gfx_frameset->GetHeight();
		}

// ---- calculate
	HRESULT Gfx_Frameset::Setup()
		{
			Gfx_Log *gfx_log = pimpl_gfx_frameset->GetGfxLog();
			Gfx_Frame *frame = GetFirst();
			pimpl_gfx_frameset->Setup(frame, 0);
			return SUCCESS_OK;
		}

// ---- log & report
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

	// ---- general
		std::string GetName()
			{
				return name;
			}
		VOID SetName(std::string name)
			{
				this->name = name;
			}
		VOID SetDimensions(UINT width, UINT height)
			{
				this->width = width;
				this->height = height;
			}
		UINT GetWidth()
			{
				return width;
			}
		UINT GetHeight()
			{
				return height;
			}

	// ---- links
		Gfx_Frame * GetParent()
			{
				return parent;
			};
		VOID SetParent(Gfx_Frame *parent)
			{
				this->parent = parent;
			};

		Gfx_Frame * GetNext()
			{
				return next;
			};
		VOID SetNext(Gfx_Frame *next)
			{
				this->next = next;
			};

	// ---- child links
		Gfx_Frame *AddChild(std::string name)
			{
				Gfx_Frame *child = new Gfx_Frame(name);
				if (first_child == NULL)
				{
					first_child = last_child = child;
				}
				else
				{
					last_child->SetNext(child);
					last_child = child;
				}
				return child;
			}
		Gfx_Frame *GetFirst()
			{
				return first_child;
			}

	// ---- rectangles
		VOID SetFrameRect(
				Gfx_Frame::TYPE type,
				Gfx_Frame::Rectangle  *rect
			)
			{
				switch (type)
				{
					case OUTSIDE : this->outside = *(rect); break;
					case MARGIN  : this->margin  = *(rect); break;
					case BORDER  : this->border  = *(rect); break;
					case PADDING : this->padding = *(rect); break;
					case INSIDE  : this->inside  = *(rect); break;
					default:
						break;
				}
			}

		VOID GetFrameRect(
				Gfx_Frame::TYPE type,
				Gfx_Frame::Rectangle *rect
			)
			{
				switch (type)
				{
					case OUTSIDE : *(rect) = this->outside; break;
					case MARGIN  : *(rect) = this->margin;  break;
					case BORDER  : *(rect) = this->border;  break;
					case PADDING : *(rect) = this->padding; break;
					case INSIDE  : *(rect) = this->inside;  break;
					default:
						break;
				}
			}

	private:

	// ---- general
		std::string name;
		UINT width = 0;
		UINT height = 0;

	// ---- links
		Gfx_Frame *parent = NULL;
		Gfx_Frame *next = NULL;
	
	// ---- child links
		Gfx_Frame *first_child = NULL;
		Gfx_Frame *last_child = NULL;

	// ---- rectangles
		Gfx_Frame::Rectangle  outside;
		Gfx_Frame::Rectangle  margin;
		Gfx_Frame::Rectangle  border;
		Gfx_Frame::Rectangle  padding;
		Gfx_Frame::Rectangle  inside;

};


// ---------- Gfx_Frame : class derived from Pimpl_Gfx_Frame ----------

// ---- cdtor
	Gfx_Frame::Gfx_Frame(std::string name) : pimpl_gfx_frame(new Pimpl_Gfx_Frame)
		{
			pimpl_gfx_frame->SetName(name);
		}
	Gfx_Frame::~Gfx_Frame() { }

// ---- general
	std::string Gfx_Frame::GetName()
		{
			return pimpl_gfx_frame->GetName();
		}
	VOID Gfx_Frame::SetName(std::string name)
		{
			pimpl_gfx_frame->SetName(name);
		}
	VOID Gfx_Frame::SetDimensions(UINT width, UINT height)
		{
			pimpl_gfx_frame->SetDimensions(width,height);
		}
	UINT Gfx_Frame::GetWidth()
		{
			return pimpl_gfx_frame->GetWidth();
		}
	UINT Gfx_Frame::GetHeight()
		{
			return pimpl_gfx_frame->GetHeight();
		}

// ---- link
	Gfx_Frame * Gfx_Frame::GetParent()
		{
			return pimpl_gfx_frame->GetParent();
		}
	VOID Gfx_Frame::SetParent(Gfx_Frame *parent)
		{
			pimpl_gfx_frame->SetParent(parent);
		}

	Gfx_Frame * Gfx_Frame::GetNext()
		{
			return pimpl_gfx_frame->GetNext();
		}
	VOID Gfx_Frame::SetNext(Gfx_Frame *next)
		{
			pimpl_gfx_frame->SetNext(next);
		}

// ---- child links
	Gfx_Frame * Gfx_Frame::AddChild(std::string name)
		{
			Gfx_Frame *child = pimpl_gfx_frame->AddChild(name);
			child->SetParent(this);
			return child;
		}

	Gfx_Frame * Gfx_Frame::GetFirst()
		{ return pimpl_gfx_frame->GetFirst(); }

// ---- set & get bounds
	VOID Gfx_Frame::SetFrameRect(
			Gfx_Frame::TYPE type,
			Gfx_Frame::Rectangle *rect
		)
		{
			pimpl_gfx_frame->SetFrameRect(type, rect);
		}
	VOID Gfx_Frame::GetFrameRect(
			Gfx_Frame::TYPE type,
			Gfx_Frame::Rectangle *rect
		)
		{
			pimpl_gfx_frame->GetFrameRect(type, rect);
		}


////////////////////////////////////////////////////////////////////////////////