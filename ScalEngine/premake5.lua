project "ScalEngine"
    kind "SharedLib"
    language "C++"
    staticruntime "off"
    cppdialect "C++20"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/**.cpp",
        "src/**.h"
    }
    
    defines
    {
        "SCAL_BUILD_DLL"
    }

    includedirs
    {
        "src",
        "vendor"
    }

    links
    {
    }

    filter "configurations:Debug"
        defines "SCAL_DEBUG"
        buildoptions "/MDd"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "SCAL_RELEASE"
        buildoptions "/MD"
        runtime "Release"
        optimize "on"

    filter "system:Windows"
        defines "SCAL_PLATFORM_WINDOWS"
        systemversion "latest"

    --filter "system:Unix"
        --defines "SCAL_PLATFORM_LINUX"

    postbuildcommands
    {
        ("{COPYDIR} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}\\ScalEngine.dll %{wks.location}/bin/" .. outputdir .. "/%{prj.name}\\../ScalGame/")
    }
