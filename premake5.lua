workspace "Mama"
	architecture "x64"

	configurations{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}" --will generate something like 'Debug-x64'

IncludeDir = {}
IncludeDir["GLFW"] = "Mama/dependencies/GLFW/include"

include "Mama/dependencies/GLFW"

project "Mama"
	location "Mama" --Mama/Mama/
	kind "SharedLib" -- specify dll file
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" ..outputdir .. "/%{prj.name}")

	pchheader "mamapch.h"
	pchsource "Mama/src/mamapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/dependencies/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MAMA_PLATFORM_WINDOWS",
			"MAMA_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox") --copy mama.dll into Sandbox
		}

	filter "configurations:Debug"
		defines "MAMA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MAMA_RELEASE"
		symbols "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"	
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" ..outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Mama/dependencies/spdlog/include",
		"Mama/src"
	}

	links
	{
		"Mama"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

	defines
	{
		"MAMA_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "MAMA_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MAMA_RELEASE"
		symbols "On"
