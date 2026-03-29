-- change to root folder
os.chdir("../")

workspace "Nova"
	architecture "x64"
	
	configurations
    {
        "Debug",
        "Release"
    }
	
	startproject "Game"
	
	TargetDir = "bin/%{cfg.buildcfg}/%{prj.name}"
    ObjectDir = "bin-int/%{cfg.buildcfg}/%{prj.name}"
	
group "nova-dependencies" 
    include "vendor/external/ImGui"
group "game-dependencies"
	include "vendor/external/RVO2"
group ""

project "Nova"
	location "Nova"
	kind "StaticLib"
	language "C++"
    cppdialect "C++20"
	
	pchheader "pch.h"
	pchsource "Nova/src/pch.cpp"
	
	AssimpDLL = "Nova/vendor/Assimp/assimp-vc143-mt.dll"
	
	targetdir(TargetDir)
    objdir(ObjectDir)
	
	postbuildcommands { "{COPYFILE} %[" .. AssimpDLL ..  "] %[" .. TargetDir .. "]" }
	
	defines
	{
        "UNICODE",
        "NOMINMAX"
    }
    
	libdirs
	{
		"libs"
	}
	
	links
	{
        "d3d11.lib",
        "D3DCompiler.lib",
        "assimp-vc143-mt.lib",
        "ImGui"
    }
	
	files
	{
        "%{prj.location}/resources/**",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.hlsl",

        "%{prj.location}/vendor/**.cpp",
        "%{prj.location}/vendor/**.h",
        "%{prj.location}/vendor/**.hpp",
        "%{prj.location}/vendor/**.inl"
    }
	
	includedirs
	{
        "%{prj.location}/src",
        "%{prj.location}/vendor",
        "%{wks.location}/vendor",
        "%{wks.location}/vendor/external/ImGui" -- ImGuizmo needs this
    }
	
	filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"
		libdirs { "libs/libs-Debug" }
        links { "DirectXTex_Debug.lib" }

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"
		libdirs { "libs/libs-Release" }
        links { "DirectXTex_Release.lib" }
		
	-- Shader options
    shaderobjectfileoutput("../Game/Assets/CompiledShaders/%%(Filename).cso")

    filter("files:**_vs.hlsl")
    shadertype("Vertex")
    shadermodel "5.0"
    
    filter("files:**_ps.hlsl")
    shadertype("Pixel")
    shadermodel "5.0"
	
project "Game"
	location "Game"
	language "C++"
    cppdialect "C++20"
	
	targetdir(TargetDir)
    objdir(ObjectDir)
	
	defines
	{
        "UNICODE",
        "NOMINMAX"
    }
	
	links
	{
		"Nova",
		"RVO2"
	}
	
	files
	{
		"%{prj.location}/resources/**",
        "%{prj.location}/src/**.cpp",
        "%{prj.location}/src/**.h",
        "%{prj.location}/src/**.hpp",
        "%{prj.location}/src/**.hlsl"
    }
	
	includedirs
	{
        "%{prj.location}/src",
		"%{wks.location}/Nova/src",
		"%{wks.location}/Nova/vendor",
		"%{wks.location}/vendor/external/RVO2/src"
	}
	
	filter "configurations:Debug"
        defines "DEBUG"
        runtime "Debug"
        symbols "on"
		kind "ConsoleApp"

    filter "configurations:Release"
        defines "RELEASE"
        runtime "Release"
        optimize "on"
		kind "WindowedApp"
		linkoptions { "/ENTRY:mainCRTStartup" }
		