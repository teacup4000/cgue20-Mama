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
IncludeDir["ImGui"] = "Mama/dependencies/imgui"
IncludeDir["glm"] = "Mama/dependencies/glm"

include "Mama/dependencies/GLFW"
include "Mama/dependencies/Glad"
include "Mama/dependencies/imgui"

project "Mama"
	location "Mama" --Mama/Mama/
	kind "StaticLib" -- specify dll file
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/dependencies/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MAMA_PLATFORM_WINDOWS",
			"MAMA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "MAMA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MAMA_RELEASE"
		runtime "Release"
		symbols "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Mama/dependencies",
		"%{IncludeDir.glm}"
	}

	links
	{
		"Mama"
	}

	filter "system:windows"
		systemversion "latest"

	defines
	{
		"MAMA_PLATFORM_WINDOWS"
	}

	filter "configurations:Debug"
		defines "MAMA_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "MAMA_RELEASE"
		runtime "Release"
		symbols "on"
