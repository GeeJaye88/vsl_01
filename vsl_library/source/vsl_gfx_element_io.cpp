////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_io.cpp ----------
/*!
\file vsl_gfx_element_io.cpp
\brief Implementation of the Gfx_Element_IO class
\author Gareth Edwards
*/


#include "../header/vsl_gfx_element_io.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


// ---------- Private Implementation ----------
class Gfx_Element_IO::Pimpl_Gfx_Element_IO
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Element_IO() {}
		~Pimpl_Gfx_Element_IO() { ; }

	private:

};


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------

Gfx_Element_IO::Gfx_Element_IO() : pimpl_gfx_element_io(new Pimpl_Gfx_Element_IO)
{
	;
}

Gfx_Element_IO::~Gfx_Element_IO()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Element_IO::Read(std::string filename)
{

	return SUCCESS_OK;
}

HRESULT Gfx_Element_IO::Write(std::string filename)
{

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////