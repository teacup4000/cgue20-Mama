workspace "Mama"
	architecture "x64"
	startproject "Sandbox"

	configurations{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}" --will generate something like 'Debug-x64'

IncludeDir = {}
IncludeDir["GLFW"] = "Mama/dependencies/GLFW/include"
IncludeDir["Glad"] = "Mama/dependencies/Glad/include"
IncludeDir["imgui"] = "Mama/dependencies/imgui"
IncludeDir["glm"] = "Mama/dependencies/glm"

include "Mama/dependencies/GLFW"
include "Mama/dependencies/Glad"
include "Mama/dependencies/imgui"

project "Mama"
	location "Mama" --Mama/Mama/
	kind "SharedLib" -- specify dll file
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" ..outputdir .. "/%{prj.name}")

	pchheader "mamapch.h"
	pchsource "Mama/src/mamapch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/dependencies/glm/glm/**.hpp",
		"%{prj.name}/dependencies/glm/glm/**.inl"

	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/dependencies/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}";
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"imgui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"MAMA_PLATFORM_WINDOWS",
			"MAMA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"") --copy mama.dll into Sandbox
		}

	filter "configurations:Debug"
		defines "MAMA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MAMA_RELEASE"
		runtime "Release"
		symbols "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"	
	language "C++"
	staticruntime "off"

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
		"Mama/src",
		"%{IncludeDir.glm}"

	}

	links
	{
		"Mama"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	defines
	{
		"MAMA_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "MAMA_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "MAMA_RELEASE"
		runtime "Release"
		symbols "On"
