cmake_minimum_required(VERSION 3.26)
project(chokbar_agenda C)

set(CMAKE_C_STANDARD 11)

add_executable(chokbar_agenda
        main.c
        timer.h
        timer.c)
