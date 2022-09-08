////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx.cpp ----------
/*!
\file vsl_gfx.cpp
\brief Implementation of the vsl_gfx class
\author Gareth Edwards 
*/


#include "../../vsl_library/header/vsl_gfx.h"


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_WndCreate class ----------

class VSL_Gfx::PI_VSL_Gfx
{

public:

	// ---- constructor/destructor ----
		PI_VSL_Gfx() { ; }
		~PI_VSL_Gfx() { ; }

		Element elm_root;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- ctor ----------

VSL_Gfx::VSL_Gfx()
{
	pi_vsl_gfx = new PI_VSL_Gfx();
}

VSL_Gfx::~VSL_Gfx()
{
	delete pi_vsl_gfx;
	pi_vsl_gfx = NULL;
}


////////////////////////////////////////////////////////////////////////////////


// ---------- framework methods ----------


// ---------- FW_Setup ----------
/*!
\brief framework setup
\author Gareth Edwards
\return bool (TRUE if ok)
*/
bool VSL_Gfx::FW_Setup()
{

	Element *elm_root = GetElmRoot();

	Element *all_lists = elm_root->Append("Lists", 0, 0);

	Element *dsp_lists = all_lists->Append("Display", 0, 0);
		dsp_lists->Append("Axis", 0, 0);
		dsp_lists->Append("Surface", 0, 0);
		dsp_lists->Append("Light A", 0, 0);
		dsp_lists->Append("Light B", 0, 0); 

		elm_root->List();

		int n = 12345;
 
	return true;
}


// ---------- FW_SetupDX ----------
/*!
\brief framework setup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return bool (TRUE if ok)
*/
bool VSL_Gfx::FW_SetupDX(LPDIRECT3DDEVICE9 device)
{

	return true;
}


// ---------- FW_Display ----------
/*!
\brief framework display
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return bool (TRUE if ok)
*/
bool VSL_Gfx::FW_Display(LPDIRECT3DDEVICE9 device)
{

	return true;
}


// ---------- FW_Cleanup dx ----------
/*!
\brief framework cleanup dx
\author Gareth Edwards
\param LPDIRECT3DDEVICE9 - pointer to an IDirect3DDevice9 structure
\return bool (TRUE if ok)
*/
bool VSL_Gfx::FW_CleanupDX (LPDIRECT3DDEVICE9 device)
{

 
	return true;
}


// ---------- Cleanup ----------
/*!
\brief framework cleanup
\author Gareth Edwards
\return bool (TRUE if ok)
*/
bool VSL_Gfx::FW_Cleanup()
{
	
	return true;
}


////////////////////////////////////////////////////////////////////////////////


// ---- cdtor
	Node::Node() { ; }
	Node::~Node() { ; }

// ---- create

	void Node::Append(Node *child)
	{
		child->parent = this;
		if ( first == NULL && last == NULL )
		{
			first = last = child;
		}
		else
		{
			Node * current_last = this->last;
			this->last = child;
			current_last->next = child;
			child->previous = current_last;
		}
	};

	void Node::InsertAfter(Node *sibling)
	{
		if ( next == NULL )
		{
			sibling->previous = this;
			if ( parent != NULL )
			{
				parent->last = sibling;
			}
		}
		else
		{
			sibling->next = this->next;
			sibling->previous = this->previous;
		}
	};

	Node *Node::GetParent()   { return parent;  }
	Node *Node::GetFirst()    { return first; }
	Node *Node::GetLast()     { return last; }
	Node *Node::GetPrevious() { return previous; }
	Node *Node::GetNext()     { return next; }

	void  Node::SetParent(Node *parent)     { this->parent = parent; }
	void  Node::SetFirst(Node *first)       { this->first = first; }
	void  Node::SetLast(Node *last)         { this->last = last; }
	void  Node::SetPrevious(Node *previous) { this->previous = previous; }
	void  Node::SetNext(Node *next)         { this->next = next; }


////////////////////////////////////////////////////////////////////////////////


// ---------- Private Implementation of PI_Element class ----------

class Element::PI_Element
{

public:

	// ---- constructor/destructor ----
		PI_Element() { ; }
		~PI_Element() { ; }

	// ---- properties
		INT   id = 0;
		DWORD handle = 0;
		std::string name;
		Element * parent   = NULL;
		Element * first    = NULL;
		Element * last     = NULL;
		Element * previous = NULL;
		Element * next     = NULL;

};


////////////////////////////////////////////////////////////////////////////////


// ---- cdtor
	Element::Element()
	{
		pi_element = new PI_Element();
		pi_element->id = 0;
	}
	Element::Element(INT id)
	{
		pi_element = new PI_Element();
		pi_element->id = id;
	}
	Element::~Element()
	{
		delete pi_element;
		pi_element = NULL;
	}


// ---- get
	INT     Element::GetId()       { return pi_element->id;    }
	DWORD   Element::GetHandle()   { return pi_element->handle; }
	std::string Element::GetName() { std::string something(pi_element->name); return something; };

// ---- set
	void    Element::SetHandle(DWORD handle)   { pi_element->handle = handle; }
	void    Element::SetId(INT id)             { pi_element->id = id; }
	void    Element::SetName(std::string name) { pi_element->name = name; };


// ---- create

	Element * Element::Append()
	{
		Element *child = new Element();
		child->pi_element->parent = this;
		if ( pi_element->first == NULL && pi_element->last == NULL )
		{
			pi_element->first = pi_element->last = child;
		}
		else
		{
			Element * current_last = pi_element->last;
			pi_element->last = child;
			current_last->pi_element->next = child;
			child->pi_element->previous = current_last;
		}
		return child;
	}

	Element * Element::Append(std::string name, DWORD handle, INT id)
	{
		Element *child = Append();
		child->SetHandle(handle);
		child->SetId(id);
		child->SetName(name);
		return child;
	}

	Element * Element::InsertAfter()
	{
		Element *sibling = new Element();
		if ( pi_element->next == NULL )
		{
			sibling->pi_element->previous = this;
			if ( pi_element->parent != NULL )
			{
				pi_element->parent->pi_element->last = sibling;
			}
		}
		else
		{
			sibling->pi_element->next = this->pi_element->next;
			sibling->pi_element->previous = this->pi_element->previous;
		}
		return sibling;
	}

	Element * Element::InsertAfter(std::string name, DWORD handle, INT id)
	{
		Element *sibling = InsertAfter();
		sibling->SetHandle(handle);
		sibling->SetId(id);
		sibling->SetName(name);
		return sibling;
	}


// ---- list

	void Element::List()
	{
		CHAR msg[128];
		Element *child = this->GetFirst();
		while (child != NULL )
		{
			Element * first = child->GetFirst();
			sprintf_s(msg, 128, "%s\n", child->GetName().c_str());
			OutputDebugString(msg);
			if (first) first->List();
			child = child->GetNext();
		}
	}


// ---- access

	Element * Element::GetParent()   { return pi_element->parent;  }
	Element * Element::GetFirst()    { return pi_element->first; }
	Element * Element::GetLast()     { return pi_element->last; }
	Element * Element::GetPrevious() { return pi_element->previous; }
	Element * Element::GetNext()     { return pi_element->next; }

	void  Element::SetParent(Element *parent)     { pi_element->parent = parent; }
	void  Element::SetFirst(Element *first)       { pi_element->first = first; }
	void  Element::SetLast(Element *last)         { pi_element->last = last; }
	void  Element::SetPrevious(Element *previous) { pi_element->previous = previous; }
	void  Element::SetNext(Element *next)         { pi_element->next = next; }

////////////////////////////////////////////////////////////////////////////////


Element * VSL_Gfx::GetElmRoot() { return &pi_vsl_gfx->elm_root; }


////////////////////////////////////////////////////////////////////////////////
