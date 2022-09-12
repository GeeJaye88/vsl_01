////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_base_01.cpp ----------
/*!
\file vsl_base_01.cpp
\brief implementation of PyRhoDo_01 class derived from Base_01 class
\author Gareth Edwards
*/

// ---- application
	#include "../header/vsl_base_01.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_dotobj.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_application;


////////////////////////////////////////////////////////////////////////////////


// ----- Base_01 base class derived from private implementation Pimpl_Base_01!

class Base_01::Pimpl_Base_01
{

	public:

	// ---- cdtor
		Pimpl_Base_01() {}

	// ---- required by windows framework: 
		vsl_system::Win_Create fw_win_create;
		vsl_system::Win_Engine fw_win_engine;

	// ---- required gfx:
		vsl_library::Gfx_Command gfx_command;
		vsl_library::Gfx_D3dx    gfx_d3dx;
		vsl_library::Gfx_Log     gfx_log;

		vsl_library::Gfx_Element_Engine gfx_element_engine;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---------- cdtor ----------

Base_01::Base_01(VOID) : pimpl_base_v01(new Pimpl_Base_01)
{

	// ---- local
		HRESULT hr;

	// ---- win: Win_Create - get & initialise window create parameters 
		vsl_system::Win_Create *win_cr8 = Get_Win_Create();
		win_cr8->SetName("Base_01");
		win_cr8->SetCentred(TRUE);
		win_cr8->SetDesktop(FALSE);
		win_cr8->SetDimensions(800,600);
		win_cr8->SetAaq(4);

	// ---- win: Get_Win_Engine - get & initialise windows engine parameters
		vsl_system::Win_Engine *win_eng = Get_Win_Engine();
		win_eng->SetColour(92, 92, 92);
		win_eng->SetFps(60);

	// ---- gfx: Gfx_Command (IO)
		vsl_library::Gfx_Command *gfx_com = GetCmd();
		gfx_com->SetDefaultMouseWheelClick(-50);
		hr = gfx_com->SetToggle((CHAR)'T');

	// ---- gfx: Gfx_D3dx (Direct 3D)
		vsl_library::Gfx_D3dx *gfx_d3dx = GetD3D();
		gfx_d3dx->Set_Win_Create(win_cr8);
		gfx_d3dx->Set_Gfx_Command(gfx_com);

	// ---- gfx: Gfx_Log
		vsl_library::Gfx_Log *gfx_log = GetLog();
		gfx_log->SetShowLineNumber(TRUE);
		gfx_log->SetShowDate(FALSE);
		gfx_log->SetShowTime(TRUE);
		gfx_log->SetShowSimple(FALSE);

	// ---- gfx: Gfx_Log - write this app name
		hr = gfx_log->WriteBanner(win_cr8->GetName());

	// ---- gfx: Gfx_Element_Engine
		vsl_library::Gfx_Element_Engine *gfx_gee = GetGEE();
		gfx_gee->SetGfxLog(gfx_log);
}

Base_01::~Base_01()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---- private implementation interface

// ---- get Win structs
	vsl_system::Win_Create  *Base_01::Get_Win_Create(VOID) { return &pimpl_base_v01->fw_win_create; }
	vsl_system::Win_Engine  *Base_01::Get_Win_Engine(VOID) { return &pimpl_base_v01->fw_win_engine; }

// ---- get Gfx objects
	vsl_library::Gfx_Command *Base_01::GetCmd(VOID) { return &pimpl_base_v01->gfx_command; }
	vsl_library::Gfx_D3dx    *Base_01::GetD3D(VOID) { return &pimpl_base_v01->gfx_d3dx; }
	vsl_library::Gfx_Log     *Base_01::GetLog(VOID) { return &pimpl_base_v01->gfx_log; }
	vsl_library::Gfx_Element_Engine *Base_01::GetGEE(VOID) { return &pimpl_base_v01->gfx_element_engine; }



////////////////////////////////////////////////////////////////////////////////


// ---------- framework: methods ----------


// ---------- Fw_Setup ----------
/*!
\brief required by framework: setup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Base_01::Fw_Setup(VOID)
{

	// ---- local
		HRESULT hr;

	// ---- scope
		using namespace vsl_library;

	// ---- device
		hr = GetD3D()->Setup();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- setup & config hierarchical project structure
		hr = Gfx_Setup_Project();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- recursively setup elements, element instancing & verify kandinsky component
		hr = GetGEE()->Setup();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- setup specific element coordinate values
		hr = Gfx_Setup_Coordinates();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- setup specific element component values
		hr = Gfx_Setup_Components();
		if (FAILED(hr)) return ERROR_FAIL;

	return SUCCESS_OK;
}


// ---------- Fw_SetupDX ----------
/*!
\brief required by framework: setup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Base_01::Fw_SetupDX(LPDIRECT3DDEVICE9 device)
{

	// ---- local
		HRESULT hr;

	// ---- d3d
		hr = GetD3D()->SetupDX(device);
		if (FAILED(hr)) return hr;

	// ---- setup DX gfx element engine
		hr = GetGEE()->SetupDX(device);
		if (FAILED(hr)) return hr;

	// --- ?
		if (FALSE)
		{
			using namespace vs_library;

			// ---- create
				DotObjElement *object = new DotObjElement();

			// ---- read
				CHAR *filename = "object\\teapot\\teapot.obj";
				vs_library::DotObjUtilities dot_obj_utilities;
				HRESULT hr = (HRESULT)dot_obj_utilities.Read(object, filename);
				if (SUCCEEDED(hr))
				{
					dot_obj_utilities.Report(object, filename);
				}
		}

	return SUCCESS_OK;
}


// ---------- Fw_Display ----------
/*!
\brief required by framework: display
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Base_01::Fw_Display(LPDIRECT3DDEVICE9 device)
{

	// ---- local
		HRESULT hr;

	// ---- log timed event
		//DWORD seconds_last = Get_Win_Engine()->GetSecondsLast();
		//DWORD seconds_now = Get_Win_Engine()->GetSecondsNow();
		//if (seconds_now % 1 == 0 && seconds_now != seconds_last)
		//{
		//	hr = GetLog()->Write("Test");
		//}

	// ---- update (no fail conditions)
		Update_Gfx_Command_Param();
		Update_If_AsyncKey_Pressed();
		Update_On_Screen_Text();

	// ---- handle gfx elements that have been bookmarked
		hr = Gfx_Element_Bookmarks();
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- d3d (no fail conditions)
		hr = GetD3D()->Projection(device);
		hr = GetD3D()->LookAtLH(device);

	// ---- set render states and lighting
		hr = GetD3D()->Display(device);
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- recursively display gfx elements
		hr = GetGEE()->Display();
		if (FAILED(hr)) return ERROR_FAIL;

	return SUCCESS_OK;
}


// ---------- Fw_CleanupDX ----------
/*!
\brief required by framework: cleanup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Base_01::Fw_CleanupDX (LPDIRECT3DDEVICE9 device)
{

	// ---- local
		HRESULT hr;

	//---- d3d
		hr = GetD3D()->CleanupDX(device);
		if (FAILED(hr)) return ERROR_FAIL;

	// ---- cleanup DX gfx element engine
		hr = GetGEE()->CleanupDX();
		if (FAILED(hr)) return ERROR_FAIL;

	return SUCCESS_OK;
}


// ---------- Fw_Cleanup ----------
/*!
\brief required by framework: cleanup
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT Base_01::Fw_Cleanup(VOID)
{
	
// ---- cleanup gfx element engine
	HRESULT hr = GetGEE()->Cleanup();
	if (FAILED(hr)) return ERROR_FAIL;

	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- framework get window structs ----------


// ---------- Fw_Get_Win_Create ----------
/*!
\brief required by framework: get pointer to wnd create struct
\author Gareth Edwards
\param Win_Create **wc
*/
VOID Base_01::Fw_Get_Win_Create(vsl_system::Win_Create **fw_win_create)
{
	*fw_win_create = Get_Win_Create();
}


// ---------- Fw_Get_Win_Engine ----------
/*!
\brief required by framework: get pointer to wnd engine struct
\author Gareth Edwards
\param Win_Engine **wc
*/
VOID Base_01::Fw_Get_Win_Engine(vsl_system::Win_Engine **fw_win_engine)
{
	*fw_win_engine = Get_Win_Engine();
}


////////////////////////////////////////////////////////////////////////////////


// ---------- framework Gfx_Command interface ----------


// ---------- Fw_Set_MouseLeftButtonDownMove ----------
/*!
\brief required by framework: if mouse left button down then update move x y
\author Gareth Edwards
\param FLOAT x
\param FLOAT y
*/
VOID Base_01::Fw_Set_MouseLeftButtonDownMove(INT x, INT y)
{
	GetCmd()->SetMouseLeftButtonDownMove((FLOAT)x, (FLOAT)y);
}


// ---------- Fw_Set_MouseWheelClick ----------
/*!
\brief required by framework: if mouse wheel click event update move z
\author Gareth Edwards
\param intd d (+/- 1)
*/
VOID Base_01::Fw_Set_MouseWheelClick(INT d)
{
	FLOAT mouse_wheel_click = GetCmd()->GetMouseWheelClick();
	mouse_wheel_click += (FLOAT)d;

	GetCmd()->SetMouseWheelClick(mouse_wheel_click);
}


// ---------- Fw_Set_Keydown ----------
/*!
\brief required by framework: if key down handle toggle, etc.
\author Gareth Edwards
\param WPARAM (parameter)
*/
VOID Base_01::Fw_Set_Keydown(WPARAM param)
{
	GetCmd()->SetToggle((CHAR)param);
}


////////////////////////////////////////////////////////////////////////////////


// ---------- Gfx_Setup_Project --------
/*!
\brief setup gfx element projects
\author Gareth Edwards
*/
HRESULT Base_01::Gfx_Setup_Project(VOID)
{

	// ---- scope
		using namespace vsl_library;

	// ---- stop gap for "read file"
		try
		{

			// ---- assemble test data
				Gfx_Element *engine_root_element = NULL;
				if (engine_root_element = GetGEE()->GetEngineRoot())
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
							GetGEE()->AddBookMark(element_group_1);
							{
								Gfx_Element *element_tr1 = element_group_1->Append("TR1", 4);
								GetGEE()->AddBookMark(element_tr1);

								Gfx_Element_Configure *configure = element_tr1->GetConfigure();
								configure->SetComponentName("Cuboid_VBO");
								configure->SetVisible(TRUE);
								configure->SetInstance(TRUE);
								configure->SetInstanceName(element_tr0->GetName());
							}

						// ---- group id 2 : element id 4
							Gfx_Element *element_group_2 = project->Append("G2", 2);
							GetGEE()->AddBookMark(element_group_2);
							{
								Gfx_Element *element_tr2 = element_group_2->Append("TR2", 4);
								GetGEE()->AddBookMark(element_tr2);

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
HRESULT Base_01::Gfx_Setup_Configurations(VOID)
{
	return SUCCESS_OK;
}


// ---------- Gfx_Setup_Coordinates --------
/*!
\brief setup gfx element coordinates
\author Gareth Edwards
*/
HRESULT Base_01::Gfx_Setup_Coordinates(VOID)
{
	
	// ---- scope
		using namespace vsl_library;

	// ---------- update element component properties & parameters ----------
		Gfx_Element *engine_root_element = GetGEE()->GetEngineRoot();

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
HRESULT Base_01::Gfx_Setup_Components(VOID)
{

	// ---- scope
		using namespace vsl_library;

	// ---------- update element properties & parameters ----------
		Gfx_Element *engine_root_element = GetGEE()->GetEngineRoot();
		Gfx_Element *element_tr0 = engine_root_element->Find("TR0");
		if (element_tr0 != NULL)
		{
			// ---- PyRhoDo_VBO
				Gfx_Element_Component *component = element_tr0->GetComponent();
				Gfx_Kandinsky *kandinsky = component->GetKandinsky();

			// ---- build - include inside -> update element component properties
				kandinsky->Set(Gfx_Kandinsky_Param::INSIDE, 1);

			// ---- move -> update both component & corresponding kandinsky parameters
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
HRESULT Base_01::Gfx_Element_Bookmarks(VOID)
{

	// ---- scope
		using namespace vsl_library;

	// ----- using bookmarks - update element coordinates
		{

			// ---- time
				auto to_radian = [](FLOAT degree) ->FLOAT { return degree * 3.1415296499f / 180.0f; };

				FLOAT time_elapsed = Get_Win_Engine()->GetMsElapsed();
				FLOAT rotate = to_radian(time_elapsed / 10);

			// ---- bookmarks
				std::list<vsl_library::Gfx_Element *>bookmarks = GetGEE()->GetBookMarks();
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
								GetCmd()->GetToggleValue((CHAR)'T', &t_value);
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


////////////////////////////////////////////////////////////////////////////////


// ---------- Gfx_Read_Project_SDL --------
/*!
\brief
\author Gareth Edwards
*/
VOID Base_01::Gfx_Read_Project_SDL(VOID)
{



}


////////////////////////////////////////////////////////////////////////////////


// ---- application housekeeping methods


// ---------- Update_Gfx_Command_Param --------
/*!
\brief update Win_Command parameters
\author Gareth Edwards
*/
VOID Base_01::Update_Gfx_Command_Param(VOID)
{
	GetCmd()->Update(
			Get_Win_Engine()->GetMsDelta()
		);
}


// ---------- Update_If_Key_Pressed ----------
/*!
\brief update application state (key async pressed)
\author Gareth Edwards
*/
VOID Base_01::Update_If_AsyncKey_Pressed(VOID)
{

	// ---- store
		vsl_library::Gfx_Command *gfx_com = GetCmd();
		gfx_com->SetKeyLastPressed(gfx_com->GetKeyJustPressed());

	// ---- local
		DWORD  key_just_pressed = 0;

	// ---- key pressed ?
		if ( GetAsyncKeyState(VK_LEFT)  & 0x8000f )
			key_just_pressed = VK_LEFT;
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			key_just_pressed = VK_RIGHT;
		else if ( GetAsyncKeyState('1') & 0x8000f )
			key_just_pressed = 1;
		else if ( GetAsyncKeyState('2') & 0x8000f )
			key_just_pressed = 2;
		else if ( GetAsyncKeyState('3') & 0x8000f )
			key_just_pressed = 3;
		else if ( GetAsyncKeyState('4') & 0x8000f )
			key_just_pressed = 4;
		else if ( GetAsyncKeyState('5') & 0x8000f )
			key_just_pressed = 5;
		else if ( GetAsyncKeyState('6') & 0x8000f )
			key_just_pressed = 6;
		else if ( GetAsyncKeyState('7') & 0x8000f )
			key_just_pressed = 7;
		else if ( GetAsyncKeyState('8') & 0x8000f )
			key_just_pressed = 8;
		else if ( GetAsyncKeyState('X') & 0x8000f )
			key_just_pressed = 'X';
		else
			key_just_pressed = 0;

	// ---- store
		gfx_com->SetKeyJustPressed(key_just_pressed);

	// ---- not pressed ?
		if (key_just_pressed == 0 )
			return;

	// ---- select action (if any)
		switch (key_just_pressed )
		{
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				Sleep(250);
				break;
			case 8:
				Sleep(250);
				break;
			case 'X':
				gfx_com->Reset();
				break;
			default:
				break;
		}

	// ---- report
		#if DEBUG
		CHAR ods[128];
		sprintf_s(ods, 128, " +-> key just pressed is %d \n", key_just_pressed);
		OutputDebugString(ods);
		#endif
}


// ---------- Update_On_Screen_Text --------
/*!
\brief update on screen text (e.g. app name, instructions, fps)
\author Gareth Edwards
*/
VOID Base_01::Update_On_Screen_Text(VOID)
{

	// ---- local
		HRESULT hr;
		D3DCOLOR colour = D3DCOLOR_XRGB(255, 255, 255);

	// ---- position text (left, top, right, bottom)

		RECT rct = { 20, 20, 700, 40 };
		hr = GetD3D()->DisplayText(
				Get_Win_Create()->GetName(),
				colour,
				rct
			);

		rct.top = Get_Win_Create()->GetHeight() - 80;
		rct.bottom = rct.top + 20;
		hr = GetD3D()->DisplayText(
				Get_Win_Engine()->GetFpsReport(),
				colour,
				rct
			);

}


////////////////////////////////////////////////////////////////////////////////