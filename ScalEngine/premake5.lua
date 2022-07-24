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
    }

    includedirs
    {
        "src"
    }

    links
    {
    }

    filter { "configurations:Debug" }
        defines "SCAL_DEBUG"
        buildoptions "/MTd"
        runtime "Debug"
        symbols "on"

    filter { "configurations:Release" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"