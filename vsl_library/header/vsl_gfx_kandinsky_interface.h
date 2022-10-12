////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_interface.h ----------
/*!
\file vsl_gfx_kandinsky_interface.h
\brief Interface to the Kandinsky_Interface class
\author Gareth Edwards
*/ 

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element.h"


////////////////////////////////////////////////////////////////////////////////

namespace vsl_library
{

	// ---- class
		class Gfx_Kandinsky_Interface
		{

		public:

		// ---- cdtor
			Gfx_Kandinsky_Interface(VOID);
			virtual ~Gfx_Kandinsky_Interface();

		// ---- get component id
			HRESULT GetComponentTypeId(const std::string& name);

		// ---- append standard colour & transform parameters
			static HRESULT AppendColourParameters(Gfx_Element *component_param_group);
			static HRESULT AppendTransformParameters(Gfx_Element *component_param_group);

		// ---- Add a New Kandinsky Component

		// ---- cuboid - vertex buffer version 
			static HRESULT Cuboid_VBO_Config_Kandinsky_Parameters(Gfx_Element *component_param_group);
			static HRESULT Cuboid_VBO_Config_Kandinsky_Component(Gfx_Element_Component *gfx_element_component);

		// ---- cuboid - index & vertex buffer version
			static HRESULT Cuboid_VIBO_Config_Kandinsky_Parameters(Gfx_Element *component_param_group);
			static HRESULT Cuboid_VIBO_Config_Kandinsky_Component(Gfx_Element_Component *gfx_element_component);

		// ---- pyramidal rhombic dodecahedron - vertex buffer version
			static HRESULT PyRhoDo_VBO_Config_Kandinsky_Parameters(Gfx_Element *component_param_group);
			static HRESULT PyRhoDo_VBO_Config_Kandinsky_Component(Gfx_Element_Component *gfx_element_component);

			static HRESULT GetCallbacks(
					Gfx_Element_Component *gfx_element_component,
					Gfx_Kandinsky_Interface_Callbacks *gfx_kandinsky_interface_callbacks
				);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Kandinsky_Interface;
				std::unique_ptr<Pimpl_Gfx_Kandinsky_Interface> pimpl_gfx_kandinsky_interface;

		};

}


////////////////////////////////////////////////////////////////////////////////