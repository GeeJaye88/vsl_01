////////////////////////////////////////////////////////////////////////////////

// ---------- vsl_include.h ----------
/*!
\file vsl_include.h
\brief vsl preproc includes
\author Gareth Edwards
*/  

#if _MSC_VER > 1000
#pragma once
#endif


// ---------- include ----------
	#include <windows.h> // for C and C++, and contains declarations for all of the functions in the Windows API, etc.
	#include <stdio.h>   // C Standard Input and Output Library
	#include <d3d9.h>    // interface to create Microsoft Direct3D objects and set up the environment
	#include <d3dx9.h>   // import D3D9+ headers from Jun 2010 DXSDK

	#include <math.h>    // compute common mathematical operations and transformations
	#include <stdlib.h>  // general purpose functions, including dynamic memory management, random number generation
	#include <time.h>    // get and manipulate date and time information
	#include <timeapi.h> // get time begin & end period, dev caps, system & elapsed

	#include <chrono>    // date & time utilities that track time with varying degrees of precision
	#include <fstream>   // performs input/output operations on the file they are associated with (if any)
	#include <iostream>  // defines the standard input/output stream objects
	#include <list>      // container that supports constant time insertion and removal of elements from anywhere in the container
	#include <memory>    // defines general utilities to manage dynamic memory
	#include <stack>     // type of container adaptor, specifically designed to operate in a LIFO context
	#include <string>    // provides support for such objects with an interface similar to that of a standard container of bytes, but adding features
	#include <sstream>   // provides string stream classes
	#include <thread>    // single thread of execution, threads allow multiple functions to execute concurrently
	#include <vector>    // sequence container that encapsulates dynamic size arrays


////////////////////////////////////////////////////////////////////////////////



