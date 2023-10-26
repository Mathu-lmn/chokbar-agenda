cmake_minimum_required(VERSION 3.26)
project(chokbar_agenda C)

set(CMAKE_C_STANDARD 11)

add_executable(chokbar_agenda
        main.c
        lib/timer.h
        lib/timer.c
        lib/cells.c
        lib/cells.h
        lib/lists.c
        lib/lists.h)
