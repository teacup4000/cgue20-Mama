#pragma once

#ifdef MAMA_PLATFORM_WINDOWS
	#ifdef MAMA_BUILD_DLL
		#define MAMA_API __declspec(dllexport) 
	#else
		#define MAMA_API __declspec(dllimport)
	#endif
#else
	#error Mama only supports Windows
#endif

#ifdef MAMA_DEBUG
	#define MAMA_ENABLE_ASSERTS
#endif

//Don't know if needed
#ifdef MAMA_ENABLE_ASSERTS
	#define MAMA_ASSERT(x, ...) { if(!(x)) { MAMA_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define MAMA_CORE_ASSERT(x, ...) { if(!(x)) { MAMA_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define MAMA_ASSERT(x, ...)
	#define MAMA_CORE_ASSERT(x, ...)
#endif

#define BIT_FIELD(x) (1 << x) // one shifted by x places
