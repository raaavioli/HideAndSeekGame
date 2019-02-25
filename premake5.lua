-- premake5.lua
workspace "HideAndSeek"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "HideAndSeek/vendor/spdlog/include"

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
		"%{prj.name}/vendor/spdlog/include/**.h", 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",	
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		-- Define ENGINE_BUILD_DLL if we want to export to dll
		defines { 
			"ENGINE_PLATFORM_WINDOWS" 
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"



		
