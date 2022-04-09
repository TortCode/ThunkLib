cmake_minimum_required(VERSION 3.21)
project(ThunkLib C)

set(CMAKE_C_STANDARD 11)

include_directories(data)
include_directories(src)

add_executable(ThunkLib
        data/list.c
        data/list.h
        src/core/utils.h
        src/main.c
        src/core/primitive.c
        src/core/primitive.h
        src/core/thunk.c
        src/core/thunk.h src/core/utils/mem_utils.h src/core/utils/lang_utils.h src/core.h)
