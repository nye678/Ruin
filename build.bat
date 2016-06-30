@echo off
cls
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64

devenv proj/vstudio/ruin.vcxproj /Build

rem set includeDirs=/I"..\code"
rem set includeDirs=%includeDirs% /I"..\code\common"
rem set includeDirs=%includeDirs% /I"..\code\common\collections"
rem set includeDirs=%includeDirs% /I"..\code\common\math"
rem set includeDirs=%includeDirs% /I"..\code\common\memory"
rem set includeDirs=%includeDirs% /I"..\code\engine"
rem set includeDirs=%includeDirs% /I"..\code\engine\entities"
rem set includeDirs=%includeDirs% /I"..\code\engine\graphics"
rem set includeDirs=%includeDirs% /I"..\code\game"
rem set includeDirs=%includeDirs% /I"..\code\platform"
rem set includeDirs=%includeDirs% /I"..\lib\gl3w\include"
rem set includeDirs=%includeDirs% /I"..\lib\rapidxml-1.13"
rem set includeDirs=%includeDirs% /I"..\lib\rapidjson\include"
rem set includeDirs=%includeDirs% /I"C:\VulkanSDK\1.0.3.1\Include\vulkan"
rem 
rem set sources=
rem rem set sources=%sources% ..\code\*.cpp
rem set sources=%sources% ..\code\common\*.cpp
rem set sources=%sources% ..\code\common\collections\*.cpp
rem set sources=%sources% ..\code\common\math\*.cpp
rem set sources=%sources% ..\code\common\memory\*.cpp
rem rem set sources=%sources% ..\code\engine\*.cpp
rem set sources=%sources% ..\code\engine\entities\*.cpp
rem set sources=%sources% ..\code\engine\graphics\*.cpp
rem set sources=%sources% ..\code\game\*.cpp
rem set sources=%sources% ..\code\platform\win32\*.cpp
rem set sources=%sources% ..\lib\gl3w\src\gl3w.c
rem 
rem rem wd4127 - Conditional expression is constant.
rem rem wd4189 - local variable is initialized but not referenced.
rem rem wd4201 - nonstandard extension used : nameless struct/union
rem rem wd4100 - unreferenced formal parameter
rem set warnings=/W4 /WX /wd4127 /wd4189 /wd4201 /wd4100
rem 
rem set ouputFiles=/Feruin
rem set preprocessor=/D_CRT_SECURE_NO_WARNINGS
rem set debugflags=/Z7 /D_debug
rem set win32libs=opengl32.lib user32.lib gdi32.lib Xinput.lib Shlwapi.lib
rem set linkerflags=/link /incremental:no
rem 
rem set buildFlags=%ouputFiles% %debugflags% %warnings% %preprocessor%
rem 
rem pushd .\build
rem 
rem cl %buildFlags% %includeDirs% %sources% %win32libs% %linkerflags%
rem 
rem popd