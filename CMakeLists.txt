cmake_minimum_required(VERSION 3.30)
project(tetris C)

set(CMAKE_C_STANDARD 11)

add_executable(tetris main.c
        lib/console.h lib/console.c)
