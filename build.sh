#!/bin/sh

pushd bin/linux
    CLFLAGS="-I../.. -I/home/anto/libs/glad/include"
    LDFLAGS="-lm -lglfw -lGL -lcglm"
    # gcc -ggdb -o 1_window ../../part1/1_window.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode1=$?
    # gcc -ggdb -o 2_triangle ../../part1/2_triangle.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode2=$?
    # gcc -ggdb -o 2_rectangle ../../part1/2_rectangle.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode3=$?
    # gcc -ggdb -o 3_shaders ../../part1/3_shaders/3_shaders.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode4=$?
    # gcc -ggdb -o 4_textures ../../part1/4_textures/4_textures.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode5=$?
    # gcc -ggdb -o 5_transformations ../../part1/5_transformations/5_transformations.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode6=$?
    # gcc -ggdb -o 6_coordinate_systems ../../part1/6_coordinate_systems/6_coordinate_systems.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode7=$?
    # gcc -ggdb -o 6_cube ../../part1/6_coordinate_systems/6_cube.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode8=$?
    gcc -ggdb -o 7_camera ../../part1/7_camera/7_camera.c ../../glad.c $CLFLAGS $LDFLAGS; exitcode9=$?
popd

# if [ $exitcode1 -eq 0 ]; then ./bin/linux/1_window; fi
# if [ $exitcode2 -eq 0 ]; then ./bin/linux/2_triangle; fi
# if [ $exitcode3 -eq 0 ]; then ./bin/linux/2_rectangle; fi
# if [ $exitcode4 -eq 0 ]; then ./bin/linux/3_shaders; fi
# if [ $exitcode5 -eq 0 ]; then ./bin/linux/4_textures; fi
# if [ $exitcode6 -eq 0 ]; then ./bin/linux/5_transformations; fi
# if [ $exitcode7 -eq 0 ]; then ./bin/linux/6_coordinate_systems; fi
# if [ $exitcode8 -eq 0 ]; then ./bin/linux/6_cube; fi
if [ $exitcode9 -eq 0 ]; then ./bin/linux/7_camera; fi
echo Return Code: ${exit_code}
