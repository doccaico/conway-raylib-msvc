@echo off

setlocal

set RAYLIB=C:\libs\raylib-5.0_win64_msvc16

set CFLAGS=/nologo /std:c++20 /Zc:__cplusplus /permissive- /W4 /utf-8 /GF /D_HAS_EXCEPTIONS=0 /GR- ^
          /I %RAYLIB%\include

set LIBFLAGS=/LIBPATH:%RAYLIB%\lib ^
             raylib.lib winmm.lib shell32.lib gdi32.lib user32.lib ^
             /NODEFAULTLIB:libcmt

set DEBUG=FALSE
if "%1" == "debug" if not "%2" == "" set DEBUG=TRUE
set RELEASE=FALSE
if "%1" == "release" if not "%2" == "" set RELEASE=TRUE

if        %DEBUG%   == TRUE ( goto :DEBUG
) else if %RELEASE% == TRUE ( goto :RELEASE
) else                      (
    echo Usage: %0 [debug/release] FILE
    exit /b 1
)

:DEBUG
    REM set CFLAGS=%CFLAGS%
goto :MAIN

:RELEASE
    set CFLAGS=%CFLAGS% /O2 /DNDEBUG
goto :MAIN

:MAIN
    set EXE=%~n2%.exe

    cl %CFLAGS% %2 /link /OUT:%EXE% %LIBFLAGS%
goto :EOF

REM vim: ft=dosbatch fenc=cp932 ff=dos
