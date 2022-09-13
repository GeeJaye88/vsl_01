////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_component.h ----------
/*!
\file vsl_gfx_element_component.h
\brief Interface to the Gfx_Element_Component class
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_kandinsky.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---- "forward" declarations
		class Gfx_Element;
		class Gfx_Element_Component;

	// ---- kandinsky interface callbacks
		struct Gfx_Kandinsky_Interface_Callbacks
		{
			HRESULT(*kandinsky_interface_append_parameters) (Gfx_Element *) = NULL;
			HRESULT(*kandinsky_interface_config_and_create) (Gfx_Element_Component *) = NULL;
		};

	// ---- enum component buffer bitmasks
		typedef enum Gfx_Component_Buffer_Bitmasks
		{ 
			NONE           = 0,
			KANDINSKY      = 1,
			VERTEX_BUFFER  = 2,
			INDEX_BUFFER   = 4
		} Gfx_Component_Buffer_Bitmasks;

	// ---- element component interface to the Kandinsky system
		class Gfx_Element_Component
		{

		public:

			// ---- cdtor
				Gfx_Element_Component(VOID);
				~Gfx_Element_Component();

			// ---- get
				Gfx_Kandinsky                     *GetKandinsky(VOID);
				LPDIRECT3DVERTEXBUFFER9           *GetVertexBuffer(VOID);
				LPDIRECT3DINDEXBUFFER9            *GetIndexBuffer(VOID);
				UINT                               GetConfigBitmask(VOID);
				Gfx_Kandinsky_Interface_Callbacks *GetKandinskyInterfaceCallbacks(VOID);

			// ---- set
				VOID SetVertexBuffer(LPDIRECT3DVERTEXBUFFER9 vertex_buffer);
				VOID SetIndexBuffer(LPDIRECT3DINDEXBUFFER9 index_buffer);
				VOID SetConfigBitmask(UINT config_bitmask);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Element_Component;
				std::unique_ptr<Pimpl_Gfx_Element_Component> pimpl_gfx_element_component;

		};

}

////////////////////////////////////////////////////////////////////////////////