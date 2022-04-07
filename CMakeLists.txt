cmake_minimum_required(VERSION 3.21)
project(ThunkLib C)

set(CMAKE_C_STANDARD 11)

include_directories(data)
include_directories(src)

add_executable(ThunkLib
        data/list.c
        data/list.h
        src/utils.h
        src/main.c
        src/primitive.c
        src/primitive.h
        src/thunk.c
        src/thunk.h)
