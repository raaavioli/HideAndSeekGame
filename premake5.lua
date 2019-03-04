-- premake5.lua
workspace "HideAndSeek"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "HideAndSeek/vendor/spdlog/"
IncludeDir["GLFW"] = "HideAndSeek/vendor/GLFW/include"

include "HideAndSeek/vendor/GLFW"

project "HideAndSeek"
	location "HideAndSeek"
	kind "ConsoleApp"
	language "C++"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp", 
		"%{prj.name}/vendor/spdlog/**.h"
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",	
		"%{IncludeDir.GLFW}",
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

		defines { 
			"ENGINE_PLATFORM_WINDOWS",
			--"ENGINE_BUILD_DLL", // if we want to export to dll
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"



		
