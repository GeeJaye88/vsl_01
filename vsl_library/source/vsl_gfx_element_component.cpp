////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_Gfx_Element_Component.cpp ----------
/*!
\file vsl_Gfx_Element_Component.cpp
\brief Implementation of the Gfx_Element_Component class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element_component.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky_interface.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Element_Component::Pimpl_Gfx_Element_Component
{

public:

	// ---- cdtor
		Pimpl_Gfx_Element_Component(VOID)
		{
			;
		}

		~Pimpl_Gfx_Element_Component()
		{
			if (index_buffer != 0) index_buffer->Release();
			if (vertex_buffer != 0) vertex_buffer->Release();
		}

	// ---- properties
		Gfx_Kandinsky_Interface_Callbacks component_kandinsky_callbacks;

	// ---- config
		UINT config_bitmask = 0;

	// ---- buffers
		Gfx_Kandinsky kandinsky;
		LPDIRECT3DVERTEXBUFFER9 vertex_buffer = NULL;
		LPDIRECT3DINDEXBUFFER9  index_buffer = NULL;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---------- ctor ----------

Gfx_Element_Component::Gfx_Element_Component(VOID) : pimpl_gfx_element_component(new Pimpl_Gfx_Element_Component)
{
	;
}

Gfx_Element_Component::~Gfx_Element_Component()
{
	;
}


// ---------- get ----------

Gfx_Kandinsky *Gfx_Element_Component::GetKandinsky(VOID)
{
	return &pimpl_gfx_element_component->kandinsky;
}

LPDIRECT3DVERTEXBUFFER9 *Gfx_Element_Component::GetVertexBuffer(VOID)
{
	return &pimpl_gfx_element_component->vertex_buffer;
}

LPDIRECT3DINDEXBUFFER9 *Gfx_Element_Component::GetIndexBuffer(VOID)
{
	return &pimpl_gfx_element_component->index_buffer;
}

UINT Gfx_Element_Component::GetConfigBitmask(VOID)
{
	return pimpl_gfx_element_component->config_bitmask;
}

Gfx_Kandinsky_Interface_Callbacks *Gfx_Element_Component::GetKandinskyInterfaceCallbacks(VOID)
{
	return &pimpl_gfx_element_component->component_kandinsky_callbacks;
}



// ---------- set ----------

VOID Gfx_Element_Component::SetVertexBuffer(LPDIRECT3DVERTEXBUFFER9 vertex_buffer)
{
	pimpl_gfx_element_component->vertex_buffer = vertex_buffer;
}

VOID Gfx_Element_Component::SetIndexBuffer(LPDIRECT3DINDEXBUFFER9 index_buffer)
{
	pimpl_gfx_element_component->index_buffer = index_buffer;
}

VOID Gfx_Element_Component::SetConfigBitmask(UINT config_bitmask)
{
	pimpl_gfx_element_component->config_bitmask = config_bitmask;
}



////////////////////////////////////////////////////////////////////////////////
