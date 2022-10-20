////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_surface_01.cpp ----------
/*!
\file vsl_surface_01.cpp
\brief implementation of the Surface_V01 class derived from the Base_01 class
\author Gareth Edwards
*/

// ---- application
	#include "../header/vsl_surface_01.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_dotobj.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_application;


////////////////////////////////////////////////////////////////////////////////


// ---------- Gfx_Setup_Project --------
/*!
\brief setup gfx element projects
\author Gareth Edwards
*/
HRESULT Surface_01::Gfx_Setup_Project(VOID)
{
	// ---- rename
		vsl_system::Win_Create *win_cr8 = Get_Win_Create();
		win_cr8->SetName("Surface_V01");

	// ---- scope
		using namespace vsl_library;

	// ---- stop gap for "read file"
		try
		{

			// ---- gfx component generator
				Gfx_Kandinsky gfx_kandinsky;

			// ---- assemble test data
				Gfx_Element *engine_root_element = NULL;
				if (engine_root_element = GetElementEngine()->GetEngineRoot())
				{
					if (Gfx_Element *project = engine_root_element->Find("Project"))
					{

						// ---- element to be instanced
							Gfx_Element *element_tr0 = project->Append("TR0", 0);

							Gfx_Element_Configure *configure = element_tr0->GetConfigure();
							configure->SetComponentName("PyRhoDo_VBO");
							configure->SetVisible(FALSE);
							 
						// ---- group id 1 : element id 4 (or could be 3!)
							Gfx_Element *element_group_1 = project->Append("G1", 1);
							GetElementEngine()->AddBookMark(element_group_1);
							{
								Gfx_Element *element_tr1 = element_group_1->Append("TR1", 4);
								GetElementEngine()->AddBookMark(element_tr1);

								Gfx_Element_Configure *configure = element_tr1->GetConfigure();
								configure->SetComponentName("Cuboid_VBO");
								configure->SetVisible(TRUE);
								configure->SetInstance(TRUE);
								configure->SetInstanceName(element_tr0->GetName());
							}

						// ---- group id 2 : element id 4
							Gfx_Element *element_group_2 = project->Append("G2", 2);
							GetElementEngine()->AddBookMark(element_group_2);
							{
								Gfx_Element *element_tr2 = element_group_2->Append("TR2", 4);
								GetElementEngine()->AddBookMark(element_tr2);

								Gfx_Element_Configure *configure = element_tr2->GetConfigure();
								configure->SetComponentName("Cuboid_VIBO");
								configure->SetVisible(TRUE);
								configure->SetInstance(TRUE);
								configure->SetInstanceName(element_tr0->GetName());
							}

					}
					else throw("Project");
				}
				else throw("Engine");

			// ---- report
				engine_root_element->List();

		}
		catch (CHAR *element_name)
		{
			CHAR msg[128];
			sprintf_s(msg, 128, "Element %s not found!\n Select OK to Exit", element_name);
			INT msgboxID = MessageBox(NULL, msg, "VSL method: Base_01::Fw_Setup()", MB_ICONWARNING);
			return FALSE;
		}	

	return SUCCESS_OK;
}


// ---------- Gfx_Setup_Configurations --------
/*!
\brief setup gfx element configurations
\author Gareth Edwards
*/
HRESULT Surface_01::Gfx_Setup_Configurations(VOID)
{
	return SUCCESS_OK;
}


// ---------- Gfx_Setup_Coordinates --------
/*!
\brief setup gfx element coordinates
\author Gareth Edwards
*/
HRESULT Surface_01::Gfx_Setup_Coordinates(VOID)
{
	
	// ---- scope
		using namespace vsl_library;

	// ---------- update element properties & parameters ----------
		Gfx_Element *engine_root_element = GetElementEngine()->GetEngineRoot();

	// ---------- update element coordinate properties ----------
		Gfx_Element *element_tr1 = engine_root_element->Find("TR1");
		if (element_tr1 != NULL)
		{
			Gfx_Element_Coordinate *coordinate = element_tr1->GetCoordinate();
			vsl_system::Vsl_Vector3 v = { 1, 2, 1 };
			coordinate->SetScale(vsl_system::Vsl_Vector3(1, 2, 1));
		}

	return SUCCESS_OK;
}


// ---------- Gfx_Setup_Components --------
/*!
\brief setup gfx element components
\author Gareth Edwards
*/
HRESULT Surface_01::Gfx_Setup_Components(VOID)
{

	// ---- scope
		using namespace vsl_library;

	// ---------- update element properties & parameters ----------
		Gfx_Element *engine_root_element = GetElementEngine()->GetEngineRoot();
		Gfx_Element *element_tr0 = engine_root_element->Find("TR0");
		if (element_tr0 != NULL)
		{
			// ---- PyRhoDo_VBO
				Gfx_Element_Component *component = element_tr0->GetComponent();
				Gfx_Kandinsky *kandinsky = component->GetKandinsky();

			// ---- build - include inside -> update kandinsky properties
				kandinsky->Set(Gfx_Kandinsky_Param::INSIDE, 1);

			// ---- move -> update both element & corresponding kandinsky parameters
				Gfx_Element *element_param_group = element_tr0->FindParamGroup("Component");
				if (element_param_group)
				{
					std::string move = "0.5";
					HRESULT hr = element_param_group->SetParameterValue("Dimension", "Move", move);
					if (SUCCEEDED(hr))
					{
						kandinsky->SetParameterValue("Dimension", "Move", move);
					}
				}

		}

	return SUCCESS_OK;
}


// ---------- Gfx_Element_Bookmarks --------
/*!
\brief handle gfx elements that have been bookmarked
\author Gareth Edwards
*/
HRESULT Surface_01::Gfx_Element_Bookmarks(VOID)
{

	// ---- scope
		using namespace vsl_library;

	// ----- using bookmarks - update element coordinates
		{

			// ---- time
				auto to_radian = [](FLOAT degree) ->FLOAT { return - degree * 3.1415296499f / 180.0f; };

				FLOAT time_elapsed = Get_Win_Engine()->GetMsElapsed();
				FLOAT rotate = to_radian(time_elapsed / 10);

			// ---- bookmarks
				std::list<vsl_library::Gfx_Element *>bookmarks = GetElementEngine()->GetBookMarks();
				std::list<Gfx_Element *>::iterator bookmarks_begin = bookmarks.begin();
				std::list<Gfx_Element *>::iterator bookmarks_end = bookmarks.end();

				for (std::list<Gfx_Element*>::iterator bookmarks = bookmarks_begin; bookmarks != bookmarks_end; ++bookmarks)
				{
					Gfx_Element *element = *bookmarks;
					Gfx_Element_Coordinate *coordinate = element->GetCoordinate();
					switch (element->GetId())
					{
						case 1:
							{
								coordinate->SetTranslate(vsl_system::Vsl_Vector3(10, 0, 0));
								coordinate->SetRotate(vsl_system::Vsl_Vector3(0, rotate, 0));
							}
							break;
						case 2:
							{
								coordinate->SetTranslate(vsl_system::Vsl_Vector3(-10, 0, 0));
								coordinate->SetRotate(vsl_system::Vsl_Vector3(-rotate, -rotate, 0));
							}
							break;
						case 3:
						case 4:
							{
								FLOAT t_value = 0;
								GetCommand()->GetToggleValue((CHAR)'T', &t_value);
								coordinate->SetRotate(vsl_system::Vsl_Vector3(0, to_radian(t_value), 0));
							}
							break;
						default:
							break;
					}
				}

		}

	return SUCCESS_OK;
}


// ---------- Update_If_Key_Pressed ----------
/*!
\brief update application state (key async pressed)
\author Gareth Edwards
*/
VOID Surface_01::Update_If_AsyncKey_Pressed(VOID)
{

	// ---- global
		vsl_system::Win_Engine *win_eng = Get_Win_Engine();
		vsl_library::Gfx_Command *gfx_command = GetCommand();

	// ---- local
		DWORD key_just_pressed = gfx_command->GetKeyJustPressed() & 0x8000f;

	// ---- check async
		if (GetAsyncKeyState(';') & 0x8000f)
		{
			win_eng->SetCmdLineMode(TRUE);
			Sleep(200);
			OutputDebugString("{\n");
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
		{
			win_eng->SetCmdLineMode(FALSE);
			Sleep(200);
			OutputDebugString("}\n");
		}

}


// ---------- Update_On_Screen_Text --------
/*!
\brief update on screen text (e.g. app name, instructions, fps)
\author Gareth Edwards
*/
VOID Surface_01::Update_On_Screen_Text(VOID)
{

	// ---- local
		HRESULT hr;
		D3DCOLOR colour = D3DCOLOR_XRGB(255, 255, 255);

	// ---- position text (left, top, right, bottom)

		RECT rct = { 20, 20, 700, 40 };
		hr = GetD3dx()->DisplayText(
				Get_Win_Create()->GetName(),
				colour,
				rct
			);

		rct.top += 20;
		rct.bottom += 20;
		hr = GetD3dx()->DisplayText(
				Get_Win_Engine()->GetFpsReport(),
				colour,
				rct
			);

	// ---- console
		rct.top = Get_Win_Create()->GetHeight() - 160;
		rct.bottom = rct.top + 20;
		for (UINT i = 0; i < 6; i++)
		{

			// note: unformatted std::string line_number = std::to_string(i);
			// note: from C++17 can use std::to_chars instead in a similar way
			//       to std::snprintf, and from C++20 can use std::format.

			char buffer[20];
			std::snprintf(buffer, 20, "%.4d", i);
			std::string line_number(buffer);

			std::string line_text = "take sword";
			hr = GetD3dx()->DisplayText(
					line_number + ": " + line_text,
					colour,
					rct
				);

			rct.top += 20;
			rct.bottom += 20;
		}
}


////////////////////////////////////////////////////////////////////////////////
