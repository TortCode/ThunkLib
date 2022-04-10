cmake_minimum_required(VERSION 3.21)
project(ThunkLib)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/Data)
include_directories(src)

add_executable(ThunkLib
        src/Data/List.cpp
        src/Data/List.h
        src/Base/Utils.h
        src/main.cpp
        src/Base/Primitive.cpp
        src/Base/Primitive.h
        src/Base/Thunk.cpp
        src/Base/Thunk.h
        src/Base/Utils/Mem.h
        src/Base/Utils/Lang.h
        src/Base.h)
