////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_configure.cpp ----------
/*!
\file vsl_gfx_element_configure.cpp
\brief Implementation of the Gfx_Element_Configure class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element.h"
	#include "../../vsl_library/header/vsl_gfx_element_configure.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Element_Configure::Pimpl_Gfx_Element_Configure
{

public:

	// ---- cdtor
		Pimpl_Gfx_Element_Configure(VOID)
		{
			D3DXMatrixIdentity(&matrix);
		}

		~Pimpl_Gfx_Element_Configure()
		{
			;
		}

	// ---- basic
		BOOL component = FALSE;
		std::string component_name;
		BOOL process   = FALSE;
		BOOL visible   = TRUE;

	// ---- instance
		BOOL instance = FALSE;
		std::string instance_name;
		Gfx_Element *instance_element = NULL;

	// ---- display
		D3DXMATRIX matrix;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---------- cdtor ----------

	Gfx_Element_Configure::Gfx_Element_Configure(VOID) : pimpl_gfx_element_configure(new Pimpl_Gfx_Element_Configure)
	{
		;
	}

	Gfx_Element_Configure::~Gfx_Element_Configure()
	{
		;
	}

// ---------- is ----------

	BOOL Gfx_Element_Configure::IsComponent(VOID)
	{
		return pimpl_gfx_element_configure->component;
	}
	BOOL Gfx_Element_Configure::IsInstance(VOID)
	{
		return pimpl_gfx_element_configure->instance;
	}
	BOOL Gfx_Element_Configure::IsVisible(VOID)
	{
		return pimpl_gfx_element_configure->visible;
	}

// ---------- get ----------

	BOOL Gfx_Element_Configure::GetComponent(VOID)
	{
		return pimpl_gfx_element_configure->component;
	}
	std::string Gfx_Element_Configure::GetComponentName(VOID)
	{
		std::string something = pimpl_gfx_element_configure->component_name;
		return something;
	}
	BOOL Gfx_Element_Configure::GetVisible(VOID)
	{
		return pimpl_gfx_element_configure->visible;
	}

// ---------- get instance----------

	BOOL Gfx_Element_Configure::GetInstance(VOID)
	{
		return pimpl_gfx_element_configure->instance;
	}
	std::string Gfx_Element_Configure::GetInstanceName(VOID)
	{
		std::string something(pimpl_gfx_element_configure->instance_name);
		return something;
	}
	Gfx_Element *Gfx_Element_Configure::GetInstanceElement(VOID)
	{
		return pimpl_gfx_element_configure->instance_element;
	}

// ---------- get matrix ----------

	D3DXMATRIX *Gfx_Element_Configure::GetMatrix(VOID)
	{
		return &pimpl_gfx_element_configure->matrix;
	}

// ---------- set ----------

	VOID Gfx_Element_Configure::SetComponent(BOOL component)
	{
		pimpl_gfx_element_configure->component = component;
	}
	VOID Gfx_Element_Configure::SetComponentName(std::string component_name)
	{
		pimpl_gfx_element_configure->component_name = component_name;
	}
	VOID Gfx_Element_Configure::SetVisible(BOOL visible)
	{
		pimpl_gfx_element_configure->visible = visible;
	}

// ---------- set instance ----------

	VOID Gfx_Element_Configure::SetInstance(BOOL instance)
	{
		pimpl_gfx_element_configure->instance = instance;
	}
	VOID Gfx_Element_Configure::SetInstanceName(std::string instance_name)
	{
		pimpl_gfx_element_configure->instance_name = instance_name;
	}
	VOID Gfx_Element_Configure::SetInstanceElement(Gfx_Element *instance_element)
	{
		pimpl_gfx_element_configure->instance_element = instance_element;
	}

// ---------- get parameter groups ----------
	VOID Gfx_Element_Configure::GetParameterGroups(
			Gfx_Element *parameter_groups
		)
	{
		Gfx_Element *basic = parameter_groups->Append("Basic");
		basic->Append("Component", IsComponent() ? "1" : "0", 0);
		basic->Append("Visible",   IsVisible()   ? "1" : "0", 0);

		Gfx_Element *instance = parameter_groups->Append("Instance");
		instance->Append("Instance", IsInstance() ? "1" : "0", 0);
		instance->Append("Name",     GetInstanceName(), 0);
	}

// ---------- set matrix ----------

	VOID Gfx_Element_Configure::SetMatrix(D3DXMATRIX &matrix)
	{
		(pimpl_gfx_element_configure->matrix) = matrix;
	}


////////////////////////////////////////////////////////////////////////////////
