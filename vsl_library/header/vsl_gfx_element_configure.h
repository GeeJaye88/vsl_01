////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_configure.h ----------
/*!
\file vsl_gfx_element_configure.h
\brief Gfx_Element_Configure class
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	// ---- "forward" declarations
		class Gfx_Element;

	// ---- element configure
		class Gfx_Element_Configure
		{

		public:

			// ---- cdtor
				Gfx_Element_Configure(VOID);
				~Gfx_Element_Configure();

			// ---- is
				BOOL IsComponent(VOID);
				BOOL IsInstance(VOID);
				BOOL IsVisible(VOID);

			// ---- get
				BOOL GetComponent(VOID);
				std::string GetComponentName(VOID);
				BOOL GetVisible(VOID);
				D3DXMATRIX *GetMatrix(VOID);

			// ---- set
				VOID SetComponent(BOOL component);
				VOID SetComponentName(std::string component_name);
				VOID SetVisible(BOOL visible);
				VOID SetMatrix(D3DXMATRIX &matrix);

			// ---- get instance
				BOOL GetInstance(VOID);
				std::string GetInstanceName(VOID);
				Gfx_Element *GetInstanceElement(VOID);

			// ---- set instance
				VOID SetInstance(BOOL instance);
				VOID SetInstanceName(std::string instance_name);
				VOID SetInstanceElement(Gfx_Element *instance_element);

			// ---- get parameter groups
				VOID GetParameterGroups(Gfx_Element *parameter_groups);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Element_Configure;
				std::unique_ptr<Pimpl_Gfx_Element_Configure> pimpl_gfx_element_configure;

		};

}

////////////////////////////////////////////////////////////////////////////////