workspace "ScalEngine"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

    startproject "ScalGame"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "ScalEngine"
include "ScalGame"

newaction
{
    trigger = "clean",
    description = "Remove all binaries, int-binaries, vs files",
    execute = function()
        os.rmdir("./bin")
        print("Successfully removed binaries")
        os.rmdir("./bin-int")
        print("Successfully removed intermediate binaries")
        os.rmdir("./.vs")
        os.remove("**.sln")
        os.remove("**.vcxproj")
        os.remove("**.vcxproj.filters")
        os.remove("**.vcxproj.user")
        print("Successfully removed vs project files")
        print("Done!")
    end
}
