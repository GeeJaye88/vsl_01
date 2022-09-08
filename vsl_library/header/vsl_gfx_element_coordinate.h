////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_coordinate.h ----------
/*!
\file vsl_gfx_element_coordinate.h
\brief Gfx_Element_Coordinate class
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"
	#include "../../vsl_system/header/vsl_maths.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---- "forward" declarations
		class Gfx_Element;

	// ---- element coordinates
		class Gfx_Element_Coordinate
		{

		public:

			// ---- cdtor
				Gfx_Element_Coordinate(VOID);
				~Gfx_Element_Coordinate();

			// ---- get
				VOID GetScale(vsl_system::Vsl_Vector3& scale);
				VOID GetRotate(vsl_system::Vsl_Vector3& rotate);
				VOID GetTranslate(vsl_system::Vsl_Vector3& translate);
				VOID GetRotationOrder(std::string& rotation_order);
				VOID GetRotationOrderIndex(UINT& rotation_order_index);

			// ---- set
				VOID SetScale(vsl_system::Vsl_Vector3& scale);
				VOID SetRotate(vsl_system::Vsl_Vector3& rotate);
				VOID SetTranslate(vsl_system::Vsl_Vector3& translate);
				VOID SetRotationOrderIndex(UINT& rotation_order_index);

			// ---- get parameter groups
				VOID GetParameterGroups(Gfx_Element *parameter_groups);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Element_Coordinate;
				std::unique_ptr<Pimpl_Gfx_Element_Coordinate> pimpl_gfx_element_coordinate;

		};

}

////////////////////////////////////////////////////////////////////////////////