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
set INCLUDES=/I "C:/libs/glad/include" /I "C:/libs/glfw/include" /I "C:/libs/cglm/include" /I ".."
set LIBPATHS=/LIBPATH:"C:/libs/glfw/lib" /LIBPATH:"C:/libs/cglm/lib"
set CONFLICT=/NODEFAULTLIB:MSVCRT /NODEFAULTLIB:LIBCMT

pushd bin
    del *.exe *.pdb *.idb *.obj
    set CLFLAGS=/nologo /MDd /RTCscu /Zi %INCLUDES%
    set LDFLAGS=/INCREMENTAL:NO %LIBPATHS% %LIBS% %CONFLICT% /DEBUG:FULL /SUBSYSTEM:CONSOLE
    rem call cl "..\part1\1_window.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\2_triangle.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\2_rectangle.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\3_shaders\3_shaders.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\4_textures\4_textures.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\5_transformations\5_transformations.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\6_coordinate_systems\6_coordinate_systems.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    rem call cl "..\part1\6_coordinate_systems\6_cube.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
    call cl "..\part1\7_camera\7_camera.c" "..\glad.c" %CLFLAGS% /link %LDFLAGS%
popd

rem if %ERRORLEVEL% == 0 bin\1_window.exe
rem if %ERRORLEVEL% == 0 bin\2_triangle.exe
rem if %ERRORLEVEL% == 0 bin\2_rectangle.exe
rem if %ERRORLEVEL% == 0 bin\3_shaders.exe
rem if %ERRORLEVEL% == 0 bin\4_textures.exe
rem if %ERRORLEVEL% == 0 bin\5_transformations.exe
rem if %ERRORLEVEL% == 0 bin\6_coordinate_systems.exe
rem if %ERRORLEVEL% == 0 bin\6_cube.exe
if %ERRORLEVEL% == 0 bin\7_camera.exe
echo Errorlevel: %ERRORLEVEL%
