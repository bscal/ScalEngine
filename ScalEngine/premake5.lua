project "ScalEngine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.cpp",
        "src/*.h"
    }
    
    defines
    {
        --"SCAL_BUILD_DLL"
    }

    includedirs
    {
        "src"
    }

    links
    {
    }

    filter "configurations:Debug"
        defines "SCAL_DEBUG"
        buildoptions "/MTd"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        buildoptions "/MT"
        runtime "Release"
        optimize "on"

    filter "system:Windows"
        defines "SCAL_PLATFORM_WINDOWS"
        systemversion "latest"

    --filter "system:Unix"
        --defines "SCAL_PLATFORM_LINUX"

    postbuildcommands
    {
        --("{COPY} %{cfg.buildtarget.relpath} ../bin/ .. outputdir .. /Sandbox")
    }
