////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element.h ----------
/*!
\file vsl_gfx_element.h
\brief Gfx_Element classes
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element_configure.h"
	#include "../../vsl_library/header/vsl_gfx_element_coordinate.h"
	#include "../../vsl_library/header/vsl_gfx_element_component.h"


////////////////////////////////////////////////////////////////////////////////


namespace vsl_library
{

	class Gfx_Element
	{

	public:

		// ---- cdtor
			Gfx_Element(VOID);
			virtual Gfx_Element::~Gfx_Element();

		// ---- build element (& param groups)
			Gfx_Element *Append(VOID);
			Gfx_Element *Append(std::string name);
			Gfx_Element *Append(std::string name, UINT id);
			Gfx_Element *Append(std::string name, std::string value);
			Gfx_Element *Append(std::string name, std::string value, UINT id);
			Gfx_Element *InsertAfter(VOID);
			Gfx_Element *InsertAfter(std::string name, UINT id);

		// ---- utility
			VOID List(VOID);
			Gfx_Element *Find(std::string name);

		// ---- get stuff
			Gfx_Element_Configure  *GetConfigure(VOID);
			Gfx_Element_Coordinate *GetCoordinate(VOID);
			Gfx_Element_Component  *GetComponent(VOID);

		// ---- set stuff
			VOID SetComponent(Gfx_Element_Component *component);

		// ---- get properties
			UINT   GetId(VOID);
			std::string GetName(VOID);
			std::string GetValue(VOID);

		// ---- set properties
			VOID   SetId(UINT id);
			VOID   SetName(std::string name);
			VOID   SetValue(std::string value);

		// ---- hierarchical node links
			Gfx_Element *GetParent(VOID);
			Gfx_Element *GetFirst(VOID); 
			Gfx_Element *GetLast(VOID);
			Gfx_Element *GetPrevious(VOID);
			Gfx_Element *GetNext(VOID);
			VOID  SetParent(Gfx_Element *parent);
			VOID  SetFirst(Gfx_Element *first);
			VOID  SetLast(Gfx_Element *last);
			VOID  SetPrevious(Gfx_Element *previous);
			VOID  SetNext(Gfx_Element *next);

		// ---- parameter group

			Gfx_Element *AppendParamGroup(std::string name);
			Gfx_Element *FindParamGroup(std::string name);
			Gfx_Element *GetFirstParamGroup(VOID);
			Gfx_Element *GetLastParamGroup(VOID);

			HRESULT SetParameterValue(std::string group,
					std::string name,
					std::string value
				);
			VOID SetFirstParamGroup(Gfx_Element *first_param_group);
			VOID SetLastParamGroup(Gfx_Element *last_param_group);

	private:

		// ---- private implementation
			class Pimpl_Gfx_Element;
			std::unique_ptr<Pimpl_Gfx_Element> pimpl_gfx_element;

	};

}


////////////////////////////////////////////////////////////////////////////////

namespace vsl_library
{

	class Node {

	public:
		Node(VOID);
		~Node();

	private:
		Node *parent = NULL;
		Node *first = NULL;
		Node *last = NULL;
		Node *previous = NULL;
		Node *next = NULL;

	public:

		VOID Append(Node *child);
		VOID InsertAfter(Node *append);

		Node *GetParent(VOID);
		Node *GetFirst(VOID);
		Node *GetLast(VOID);
		Node *GetPrevious(VOID);
		Node *GetNext(VOID);

		VOID  SetParent(Node *parent);
		VOID  SetFirst(Node *first);
		VOID  SetLast(Node *last);
		VOID  SetPrevious(Node *previous);
		VOID  SetNext(Node *next);

	};

}

////////////////////////////////////////////////////////////////////////////////
