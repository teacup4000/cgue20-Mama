/*
------------GENERAL INFO-------------
this file contains definitions that are used in other classes
*/
#pragma once

//this BUILD_DLL means that the MAMA_API is a nonstatic library (*.dll file)
#ifdef MAMA_PLATFORM_WINDOWS
	#define MAMA_API					//same as define zero (not needed, will be deletet in future build)
	//-----------ERROR SUPRESSING--------------
		#if (_MSC_VER >= 1915)
		#pragma warning(disable:4845)   // __declspec(no_init_all) used but d1initall not set
		#define no_init_all deprecated	//supress a common windows error
		#endif
	//-----------------------------------------
#else
	#error THIS IS A WINDOWS ONLY APPLICATION!
#endif

//use asserts when in debug mode
#ifdef MAMA_DEBUG
	#define MAMA_ENABLE_ASSERTS
#endif

//define what asserts do (used in logging files)
#ifdef MAMA_ENABLE_ASSERTS
	#define MAMA_ASSERT(x, ...) { if(!(x)) { MAMA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MAMA_CORE_ASSERT(x, ...) { if(!(x)) { MAMA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MAMA_ASSERT(x, ...)
	#define MAMA_CORE_ASSERT(x, ...)
#endif

//used for events
#define BIT_FIELD(x) (1 << x) // one shifted by x places
