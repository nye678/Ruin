@echo off
cls
call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x64

set includeDirs=/I"..\code"
set includeDirs=%includeDirs% /I"..\code\common"
set includeDirs=%includeDirs% /I"..\code\common\collections"
set includeDirs=%includeDirs% /I"..\code\common\math"
set includeDirs=%includeDirs% /I"..\code\common\memory"
set includeDirs=%includeDirs% /I"..\code\engine"
set includeDirs=%includeDirs% /I"..\code\engine\entities"
set includeDirs=%includeDirs% /I"..\code\engine\graphics"
set includeDirs=%includeDirs% /I"..\code\game"
set includeDirs=%includeDirs% /I"..\code\platform"
set includeDirs=%includeDirs% /I"..\lib\gl3w\include"
set includeDirs=%includeDirs% /I"..\lib\rapidxml-1.13"
set includeDirs=%includeDirs% /I"..\lib\rapidjson\include"
set includeDirs=%includeDirs% /I"C:\VulkanSDK\1.0.3.1\Include\vulkan"

set sources=
rem set sources=%sources% ..\code\*.cpp
set sources=%sources% ..\code\common\*.cpp
set sources=%sources% ..\code\common\collections\*.cpp
set sources=%sources% ..\code\common\math\*.cpp
set sources=%sources% ..\code\common\memory\*.cpp
rem set sources=%sources% ..\code\engine\*.cpp
set sources=%sources% ..\code\engine\entities\*.cpp
set sources=%sources% ..\code\engine\graphics\*.cpp
set sources=%sources% ..\code\game\*.cpp
set sources=%sources% ..\code\platform\win32\*.cpp
set sources=%sources% ..\lib\gl3w\src\gl3w.c

rem wd4127 - Conditional expression is constant.
rem wd4189 - local variable is initialized but not referenced.
rem wd4201 - nonstandard extension used : nameless struct/union
rem wd4100 - unreferenced formal parameter
set warnings=/W4 /WX /wd4127 /wd4189 /wd4201 /wd4100

set ouputFiles=/Feruin
set preprocessor=/D_CRT_SECURE_NO_WARNINGS
set debugflags=/Z7 /D_debug
set win32libs=opengl32.lib user32.lib gdi32.lib Xinput.lib Shlwapi.lib
set linkerflags=/link /incremental:no

set buildFlags=%ouputFiles% %debugflags% %warnings% %preprocessor%

pushd .\build

cl %buildFlags% %includeDirs% %sources% %win32libs% %linkerflags%

popd