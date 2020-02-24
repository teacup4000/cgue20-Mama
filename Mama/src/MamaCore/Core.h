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

#define BIT_FIELD(x) (1 << x) // one shifted by x places
