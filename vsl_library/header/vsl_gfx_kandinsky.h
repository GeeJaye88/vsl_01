////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_kandinsky.h ----------
/*!
\file vsl_gfx_kandinsky.h
\brief Kandinsky classes
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"

// ---- library
	#include "../../vsl_library/header/vsl_sem.h"


///////////////////////////////////////////////////////////////////////////////

namespace vsl_library
{

///////////////////////////////////////////////////////////////////////////////


	// ---- kandinsky header tokens
		typedef enum Gfx_Kandinsky_Param
		{
			// ---- section sizes
				HEADER_BUFFER_SIZE,
				INDEX_BUFFER_SIZE,
				VERTEX_BUFFER_SIZE,

			// ---- component
				COMPONENT_TYPE,

			// ---- indices
				INDEX_COUNT,

			// ---- vertices
				VERTEX_COUNT,
				VERTEX_FORMAT,
				VERTEX_FORMAT_SIZE,

			// ---- primitives
				PRIMITIVE_COUNT,
				PRIMITIVE_TYPE,

			// ---- build states
				INSIDE,      // includem inside (1 ON, 0 OFF) 

			// ---- render states
				CULL_MODE,   // cull mode (anti-clockwise(-1), all(0), clockwise(1))
				DIMENSION,   // dimension (2 or 3)
				MATERIAL_ID, // material id (0 to N, -ve if none)
				MATRIX_ID,   // matrix id (0 to N, -ve if none)
				WIREFRAME    // wireframe mode (1 ON, 0 OFF)

		} Gfx_Kandinsky_Param;


///////////////////////////////////////////////////////////////////////////////


	// ----enum
		typedef enum Gfx_Kandinsky_Component
		{
			COMPONENT_UNKNOWN,
			CUBOID_VBO,
			Cuboid_VIBO,
			PYRHODO_VBO
		} Gfx_Kandinsky_Component;


///////////////////////////////////////////////////////////////////////////////


	// ---- kandinsky primitives types supported (as per d3d)
		typedef enum Gfx_Kandinsky_Primitive_Type
		{
			POINTLIST     = 1,
			LINELIST      = 2,
			LINESTRIP     = 3,
			TRIANGLELIST  = 4,
			TRIANGLESTRIP = 5,
			TRIANGLEFAN   = 6, /* deprecated post d3d dx9 */
			FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
		} Gfx_Kandinsky_Primitive_Type;


///////////////////////////////////////////////////////////////////////////////


	// ---- kandinsky flexible vertex formats bits (as per d3d) 
		typedef enum Gfx_Kandinsky_Vertex_Format
		{
			XYZ      = 0x002,
			XYZRHW   = 0x004,
			NORMAL   = 0x010,
			DIFFUSE  = 0x040,
			SPECULAR = 0x080,
			TEX0     = 0x000,
			TEX1     = 0x100
		} Gfx_Kandinsky_Vertex_Format;


///////////////////////////////////////////////////////////////////////////////


	// ---- kandinsky
		class Gfx_Kandinsky
		{

		public:

			// ---- cdtor
				Gfx_Kandinsky(VOID);
				virtual ~Gfx_Kandinsky();

			// ---- param
				HRESULT Get(Gfx_Kandinsky_Param token, UINT *token_value);
				UINT    Get(Gfx_Kandinsky_Param token);
				HRESULT Set(Gfx_Kandinsky_Param token, UINT token_value);

			// ---- index
				UINT GetIndexBuffer(UINT **index_buffer);
				UINT GetIndexBufferSize(VOID);
				HRESULT SetIndexBufferSize(UINT index_buffer_size);

			// ---- vertex
				UINT GetVertexBuffer(FLOAT **vertex_buffer);
				UINT GetVertexBufferSize(VOID);
				UINT GetVertexFormat(VOID);
				UINT GetVertexFormatSize(VOID);
				HRESULT SetVertexBufferSize(UINT vertex_buffer_size);

			// ---- primitive
				UINT GetPrimitiveCount(VOID);
				UINT GetPrimitiveType(VOID);
		
			// ---- component (only after [gfx_element_engine]->Setup()) 
				BOOL Gfx_Kandinsky::IsComponent();
				Gfx_Kandinsky_Component Gfx_Kandinsky::GetComponentType();

			// ---- component
				HRESULT SetComponentType(const std::string& name);

				HRESULT AppendParameter(
						const std::string& group_name,
						const std::string& param_name,
						const std::string& param_value
					);
				HRESULT GetParameterValue(
						const std::string& group_name,
						const std::string& param_name,
						FLOAT& value
					);
				HRESULT SetParameterValue(
						const std::string& group_name,
						const std::string& param_name,
						const std::string& param_value
					);

			// ---- declaration of the component config & create methods

				// ---- dev only
					HRESULT Cuboid_v1(VOID);

				// ---- cuboid
					HRESULT Cuboid_VBO_Config(VOID);
					HRESULT Cuboid_VBO_Create(VOID);

				// ---- cuboid
					HRESULT Cuboid_VIBO_Config(VOID);
					HRESULT Cuboid_VIBO_Create(VOID);

				// ---- pyramidal rhombic dodecahedron
					HRESULT PyRhoDo_VBO_Config(VOID);
					HRESULT PyRhoDo_VBO_Create(VOID);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Kandinsky;
				std::unique_ptr<Pimpl_Gfx_Kandinsky> pimpl_gfx_kandinsky;

		};


///////////////////////////////////////////////////////////////////////////////


} // namespace vsl_library


////////////////////////////////////////////////////////////////////////////////