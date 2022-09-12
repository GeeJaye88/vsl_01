////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_config.cpp ----------
/*!
\file vsl_gfx_kandinsky_config.cpp
\brief Implementation of the Gfx_Kandinsky_Config class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_kandinsky_config.h"
	#include "../../vsl_library/header/vsl_gfx_element_component.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Kandinsky_Config::Pimpl_Gfx_Kandinsky_Config
{

public:

	// ---- cdtor
		Pimpl_Gfx_Kandinsky_Config(VOID) { ; }
		~Pimpl_Gfx_Kandinsky_Config() { ; }

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------

// ---------- ctor ----------

Gfx_Kandinsky_Config::Gfx_Kandinsky_Config(VOID) : pimpl_gfx_kandinsky_config(new Pimpl_Gfx_Kandinsky_Config)
{
	;
}

Gfx_Kandinsky_Config::~Gfx_Kandinsky_Config()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky_Config::GetComponentTypeId(const std::string& name)
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


HRESULT Gfx_Kandinsky_Config::AppendColourParameters(
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


HRESULT Gfx_Kandinsky_Config::AppendTransformParameters(
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

HRESULT Gfx_Kandinsky_Config::Cuboid_VBO_Config_Kandinsky_Parameters(
		Gfx_Element *component_param_group
	)
{
	AppendColourParameters(component_param_group);
	AppendTransformParameters(component_param_group);

	return SUCCESS_OK;
}

HRESULT Gfx_Kandinsky_Config::Cuboid_VBO_Config_Kandinsky_Component(
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

HRESULT Gfx_Kandinsky_Config::Cuboid_VIBO_Config_Kandinsky_Parameters(
		Gfx_Element *component_param_group
	)
{
	AppendColourParameters(component_param_group);
	AppendTransformParameters(component_param_group);

	return SUCCESS_OK;
}

HRESULT Gfx_Kandinsky_Config::Cuboid_VIBO_Config_Kandinsky_Component(
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

HRESULT Gfx_Kandinsky_Config::PyRhoDo_VBO_Config_Kandinsky_Parameters(
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

HRESULT Gfx_Kandinsky_Config::PyRhoDo_VBO_Config_Kandinsky_Component(
		Gfx_Element_Component *gfx_element_component
	)
{

	Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();
	kandinsky->PyRhoDo_VBO_Config();
	kandinsky->PyRhoDo_VBO_Create();

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


HRESULT Gfx_Kandinsky_Config::GetCallbacks(
		Gfx_Element_Component *gfx_element_component,
		Gfx_Config_Callbacks *gfx_config_callbacks
	)
{

	Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();
	UINT component_type = 0;
	HRESULT hr = kandinsky->Get(Gfx_Kandinsky_Param::COMPONENT_TYPE, &component_type);

	switch (component_type)
	{
		case Gfx_Kandinsky_Component::CUBOID_VBO:
			{
				gfx_config_callbacks->config_kandinsky_parameters = Cuboid_VBO_Config_Kandinsky_Parameters;
				gfx_config_callbacks->config_kandinsky_components = Cuboid_VBO_Config_Kandinsky_Component;
			}
			break;
		case Gfx_Kandinsky_Component::Cuboid_VIBO:
			{
				gfx_config_callbacks->config_kandinsky_parameters = Cuboid_VIBO_Config_Kandinsky_Parameters;
				gfx_config_callbacks->config_kandinsky_components = Cuboid_VIBO_Config_Kandinsky_Component;
			}
			break;
		case Gfx_Kandinsky_Component::PYRHODO_VBO:
			{
				gfx_config_callbacks->config_kandinsky_parameters = PyRhoDo_VBO_Config_Kandinsky_Parameters;
				gfx_config_callbacks->config_kandinsky_components = PyRhoDo_VBO_Config_Kandinsky_Component;
			}
			break;
		default:
			{
				gfx_config_callbacks->config_kandinsky_parameters = NULL;
				gfx_config_callbacks->config_kandinsky_components = NULL;
			}
			return SUCCESS_FAULT;
	}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////

