project "GLAD"
    kind "StaticLib"
    language "C"
    staticruntime "on"

    targetdir ("%{wks.location}/Vendor/Debug/bin/%{cfg.buildcfg}")
    objdir    ("%{wks.location}/Vendor/Debug/bin-int/%{cfg.buildcfg}")

    files
    {
        "include/glad/glad.h",
        "include/KHR/khrplatform.h",
        "src/glad.c"
    }

    includedirs
    {
        "include"
    }

    filter "system:windows"
        systemversion "latest"
