/*
-----------GENERAL INFO-----------
This is a precompiled header file.
It compiles the following includes before any of our code has been compiled.
*/


#pragma once


#include <iostream>
#include <functional>
#include <memory>

#include <sstream>
#include <string>
#include <array>
#include <vector>

#include "Core/Logger/Log.h"

#ifdef MAMA_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
