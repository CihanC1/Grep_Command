cmake_minimum_required(VERSION 3.29)
project(Test_C_In_Linux C)

set(CMAKE_C_STANDARD 11)

include_directories(project/modules)

add_executable(Test_C_In_Linux
    project/modules/globals.h
    project/modules/search.h
    project/src/globals.c
    project/src/main.c
    project/src/search.c)
