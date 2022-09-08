////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_coordinate.cpp ----------
/*!
\file vsl_gfx_element_coordinate.cpp
\brief Implementation of the Gfx_Element_Coordinate class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element.h"
	#include "../../vsl_library/header/vsl_gfx_element_coordinate.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Element_Coordinate::Pimpl_Gfx_Element_Coordinate
{

	public:

	// ---- cdtor
		Pimpl_Gfx_Element_Coordinate(VOID)
		{
			;
		}
		~Pimpl_Gfx_Element_Coordinate()
		{
			;
		}

	// ---- coordinates
		//
		//  note : matrix multiplication is done in the
		//  order scale, rotate and then translate
		//
		vsl_system::Vsl_Vector3 scale;
		vsl_system::Vsl_Vector3 rotate;
		vsl_system::Vsl_Vector3 translate;

	// ---- rotation matrix multiplication order
		UINT  rotation_order_index = 3; // "YXZ" - aka Yaw, Pitch & Roll
		std::string rotation_order[7] =
		{
			"HPB", "XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX"
		};

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---------- cdtor ----------

	Gfx_Element_Coordinate::Gfx_Element_Coordinate(VOID) : 
		pimpl_gfx_element_coordinate(new Pimpl_Gfx_Element_Coordinate)
	{
		SetScale(vsl_system::Vsl_Vector3(1, 1, 1));
		SetRotate(vsl_system::Vsl_Vector3(0, 0, 0));
		SetTranslate(vsl_system::Vsl_Vector3(0, 0, 0));
	}

	Gfx_Element_Coordinate::~Gfx_Element_Coordinate()
	{
		;
	}

// ---------- get ----------

	VOID Gfx_Element_Coordinate::GetScale(vsl_system::Vsl_Vector3& scale)
	{
		scale = pimpl_gfx_element_coordinate->scale;
	}
	VOID Gfx_Element_Coordinate::GetRotate(vsl_system::Vsl_Vector3& rotate)
	{
		rotate = pimpl_gfx_element_coordinate->rotate;
	}
	VOID Gfx_Element_Coordinate::GetTranslate(vsl_system::Vsl_Vector3& translate)
	{
		translate = pimpl_gfx_element_coordinate->translate;
	}
	VOID Gfx_Element_Coordinate::GetRotationOrder(std::string& rotation_order)
	{
		rotation_order = pimpl_gfx_element_coordinate->rotation_order[pimpl_gfx_element_coordinate->rotation_order_index];
	}
	VOID Gfx_Element_Coordinate::GetRotationOrderIndex(UINT& rotation_order_index)
	{
		rotation_order_index = pimpl_gfx_element_coordinate->rotation_order_index;
	}

// ---------- set ----------

	VOID Gfx_Element_Coordinate::SetScale(vsl_system::Vsl_Vector3& scale)
	{
		pimpl_gfx_element_coordinate->scale = scale;
	}
	VOID Gfx_Element_Coordinate::SetRotate(vsl_system::Vsl_Vector3& rotate)
	{
		pimpl_gfx_element_coordinate->rotate = rotate;
	}
	VOID Gfx_Element_Coordinate::SetTranslate(vsl_system::Vsl_Vector3& translate)
	{
		pimpl_gfx_element_coordinate->translate = translate;
	}
	VOID Gfx_Element_Coordinate::SetRotationOrderIndex(UINT& rotation_order_index)
	{
		pimpl_gfx_element_coordinate->rotation_order_index = rotation_order_index;
	}

// ---------- get parameter groups ----------

	VOID Gfx_Element_Coordinate::GetParameterGroups(
			Gfx_Element *parameter_groups
		)
	{
		vsl_system::Vsl_Vector3 v;

		GetTranslate(v);
		Gfx_Element *translate = parameter_groups->Append("Translate");
		translate->Append("X", std::to_string(v.x), 0);
		translate->Append("Y", std::to_string(v.y), 0);
		translate->Append("Z", std::to_string(v.z), 0);

		GetScale(v);
		Gfx_Element *scale = parameter_groups->Append("Scale");
		scale->Append("X", std::to_string(v.x), 0);
		scale->Append("Y", std::to_string(v.y), 0);
		scale->Append("Z", std::to_string(v.z), 0);

		GetRotate(v);
		Gfx_Element *rotate = parameter_groups->Append("Rotate");
		rotate->Append("X", std::to_string(v.x), 0);
		rotate->Append("Y", std::to_string(v.y), 0);
		rotate->Append("Z", std::to_string(v.z), 0);

		UINT order = 0;
		GetRotationOrderIndex(order);
		rotate->Append("Order", std::to_string(order), 0);
	}


////////////////////////////////////////////////////////////////////////////////
