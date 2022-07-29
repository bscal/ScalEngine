@echo off

IF "%~1" == "" GOTO PrintHelp
IF "%~1" == "generate" GOTO Generate
IF "%~1" == "build" GOTO Build
IF "%~1" == "debug" GOTO Debug
if "%~1" == "build_clang" GOTO BuildClang

SET premakePath="vendor\premake5.exe"

%premakePath% %1
GOTO Done

:PrintHelp
echo Input 'build.bat [action]'
echo.
echo ACTIONS
echo    generate          Generates default windows vs solution
echo    build             Builds solutions with msbuild
echo    debug             Executes debug build
echo    build_clang       Builds with clang and without premake files
echo.
echo    clean             Remove all binaries and generated files
echo    codelite          Generate CodeLite project files
echo    gmake             Generate GNU makefiles for POSIX, MinGW, and Cygwin
echo    gmake2            Generate GNU makefiles for POSIX, MinGW, and Cygwin
echo    vs2005            Generate Visual Studio 2005 project files
echo    vs2008            Generate Visual Studio 2008 project files
echo    vs2010            Generate Visual Studio 2010 project files
echo    vs2012            Generate Visual Studio 2012 project files
echo    vs2013            Generate Visual Studio 2013 project files
echo    vs2015            Generate Visual Studio 2015 project files
echo    vs2017            Generate Visual Studio 2017 project files
echo    vs2019            Generate Visual Studio 2019 project files
echo    vs2022            Generate Visual Studio 2022 project files
echo    xcode4            Generate Apple Xcode 4 project files
GOTO Done

:Generate
%premakePath% vs2022
GOTO Done

:Build
SET solutionFile="ScalEngine.sln"
SET debugExePath="bin\Debug-windows-x86_64\ScalGame\"
SET enginePath="bin\Debug-windows-x86_64\ScalEngine\"

echo Building %solutionFile%...
msbuild -m %solutionFile%
GOTO Done

:Debug
echo Executing Debug Build...
%debugExePath%ScalGame.exe
GOTO Done

:BuildClang
SET debugExePath=bin\Debug-windows-x86_64\ScalGame\
SET enginePath=bin\Debug-windows-x86_64\ScalEngine\

SetLocal EnableDelayedExpansion
SET filenames=
FOR /R "ScalEngine" %%f in (*.cpp) do (
    SET filenames=!filenames! %%f
)
SET assembly=ScalEngine
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
SET includes=-IScalEngine/src
SET links=-luser32
SET defines=-DSCAL_DEBUG -DSCAL_BUILD_DLL -DSCAL_PLATFORM_WINDOWS
ECHO "Building %assembly%..."

clang++ %filenames% %compilerFlags% -o %enginePath%%assembly%.dll %defines% %includes% %links%

SET filenames=
FOR /R "ScalGame" %%f in (*.cpp) do (
    echo %%f
    SET filenames=!filenames! %%f
)
SET assembly=ScalGame
SET compilerFlags=-g -Wvarargs -Wall -Werror
SET includes=-IScalGame/src -IScalEngine/src
SET links=-luser32 -l%enginePath%ScalEngine
SET defines=-DSCAL_DEBUG -DSCAL_PLATFORM_WINDOWS
ECHO "Building %assembly%..."

clang++ %filenames% %compilerFlags% -o %debugExePath%%assembly%.exe %defines% %includes% %links%


:Done
