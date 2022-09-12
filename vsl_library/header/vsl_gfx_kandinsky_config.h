////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_config.h ----------
/*!
\file vsl_gfx_kandinsky_config.h
\brief Interface to the Kandinsky_Config class
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

	// ---- "forward" declarations
		class  Gfx_Element;					// config     - Gfx_Element_Configure
											// coordinate - Gfx_Element_Coordinate
											// component  - Gfx_Element_Component
											// identity   - name & id
											// node

		class  Gfx_Element_Component;		// kandinsky
											// vertex buffer
											// index buffer
											// bitmask
											// config callbacks

		struct Gfx_Config_Callbacks;		// component config kandinsky callbacks


	// ---- class
		class Gfx_Kandinsky_Config
		{

		public:

		// ---- cdtor
			Gfx_Kandinsky_Config(VOID);
			virtual ~Gfx_Kandinsky_Config();

		// ---- get component id
			HRESULT GetComponentTypeId(const std::string& name);

		// ---- append standard colour & transform parameters
			static HRESULT AppendColourParameters(Gfx_Element *component_param_group);
			static HRESULT AppendTransformParameters(Gfx_Element *component_param_group);

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
					Gfx_Config_Callbacks *gfx_config_callbacks
				);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Kandinsky_Config;
				std::unique_ptr<Pimpl_Gfx_Kandinsky_Config> pimpl_gfx_kandinsky_config;

		};

}

////////////////////////////////////////////////////////////////////////////////