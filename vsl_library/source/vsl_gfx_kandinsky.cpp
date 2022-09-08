////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky.cpp ----------
/*!
\file vsl_gfx_kandinsky.cpp
\brief Implementation of the Kandinsky class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_kandinsky.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Kandinsky::Pimpl_Gfx_Kandinsky
{

public:


	// ---- structs - TBD : group link list ?
		struct Parameter
		{
			Parameter *previous = NULL;
			Parameter *next = NULL;
			Parameter *first = NULL;
			Parameter *last = NULL;
			std::string name;
			std::string value;
		};
		Parameter *root = NULL;
		UINT parameter_top = 0;
		static const UINT parameter_max = 32;

	// ---- cdtor
		Pimpl_Gfx_Kandinsky(VOID);
		~Pimpl_Gfx_Kandinsky();

	// ---- methods
		INT HeaderBufferInitialise(UINT);
		INT IndexBufferInitialise(UINT);
		INT VertexBufferInitialise(UINT);

	// ---- properties
		INT   *header_buffer = NULL;
		UINT  *index_buffer = NULL;
		FLOAT *vertex_buffer = NULL;

};

Gfx_Kandinsky::Pimpl_Gfx_Kandinsky::Pimpl_Gfx_Kandinsky(VOID)
{
	HeaderBufferInitialise(32);
	IndexBufferInitialise(0);
	VertexBufferInitialise(1024);
};

Gfx_Kandinsky::Pimpl_Gfx_Kandinsky::~Pimpl_Gfx_Kandinsky()
{
	if (header_buffer != NULL)
	{
		delete[] header_buffer;
		header_buffer = NULL;
	}
	if (index_buffer != NULL)
	{
		delete[] index_buffer;
		index_buffer = NULL;
	}
	if (vertex_buffer != NULL)
	{
		delete[] vertex_buffer;
		vertex_buffer = NULL;
	}
};

INT Gfx_Kandinsky::Pimpl_Gfx_Kandinsky::HeaderBufferInitialise(UINT header_buffer_size = 32)
{

	// ---- delete ?
		if (header_buffer != NULL)
		{
			delete[] header_buffer;
			header_buffer = NULL;
		}

	// ---- allocate
		header_buffer = new INT[header_buffer_size];

	// ---- initialise
		*(header_buffer + HEADER_BUFFER_SIZE) = header_buffer_size;
		*(header_buffer + INDEX_BUFFER_SIZE) = 0;
		*(header_buffer + VERTEX_BUFFER_SIZE) = 0;

	// ---- see enum Gfx_Kandinsky_Component
		*(header_buffer + COMPONENT_TYPE) =  0;

		*(header_buffer + INDEX_COUNT) = 0;

		*(header_buffer + VERTEX_COUNT)  = 0;

	// ---- see enum Gfx_Kandinsky_Vertex_Format
		*(header_buffer + VERTEX_FORMAT) = 0;

	// ---- see enum Gfx_Kandinsky_Primitive_Type
		*(header_buffer + PRIMITIVE_COUNT) = 0;
		*(header_buffer + PRIMITIVE_TYPE)  = 0;

	// ---- build states
		*(header_buffer + INSIDE)      =  0;

	// ---- render states
		*(header_buffer + CULL_MODE)   =  3;
		*(header_buffer + DIMENSION)   =  3;
		*(header_buffer + MATERIAL_ID) = -1;
		*(header_buffer + MATRIX_ID)   = -1;
		*(header_buffer + WIREFRAME)   =  1;

	return 0;
}

INT Gfx_Kandinsky::Pimpl_Gfx_Kandinsky::IndexBufferInitialise(UINT index_buffer_size = 1024)
{

	// ---- delete ?
		if (index_buffer != NULL)
		{
			delete[] index_buffer;
			index_buffer = NULL;
		}

	// ----- allocate
		index_buffer = new UINT[index_buffer_size];

	// ---- initialise header_buffer size
		*(header_buffer + INDEX_BUFFER_SIZE) = index_buffer_size;

	return 0;
}

INT Gfx_Kandinsky::Pimpl_Gfx_Kandinsky::VertexBufferInitialise(UINT vertex_buffer_size = 1024)
{

	// ---- delete ?
		if (vertex_buffer != NULL)
		{
			delete[] vertex_buffer;
			vertex_buffer = NULL;
		}

	// ----- allocate
		vertex_buffer = new FLOAT[vertex_buffer_size];

	// ---- initialise header_buffer size
		*(header_buffer + VERTEX_BUFFER_SIZE) = vertex_buffer_size;

	return 0;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---------- ctor ----------

Gfx_Kandinsky::Gfx_Kandinsky(VOID) : pimpl_gfx_kandinsky(new Pimpl_Gfx_Kandinsky)
{
	;
}

Gfx_Kandinsky::~Gfx_Kandinsky()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- component ----------

BOOL Gfx_Kandinsky::IsComponent()
{
	if (Get(Gfx_Kandinsky_Param::COMPONENT_TYPE) != (UINT)COMPONENT_UNKNOWN)
	{
		return TRUE;
	}
	return FALSE;
}

Gfx_Kandinsky_Component Gfx_Kandinsky::GetComponentType()
{
	return (Gfx_Kandinsky_Component)Get(Gfx_Kandinsky_Param::COMPONENT_TYPE);
}

HRESULT Gfx_Kandinsky::SetComponentType(const std::string& name)
{

	if (name == "Cuboid_VBO")
	{
		Set(Gfx_Kandinsky_Param::COMPONENT_TYPE, Gfx_Kandinsky_Component::CUBOID_VBO);
	}
	else if (name == "Cuboid_VIBO")
	{
		Set(Gfx_Kandinsky_Param::COMPONENT_TYPE, Gfx_Kandinsky_Component::Cuboid_VIBO);
	}
	else if (name == "PyRhoDo_VBO")
	{
		Set(Gfx_Kandinsky_Param::COMPONENT_TYPE, Gfx_Kandinsky_Component::PYRHODO_VBO);
	}
	else
	{
		return ERROR_FAIL;
	}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- param ----------


HRESULT Gfx_Kandinsky::Get(Gfx_Kandinsky_Param token, UINT *token_value)
{
	switch (token)
	{
		case HEADER_BUFFER_SIZE:
		case INDEX_BUFFER_SIZE:
		case VERTEX_BUFFER_SIZE:
		case COMPONENT_TYPE:
		case INDEX_COUNT:
		case VERTEX_COUNT:
		case VERTEX_FORMAT:
		case VERTEX_FORMAT_SIZE:
		case PRIMITIVE_COUNT:
		case PRIMITIVE_TYPE:
		case INSIDE:
		case CULL_MODE:
		case DIMENSION:
		case MATERIAL_ID:
		case MATRIX_ID:
		case WIREFRAME:
			*token_value = *(pimpl_gfx_kandinsky->header_buffer + token);
			break;
		default:
			return ERROR_FAIL;
			break;
	}
	return SUCCESS_OK;
}

UINT Gfx_Kandinsky::Get(Gfx_Kandinsky_Param token)
{
	UINT token_value = 0;
	HRESULT hr = Get(token, &token_value);
	return token_value;
}

HRESULT Gfx_Kandinsky::Set(Gfx_Kandinsky_Param token, UINT token_value)
{
	switch (token)
	{
		case HEADER_BUFFER_SIZE:
		case INDEX_BUFFER_SIZE:
		case VERTEX_BUFFER_SIZE:
		case COMPONENT_TYPE:
		case INDEX_COUNT:
		case VERTEX_COUNT:
		case VERTEX_FORMAT:
		case VERTEX_FORMAT_SIZE:
		case PRIMITIVE_COUNT:
		case PRIMITIVE_TYPE:
		case INSIDE:
		case CULL_MODE:
		case DIMENSION:
		case MATERIAL_ID:
		case MATRIX_ID:
		case WIREFRAME:
			*(pimpl_gfx_kandinsky->header_buffer + token) = token_value;
			break;
		default:
			return ERROR_FAIL;
			break;
	}
	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- index ----------


UINT Gfx_Kandinsky::GetIndexBuffer(UINT **index_buffer)
{
	*index_buffer = pimpl_gfx_kandinsky->index_buffer;
	return 0;
}

UINT Gfx_Kandinsky::GetIndexBufferSize(VOID)
{
	return *(pimpl_gfx_kandinsky->header_buffer + INDEX_BUFFER_SIZE);
}

HRESULT Gfx_Kandinsky::SetIndexBufferSize(UINT index_buffer_size)
{
	pimpl_gfx_kandinsky->IndexBufferInitialise(index_buffer_size);
	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- vertex ----------


UINT Gfx_Kandinsky::GetVertexBuffer(FLOAT **vertex_buffer)
{
	*vertex_buffer = pimpl_gfx_kandinsky->vertex_buffer;
	return 0;
}

UINT Gfx_Kandinsky::GetVertexBufferSize(VOID)
{
	return *(pimpl_gfx_kandinsky->header_buffer + VERTEX_BUFFER_SIZE);
}

UINT Gfx_Kandinsky::GetVertexFormat(VOID)
{
	return Get(Gfx_Kandinsky_Param::VERTEX_FORMAT);
}

UINT Gfx_Kandinsky::GetVertexFormatSize(VOID)
{
	UINT vertex_format = Get(Gfx_Kandinsky_Param::VERTEX_FORMAT);
	UINT vertex_format_size = 0;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::XYZ) vertex_format_size += 3;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::XYZRHW) vertex_format_size += 4;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::NORMAL) vertex_format_size += 3;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::DIFFUSE) vertex_format_size += 1;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::SPECULAR) vertex_format_size += 1;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::TEX0) vertex_format_size += 2;
	if (vertex_format & Gfx_Kandinsky_Vertex_Format::TEX1) vertex_format_size += 2;
	*(pimpl_gfx_kandinsky->header_buffer + VERTEX_FORMAT_SIZE) = vertex_format_size;
	return vertex_format_size;
}

HRESULT Gfx_Kandinsky::SetVertexBufferSize(UINT vertex_buffer_size)
{
	pimpl_gfx_kandinsky->VertexBufferInitialise(vertex_buffer_size);
	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- primitive ----------


UINT Gfx_Kandinsky::GetPrimitiveCount(VOID)
{
	return *(pimpl_gfx_kandinsky->header_buffer + PRIMITIVE_COUNT);
}

UINT Gfx_Kandinsky::GetPrimitiveType(VOID)
{
	return *(pimpl_gfx_kandinsky->header_buffer + PRIMITIVE_TYPE);
}


////////////////////////////////////////////////////////////////////////////////


// ---------- OLD component parameters ----------

/*
HRESULT Gfx_Kandinsky::AppendParameter(
		const std::string& group_name,
		const std::string& param_name,
		const std::string& param_value
	)
{

	UINT top = pimpl_gfx_kandinsky->parameter_top;
	if (top >= 0 && top < pimpl_gfx_kandinsky->parameter_max)
	{
		pimpl_gfx_kandinsky->parameter[top].group = group_name;
		pimpl_gfx_kandinsky->parameter[top].name  = param_name;
		pimpl_gfx_kandinsky->parameter[top].value = param_value;
		pimpl_gfx_kandinsky->parameter_top++;
		return SUCCESS_OK;
	}

	return SUCCESS_OK;
}
*/

/*
std::string Gfx_Kandinsky::GetComponentParameterValue(
		const std::string& group_name,
		const std::string& param_name
	)
{

	UINT top = pimpl_gfx_kandinsky->parameter_top;
	for (UINT index = 0; index < top; index++)
	{
		if (pimpl_gfx_kandinsky->parameter[index].group == group_name)
		{
			if (pimpl_gfx_kandinsky->parameter[index].name == param_name)
			{
				std::string something(pimpl_gfx_kandinsky->parameter[index].value);
				return something;
			}
		}
	}

	std::string something("NULL");
	return something;
}
*/

////////////////////////////////////////////////////////////////////////////////


// ---------- component parameters ----------


HRESULT Gfx_Kandinsky::AppendParameter(
		const std::string& group_name,
		const std::string& param_name,
		const std::string& param_value
	)
{

	// ---- root : create ?
		if (pimpl_gfx_kandinsky->root == NULL)
		{
			pimpl_gfx_kandinsky->root = new Pimpl_Gfx_Kandinsky::Parameter();
		}
		Pimpl_Gfx_Kandinsky::Parameter *root = pimpl_gfx_kandinsky->root;

	// ---- group: either create OR find & create
		Pimpl_Gfx_Kandinsky::Parameter *group_param = NULL;
		if (root->first == NULL)
		{
			group_param = new Pimpl_Gfx_Kandinsky::Parameter();
			group_param->name = group_name;
			root->first = root->last = group_param;
		}
		else
		{
			// ---- find
				BOOL group_found = FALSE;
				group_param = root->first;
				while (group_param)
				{
					if (group_param->name == group_name)
					{
						group_found = TRUE;
						break;
					}
					group_param = group_param->next;
				}

			// ---- create
				if (!group_found)
				{
					group_param = new Pimpl_Gfx_Kandinsky::Parameter();
					group_param->name = group_name;
					root->last->next = group_param;
					root->last = group_param;
				}
		}

	// ---- item : create & append 
		Pimpl_Gfx_Kandinsky::Parameter *new_param = new Pimpl_Gfx_Kandinsky::Parameter();
		new_param->name  = param_name;
		new_param->value = param_value;

		// ---- add
			if (group_param->first == NULL && group_param->last == NULL)
			{
				group_param->first = new_param;
				group_param->last = new_param;
			}
		// ---- append
			else
			{
				Pimpl_Gfx_Kandinsky::Parameter *current_last = group_param->last;
				group_param->last = new_param;
				current_last->next = new_param;
				new_param->previous = current_last;
			}

	return SUCCESS_OK;
}


HRESULT Gfx_Kandinsky::GetParameterValue(
		const std::string& group_name,
		const std::string& param_name,
		FLOAT& value
	)
{

	// ---- match group name
		Pimpl_Gfx_Kandinsky::Parameter *root = pimpl_gfx_kandinsky->root;
		Pimpl_Gfx_Kandinsky::Parameter *group_param = root->first;
		while (group_param)
		{
			if (group_param->name == group_name)
			{
				// ---- match param name
					Pimpl_Gfx_Kandinsky::Parameter *name_param = group_param->first;
					while (name_param)
					{
						if (name_param->name == param_name)
						{
							size_t value_size = name_param->value.size();
							if (value_size > 0)
							{
								value = std::stof(name_param->value);
								return SUCCESS_OK;
							}
							else
							{
								value = 0;
								return ERROR_FAIL;
							}
						}
						name_param = name_param->next;
					}
			}
			group_param = group_param->next;
		}

	return ERROR_FAIL;
}


HRESULT Gfx_Kandinsky::SetParameterValue(
		const std::string& group_name,
		const std::string& param_name,
		const std::string& param_value
)
{

	// ---- match group name
		Pimpl_Gfx_Kandinsky::Parameter *root = pimpl_gfx_kandinsky->root;
		Pimpl_Gfx_Kandinsky::Parameter *group_param = root->first;
		while (group_param)
		{
			if (group_param->name == group_name)
			{
				// ---- match param name
					Pimpl_Gfx_Kandinsky::Parameter *name_param = group_param->first;
					while (name_param)
					{
						if (name_param->name == param_name)
						{
							name_param->value = param_value;
							return SUCCESS_OK;
						}
						name_param = name_param->next;
					}
			}
			group_param = group_param->next;
		}

	return ERROR_FAIL;
}


////////////////////////////////////////////////////////////////////////////////


#include "../hpp_obj/vsl_gfx_kandinsky_cuboid_vibo.hpp"
#include "../hpp_obj/vsl_gfx_kandinsky_cuboid_vbo.hpp"
#include "../hpp_obj/vsl_gfx_kandinsky_pyrhodo_vbo.hpp"


////////////////////////////////////////////////////////////////////////////////
