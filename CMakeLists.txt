cmake_minimum_required(VERSION 3.29)
project(Grep_Command C)

set(CMAKE_C_STANDARD 11)

include_directories(project/modules)
include_directories(project/src)

add_executable(Grep_Command
    project/src/globals.c
    project/src/main.c
    project/src/search.c
    project/src/args_parser.c
        project/modules/globals.h
        project/modules/search.h
        project(/modules/args_parser.h))
