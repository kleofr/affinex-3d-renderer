-- premake5.lua
workspace "AffineX"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }
   startproject "App"

   group "Vendor"
      include "Vendor/Binaries/IMGUI"
      include "Vendor/Binaries/GLAD"
      include "Vendor/Binaries/GLFW"

    group ""

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "Vendor/Binaries/GLFW/include"
IncludeDirs["GLAD"] = "Vendor/Binaries/GLAD/include/glad"
IncludeDirs["IMGUI"] = "Vendor/Binaries/IMGUI"


-- Include App Project

project "App"
   location "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   staticruntime "on"

    files
    {
        "%{prj.name}/Source/**.h",
        "%{prj.name}/Source/**.cpp"
    }

    vpaths
    {
        ["*"] = { "App/**" }
    }

    includedirs
    {
        "%{prj.name}/Source",
        "Vendor/Binaries/GLFW/include",
        "Vendor/Binaries/GLAD/include",
        "Vendor/Binaries/IMGUI",
        "Vendor/Binaries/IMGUI/backends"
    }

    links
    {
      "GLFW",
      "GLAD",
      "IMGUI",
      "opengl32.lib"
    }

   targetdir ("Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")

    filter "system:windows"
        systemversion "latest"
        defines { "WINDOWS" }

    filter "configurations:Debug"
        defines { "DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        runtime "Release"
        optimize "On"
        symbols "On"

    filter "configurations:Dist"
        defines { "DIST" }
        runtime "Release"
        optimize "On"
        symbols "Off"