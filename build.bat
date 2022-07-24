@echo off

IF "%~1" == "" GOTO PrintHelp
IF "%~1" == "build" GOTO Build

set premakePath="vendor\premake5.exe"

%premakePath% %1
GOTO Done

:PrintHelp
echo Input 'build.bat [action]'
echo.
echo ACTIONS
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

:Build
%premakePath% vs2022
set solutionFile="Workspace.sln"
msbuild /t:Build /p:Configuration=Debug /p:Platform=x64 %solutionFile%

:Done
