////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_io.h ----------
/*!
\file vsl_gfx_element_io.h
\brief vs lite Gfx_Element_IO class interface
\author Gareth Edwards
\note 
*/  

#pragma once

// ---- framework
	#include "../../vsl_system/header/vsl_include.h"

////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---------- Gfx_Element_IO class interface ----------
		class Gfx_Element_IO
		{

		public:

			// ---- cdtor
				Gfx_Element_IO();
				~Gfx_Element_IO();

			// ---- big picture
				HRESULT Read(std::string filename);
				HRESULT Write(std::string filename);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Element_IO;
				std::unique_ptr<Pimpl_Gfx_Element_IO> pimpl_gfx_element_io;

		};

}


////////////////////////////////////////////////////////////////////////////////