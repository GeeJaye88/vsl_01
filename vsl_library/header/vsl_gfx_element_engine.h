////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element_engine.h ----------
/*!
\file vsl_gfx_element_engine.h
\brief Interface for the Gfx_Element_Engine class
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
		class Gfx_Kandinsky_Interface;
		class Gfx_Log;

	// ----class
		class Gfx_Element_Engine
		{

		public:

			// ---- cdtor
				Gfx_Element_Engine(VOID);
				virtual ~Gfx_Element_Engine();

			// ----  framework
				HRESULT Setup(VOID);
				HRESULT SetupDX(LPDIRECT3DDEVICE9 device);
				HRESULT Display();
				HRESULT CleanupDX();
				HRESULT Cleanup(VOID);

			// ---- recursion
				HRESULT Setup(Gfx_Element *element, UINT level);
				HRESULT SetupDX(Gfx_Element *element, UINT level);
				HRESULT Transform(Gfx_Element *element, UINT level);
				HRESULT Display(Gfx_Element *element, UINT level);
				HRESULT CleanupDX(Gfx_Element *element, UINT level);
				HRESULT Cleanup(Gfx_Element *element, UINT level);

			// ---- element
				HRESULT Element_SetupDX(Gfx_Element *element);
				HRESULT Element_Transform(Gfx_Element *element);
				HRESULT Element_Display(Gfx_Element *element);
				HRESULT Element_CleanupDX(Gfx_Element *element);

			// ---- bookmarks
				VOID AddBookMark(Gfx_Element *element);
				std::list <vsl_library::Gfx_Element *>GetBookMarks();

			// ---- housekeeping
				HRESULT Log(VOID);
				HRESULT Log(std::string message);
				HRESULT Log(Gfx_Element *element, UINT level);

			// ---- get
				Gfx_Element *GetEngineRoot(VOID);
				Gfx_Log     *GetGfxLog(VOID);
				Gfx_Element *GetProjectRoot(VOID);
				Gfx_Kandinsky_Interface *GetKandinskyConfig(VOID);

			// ---- set
				HRESULT SetGfxLog(Gfx_Log *log);
				HRESULT SetGfxProject(Gfx_Element *element);

			// ---- gfx device get & set
				LPDIRECT3DDEVICE9 GetDevice(VOID);
				HRESULT SetDevice(LPDIRECT3DDEVICE9 device);

		private:

			// ---- private implementation
				class Pimpl_Gfx_Element_Engine;
				std::unique_ptr<Pimpl_Gfx_Element_Engine> pimpl_gfx_element_engine;

		};

}

////////////////////////////////////////////////////////////////////////////////