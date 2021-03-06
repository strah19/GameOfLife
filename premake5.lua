workspace "Game of Life"
	architecture "x86"
	startproject "Game of Life"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["SDL2"] = "Dependencies/SDL2/include"

project "Game of Life"
	location "Game of Life"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ember/src/Ember",
		"Ember/src/CinderFileSystem",
		"%{IncludeDir.SDL2}",
	}

	links
	{
		"Ember"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EMBER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EMBER_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EMBER_DIST"
		runtime "Release"
		optimize "on"

project "Ember"
	location "Ember"
	kind "StaticLib"
	language "C++"
	staticruntime "on"
	cppdialect "C++17"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Ember/src/Ember",
		"Ember/src/CinderFileSystem",
		"%{IncludeDir.SDL2}",
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	libdirs {
		"Dependencies/SDl2/lib/x86"
	}

	links {
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
		"SDL2_ttf.lib",
		"SDL2_mixer.lib" 
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "EMBER_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "EMBER_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "EMBER_DIST"
		runtime "Release"
		optimize "on"