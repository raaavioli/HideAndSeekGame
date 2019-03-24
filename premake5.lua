-- premake5.lua
workspace "HideAndSeek"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["spdlog"] = "HideAndSeek/vendor/spdlog/"
IncludeDir["GLFW"] = "HideAndSeek/vendor/GLFW/include"
IncludeDir["glad"] = "HideAndSeek/vendor/glad/include"
IncludeDir["glm"] = "HideAndSeek/vendor/glm"

include "HideAndSeek/vendor/GLFW"
include "HideAndSeek/vendor/glad"

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
		"%{prj.name}/src/**.glsl", 
		"%{prj.name}/vendor/spdlog/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.h",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}
	
	includedirs 
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",	
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.glm}",
	}

	links 
	{ 
		"GLFW",
		"glad",
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

project "Server"
	location "Server"
	kind "ConsoleApp"
	language "C++"
	targetdir ("server-bin/" .. outputdir .. "/%{prj.name}")
	objdir ("server-bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{ 
		"%{prj.name}/src/**.h", 
		"%{prj.name}/src/**.cpp", 
	}
	
	includedirs 
	{
		"%{prj.name}/src",
	}

	links 
	{ 

	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines { 
			
		}

	filter "configurations:Debug"
		defines "DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		symbols "On"



		


		
