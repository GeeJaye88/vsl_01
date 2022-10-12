////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky_all_notes.hpp ----------
/*!
\file vsl_gfx_kandinsky_all_notes.hpp
\brief kandinsky notes
\author Gareth Edwards
\note

Kandinsky
=========

The Big Idea

A Kandinsky component contains vertex & index buffers that store 2D or 3D
in the same format as required by the DX3D vertex & index buffers.

If a DX3D component is released and then recreated (e.g., on window resize)
then the DX3D buffers can be filled with data copied from the Kandinsky
component, which - as it persists duiring windows events - need not be
recreated.

This also provides for Kandinsky components to be updated without delaying
the DX3D Lock and Unlock methods to wait for on-the-fly data recreation.

This architecture provides for a significant increase in efficiency.

It also provides for Kandinsky Create methods to be threaded.


Kandinsky is Decoupled
----------------------

Kandinsky was created to be 2D & 3D component generator for the VSL Application
Development Framework (ADF).

As such the source ".cpp", included substantive code ".hpp" and the header ".h"
files are kept in separate directories withing the VSL library directory.

This is the "vsl_library/" directory.

To provide for easy integration and development Kandinsky is decoupled from all
other VSL code, though it does share two VSL header files.

These are "vsl_system/header/vsl_include.h" and "vsl_system/header/vsl_sem.h".

The first includes the required c & c++ header file.

The second includes required structs, enumerations and macros.

Typically an ADF that uses Kandinsky components is structured:

	[ADF] -> [Kandinsky Interface] -> [Kandinsky]


Kandinsky Files
---------------

Additions are required to the files listed below to add a new Kandinsky component.

headers : "vsl_library/header/"
			-> "vsl_gfx_kandinsky.h"
            -> "vsl_gfx_kandinsky_interface.h"

c++ : "vsl_library/source/"
			-> "vsl_gfx_kandinsky.cpp"
			-> "vsl_gfx_kandinsky_interface.cpp"


Kandinsky Component Files
-------------------------

For convenience the substantive code for configuring & creating Kandinsky
components is located in separate files.

These are included explicitly within Kandinsky.

Included substantive code is kept in files with a ".hpp" suffix.

They are located in the same directory path as the VSL headers & c++ files.

Included substantive c++ code: "vsl_library/hpp_obj/"

Details are given below.


Kandinsky Component File Naming
-------------------------------

"vsl_gfx_kandinsky_[component name]_[type, e.g, VBO or VIBO].hpp"

e.g., "vsl_gfx_kandinsky_cuboid_vbo.hpp", "vsl_gfx_kandinsky_cuboid_vbo.hpp"

Details are given below.


To Add a New Kandinsky Component
--------------------------------

1:	Create a Kandinsky component file.

	Tip: duplicate to bootstrap.

	A Kandinsksy component requires Config & Create methods.

	The Config method "sets" the parameters required to Create an component.
	The Create method generates vertex & index buffers.

	This file must be located in "vsl_library/hpp_obj/" directory.

2:  Add to the list of included substantive c++ code components at the
	bottom of "vsl_gfx_kandinsky.cpp".

	e.g., 

		#include "../hpp_obj/[new_name].hpp"

3:	Add declerations for the Config & Create methods to the "kandinsky.h" file.

	e.g.,

	With only vertex buffer
		HRESULT ::[new_name]_VBO_Config(VOID), and
		HRESULT ::[new_name]_VBO_Create(VOID).

	With vertex & index buffer
		HRESULT ::[new_name]_VIBO_Config(VOID), and
		HRESULT ::[new_name]_VIBO_Create(VOID).

4:	In "kandinsky.h" enumerate the [new_name] component.

	e.g.,

		// ----enum
		typedef enum Gfx_Kandinsky_Component
		{
			COMPONENT_UNKNOWN,
			CUBOID_VBO,
			Cuboid_VIBO,
			...,
			[NEW_NAME]
		} Gfx_Kandinsky_Component;

	Now compile to verify that the above has been successful.


To Add an Interface for a New Kandinsky Component
-------------------------------------------------

1:	Add the Parameter & Component methods to the "kandinsky.cpp" file.

	Tip: duplicate to bootstrap.

	e.g.,

		HRESULT Gfx_Kandinsky_Interface::[new_name]_Config_Kandinsky_Parameters(
				Gfx_Element *component_param_group
			)
		{
			AppendColourParameters(component_param_group);
			AppendTransformParameters(component_param_group);
			return SUCCESS_OK;
		}

		HRESULT Gfx_Kandinsky_Interface::[new_name]_Config_Kandinsky_Component(
				Gfx_Element_Component *gfx_element_component
			)
		{
			Gfx_Kandinsky *kandinsky = gfx_element_component->GetKandinsky();
			kandinsky->[new_name]_Config();
			kandinsky->[new_name]_Create();
			return SUCCESS_OK;
		}

2:	Add [new_name] and it's corresponding enumerated Kandinksy id to the
	"kandinsky.cpp" file.

	e.g.,

	else if (name == "[new_name]")
	{
		return Gfx_Kandinsky_Component::[NEW_NAME];
	}

3:	Add the decleration for the Config & Create methods to the
	"kandinsky.h" file.

	e.g.,

		static HRESULT [new_name]_Config_Kandinsky_Parameters(
			Gfx_Element *component_param_group);
		static HRESULT [new_name]_Config_Kandinsky_Component(
			Gfx_Element_Component *gfx_element_component);


4:	Add the Kandinsky interface callbacks

	The GetCallbacks method takes two arguments:
		. a pointer to a Gfx_Element_Component, and
		. a pointer to a Kandinsky_Interface_Callbacks struct

	Using the component type this method initialises a
	Kandinsky_Interface_Callbacks struct.

	This contains the addresses of Kandinsky_Interface "append parameters"
	(see below) and "config and create" methods.

	The first hierarchicaly appends groups of parameters unique to a
	specific Kandinsky component.

	The second invokes a specific Kandinsky components Config & Create
	methods.

	Now compile to verify that the above has been successful.

	Thats it...

Summary
=======

To add New Kandinsky Component
1:	Add Config & Create methods
2:	Create a Kandinsky component file
3:	Add to list of substantive c++ code components
4:	Add declerations for the Config & Create methods

To add an Interface for a New Kandinsky Component
1:	Add the Parameter & Component methods
2:	Add enumerated [new_name] & enumerated [NEW_NAME] id
3:	Add declerations for the Parameter & Component methods
4:	Add the Kandinsky interface callbacks

*/

////////////////////////////////////////////////////////////////////////////////

/*

Supplementary Notes
-------------------

It might be that in time a data driven application could automate the
compilation of a Kandinsky_Interface. For the moment (150922) I have other
higher priority things that need doing.

Kandinsky Component Parameters
------------------------------

To provide for full decoupling of the Kandinsky Interface from Kandinsky a
components parameters are comprised of Gfx_Elements (see VSL) which are
hierarchicaly appended as groups of parameters unique to a specific Kandinsky
component when a Kandinsky Interface append parameters method is invoked.

e.g.,

	HRESULT Gfx_Kandinsky_Interface::[name]_VBO_Config_Kandinsky_Parameters(
			Gfx_Element *component_param_group
		)
	{

		// ---- method for adding a common parameter group(s)
			AppendColourParameters(component_param_group);

		// ---- add specific parameter group
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

At run time these are passed as "group name", "param name", and "param value"
to a Kandinsky component and a duplicate hierarchical group/parameter structure
created.

	e.g.,

		(within Gfx_Element_Engine::Setup method)

		hr = gfx_component_kandinsky->AppendParameter(
			group_name,
			param->GetName(),
			param->GetValue()
		);

This has two major benifits:
	. it preserves the full decoupling of the Kandinsky Interface from  Kandinsky, and
	. it provides for faster retrieval of a value for a named parameter.

*/

////////////////////////////////////////////////////////////////////////////////
