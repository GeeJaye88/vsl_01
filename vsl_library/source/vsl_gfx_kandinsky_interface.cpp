////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_interface.cpp ----------
/*!
\file vsl_gfx_kandinsky_interface.cpp
\brief Implementation of the Gfx_Kandinsky_Config class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_kandinsky_interface.h"
	#include "../../vsl_library/header/vsl_gfx_element_component.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Kandinsky_Interface::Pimpl_Gfx_Kandinsky_Interface
{

public:

	// ---- cdtor
		Pimpl_Gfx_Kandinsky_Interface(VOID) { ; }
		~Pimpl_Gfx_Kandinsky_Interface() { ; }

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------

// ---------- ctor ----------

Gfx_Kandinsky_Interface::Gfx_Kandinsky_Interface(VOID) : pimpl_gfx_kandinsky_interface(new Pimpl_Gfx_Kandinsky_Interface)
{
	;
}

Gfx_Kandinsky_Interface::~Gfx_Kandinsky_Interface()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky_Interface::GetComponentTypeId(const std::string& name)
{

	if (name == "Cuboid_VBO")
	{
		return Gfx_Kandinsky_Component::CUBOID_VBO;
	}
	else if (name == "Cuboid_VIBO")
	{
		return Gfx_Kandinsky_Component::Cuboid_VIBO;
	}
	else if (name == "PyRhoDo_VBO")
	{
		return Gfx_Kandinsky_Component::PYRHODO_VBO;
	}
	else
	{
		return ERROR_FAIL;
	}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky_Interface::AppendColourParameters(
		Gfx_Element *component_param_group
	)
{

	Gfx_Element *colour = component_param_group->Append("Colour");
		colour->Append("Red", "1", 0);
		colour->Append("Green", "1", 0);
		colour->Append("Blue", "1", 0);
		colour->Append("Alpha", "1", 0);

	return SUCCESS_OK;
}


HRESULT Gfx_Kandinsky_Interface::AppendTransformParameters(
		Gfx_Element *component_param_group
	)
{

	Gfx_Element *dimension = component_param_group->Append("Dimension");
		dimension->Append("Width", "5", 0);
		dimension->Append("Height", "5", 0);
		dimension->Append("Depth", "5", 0);

	Gfx_Element *fx = component_param_group->Append("FX");
		fx->Append("Explode", "1.0", 0);

	Gfx_Element *shift = component_param_group->Append("Shift");
		shift->Append("X", "0", 0);
		shift->Append("Y", "0", 0);
		shift->Append("Z", "0", 0);

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- cuboid - vertex buffer vesion ----------

HRESULT Gfx_Kandinsky_Interface::Cuboid_VBO_Config_Kandinsky_Parameters(
		Gfx_Element *component_param_group
	)
{
	AppendColourParameters(component_param_group);
	AppendTransformParameters(component_param_group);

	return SUCCESS_OK;
}

HRESULT Gfx_Kandinsky_Interface::Cuboid_VBO_Config_Kandinsky_Component(
		Gfx_Element_Component *gfx_element_component
	)
{

	Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();

	kandinsky->Cuboid_VBO_Config();
	kandinsky->Cuboid_VBO_Create();

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- cuboid - index & vertex buffer version ----------

HRESULT Gfx_Kandinsky_Interface::Cuboid_VIBO_Config_Kandinsky_Parameters(
		Gfx_Element *component_param_group
	)
{
	AppendColourParameters(component_param_group);
	AppendTransformParameters(component_param_group);

	return SUCCESS_OK;
}

HRESULT Gfx_Kandinsky_Interface::Cuboid_VIBO_Config_Kandinsky_Component(
		Gfx_Element_Component *gfx_element_component
	)
{

	Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();
	kandinsky->Cuboid_VIBO_Config();
	kandinsky->Cuboid_VIBO_Create();

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ----------  pyramidal rhombic dodecahedron - vertex buffer version ----------

HRESULT Gfx_Kandinsky_Interface::PyRhoDo_VBO_Config_Kandinsky_Parameters(
		Gfx_Element *component_param_group
	)
{
	AppendColourParameters(component_param_group);

	Gfx_Element *value = component_param_group->Append("Dimension");
		value->Append("Size", "4", 0);
		value->Append("Move", "0", 0);

	Gfx_Element *fx = component_param_group->Append("FX");
		fx->Append("Explode", "0.0", 0);

	Gfx_Element *shift = component_param_group->Append("Shift");
		shift->Append("X", "0", 0);
		shift->Append("Y", "0", 0);
		shift->Append("Z", "0", 0);

	return SUCCESS_OK;
}

HRESULT Gfx_Kandinsky_Interface::PyRhoDo_VBO_Config_Kandinsky_Component(
		Gfx_Element_Component *gfx_element_component
	)
{

	Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();
	kandinsky->PyRhoDo_VBO_Config();
	kandinsky->PyRhoDo_VBO_Create();

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky_Interface::GetCallbacks(
		Gfx_Element_Component *gfx_element_component,
		Gfx_Kandinsky_Interface_Callbacks *gfx_kandinsky_interface_callbacks
	)
{

	Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();
	UINT component_type = 0;
	HRESULT hr = kandinsky->Get(Gfx_Kandinsky_Param::COMPONENT_TYPE, &component_type);

	switch (component_type)
	{
		case Gfx_Kandinsky_Component::CUBOID_VBO:
			{
				gfx_kandinsky_interface_callbacks->kandinsky_interface_append_parameters = Cuboid_VBO_Config_Kandinsky_Parameters;
				gfx_kandinsky_interface_callbacks->kandinsky_interface_config_and_create = Cuboid_VBO_Config_Kandinsky_Component;
			}
			break;
		case Gfx_Kandinsky_Component::Cuboid_VIBO:
			{
				gfx_kandinsky_interface_callbacks->kandinsky_interface_append_parameters = Cuboid_VIBO_Config_Kandinsky_Parameters;
				gfx_kandinsky_interface_callbacks->kandinsky_interface_config_and_create = Cuboid_VIBO_Config_Kandinsky_Component;
			}
			break;
		case Gfx_Kandinsky_Component::PYRHODO_VBO:
			{
				gfx_kandinsky_interface_callbacks->kandinsky_interface_append_parameters = PyRhoDo_VBO_Config_Kandinsky_Parameters;
				gfx_kandinsky_interface_callbacks->kandinsky_interface_config_and_create = PyRhoDo_VBO_Config_Kandinsky_Component;
			}
			break;
		default:
			{
				gfx_kandinsky_interface_callbacks->kandinsky_interface_append_parameters = NULL;
				gfx_kandinsky_interface_callbacks->kandinsky_interface_config_and_create = NULL;
			}
			return SUCCESS_FAULT;
	}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////

