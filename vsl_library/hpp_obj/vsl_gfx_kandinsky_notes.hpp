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

This architecture provides for an enormous potential increase in framerate.

It also provides for Kandinsky Create methods to be threaded.

Kandinsky Directories
---------------------

header : "vsl_application/vsl_library/header/"
      -> "vsl_gfx_kandinsky.h" 
      -> "vsl_gfx_kandinsky_config.h"

hpp : "vsl_application/vsl_library/hpp/"
      -> "vsl_gfx_kandinsky.hpp"
      -> "vsl_gfx_kandinsky_config.hpp"

cpp : "vsl_application/vsl_library/source/"
      -> "vsl_gfx_kandinsky.cpp"
      -> "vsl_gfx_kandinsky_config.cpp"

Kandinsky Object Directory
--------------------------
"vsl_application/vsl_library/hpp_obj/"

Kandinsky Object File Naming
-----------------------------
"vsl_gfx_kandinsky_[component name]_[type, e.g, vbo or vibo].hpp"

Adding a New Kandinsky Object
-----------------------------

1: Create a Kandinsky component file (tip: duplicate to bootstrap)

   A Kandinsksy component requires Config & Create methods.

   The Config method "sets" the parameters required to Create an component.
   The Create method generates vertex & index buffers.

2: Add to the "#include" component list below

   E.g., if new component is called "Demo" and has both vertex & index
   buffers then file name will be "vsl_gfx_kandinsky_demo_vibo.hpp.

   If has only a vertex buffer then "vsl_gfx_kandinsky_demo_vbo.hpp.

3: The "Demo" component Config & Create methods must be named:

   - with vertex & index buffer
   HRESULT Gfx_Kandinsky::Demo_VIBO_Config(), and
   HRESULT Gfx_Kandinsky::Demo_VIBO_Create().

   - with vertex buffer
   HRESULT Gfx_Kandinsky::Demo_VBO_Config(), and
   HRESULT Gfx_Kandinsky::Demo_VBO_Create().

4: Add "Demo" component Config & Create methods to Kandinsky component.

   Edit "vsl_gfx_kandinsky.hpp"

   Add - if with vertex & index buffer
   HRESULT Demo_IVBO_Config(VOID);
   HRESULT Demo_IVBO_Create(VOID);

5: Add "Demo" component methods to the Kandinsky_Config component.

   Edit "vsl_gfx_kandinsky_config.hpp"

   Add - if with vertex & index buffer:

   static HRESULT Cuboid_VBO_Config_Kandinsky_Parameters(vsl_library::Gfx_Element *element)
   {
      [... see "vsl_gfx_kandinsky_config.hpp" for example]
      return SUCCESS_OK;
   }

   static HRESULT Cuboid_VBO_Config_Kandinsky_Component(vsl_library::Gfx_Element_Component *element_component)
   {
      [... see "vsl_gfx_kandinsky_config.hpp" for example]
      return SUCCESS_OK;
   }

   These mothods are included in the "vsl_gfx_kandinsky_config.cpp" file.

6: Thats it...

*/

////////////////////////////////////////////////////////////////////////////////
