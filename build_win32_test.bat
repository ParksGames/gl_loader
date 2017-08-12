@echo off

set BASE_NAME=win32_test
set PROGRAM_NAME="Win32 GL Loader Test"

set WINDOWS_LIBPATHS= -LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\um\x64" -LIBPATH:"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\lib\x64" -LIBPATH:"C:\Program Files (x86)\Windows Kits\10\Lib\10.0.15063.0\ucrt\x64"
set WINDOWS_INCLUDEPATHS= -I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\um" -I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\shared" -I "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Tools\MSVC\14.10.25017\include" -I "C:\Program Files (x86)\Windows Kits\10\Include\10.0.15063.0\ucrt"

set LIBPATHS= %WINDOWS_LIBPATHS%
set INCLUDEPATHS= %WINDOWS_INCLUDEPATHS% 
set LIBFILES= user32.lib gdi32.lib opengl32.lib
set DISABLED_WARNINGS= -wd4201 -wd4100 -wd4189 -wd4996 -wd4505 -wd4312
set DEBUG_INFO= -Z7

cl -EHsc -MT -nologo -Gm- -GR- -Oi -W3 -Fe%PROGRAM_NAME% %DISABLED_WARNINGS% -FC -DDEBUG=1 -DDEBUG_MODE=1 -EHa- %DEBUG_INFO% %INCLUDEPATHS% %BASE_NAME%.cpp %LIBFILES% -link -opt:ref -SUBSYSTEM:WINDOWS,5.02 %LIBPATHS%
