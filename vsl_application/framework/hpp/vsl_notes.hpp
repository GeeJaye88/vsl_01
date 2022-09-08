////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_notes.h ----------
/*!
\file vsl_notes.h
\author Gareth Edwards

\note ---------- NOT USED ----------

    - using a singleton as a global
	- shared pointer as a pimpl
	- lists & iterators in
	- intern string notes

*/

#if _MSC_VER > 1000
#pragma once
#endif

// ---- system
	#include "../../vsl_system/header/vsl_include.h"


////////////////////////////////////////////////////////////////////////////////

// ---------- USING A SIGLETON AS A GLOBAL ----------

/*!

Global Variables
================

The entire point of a global variable is to have a single instance that can be
accessed or modified by multiple modules, classes, etc.

A global variable may be the easiest to read and maintain solution.

Singletons
==========

A Singleton candidate must satisfy three requirements:

1. controls concurrent access to a shared resource.
2. access to the resource will be requested from multiple, disparate parts of the system.
3. there can be only one object.

If a Singleton candidate has only one or two of these requirements, a redesign
is almost always the correct option.

*/

class Singleton
{

	private:

		// ---- instance stored
			static Singleton *instance;

		// ---- private constructor to prevent instancing
			Singleton(VOID);

		// ---- no destructor required ( and therefore no memory leak)
			// as on exit application memory will be freed by the system

	public:

		// ---- C++ 11, you may leave the copy constructor and the copy 
			// assignment operator public but explicitly delete them
			Singleton(const Singleton&) = delete;
			Singleton& operator=(const Singleton&) = delete;

		// ---- static access method
			static Singleton *GetInstance(VOID);

		// ---- methods
			VOID SetDefault(VOID)
			{
				mouse_x = 0;
				mouse_y = -30;
				mouse_wheel = 25;
				key_t_value = 0;
			}

			VOID UpdateKeyTValue(FLOAT time_ms_delta)
			{
				key_t_value += (DOUBLE)(time_ms_delta / 20);
			}

		// ---- properties
			FLOAT  mouse_x = 0;
			FLOAT  mouse_y = 0;
			FLOAT  mouse_wheel = 0;
			DOUBLE key_t_value;

};

// ---- init null, because instance will be initialised on demand
	Singleton *Singleton::instance = 0;

// ---- static access method
	Singleton *Singleton::GetInstance(VOID)
	{
		if (instance == 0)
		{
			instance = new Singleton();
		}
		return instance;
	}

// ---- public constructor
	Singleton::Singleton(VOID) {}

// ---- & finally....
Singleton *whatever = Singleton::GetInstance();


////////////////////////////////////////////////////////////////////////////////

// ---------- SHARED POINTER PIMPL ----------

/*

	The deleter for a shared pointer is created here:

	Widget::Widget(): pImpl(new Impl) {}

	Until that point, all the shared pointer has is the equivalent
	of a std::funciton<VOID(Impl*)>.

	When you construct a shared_ptr with a T*, it writes a deleter
	and stores it in the std::function equivalent.

	At that point the type must be complete.

	So the only functions you have to define after Impl is fully
	defined are those that create a pImpl from a T* of some kind.

	Note: The unique_ptr<> template holds a pointer to an object
	and deletes this object when the unique_ptr<> object is deleted.

	So, in the example above, it does not matter if the function
	scope is left through the return statement, at the end of the
	function or even through an exception:

	The unique_ptr<> destructor is always called and therefore the
	object (int in the example) always deleted.

	Note: deleted when it goes out of scope.

	See: Herb Sutter’s Exceptional C++.
*/

// ---- interface

class Widget
{
public:
	Widget();
private:
	struct Impl;
	std::shared_ptr<Impl> pImpl;
};

// ---- implementation

struct Widget::Impl
{
	std::string name;
};

Widget::Widget() : pImpl(new Impl) {}


////////////////////////////////////////////////////////////////////////////////

// ---------- Lists & Iterators in C++ STL ----------
/*

begin () :
This function is used to return the beginning position of the container.

end () :
This function is used to return the after end position of the container.

advance () :
This function is used to increment the iterator position till the
specified number mentioned in its arguments.

*/

// ---- declaration of a struct containing a D3D9 vertex buffer
struct I3D_Obj
{
	I3D_Obj(std::string n, LPDIRECT3DVERTEXBUFFER9 vbs, INT np)
	{
		name = n;
		vb_source = vbs;
		num_prim = np;
	}
	std::string name;
	LPDIRECT3DVERTEXBUFFER9 vb_source = NULL;
	INT num_prim = 0;
};
std::list < I3D_Obj > ListOf_I3D_Obj;


// ---- setup a list (i.e., create & setup)
#if defined(OUPUTDEBUGSTRING)
OutputDebugString("Create & Fill VertexBuffers\n");
#endif
for (int i = 0; i < 6; i++)
{
	char c[4];
	sprintf_s(c, 4, "%d", i);
	std::string name = c;
	ListOf_I3D_Obj.push_back(I3D_Obj(name, NULL, 0));
}

// ---- iterate through list (i.e., render)
#if defined(OUPUTDEBUGSTRING)
OutputDebugString("Render Created VertexBuffers:");
#endif
std::list <I3D_Obj> ::iterator it;
for (it = ListOf_I3D_Obj.begin(); it != ListOf_I3D_Obj.end(); it++)
{
	#if defined(OUPUTDEBUGSTRING)
	OutputDebugString(" ");
	OutputDebugString(it->name.c_str());
#endif
}
#if defined(OUPUTDEBUGSTRING)
OutputDebugString("\n");
#endif

// ---- cleanup (i.e., release & cleanup)
#if defined(OUPUTDEBUGSTRING)
OutputDebugString("Release:");
#endif
while (!ListOf_I3D_Obj.empty())
{
	#if defined(OUPUTDEBUGSTRING)
	OutputDebugString(" ");
	OutputDebugString(ListOf_I3D_Obj.front().name.c_str());
	#endif
	if (ListOf_I3D_Obj.front().vb_source != NULL)
	{
		INT new_ref_count = ListOf_I3D_Obj.front().vb_source->Release();
		// error new_ref_count...?
		// should not need - ListOf_I3D_Obj.front().vb_source = NULL;
	}
	ListOf_I3D_Obj.pop_front();
}

#if defined(OUPUTDEBUGSTRING)
OutputDebugString("\n");
#endif


////////////////////////////////////////////////////////////////////////////////

// ---------- intern string notes ----------
/*

Q: What is the difference between <string> & <string.h>

A: Those are two different headers.

	<string>   is for c++ std::string class
	<string.h> is for c string functions(like strlen(), etc.),
				which should be <cstring> for c++ project.

A: The C++ Standard Library also incorporates 18 headers of the ISO C90
	C standard library ending with ".h", but their use is deprecated.

	All other headers in the C++ Standard Library DO NOT end in ".h".

	Each header from the C Standard Library is included in the C++ Standard
	Library under a different name, generated by removing the .h, and adding
	a 'c' at the start; for example, 'time.h' becomes 'ctime'.

A: The difference is: <cstring> wraps everything in the std namespace
	whereas <string.h> puts everything in the global namespace.

	Also, expect some stricter type safety rules from <cstring>

*/


////////////////////////////////////////////////////////////////////////////////

/*
	---------- Error Handling ----------

	S_FALSE: Everything is fine, just the image does not have such optional attribute.

	Force user to check hr == S_OK
	Querying a image without such optional attribute is not a error.
	E_FAIL : No!Something is wrong.You should not query this key.

	Client can easily check by FAILED(hr)
	Using this key to query this non - existed value is a error.

	Error Handling
	==============

	COM uses HRESULT values to indicate the success or failure of a method or function call.
	Various SDK headers define various HRESULT constants.
	A common set of system-wide codes is defined in WinError.h.

	The following table shows some of those system-wide return codes.

	E_ACCESSDENIED	0x80070005	Access denied.
	E_FAIL          0x80004005	Unspecified error.
	E_INVALIDARG	0x80070057	Invalid parameter value.
	E_OUTOFMEMORY	0x8007000E	Out of memory.
	E_POINTER       0x80004003	NULL was passed incorrectly for a pointer value.
	E_UNEXPECTED	0x8000FFFF	Unexpected condition.
	S_OK            0x0         Success.
	S_FALSE         0x1         Success (but not a success!).

	Fail
	====

	// ---- Wrong.
		HRESULT hr = SomeFunction();
		if (hr != S_OK)
		{
			printf("Error!\n"); // Bad. hr might be another success code.
		}

	// ---- Right.
		HRESULT hr = SomeFunction();
		if (FAILED(hr))
		{
			printf("Error!\n");
		}

	The success code S_FALSE deserves mention.

	Some methods use S_FALSE to mean, roughly, a negative condition that is not a failure.
	It can also indicate a "no-op"—the method succeeded, but had no effect. 

		if (hr == S_FALSE)
		{
			// Handle special case.
		}
		else if (SUCCEEDED(hr))
		{
			// Handle general success case.
		}
		else
		{
			// Handle errors.
			printf("Error!\n"); 
		}

*/