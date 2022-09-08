////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_lsystem.cpp ----------
/*!
\file vsl_lsystem.cpp
\brief implementation of the LSystem class
\author Gareth Edwards
*/


// ----- include eLsystem header -----
	#include "../header/vsl_lsystem.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_application;


////////////////////////////////////////////////////////////////////////////////


// ---------- constructor ----------
/*!
\brief constructor
\author Gareth Edwards
*/
LSystem::LSystem()
{

	// ---- system windows config
		fw_win_create.SetName("VS Lite L System Demo");
		fw_win_create.SetDesktop(TRUE);

	// ---- display
		instructions_on = true;
		text_on         = true;
		turntable_on    = true;
		fog_on          = true;

	// ---- set default display values
		Display_SetDefaults();

	// ---- input
		key_just_pressed = 0;
		key_last_pressed = key_just_pressed;

}


// ---------- destructor ----------
/*!
\brief destructor
\author Gareth Edwards
*/
LSystem::~LSystem()
{
	;
}


////////////////////////////////////////////////////////////////////////////////


// ---- system methods


// ---------- Fw_Setup --------
/*!
\brief system one time setup 
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT LSystem::Fw_Setup()
{
	
	// ---- init props
		camera_y_offset    = -3;
		variable_selected  = 1;
		variable_increment = 0;


	// ---- auto
		auto init_material = [](D3DMATERIAL9 *d)
		{
			D3DCOLORVALUE zero = { 0, 0, 0, 0 };
			D3DCOLORVALUE full = { 1, 1, 1, 0 };
			d->Ambient  = zero;
			d->Emissive = full;
			d->Diffuse  = full;
			d->Specular = full;
			d->Power = 40;
		};


	// ---- material
		ZeroMemory(&lsystem_material, sizeof(D3DMATERIAL9));
		init_material(&lsystem_material);

	return SUCCESS_OK;
}


// ---------- Fw_SetupDX ----------
/*!
\brief system setup device dependant stuff
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 (device)
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT LSystem::Fw_SetupDX(LPDIRECT3DDEVICE9 p_d3d_device)
{

	// ---- create an instance of the font object
		D3DXCreateFont(
				p_d3d_device,
				20, 0, FW_NORMAL, 0,
				FALSE,
				DEFAULT_CHARSET, 
				OUT_DEFAULT_PRECIS, 
				DEFAULT_QUALITY, 
				DEFAULT_PITCH | FF_DONTCARE, 
				TEXT("Arial"),
				&font
			);

	// ---- start up with...
		Lsystem_ReadTextfile("tF.txt");
		Tree   = "F";             // display name
		rule_2 = "n/a";


	// ---- iterate through the text file
		Lsystem_Iterate(iteration_depth);

	// ---- draw instructions
		Lsystem_DrawTree();

	return SUCCESS_OK;

}


// ---------- Fw_Display ---------
/*!
\brief system render the list of vertices to the screen
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 (device)
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT LSystem::Fw_Display (LPDIRECT3DDEVICE9 p_d3d_device)
{

	// ---- update if key pressed
		Display_AsyncKeyState();


    // ---- local matrices
		D3DXMATRIX mat_view;
		D3DXMATRIX mat_spin;
		D3DXMATRIX matrix_rotation;
		D3DXMATRIX matrix_translation;
		D3DXMATRIX matrix_world;


	// ---- handle fog
		Lsystem_Fog(p_d3d_device);


	// ---- increment turntable ?
		if ( turntable_on ) 
			y_turntable += (DOUBLE)(fw_win_engine.GetMsDelta() / 20);


	// ---- view matrix
		D3DXMatrixIdentity( &mat_view);
		p_d3d_device->SetTransform( D3DTS_VIEW, &mat_view);


	// ---- rotation matrix
		D3DXMatrixRotationYawPitchRoll( &matrix_rotation, D3DXToRadian(x_rotate), D3DXToRadian(y_rotate), 0 );


	// ---- set render states
		p_d3d_device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		p_d3d_device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		p_d3d_device->SetMaterial(&lsystem_material);


	// ---- locate & turntable
		D3DXMatrixTranslation( &matrix_translation, 0, camera_y_offset, z_translate);
		D3DXMatrixRotationY(&mat_spin, D3DXToRadian( (FLOAT)y_turntable ));
		matrix_world = matrix_rotation * matrix_translation;
		matrix_world = mat_spin * matrix_world;
		p_d3d_device->SetTransform( D3DTS_WORLD, &matrix_world);


	// ---- FVF flags
		p_d3d_device->SetFVF(LVertex::FVF_Flags );


	// ---- pass list of vertices to LINELIST
		p_d3d_device->DrawPrimitiveUP( D3DPT_LINELIST, (INT)(verts.size() * 0.5f), &verts[0], sizeof(LVertex));


	// ---- toggle text
		if ( text_on )
		{
			Display_Text();
		}

	return SUCCESS_OK;
}


// ---------- Fw_CleanupDX --------
/*!
\brief system cleanup device dependant stuff
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 (device)
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT LSystem::Fw_CleanupDX(LPDIRECT3DDEVICE9 p_d3d_device)
{

	// ---- clean up font object
		if ( font != NULL )
		{
			INT nNewRefCount = font->Release();

			if ( nNewRefCount > 0 )
			{
				static CHAR strError[256];
				sprintf_s ( strError, 256,
					"The font object failed to cleanup properly.\n"
					"Release() returned a reference count of %d",
					nNewRefCount );
				MessageBox( NULL, strError, "ERROR", MB_OK | MB_ICONEXCLAMATION );
			}

			font = NULL;

		}
	
	return SUCCESS_OK;

}


// ---------- Fw_Cleanup --------
/*!
\brief system one time cleanup 
\author Gareth Edwards
\return HRESULT (SUCCESS_OK if ok)
*/
HRESULT LSystem::Fw_Cleanup( )
{
	return SUCCESS_OK;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- system get methods ----------


// ---------- Fw_Get_Win_Create ----------
/*!
\brief system get pointer to wnd create struct
\author Gareth Edwards
\param Win_Create **wc
*/
VOID LSystem::Fw_Get_Win_Create(vsl_system::Win_Create **wc)
	{ *wc = &fw_win_create; }


// ---------- Fw_Get_Win_Engine ----------
/*!
\brief system get pointer to wnd engine struct
\author Gareth Edwards
\param Win_Engine **wc
*/
VOID LSystem::Fw_Get_Win_Engine(vsl_system::Win_Engine **wc)
	{ *wc = &fw_win_engine; }


////////////////////////////////////////////////////////////////////////////////


// ---------- system notify event methods ----------


// ---------- Fw_Set_MouseLeftButtonDownMove ----------
/*!
\brief system notify mouse left button down move event
\author Gareth Edwards
\param FLOAT x
\param FLOAT y
*/
VOID LSystem::Fw_Set_MouseLeftButtonDownMove(INT x, INT y)
{
	fw_win_engine.SetMouseLeftButtonDownMove(x,y);
	x_rotate -= x;
	y_rotate -= y;
}


// ---------- Fw_Set_MouseWheelClick ----------
/*!
\brief system notify mouse wheel click event
\author Gareth Edwards
\param int d (+/- 1)
*/
VOID LSystem::Fw_Set_MouseWheelClick(INT d)
{
	fw_win_engine.SetMouseWheelClickDelta(d);
	z_translate += (FLOAT)d;
}


// ---------- Fw_Set_Keydown ---------
/*!
\brief system handle keyboard input
\author Gareth Edwards
\param WPARAM (param)
\return BOOL (TRUE if ok)
*/
VOID LSystem::Fw_Set_Keydown(WPARAM param)
{

	// ---- select actions
		switch( param )
	    {

		    case 'F':    // toggles fog
				{
					fog_on = fog_on ? FALSE : TRUE;
				}
				break;

		    case 'R':    // toggles rotation
				{
					turntable_on = turntable_on ? FALSE : TRUE;
				}
				break;

		    case 'T':    // toggles text on/off
				{
					text_on = text_on ? FALSE : TRUE;
				}
				break;

		    case 'V':    // toggles lsystem info & variables
				{
					instructions_on = instructions_on ? FALSE : TRUE;
				}
				break;

		    case 'X':    // reset
				Display_SetDefaults();
				break;

			case '1': // keys 1 - 8 read different 'Trees' from text files
				{
					Lsystem_ReadTextfile("newTree.txt");   // new text file
					Tree = "MkII Fern";            // display name
					rule_1 = "n/a";                // display n/a if no rule present
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);      // re-iterate rule(s)
					verts.clear();                 // clear vertex list
			        camera_y_offset = -3;          // adjust camera Y-axis
				}
				break;

			case '2':    
				{
					Lsystem_ReadTextfile("tA.txt");
					Tree = "A";
					rule_1 = "n/a";
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();   
			        camera_y_offset = -3;
				}
				break;

			case '3':
				{
					Lsystem_ReadTextfile("wikiTree.txt");
					Tree = "wikiTree";
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();
			        camera_y_offset = -3;
				}
				break;

			case '4':
				{

					Lsystem_ReadTextfile("tD.txt");
					Tree = "D";
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();	
			        camera_y_offset = -3;
				}
				break;

			case '5':
				{
					Lsystem_ReadTextfile("tE.txt");
					Tree = "E";
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();
			        camera_y_offset = -3;
				}
				break;

			case '6':
				{
					Lsystem_ReadTextfile("tF.txt");
					Tree = "F";
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();
			        camera_y_offset = -3;
				}
				break;

			case '7':
				{
					Lsystem_ReadTextfile("koch2.txt");
					Tree = "Koch-based 3D structure";
					rule_1 = "n/a";
					rule_2 = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();
			        camera_y_offset = -1;
				}
				break;

			case '8':
				{
					Lsystem_ReadTextfile("dragon.txt");
					Tree = "Dragon 2D";
					rule = "n/a";
					Lsystem_Iterate(iteration_depth);
					verts.clear();
			        camera_y_offset = -1;
					break;
				}
			case 'A':    // initials change variable selected
				{
					variable_selected = 1;
					verts.clear();
				}
				break;

			case 'L':
				{
					variable_selected = 2;
				    verts.clear();
				}
				break;

			case 'D':
				{
					variable_selected = 3;
					verts.clear();
					break;
				}

			case VK_LEFT:    // adjust selected variable with up and down arrows
				{
					verts.clear();
					Lsystem_SelectVariable(variable_increment = -1);
				}
				break;

			case VK_RIGHT:
				{
					verts.clear();
					Lsystem_SelectVariable(variable_increment = 1);
				}
				break;

			case VK_UP:
				camera_y_offset -= 0.005f;
				return;

			case VK_DOWN:
				camera_y_offset += 0.005f;
				return;
		}


	// ---- reset angle if out of range
		if ( current_angle > 360 ) { current_angle = 0;   }
		if ( current_angle < 0   ) { current_angle = 360; }


	// ---- re-draw
		Lsystem_DrawTree();

}


////////////////////////////////////////////////////////////////////////////////


// ---- lsystem specific methods


// ---------- Lsystem_ReadTextfile --------
/*!
\brief read a text file and seperate into component strings
\author Gareth Edwards
\param  CHAR * (fileName)
*/
VOID LSystem::Lsystem_ReadTextfile(CHAR *filename)
{

	// ---- create ifstream object
		std::ifstream configTxt;


	// ---- open text file
		CHAR paf[512];
		strcpy_s(paf, 512, "vsl_application/lsystem/data/");
		strcat_s(paf, 512, filename);
		configTxt.open(paf);


	// ---- fail ? - terminate with error
		if ( !configTxt ) 
		{
			std::cout << "Unable to open file";
			exit(1);
		}


	// ---- extract strings seperated by whitespace to seperate variables 
		configTxt >> current_angle;   
		configTxt >> current_length;
		configTxt >> iteration_depth; 
		configTxt >> axiom;
		configTxt >> rule;
		configTxt >> rule_1;
		configTxt >> rule_2;
		configTxt.close();


	// ---- set big_string to inital axiom before iteration
		big_string = axiom;    

}


// ---------- Lsystem_Iterate ----------
/*!
\brief iterate through the main L-system string
\author Gareth Edwards
\param  int (counter)
*/
VOID LSystem::Lsystem_Iterate(INT counter)
{

	// ---- 1st loop sets no. of times to loop through string
		for (INT i = 0; i < counter; i++)
		{

			// ---- empty string to copy into
				std::string string_next = "";
      
			// ---- 2nd loop iterates through main string 
				for (INT j = 0; j < (INT)big_string.length(); j++)
				{
					CHAR c = big_string.at(j);               // current CHAR in string   

					if ( c == 'F' )
					{
						string_next = string_next + rule;    // replace 'F' with rule 
					}
					else if ( c == 'X' )
					{
						string_next = string_next + rule_1;  // replace 'X' with rule_1
					}
					else if ( c == 'Y' )
					{
						string_next = string_next + rule_2;  // replace 'Y' with rule_2
					}
					else
					{
						string_next = string_next + c;       // otherwise insert current CHAR
					} 
  
				}

			// ---- copy into main string
				big_string = string_next;

			// ---- until 1st loop condition is met

		}


	// ---- complexity of tree proportional string length
		size_t big_string_length = big_string.length();


		#if DEBUG
		//CHAR ods[128];
		//sprintf_s(ods, 128, "big_string_length = %d\n", big_string_length);
		//OutputDebugString(ods);
		#endif
}  


// ---------- Lsystem_DrawTree ---------
/*!
\brief generate a list of vectors based on L-system symbols
\author Gareth Edwards
*/
VOID LSystem::Lsystem_DrawTree()
{        

	// ---- local rotation matrices for each axis
		D3DXMATRIX zRotation;
		D3DXMATRIX yRotation;
		D3DXMATRIX xRotation;


	// ---- local stack for last position vector
		std::stack<D3DXVECTOR3> position_stack;


	// ---- local stack for last direction matrix 
		std::stack<D3DXMATRIX> direction_stack;  


	// ---- set initial position vector
		position_stack.push(D3DXVECTOR3(0.0f, 0.0f, 0.0f));


	// ---- set inital direction to identity
		D3DXMATRIX initialDir;
		D3DXMatrixIdentity( &initialDir );


	// ---- set initial stack direction
		direction_stack.push(initialDir);
 

	// ---- iterate through string
		for (INT i = 0; i < (INT)big_string.length(); i++)
		{
				if ( big_string.at(i) == 'F' )
				{   

					v.pos = position_stack.top();                                            // set position vector to last position  
					verts.push_back(v);                                                      // add to list of vertices

					D3DXVECTOR4 direction_temp;                                              // temporary 4D vector to store direction transform output 
					D3DXVECTOR3 uP = D3DXVECTOR3(0, 1, 0);                                   // temporary 3D direction vector 
					D3DXVec3Transform( &direction_temp, &uP, &direction_stack.top());        // transform by current direction
					uP = D3DXVECTOR3(direction_temp.x, direction_temp.y, direction_temp.z);  // assign to temporary 3Dvector
					D3DXVec3Normalize(&uP, &uP);                                             // normalize direction vector

					D3DXVECTOR3 newPos = D3DXVECTOR3(0, 0, 0);                               // new position vector
					newPos += ( uP * current_length );                                       // new position = direction vector * length of travel
					D3DXMATRIX matTranslation;    
					D3DXMatrixTranslation(&matTranslation,
							position_stack.top().x,
							position_stack.top().y,
							position_stack.top().z
						);  
					D3DXVECTOR4 temp;                                                        // temporary 4D vector
					D3DXVec3Transform(&temp, &newPos, &matTranslation);                      // calculate new vector

					v.pos = D3DXVECTOR3(temp.x, temp.y, temp.z);                             // set new value for vpos
					verts.push_back(v);                                                      // add to vertex list
					position_stack.top() = D3DXVECTOR3(temp.x, temp.y, temp.z);              // and replace top of position stack
				}

				if (big_string.at(i) == '^')                                                  // rotation transforms current direction in axis depending on CHAR
				{
					D3DXVECTOR3 lastDir = direction_stack.top();                             // get last direction
					D3DXMatrixRotationZ(&zRotation, D3DXToRadian(current_angle));            // positive rotation around Z-axis
					D3DXMatrixMultiply(&direction_stack.top(),
						&zRotation, &direction_stack.top());
				}

				if (big_string.at(i) == 'v')
				{
					D3DXVECTOR3 lastDir = direction_stack.top();                             // get last direction
					D3DXMatrixRotationZ(&zRotation, D3DXToRadian(-current_angle));           // negative rotation around Z-axis
					D3DXMatrixMultiply(&direction_stack.top(),
						&zRotation, &direction_stack.top());
				}
   
				if (big_string.at(i) == '>')
				{
					D3DXVECTOR3 lastDir = direction_stack.top();                             // get last direction
					D3DXMatrixRotationY(&yRotation, D3DXToRadian(current_angle));            // positive rotation around Y-axis
					D3DXMatrixMultiply(&direction_stack.top(),
						&yRotation, &direction_stack.top());
				}
   
				if (big_string.at(i) == '<')
				{
					D3DXVECTOR3 lastDir = direction_stack.top();                             // get last direction
					D3DXMatrixRotationY(&yRotation, D3DXToRadian(-current_angle));           // negative rotation around Y-axis
					D3DXMatrixMultiply(&direction_stack.top(),
						&yRotation, &direction_stack.top());
				}  
   
				if (big_string.at(i) == '+')
				{
					D3DXVECTOR3 lastDir = direction_stack.top();                             // get last direction
					D3DXMatrixRotationX(&xRotation, D3DXToRadian(current_angle));            // positive rotation around X-axis
					D3DXMatrixMultiply(&direction_stack.top(),
						&xRotation, &direction_stack.top());
				}
   
				if (big_string.at(i) == '-')
				{
					D3DXVECTOR3 lastDir = direction_stack.top();                             // get last direction
					D3DXMatrixRotationX(&xRotation, D3DXToRadian(-current_angle));           // negative rotation around X-axis
					D3DXMatrixMultiply(&direction_stack.top(),
						&xRotation, &direction_stack.top());
				} 

				if (big_string.at(i) == '[')
				{
					position_stack.push(position_stack.top());                               // push current position onto position stack 
					direction_stack.push(direction_stack.top());                             // push current direction onto direction stack 
				}

				if (big_string.at(i) == ']')
				{
					position_stack.pop();                                                    // reset position stack to previous position 
					direction_stack.pop();                                                   // reset direction stack to previous direction
				}

		}

}


// ---------- Lsystem_Fog ---------
/*!
\brief render the list of vertices to the screen
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 (device)
\return BOOL (TRUE if ok)
*/
VOID LSystem::Lsystem_Fog(LPDIRECT3DDEVICE9 p_d3d_device)
{

	p_d3d_device->SetRenderState(D3DRS_FOGENABLE, fog_on);

	if ( fog_on )
	{

		DWORD fog_color = D3DCOLOR_XRGB(60, 90, 180);
		p_d3d_device->SetRenderState(D3DRS_FOGCOLOR, fog_color);

		FLOAT start = z_translate -3.0f;
		FLOAT end  = z_translate + 3.0f;

		p_d3d_device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		p_d3d_device->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&start));
		p_d3d_device->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&end));

	}

}


// ---------- Lsystem_SelectVariable ---------
/*!
\brief select and increment active L-System variable
\author Gareth Edwards
\param INT (var)
*/
VOID LSystem::Lsystem_SelectVariable(INT var)
{

	// ---- note: up and down arrows adjust active variable

		switch( variable_selected )
		{

			case 1:    // angle
				{
					current_angle += var;
				}
				break;

			case 2:    // length
				{
					current_length += (FLOAT)var / 1000.0f;
				}
				break;

			case 3:    // iteration depth 
				{
					iteration_depth += var;
					big_string = axiom;
					verts.clear();
					Lsystem_Iterate(iteration_depth);
				}
				break;

			default:
				break;
		}

}


////////////////////////////////////////////////////////////////////////////////



// ---------- Display_SetDefault ----------
/*!
\brief set display defaults
\author Gareth Edwards
*/
VOID LSystem::Display_SetDefaults()
{

	// ---- display
		x_rotate = 0;
		y_rotate = 0;
		z_translate = 10;
		y_turntable = 0;

	// ---- process
		camera_y_offset = 0;
}


// ---------- Display_Text --------
/*!
\brief  display instructions and variables text onscreen
\author Gareth Edwards
*/
VOID LSystem::Display_Text()
{

	// ---- font
		D3DCOLOR fontColor = D3DCOLOR_XRGB(255,255,255);


	// ---- position text
		RECT rct;
		rct.left   = 20;
		rct.right  = 500;
		rct.top    = 20;
		rct.bottom = rct.top+20;


	// ---- local string variable for text
		std::string text;


	// ---- local CHAR variable for variables and rules
		CHAR temp[33];


	// ---- title
		font->DrawText(NULL, (LPCSTR)fw_win_create.GetName().c_str(), -1, &rct, 0, fontColor);
		rct.top += 20; rct.bottom += 20;

		rct.top += 20; rct.bottom += 20;


	// ---- flag switches display text
		if ( instructions_on )
		{

			text = "Instructions : ";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);
			rct.top += 20; rct.bottom += 20;

			text = "  Select l-system with number keys 1 - 8";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Select active variable by initial, e.g. 'A' selects angle";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Increment camera y offset using up & down arrows";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);
			rct.top += 20; rct.bottom += 20;

			text = "  Increment active variable using left & right arrows";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Press 'F' to toggle fog";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);
			rct.top += 20; rct.bottom += 20;

			text = "  Press 'R' to toggle rotation";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);
			rct.top += 20; rct.bottom += 20;

			text = "  Press 'T' to toggle text";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);
			rct.top += 20; rct.bottom += 20;

			text = "  Press 'V' to display l-system info & variables";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

		}
		else
		{

			text = "Type : " + Tree;    
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Axiom = ";
			text += axiom;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Rule = ";
			text += rule;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Rule1 = ";
			text += rule_1;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = "  Rule2 = ";
			text += rule_2;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			rct.top += 20; rct.bottom += 20;

			text = "Variables : ";
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor);
			rct.top += 20; rct.bottom += 20;

			text = variable_selected == 1 ? ">" : "  ";
			text += "A : Angle = ";  
			sprintf_s(temp, "%u", current_angle);
			text += temp;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = variable_selected == 2 ? ">" : "  ";
			text += "L : Length = ";
			sprintf_s(temp, "%f", current_length);
			text += temp;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

			text = variable_selected == 3 ? ">" : "  ";
			text += "D : Depth = ";
			sprintf_s(temp, "%u", iteration_depth);
			text += temp;
			font->DrawText(NULL, (LPCSTR)text.c_str(), -1, &rct, 0, fontColor );
			rct.top += 20; rct.bottom += 20;

		}


	// ---- fps
		rct.top = fw_win_create.GetHeight() - 80;
		rct.bottom = rct.top + 20;
		font->DrawText(NULL, (LPCSTR)fw_win_engine.GetFpsReport().c_str(), -1, &rct, 0, fontColor);

}


// ---------- UpdateIfKeyPressed ----------
/*!
\brie update app if key async pressed
\author Gareth Edwards

\note OLD CODE - left in for reference

*/
VOID LSystem::Display_AsyncKeyState()
{
	;
}


////////////////////////////////////////////////////////////////////////////////