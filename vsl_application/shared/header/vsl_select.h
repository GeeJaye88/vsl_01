////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_select.h ----------
/*!
\file vsl_select.h
\brief
   - define ineapplication token strings
   - set application token string
   - set global application pointer "g_app"
\author Gareth Edwards
*/

#if _MSC_VER > 1000
#pragma once
#endif


////////////////////////////////////////////////////////////////////////////////


// ---- define application token strings
	#define VSL_TEMPLATE    0
	#define VSL_MESH3D      1
	#define VSL_LSYSTEM     2

	#define VSL_LIMAGE      3

	#define VSL_Base_01     4 // test cubes
	#define VSL_PyRhoDo_01  5 // Py(ramidal) Rho(mbic) Do(decahedron)
	#define VSL_Surface_01  6 // pyramidal rhombic dodecahedron


// ---- set application token string
	#define VSL_APP VSL_Base_01


// ---- set global application pointer "g_app"
	#if VSL_APP == VSL_MESH3D
		#include "../../vsl_application/mesh3d/header/vsl_mesh3d.h"
		vsl_application::Mesh3D *g_app = new vsl_application::Mesh3D();
	#elif VSL_APP == VSL_LSYSTEM
		#include "../../vsl_application/lsystem/header/vsl_lsystem.h"
		vsl_application::LSystem *g_app = new vsl_application::LSystem();
	#elif VSL_APP == VSL_LIMAGE
		#include "../../vsl_application/lsystem/header/vsl_lsystem.h"
		vsl_application::LSystem *g_app = new vsl_application::LSystem();
	#elif VSL_APP == VSL_Base_01
		#include "../../vsl_application/framework/header/vsl_base_01.h"
		vsl_application::Base_01 *g_app = new vsl_application::Base_01();
	#elif VSL_APP == VSL_PyRhoDo_01
		#include "../../vsl_application/framework/header/vsl_base_01.h"
		#include "../../vsl_application/framework/header/vsl_pyrhodo_01.h"
		vsl_application::PyRhoDo_01 *g_app = new vsl_application::PyRhoDo_01();
	#elif VSL_APP == VSL_Surface_01
		#include "../../vsl_application/framework/header/vsl_base_01.h"
		#include "../../vsl_application/framework/header/vsl_surface_01.h"
		vsl_application::Surface_01 *g_app = new vsl_application::Surface_01();
	#else
		#include "../../vsl_application/template/header/vsl_template.h"
		vsl_application::Template *g_app = new vsl_application::Template();
	#endif


////////////////////////////////////////////////////////////////////////////////



