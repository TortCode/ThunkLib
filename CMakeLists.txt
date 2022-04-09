cmake_minimum_required(VERSION 3.21)
project(ThunkLib)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/Data)
include_directories(src)

add_executable(ThunkLib
        src/Data/List.cpp
        src/Data/List.h
        src/Core/Utils.h
        src/main.cpp
        src/Core/Primitive.cpp
        src/Core/Primitive.h
        src/Core/Thunk.cpp
        src/Core/Thunk.h
        src/Core/Utils/Mem.h
        src/Core/Utils/Lang.h
        src/Core.h)
