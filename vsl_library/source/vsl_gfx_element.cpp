////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_gfx_element.cpp ----------
/*!
\file vsl_gfx_element.cpp
\brief Implementation of the Gfx_Element class
\author Gareth Edwards 
*/

// ---- library
	#include "../../vsl_library/header/vsl_gfx_element.h"


////////////////////////////////////////////////////////////////////////////////


using namespace vsl_library;


////////////////////////////////////////////////////////////////////////////////


// ---------- private implementation ----------

class Gfx_Element::Pimpl_Gfx_Element
{

public:

	// ---- cdtor
		Pimpl_Gfx_Element(VOID)
		{
			configure  = new Gfx_Element_Configure();
			coordinate = new Gfx_Element_Coordinate();
			component  = new Gfx_Element_Component();
		}
		~Pimpl_Gfx_Element()
		{
			if (configure != NULL)
			{
				delete configure;
				configure = NULL;
			}
			if (coordinate != NULL)
			{
				delete coordinate;
				coordinate = NULL;
			}
			if (component != NULL)
			{
				delete component;
				component = NULL;
			}
		}

	// ---- properties

	// ---- gfx
		Gfx_Element_Configure  *configure  = NULL;
		Gfx_Element_Coordinate *coordinate = NULL;
		Gfx_Element_Component  *component  = NULL;

	// ---- state
		UINT  id = 0;
		std::string name;
		std::string value;

	// ---- hierarchical links
		Gfx_Element *parent   = NULL;
		Gfx_Element *first    = NULL;
		Gfx_Element *last     = NULL;
		Gfx_Element *previous = NULL;
		Gfx_Element *next     = NULL;

	// ---- parameter group links
		Gfx_Element *first_param_group = NULL;
		Gfx_Element *last_param_group = NULL;

};


////////////////////////////////////////////////////////////////////////////////


// ---------- implementation ----------


// ---- cdtor
	Gfx_Element::Gfx_Element(VOID) : pimpl_gfx_element(new Pimpl_Gfx_Element)
	{
		;
	}
	Gfx_Element::~Gfx_Element()
	{
		;
	}

// ---- create

	Gfx_Element *Gfx_Element::Append(VOID)
	{
		Gfx_Element *child = new Gfx_Element();
		child->pimpl_gfx_element->parent = this;
		if ( pimpl_gfx_element->first == NULL && pimpl_gfx_element->last == NULL )
		{
			pimpl_gfx_element->first = pimpl_gfx_element->last = child;
		}
		else
		{
			Gfx_Element * current_last = pimpl_gfx_element->last;
			pimpl_gfx_element->last = child;
			current_last->pimpl_gfx_element->next = child;
			child->pimpl_gfx_element->previous = current_last;
		}
		return child;
	}

	Gfx_Element *Gfx_Element::Append(std::string name)
	{
		Gfx_Element *child = Append();
		child->SetName(name);
		return child;
	}

	Gfx_Element *Gfx_Element::Append(std::string name, UINT id)
	{
		Gfx_Element *child = Append();
		child->SetId(id);
		child->SetName(name);
		return child;
	}

	Gfx_Element *Gfx_Element::Append(std::string name, std::string value)
	{
		Gfx_Element *child = Append();
		child->SetName(name);
		child->SetValue(value);
		return child;
	}

	Gfx_Element *Gfx_Element::Append(std::string name, std::string value, UINT id)
	{
		Gfx_Element *child = Append();
		child->SetId(id);
		child->SetName(name);
		child->SetValue(value);
		return child;
	}

	Gfx_Element *Gfx_Element::InsertAfter(VOID)
	{
		Gfx_Element *sibling = new Gfx_Element();
		if ( pimpl_gfx_element->next == NULL )
		{
			sibling->pimpl_gfx_element->previous = this;
			if ( pimpl_gfx_element->parent != NULL )
			{
				pimpl_gfx_element->parent->pimpl_gfx_element->last = sibling;
			}
		}
		else
		{
			sibling->pimpl_gfx_element->next = this->pimpl_gfx_element->next;
			sibling->pimpl_gfx_element->previous = this->pimpl_gfx_element->previous;
		}
		return sibling;
	}

	Gfx_Element *Gfx_Element::InsertAfter(std::string name, UINT id)
	{
		Gfx_Element *sibling = InsertAfter();
		sibling->SetId(id);
		sibling->SetName(name);
		return sibling;
	}


// ---- list

	VOID Gfx_Element::List(VOID)
	{

		// ---- avoid passing depth...
			const UINT spacing = 2;
			UINT depth = 0;
			Gfx_Element *parent = this->GetParent();
			while (parent != NULL)
			{
				depth += spacing;
				parent = parent->GetParent();
			}

		// ---- list children
			CHAR msg[128];
			Gfx_Element *child = this->GetFirst();
			while (child != NULL)
			{
				sprintf_s( msg, 128, "%*s%s\n", depth, "", child->GetName().c_str() );
				OutputDebugString(msg);
				child->List();
				child = child->GetNext();
			}

	}

// ---- search

	Gfx_Element *Gfx_Element::Find(std::string name)
	{
		if (name == this->GetName()) return this;

		Gfx_Element *child = this->GetFirst();
		while (child != NULL )
		{
			if (name == child->GetName()) return child;
			Gfx_Element *found = child->Find(name);
			if (found) return found;
			child = child->GetNext();
		}

		return NULL;
	}

// ---- get parameter groups

	Gfx_Element_Configure *Gfx_Element::GetConfigure(VOID)
		{ return pimpl_gfx_element->configure; }
	Gfx_Element_Coordinate *Gfx_Element::GetCoordinate(VOID)
		{ return pimpl_gfx_element->coordinate; }
	Gfx_Element_Component *Gfx_Element::GetComponent(VOID)
		{ return pimpl_gfx_element->component; }

	VOID Gfx_Element::SetComponent(Gfx_Element_Component *component)
		{ pimpl_gfx_element->component = component; }

// ---- get properties

	UINT Gfx_Element::GetId(VOID)
		{ return pimpl_gfx_element->id; }
	std::string Gfx_Element::GetName(VOID)
		{ std::string something(pimpl_gfx_element->name); return something; }
	std::string Gfx_Element::GetValue(VOID)
		{ std::string something(pimpl_gfx_element->value); return something; }

// ---- set properties

	VOID Gfx_Element::SetId(UINT id)             { pimpl_gfx_element->id = id; }
	VOID Gfx_Element::SetName(std::string name)  { pimpl_gfx_element->name = name; };
	VOID Gfx_Element::SetValue(std::string value){ pimpl_gfx_element->value = value; };

// ---- links

	Gfx_Element *Gfx_Element::GetParent(VOID)   { return pimpl_gfx_element->parent;  }
	Gfx_Element *Gfx_Element::GetFirst(VOID)    { return pimpl_gfx_element->first; }
	Gfx_Element *Gfx_Element::GetLast(VOID)     { return pimpl_gfx_element->last; }
	Gfx_Element *Gfx_Element::GetPrevious(VOID) { return pimpl_gfx_element->previous; }
	Gfx_Element *Gfx_Element::GetNext(VOID)     { return pimpl_gfx_element->next; }

	VOID Gfx_Element::SetParent(Gfx_Element *parent)     { pimpl_gfx_element->parent = parent; }
	VOID Gfx_Element::SetFirst(Gfx_Element *first)       { pimpl_gfx_element->first = first; }
	VOID Gfx_Element::SetLast(Gfx_Element *last)         { pimpl_gfx_element->last = last; }
	VOID Gfx_Element::SetPrevious(Gfx_Element *previous) { pimpl_gfx_element->previous = previous; }
	VOID Gfx_Element::SetNext(Gfx_Element *next)         { pimpl_gfx_element->next = next; }

// ---- parameter group

	/*
		Note: parameter groups do not require a kandinsky object
	*/

	Gfx_Element *Gfx_Element::AppendParamGroup(std::string name)
	{
		Gfx_Element *parent = this;

		Gfx_Element *new_param_group = new Gfx_Element();
		new_param_group->SetParent(parent);
		new_param_group->SetName(name);

		Gfx_Element *first_param_group = GetFirstParamGroup();
		Gfx_Element *last_param_group = GetLastParamGroup();

		if (first_param_group == NULL && last_param_group == NULL)
		{
			SetFirstParamGroup(new_param_group);
			SetLastParamGroup(new_param_group);
		}
		else
		{
			Gfx_Element *current_last = last_param_group;
			SetLastParamGroup(new_param_group);
			current_last->SetNext(new_param_group);
			new_param_group->SetPrevious(current_last);
		}
		return new_param_group;
	}

	Gfx_Element *Gfx_Element::FindParamGroup(std::string name)
	{
		Gfx_Element *sibling = this->GetFirstParamGroup();
		while (sibling != NULL)
		{
			if (name == sibling->GetName()) return sibling;
			sibling = sibling->GetNext();
		}
		return NULL;
	}

	Gfx_Element *Gfx_Element::GetFirstParamGroup(VOID)
		{ return pimpl_gfx_element->first_param_group; }

	Gfx_Element *Gfx_Element::GetLastParamGroup(VOID)
		{ return pimpl_gfx_element->last_param_group; }

	HRESULT Gfx_Element::SetParameterValue(
			std::string group,
			std::string name,
			std::string value
		)
	{
		Gfx_Element *parameter_group = this->Find(group);
		if (parameter_group != NULL)
		{
			Gfx_Element *parameter_name = parameter_group->Find(name);
			if (parameter_name != NULL)
			{
				parameter_name->SetValue("1");
				return SUCCESS_OK;
			}
		}
		return ERROR_FAIL;
	}

	VOID Gfx_Element::SetFirstParamGroup(Gfx_Element *first_param_group)
		{ pimpl_gfx_element->first_param_group = first_param_group; }

	VOID Gfx_Element::SetLastParamGroup(Gfx_Element *last_param_group)
		{ pimpl_gfx_element->last_param_group = last_param_group; }


////////////////////////////////////////////////////////////////////////////////


// ---- cdtor
	Node::Node(VOID) { ; }
	Node::~Node() { ; }


// ---- create

	VOID Node::Append(Node *child)
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

	VOID Node::InsertAfter(Node *sibling)
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


// ---- access

	Node *Node::GetParent(VOID)   { return parent;  }
	Node *Node::GetFirst(VOID)    { return first; }
	Node *Node::GetLast(VOID)     { return last; }
	Node *Node::GetPrevious(VOID) { return previous; }
	Node *Node::GetNext(VOID)     { return next; }

	VOID  Node::SetParent(Node *parent)     { this->parent = parent; }
	VOID  Node::SetFirst(Node *first)       { this->first = first; }
	VOID  Node::SetLast(Node *last)         { this->last = last; }
	VOID  Node::SetPrevious(Node *previous) { this->previous = previous; }
	VOID  Node::SetNext(Node *next)         { this->next = next; }


////////////////////////////////////////////////////////////////////////////////
