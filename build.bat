@echo off

cl >nul 2>&1 && (
    echo Using MSVC cl
) || (
    call vcvars64.bat >nul 2>&1 && (
        echo Initialized vcvars64.bat
    ) || (
        echo MSVC compiler isn't installed
        exit /b 1
    )
)

set LIBS=glfw3.lib opengl32.lib cglm.lib kernel32.lib user32.lib gdi32.lib shell32.lib
set INCLUDES=/I "C:/libs/glad/include" /I "C:/libs/glfw/include" /I "C:/libs/cglm/include"
set LIBPATHS=/LIBPATH:"C:/libs/glfw/lib" /LIBPATH:"C:/libs/cglm/lib"
set CONFLICT=/NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT

pushd bin
    del *.exe *.pdb *.idb *.obj
    set CLFLAGS=/nologo /MDd /RTCscu /Zi %INCLUDES%
    set LDFLAGS=/INCREMENTAL:NO %LIBPATHS% %LIBS% %CONFLICT% /DEBUG:FULL /SUBSYSTEM:CONSOLE
    call cl "..\part1\1_window.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    call cl "..\part1\2_triangle.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    call cl "..\part1\2_rectangle.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
popd
if %ERRORLEVEL% == 0 bin\2_rectangle.exe
echo Errorlevel: %ERRORLEVEL%
