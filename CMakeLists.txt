cmake_minimum_required(VERSION 3.21)
project(ThunkLib C)

set(CMAKE_C_STANDARD 11)

include_directories(src/Data)
include_directories(src)

add_executable(ThunkLib
        src/Data/List.c
        src/Data/List.h
        src/Core/Utils.h
        src/main.c
        src/Core/Primitive.c
        src/Core/Primitive.h
        src/Core/Thunk.c
        src/Core/Thunk.h
        src/Core/Utils/Mem.h
        src/Core/Utils/Lang.h
        src/Core.h)
