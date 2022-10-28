////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_engine.cpp ----------
/*!
\file vsl_gfx_element_engine.cpp
\brief Implementation of the Gfx_Element_Engine classes
\author Gareth Edwards 
*/

// ---- system
	#include "../../vsl_system/header/vsl_maths.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element_engine.h"
	#include "../../vsl_library/header/vsl_gfx_element.h"
	#include "../../vsl_library/header/vsl_gfx_log.h"
	#include "../../vsl_library/header/vsl_gfx_kandinsky_interface.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;

////////////////////////////////////////////////////////////////////////////////


//#define OUTPUT_ELEMENT_INFO

VOID OutputElementInfo(
	Gfx_Element *element,
	UINT spacing
)
{
	CHAR msg[128];
	sprintf_s(msg, 128,
		"%*s%s: %s\n",
		spacing, "",
		element->GetName().c_str(),
		element->GetValue().c_str()
	);
	OutputDebugString(msg);
};


////////////////////////////////////////////////////////////////////////////////

// ---------- private implementation ----------
/*!
\brief Implementation of the Pimpl_Gfx_Element_Engine class
\author Gareth Edwards
\note

Direct3d9 Matrix Stack
-https://docs.microsoft.com/en-us/windows/win32/direct3d9/id3dxmatrixstack

*/
class Gfx_Element_Engine::Pimpl_Gfx_Element_Engine
{

public:

	// ---- cdtor ----
		Pimpl_Gfx_Element_Engine(VOID) { ; }
		~Pimpl_Gfx_Element_Engine() { ; }

	// ---- elements
		Gfx_Element  gfx_element_engine_root;
		Gfx_Element *gfx_element_project_root = NULL;

	// ---- element bookmarks
		std::list <Gfx_Element*> gfx_element_bookmarks;

	// ---- log file
		Gfx_Log *gfx_log = NULL;

	// ---- recursion
		UINT level = 0;

	// ---- Gfx
		LPD3DXMATRIXSTACK matrix_stack;
		LPDIRECT3DDEVICE9 device;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---------- ctor ----------

Gfx_Element_Engine::Gfx_Element_Engine(VOID) : pimpl_gfx_element_engine(new Pimpl_Gfx_Element_Engine)
{

	// ---- the big one
		Gfx_Element *engine_root_element = GetEngineRoot();
		engine_root_element->SetName("Engine");

	// ---- scene
		SetGfxProject(engine_root_element->Append("Project"));
		GetProjectRoot()->GetConfigure()->SetVisible(TRUE);

	// ---- create & initialise matrix stack
		HRESULT hr = D3DXCreateMatrixStack(0, &pimpl_gfx_element_engine->matrix_stack);
		pimpl_gfx_element_engine->matrix_stack->LoadIdentity();

}

Gfx_Element_Engine::~Gfx_Element_Engine()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- engine methods ----------
/*

NOTE : "guarding" methods:

		IsComponent - Gfx_Element which has a Gfx_Element_Component (Gfx_Kandinsky, etc.)
		IsVisible - Gfx_Element which to be displayed

NOTE : engine methods - NOT guarded - handle all Gfx_Element

		Validate
		Setup
		SetupDX
		Display
		CleanupDX
		Cleanup

     : engine sub methods

		Validate  - recurses for all children
		Setup     - IsComponent() guards Kandinsky, then recurses for all children
		SetupDX   - invokes Element_SetupDX(), recurses for all children
		Transform - invokes Element_Transform(), then guarded IsVisible() recurses for all children 
		Display   - invokes Element_Display(), then guarded IsVisible() recurses for all children 
		CleanupDX - invokes Element_CleanupDX(), then recurses for all children 
		Cleanup   - cleans up components (if any), then recurses for all children 

	 : engine element methods - only Setup guarded

		Element_SetupDX   - guarded by IsComponent()
		Element_Transform - guarded by IsVisible()
		Element_Display   - guarded by IsComponent()
		Element_CleanupDX - guarded by IsComponent()

*/


////////////////////////////////////////////////////////////////////////////////


// ---------- engine methods ----------


// ---------- Setup ----------
/*!
\brief engine one time setup
\author Gareth Edwards
\return HRESULT (TRUE if ok)

\note recursively setup element, element instancing & verify kandinsky

*/
HRESULT Gfx_Element_Engine::Setup(VOID)
{

	if (Gfx_Element *gfx_element_project_root = GetProjectRoot())
	{
		if (GetGfxLog() != NULL) Log("Setup");

		UINT level = 0;
		HRESULT hr = Setup(gfx_element_project_root, level);
		if (FAILED(hr)) return ERROR_FAIL;

		Log();
	}

	if (GetGfxLog() != NULL) Log("Setup");

	return SUCCESS_OK;
}


// ---------- SetupDX ----------
/*!
\brief engine setup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::SetupDX(LPDIRECT3DDEVICE9 device)
{

	HRESULT hr = SetDevice(device);

	if (Gfx_Element *gfx_element_project_root = GetProjectRoot())
	{
		if (GetGfxLog() != NULL) Log("SetupDX");

		UINT level = 0;
		HRESULT hr = SetupDX(gfx_element_project_root, level);
		if (FAILED(hr)) return ERROR_FAIL;
	}

	return SUCCESS_OK;
}


// ---------- Display ----------
/*!
\brief engine display
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::Display()
{

	if (Gfx_Element *gfx_element_project_root = GetProjectRoot())
	{

		// ---- local
			UINT level = 0;

		// ---- zap matirx stack
			pimpl_gfx_element_engine->matrix_stack->LoadIdentity();

		// ---- transform
			HRESULT hr = Transform(gfx_element_project_root, level);
			if (FAILED(hr)) return ERROR_FAIL;

		// ---- display
			hr = Display(gfx_element_project_root, level);
			if (FAILED(hr)) return ERROR_FAIL;

	}

	return SUCCESS_OK;
}


// ---------- Cleanup dx ----------
/*!
\brief engine cleanup dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::CleanupDX ()
{

	if (Gfx_Element *gfx_element_project_root = GetProjectRoot())
	{
		if (GetGfxLog() != NULL) Log("CleanupDX");

		UINT level = 0;
		HRESULT hr = CleanupDX(gfx_element_project_root, level);

		if (FAILED(hr)) return ERROR_FAIL;
	}

	return SUCCESS_OK;
}


// ---------- Cleanup ----------
/*!
\brief engine cleanup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::Cleanup(VOID)
{
	
	if (Gfx_Element *gfx_element_project_root = GetProjectRoot())
	{
		if (GetGfxLog() != NULL) Log("Cleanup");

		UINT level = 0;
		HRESULT hr = Cleanup(gfx_element_project_root, level);
		if (FAILED(hr)) return ERROR_FAIL;
	}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- recursive engine methods ----------


// ---------- Setup ----------
/*!
\brief one time element setup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note guarding element->IsComponent()

\note recursively setup element, element instancing & verify kandinsky
*/
HRESULT Gfx_Element_Engine::Setup(
		Gfx_Element *element,
		UINT level
	)
{

		#if defined (OUTPUT_ELEMENT_INFO)
		OutputElementInfo(element, level * 2);
		#endif

	// ---- local

	// ---- if element has a component name then verify legit component
		if (element->GetConfigure()->GetComponentName() != "")
		{

			// ---- is a valid Kandinsky component name then set valid
				Gfx_Kandinsky_Interface gfx_kandinsky_interface;
				HRESULT component_type_id = gfx_kandinsky_interface.GetComponentTypeId(element->GetConfigure()->GetComponentName());
				if (SUCCEEDED(component_type_id))
				{

					Gfx_Element_Component *gfx_element_component = element->GetComponent();
					Gfx_Kandinsky *gfx_component_kandinsky = gfx_element_component->GetKandinsky();

					HRESULT hr = gfx_component_kandinsky->SetComponentType(component_type_id);
					element->GetConfigure()->SetComponent(TRUE);
				}
				else
				{
					// ---- LOG NOT OK
						Gfx_Log *gfx_log = GetGfxLog();
						if (gfx_log != NULL)
						{
							std::string line = "Gfx_Element_Engine: Element: " + element->GetName() +
								" has invalid Component name " + element->GetConfigure()->GetComponentName() + 
								"\n";
							gfx_log->Write(line);
						}
				}
		}

	// ---- every element has:
		
		// ---- "Configuration" child parameter groups
			Gfx_Element *config_param_group = element->AppendParamGroup("Configuration");
			Gfx_Element_Configure *configure = element->GetConfigure();
			configure->GetParameterGroups(config_param_group);

		// ---- "Coordinate" child parameter groups
			Gfx_Element *coord_param_group = element->AppendParamGroup("Coordinate");
			Gfx_Element_Coordinate *coordinate = element->GetCoordinate();
			coordinate->GetParameterGroups(coord_param_group);

	// ---- if component then "Component" child parameter groups and
		//  an initialised Kandinsky object with component parameters
		//
		if (element->GetConfigure()->IsComponent())
		{

			// ---- if component OK
				HRESULT hr;
				Gfx_Element_Component *gfx_element_component = element->GetComponent();
				if (gfx_element_component != NULL) try
				{

					// ---- 1st: get pointer to kandinsky config callbacks
						//  note, these callbacks are:
						//     Gfx_Element           * kandinsky_interface_append_parameters (used here!)
						//     Gfx_Element_Component * kandinsky_interface_config_and_create (used in Element_SetupDX)
						Gfx_Kandinsky_Interface_Callbacks *gfx_kandinsky_interface_callbacks = gfx_element_component->GetKandinskyInterfaceCallbacks();

					// ---- 2nd: use first interface callback to get kandinsky component callbacks
						Gfx_Kandinsky_Interface gfx_kandinsky_interface;
						gfx_kandinsky_interface.GetCallbacks(gfx_element_component, gfx_kandinsky_interface_callbacks);

					// ---- 3rd: append element "Component" parameter group
						Gfx_Element *component_param_group = element->AppendParamGroup("Component");

					// ---- 4th: every component element has unique kandinsky parameter group(s) appended using config callback
						if (gfx_kandinsky_interface_callbacks->kandinsky_interface_append_parameters != NULL)
						{
							hr = gfx_kandinsky_interface_callbacks->kandinsky_interface_append_parameters(component_param_group);
							if (FAILED(hr)) throw("Failed: To append Component Kandinsky Parameters");
						}

					// ---- required to append parameters
						Gfx_Kandinsky *gfx_component_kandinsky = gfx_element_component->GetKandinsky();

					// ---- 5th: create component parameters
						//
						//  note: to decouple Kandinsky from the Gfx System these
						//  parameters are not comprised of Gfx_Elements, but are 
						//  Kandinsky [group->group->...]->param->param->... lists
						//
						//  note: also speeds up the retrieval of parameter values... 
						//
						Gfx_Element *group = component_param_group->GetFirst();
						while (group)
						{
							#if defined(OUTPUT_ELEMENT_INFO)
							OutputElementInfo(group, level*2 + 2);
							#endif

							std::string group_name = group->GetName();
							Gfx_Element *param = group->GetFirst();
							while (param)
							{
								#if defined(OUTPUT_ELEMENT_INFO)
								OutputElementInfo(param, level * 2 + 4);
								#endif

								hr = gfx_component_kandinsky->AppendParameter(
										group_name,
										param->GetName(),
										param->GetValue()
									);
								if (FAILED(hr)) throw("Failed: To append Kandinsky Parameter");

								param = param->GetNext();
							}

							group = group->GetNext();
						}

				}
				catch (CHAR *error)
				{
					// ---- log
						Gfx_Log *gfx_log = GetGfxLog();
						if (gfx_log != NULL)
						{
							std::string line = "Gfx_Element_Engine: Setup of element " +
								element->GetName() +
									"failed with message " + error +
										" and error " + std::to_string((INT)hr) + "\n";
							gfx_log->Write(line);
						}

					// ---- zap
						gfx_element_component->SetConfigBitmask(0);

					return hr;
				}
		}


	// ---- recurse for all children
		Gfx_Element *child = element->GetFirst();
		while (child)
		{
			HRESULT hr = Setup(child, level + 1);
			child = child->GetNext();
		}

	return SUCCESS_OK;
}


// ---------- SetupDX ----------
/*!
\brief element setup dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::SetupDX(
		Gfx_Element *element,
		UINT level
	)
{
	#if defined(OUTPUT_ELEMENT_INFO)
	OutputElementInfo(element, level * 2);
	#endif

	// ---- element setup dx 
		Element_SetupDX(element);

	// ---- recurse for all children
		Gfx_Element *child = element->GetFirst();
		while (child)
		{
			HRESULT hr = SetupDX(child, level + 1);
			child = child->GetNext();
		}

	return SUCCESS_OK;
}


// ---------- Transform ----------
/*!
\brief element display callback transform
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note guarding element->IsVisible()

*/
HRESULT Gfx_Element_Engine::Transform(
		Gfx_Element *element,
		UINT level
	)
{

	// ---- push matrix stack
		pimpl_gfx_element_engine->matrix_stack->Push();

			// ---- transform
				Element_Transform(element);

			// ---- recurse for all children
				Gfx_Element *child = element->GetFirst();
				while (child)
				{
					Gfx_Element_Configure *gfx_element_configure = child->GetConfigure();
					if (child->GetConfigure()->IsVisible())
						HRESULT hr = Transform(child, level + 1);
					child = child->GetNext();
				}

	// ---- pop matrix stack
		pimpl_gfx_element_engine->matrix_stack->Pop();

	return SUCCESS_OK;
}


// ---------- Display ----------
/*!
\brief element cleanup dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note guarding element->IsVisible()

*/
HRESULT Gfx_Element_Engine::Display(
		Gfx_Element *element,
		UINT level
	)
{

	// ---- display components
		Element_Display(element);

	// ---- recurse for all children
		Gfx_Element *child = element->GetFirst();
		while (child)
		{
			if (child->GetConfigure()->IsVisible())
				HRESULT hr = Display(child, level + 1);
			child = child->GetNext();
		}

	return SUCCESS_OK;
}


// ---------- CleanupDX ----------
/*!
\brief element cleanup dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::CleanupDX(
		Gfx_Element *element,
		UINT level
	)
{
	#if defined(OUTPUT_ELEMENT_INFO)
	OutputElementInfo(element, level * 2);
	#endif

	// ---- cleanup
		Element_CleanupDX(element);

	// ---- recurse for all children
		Gfx_Element *child = element->GetFirst();
		while (child)
		{
			HRESULT hr = CleanupDX(child, level + 1);
			child = child->GetNext();
		}

	return SUCCESS_OK;
}


// ---------- Cleanup ----------
/*!
\brief one time element cleanup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::Cleanup(
		Gfx_Element *element,
		UINT level
	)
{

	#if defined(OUTPUT_ELEMENT_INFO)
	OutputElementInfo(element, level * 2);
	#endif
	
	// ---- cleanup
		HRESULT hr;
		Gfx_Element_Component *gfx_element_component = element->GetComponent();
		try
		{

			// ---- kandinsky ?

			// ---- release vertex buffer
				LPDIRECT3DVERTEXBUFFER9 *vertex_buffer = gfx_element_component->GetVertexBuffer();
				if (*vertex_buffer != NULL)
				{
					hr = (*vertex_buffer)->Release();
					if (FAILED(hr)) throw(hr);
					*vertex_buffer = NULL;
				}

			// ---- release index buffer
				LPDIRECT3DINDEXBUFFER9 *index_buffer = gfx_element_component->GetIndexBuffer();
				if (*index_buffer != NULL)
				{
					hr = (*index_buffer)->Release();
					if (FAILED(hr)) throw(hr);
					*index_buffer = NULL;
				}

		}
		catch (HRESULT hr)
		{
			// ---- log
				Gfx_Log *gfx_log = GetGfxLog();
				if (gfx_log != NULL)
				{
					std::string line = "Gfx_Element_Engine: Cleanup failed with error: " + std::to_string((INT)hr) + "\n";
					gfx_log->Write(line);
				}
			return hr;
		}

	// ---- zap config bitmask
		gfx_element_component->SetConfigBitmask(0);

	// ---- recurse for all children
		Gfx_Element *child = element->GetFirst();
		while (child)
		{
			HRESULT hr = Cleanup(child, level + 1);
			child = child->GetNext();
		}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- system element methods ----------


// ---------- Element_SetupDX ----------
/*!
\brief element setup dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note guarding element->IsComponent()

*/
HRESULT Gfx_Element_Engine::Element_SetupDX(
		Gfx_Element *element
	)
{

	// ---- only components
		if (element->GetConfigure()->IsComponent())
		{

			// ---- setup dx
				HRESULT hr;
				Gfx_Element_Component *gfx_element_component = element->GetComponent();
				try
				{

					// ---- lambda
						auto IsBitSet = [](byte b, int pos) -> bool
						{
							return (b & (1 << pos)) != 0;
						};

					// ---- get element component kandinsky
						Gfx_Kandinsky *gfx_component_kandinsky = gfx_element_component->GetKandinsky();

					// ---- get element component kandinsky interface callbacks
						Gfx_Kandinsky_Interface_Callbacks *gfx_kandinsky_interface_callbacks = gfx_element_component->GetKandinskyInterfaceCallbacks();

					// ---- get element component configuration bitmask
						//
						// if a kandinsky component is to be recreated then force by
						// setting to Gfx_Component_Buffer_Bitmasks::KANDINSKY
						//
						UINT config_bitmask = 0;

					// ---- initialise kandinsky buffers
						if (!IsBitSet(config_bitmask,Gfx_Component_Buffer_Bitmasks::KANDINSKY))
						{

							// ---- initialise both kandinsky vertex & index buffers
								if (gfx_kandinsky_interface_callbacks->kandinsky_interface_config_and_create != NULL)
								{
									hr = gfx_kandinsky_interface_callbacks->kandinsky_interface_config_and_create(gfx_element_component);
									if (FAILED(hr)) throw(hr);
									config_bitmask += Gfx_Component_Buffer_Bitmasks::KANDINSKY;
								}

						}

					// ---- create vertex_buffer & then copy kandinsky_vertex_buffer into vertex_buffer
						if (!IsBitSet(config_bitmask,Gfx_Component_Buffer_Bitmasks::VERTEX_BUFFER))
						{  

							// ---- release vertex_buffer
								LPDIRECT3DVERTEXBUFFER9 *vertex_buffer = gfx_element_component->GetVertexBuffer();
								if (*vertex_buffer != NULL)
								{
									hr = (*vertex_buffer)->Release();
									if (FAILED(hr)) throw(hr);
									vertex_buffer = NULL;
								}

							// ---- get kandinsky vertex buffer info
								UINT kandinsky_vertex_buffer_size = gfx_component_kandinsky->GetVertexBufferSize() ;
								UINT kandinsky_vertex_format = gfx_component_kandinsky->GetVertexFormat();

							// ---- create vertex buffer
								hr = GetDevice()->CreateVertexBuffer(
										kandinsky_vertex_buffer_size * sizeof(FLOAT), // bytes!
										D3DUSAGE_WRITEONLY,
										kandinsky_vertex_format,
										D3DPOOL_MANAGED,
										&(*vertex_buffer),
										NULL
									);
								if (FAILED(hr)) throw(hr);
								config_bitmask += Gfx_Component_Buffer_Bitmasks::VERTEX_BUFFER;

							// ---- copy kandinsky_vertex_buffer into vertex_buffer
								FLOAT *kandinsky_vertex_buffer = 0;
								gfx_component_kandinsky->GetVertexBuffer(&kandinsky_vertex_buffer);
								FLOAT *f;
								hr = (*vertex_buffer)->Lock( 0, 0, (VOID**)&f, 0);
								if (FAILED(hr)) throw(hr);
								{
									memcpy(
											(VOID *)((FLOAT*)f),
											(VOID *)((FLOAT*)kandinsky_vertex_buffer),
											kandinsky_vertex_buffer_size * sizeof(FLOAT)
										);
									hr = (*vertex_buffer)->Unlock();
									if (FAILED(hr)) throw(hr);
								}

						}


					// ---- create index_buffer & then copy kandinsky_index_buffer into index_buffer
						if (!IsBitSet(config_bitmask,Gfx_Component_Buffer_Bitmasks::INDEX_BUFFER))
						{

							// ---- release index_buffer
								LPDIRECT3DINDEXBUFFER9 *index_buffer = gfx_element_component->GetIndexBuffer();
								if (*index_buffer != NULL)
								{
									hr = (*index_buffer)->Release();
									if (FAILED(hr)) throw(hr);
									index_buffer = NULL;
								}
						
							// ---- get kandinsky index buffer info
								UINT kandinsky_index_buffer_size = gfx_component_kandinsky->GetIndexBufferSize();

							// ---- create index buffer
								if (kandinsky_index_buffer_size > 0)
								{
									hr = GetDevice()->CreateIndexBuffer(
											kandinsky_index_buffer_size * sizeof(UINT),
											D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC,
											D3DFMT_INDEX32,
											D3DPOOL_DEFAULT,
											index_buffer,
											NULL
										);
									if (FAILED(hr)) throw(hr);
									config_bitmask += Gfx_Component_Buffer_Bitmasks::INDEX_BUFFER;
								}

							// ---- copy kandinsky_index_buffer into index_buffer
								//
								// note: seperate gated block of logic for clarity
								//
								if (kandinsky_index_buffer_size > 0)
								{
									UINT *kandinsky_index_buffer = 0;
									gfx_component_kandinsky->GetIndexBuffer(&kandinsky_index_buffer);
									UINT *u;
									hr = (*index_buffer)->Lock( 0, 0, (VOID**)&u, 0);
									if (FAILED(hr)) throw(hr);
									{
										memcpy(
												(VOID *)((UINT*)u),
												(VOID *)((UINT*)kandinsky_index_buffer),
												kandinsky_index_buffer_size * sizeof(UINT)
											);
										hr = (*index_buffer)->Unlock();
										if (FAILED(hr)) throw(hr);
									}
								}
						}


					// ---- store bitmask
						gfx_element_component->SetConfigBitmask(config_bitmask);

				}
				catch (HRESULT hr)
				{
					// ---- log
						Gfx_Log *gfx_log = GetGfxLog();
						if (gfx_log != NULL)
						{
							std::string line = "Gfx_Element_Engine: Element_SetupDX failed with error: " + std::to_string((INT)hr) + "\n";
							gfx_log->Write(line);
						}
					return hr;
				}

		}

	return SUCCESS_OK;
}



// ---------- Element_Transform ----------
/*!
\brief if set, invoke element transform method
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note only invoked from Transform method
      which has guarding element->IsVisible(),
      --- so needs to be guarded 


*/
HRESULT Gfx_Element_Engine::Element_Transform(Gfx_Element *element)
{

	if (element->GetConfigure()->IsVisible())
	{
		Gfx_Element_Configure *configure = element->GetConfigure();

		// ---- coordinates
			Gfx_Element_Coordinate *coordinate = element->GetCoordinate();
			vsl_system::Vsl_Vector3 scale;
			vsl_system::Vsl_Vector3 rotate;
			vsl_system::Vsl_Vector3 translate;
			coordinate->GetScale(scale);
			coordinate->GetRotate(rotate);
			coordinate->GetTranslate(translate);

		// ---- matrices
			D3DXMATRIX world;
			D3DXMatrixIdentity(&world);

		// ---- start with translation
			D3DXMatrixTranslation(&world, translate.x, translate.y, translate.z);
				
		// ---- then rotation
			D3DXMATRIX temp, rotation;
			D3DXMatrixIdentity(&rotation);

			std::string rotation_order;
			coordinate->GetRotationOrder(rotation_order);
			for (UINT index = 0; index < 3; index++)
			{
				switch (rotation_order.at(index))
				{
					case 'P': // pitch
					case 'X': // pitch
						D3DXMatrixRotationX(&temp, rotate.x);
						D3DXMatrixMultiply(&rotation, &rotation, &temp);
						break;
					case 'H': // heading
					case 'Y': // yaw
						D3DXMatrixRotationY(&temp, rotate.y);
						D3DXMatrixMultiply(&rotation, &rotation, &temp);
						break;
					case 'B': // bank
					case 'Z': // roll
						D3DXMatrixRotationZ(&temp, rotate.z);
						D3DXMatrixMultiply(&rotation, &rotation, &temp);
						break;
				}
			}

		// ---- multiply to effect rotation
			D3DXMatrixMultiply(&world, &rotation, &world);

		// ---- then lastly rotation
			D3DXMATRIX scaling;
			D3DXMatrixScaling(&scaling, scale.x, scale.y, scale.z);
			//D3DXMatrixIdentity(&scaling);
			//scaling._11 = scale.x;
			//scaling._22 = scale.y;
			//scaling._33 = scale.z;
			
		// ---- multiply to effect scaling
			D3DXMatrixMultiply(&scaling, &scaling, &temp);
			D3DXMatrixMultiply(&world, &scaling, &world);

		// ---- stack
			pimpl_gfx_element_engine->matrix_stack->MultMatrix(&world);

		// ---- get and set matrix
			D3DXMATRIX *top = pimpl_gfx_element_engine->matrix_stack->GetTop();
			configure->SetMatrix(*top);
	}

	return SUCCESS_OK;
}


// ---------- Element_Display ----------
/*!
\brief element display dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note guarding element->IsComponent()

*/
HRESULT Gfx_Element_Engine::Element_Display(
		Gfx_Element *element
	)
{

	// ---- only components
		if (element->GetConfigure()->IsComponent())
		{
			// ---- configuration
				Gfx_Element_Configure *gfx_element_configure = element->GetConfigure();

			// ---- if instance then (one time) find and set instance element
				Gfx_Element_Component *gfx_element_component = element->GetComponent();
				if (gfx_element_configure->IsInstance())
				{
					Gfx_Element *instance= gfx_element_configure->GetInstanceElement();
					if (instance == NULL)
					{
						std::string instance_name = gfx_element_configure->GetInstanceName();
						Gfx_Element *gfx_element_project_root = GetProjectRoot();
						instance = gfx_element_project_root->Find(instance_name);
						if (instance != NULL)
						{
							gfx_element_configure->SetInstanceElement(instance);
							gfx_element_component = instance->GetComponent();
						}
						else
						{
							// ---- failed to find instance component, so set NO instance
								gfx_element_configure->SetInstance(FALSE);
							
							// ---- LOG

						}
					}
					else
					{
						gfx_element_component = instance->GetComponent();
					}
				}

			// ---- get component creator
				HRESULT hr;
				Gfx_Kandinsky *gfx_component_kandinsky = gfx_element_component->GetKandinsky();

			// ---- vertex parameters
				UINT size_of_vertex_data   = gfx_component_kandinsky->GetVertexBufferSize();
				UINT size_of_vertex_format = gfx_component_kandinsky->GetVertexFormatSize();
				UINT format_of_vertex_data = gfx_component_kandinsky->GetVertexFormat();

			// ---- primtive parameters
				UINT primitive_type  = gfx_component_kandinsky->GetPrimitiveType();
				UINT primitive_count = gfx_component_kandinsky->GetPrimitiveCount();

			// ---- vertex buffer
				LPDIRECT3DVERTEXBUFFER9 *vertex_buffer = gfx_element_component->GetVertexBuffer();

			// ---- render states#
				LPDIRECT3DDEVICE9 device = GetDevice();
				hr = device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				hr = device->SetTexture(0, NULL);
				hr = device->SetStreamSource(0, *vertex_buffer, 0, size_of_vertex_format * 4);
				hr = device->SetFVF(format_of_vertex_data);

			// ---- matrix
				hr = device->SetTransform(D3DTS_WORLDMATRIX(0), gfx_element_configure->GetMatrix());

			// ---- no index buffer ?
				UINT size_of_index_buffer = gfx_component_kandinsky->GetIndexBufferSize();
				if (size_of_index_buffer == 0)
				{
					hr = device->DrawPrimitive(
							(D3DPRIMITIVETYPE)primitive_type,
							0,
						gfx_component_kandinsky->GetPrimitiveCount()
						);
				}
				else
				{
					LPDIRECT3DINDEXBUFFER9 *index_buffer = gfx_element_component->GetIndexBuffer();
					hr = device->SetIndices(*index_buffer);
					hr = device->DrawIndexedPrimitive(
							(D3DPRIMITIVETYPE)primitive_type,
							0, // Base vertex index
							0, // Min vertex index
							size_of_index_buffer,
							0, // Start index
							primitive_count
						);
				}

		}

	return SUCCESS_OK;
}


// ---------- Element_CleanupDX ----------
/*!
\brief element cleanup dx
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::Element_CleanupDX(
		Gfx_Element *element
	)
{

	// ---- only components
		if (element->GetConfigure()->IsComponent())
		{

			// ---- cleanup dx
				HRESULT hr;
				Gfx_Element_Component *gfx_element_component = element->GetComponent();
				try {

					// ---- release vertex buffer
						LPDIRECT3DVERTEXBUFFER9 *vertex_buffer = gfx_element_component->GetVertexBuffer();
						if (*vertex_buffer != NULL)
						{
							hr = (*vertex_buffer)->Release();
							if (FAILED(hr)) throw(hr);
							*vertex_buffer = NULL;
						}

					// ---- release index buffer
						LPDIRECT3DINDEXBUFFER9 *index_buffer = gfx_element_component->GetIndexBuffer();
						if (*index_buffer != NULL)
						{
							hr = (*index_buffer)->Release();
							if (FAILED(hr)) throw(hr);
							*index_buffer = NULL;
						}

				}
				catch (HRESULT hr)
				{
					// ---- log
						Gfx_Log *gfx_log = GetGfxLog();
						if (gfx_log != NULL)
						{
							std::string line = "Gfx_Element_Engine: Element_CleanupDX failed with error: " + std::to_string((INT)hr) + "\n";
							gfx_log->Write(line);
						}
					return hr;
				}
									
			// ---- zap config bitmask
				gfx_element_component->SetConfigBitmask(0);
		}

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Log ----------
/*!
\brief output GfxElement data to logfile and debug Output Window
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::Log(VOID)
{

	OutputDebugString("---- Log ----\n");

	if (Gfx_Element *gfx_element_project_root = GetProjectRoot())
	{
		Gfx_Log *gfx_log = GetGfxLog();
		if (gfx_log != NULL)
		{
			gfx_log->SetShowSimple(TRUE);
			UINT level = 1;
			HRESULT hr = Log(gfx_element_project_root, level);
			if (FAILED(hr)) return ERROR_FAIL;
			gfx_log->SetShowSimple(FALSE);
		}
	}

	return SUCCESS_OK;
}

// ---------- Log ----------
/*!
\brief recursively output data for each GfxElement to logfile and debug Output Window
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)

\note snprintf usage : prevent overflows by enforcing a maximum size for `str` buffer

*/
HRESULT Gfx_Element_Engine::Log(Gfx_Element *element, UINT level)
{

	// ---- local
		Gfx_Log *gfx_log = GetGfxLog();
		const int max_size_of_msg = 256;
		CHAR msg[max_size_of_msg];

	// ---- lambda
		auto write_begin = [gfx_log, &msg, max_size_of_msg](std::string name, UINT level)
		{
			snprintf(msg, max_size_of_msg, "%*s%s{", level * 3, "", name.c_str());
			gfx_log->Write(msg);
		};
		auto write_element = [gfx_log, &msg, max_size_of_msg](Gfx_Element *element, UINT level) -> BOOL
		{
			BOOL is_name_value_pair = FALSE;
			size_t len = element->GetValue().length();
			if (len == 0 )
			{
				snprintf(msg, max_size_of_msg, "%*s%s{", level * 3, "", element->GetName().c_str());
			}
			else
			{
				std::string pair = element->GetName() + ":" + element->GetValue() + ";";
				snprintf(msg, max_size_of_msg, "%*s%s", level * 3, "", pair.c_str());
				is_name_value_pair = TRUE;
			}
			gfx_log->Write(msg);
			return is_name_value_pair;
		};
		auto write_end = [gfx_log, &msg, max_size_of_msg](UINT level)
		{
			snprintf(msg, max_size_of_msg, "%*s%s", level * 3, "", "}");
			gfx_log->Write(msg);
		};


	// ---- recurse for all param groups, recurse for each param
		Gfx_Element *param_group = element->GetFirstParamGroup();
		while (param_group)
		{
			Gfx_Element *param = param_group->GetFirst();
			if (param)
			{
				write_begin(param_group->GetName(), level);
				while (param)
				{
					write_begin(param->GetName(), level+1);
					HRESULT hr = Log(param, level + 2);
					write_end(level+1);
					param = param->GetNext();
				}
				write_end(level);
			}
			param_group = param_group->GetNext();

		}

	// ---- recurse for all children
		Gfx_Element *child = element->GetFirst();
		while (child)
		{
			// huh .... size_t len = child->GetValue().length();
			BOOL is_name_value_pair = write_element(child, level);
			HRESULT hr = Log(child, level + 1);
			if (!is_name_value_pair) write_end(level);
			child = child->GetNext();
		}

	return SUCCESS_OK;
}

// ---------- Log ----------
/*!
\brief output "Gfx Engine:" + [message] to logfile and debug Output Window
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Gfx_Element_Engine::Log(
		std::string message
	)
{
	Gfx_Log *gfx_log = GetGfxLog();
	if (gfx_log != NULL)
	{
		gfx_log->Write("Gfx Engine: " + message);
		//OutputDebugString(message.c_str());
		return SUCCESS_OK;
	}
	return ERROR_FAIL;
};


////////////////////////////////////////////////////////////////////////////////


VOID Gfx_Element_Engine::AddBookMark(Gfx_Element *element)
{
	pimpl_gfx_element_engine->gfx_element_bookmarks.push_back(element);
}

std::list <vsl_library::Gfx_Element *> Gfx_Element_Engine::GetBookMarks()
{
	return pimpl_gfx_element_engine->gfx_element_bookmarks;
}


////////////////////////////////////////////////////////////////////////////////


Gfx_Element *Gfx_Element_Engine::GetEngineRoot(VOID)
{
	return &pimpl_gfx_element_engine->gfx_element_engine_root;
}

Gfx_Log *Gfx_Element_Engine::GetGfxLog(VOID)
{
	return pimpl_gfx_element_engine->gfx_log;
}

Gfx_Element *Gfx_Element_Engine::GetProjectRoot(VOID)
{
	return pimpl_gfx_element_engine->gfx_element_project_root;
}

HRESULT Gfx_Element_Engine::SetGfxLog(Gfx_Log *log)
{
	// TBD error verify log
	pimpl_gfx_element_engine->gfx_log = log;
	return SUCCESS_OK;
}

HRESULT Gfx_Element_Engine::SetGfxProject(Gfx_Element *element)
{
	// TBD error verify element
	pimpl_gfx_element_engine->gfx_element_project_root = element;
	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


LPDIRECT3DDEVICE9 Gfx_Element_Engine::GetDevice(VOID)
{
	return pimpl_gfx_element_engine->device;
}

HRESULT Gfx_Element_Engine::SetDevice(LPDIRECT3DDEVICE9 device)
{
	pimpl_gfx_element_engine->device = device;
	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////
