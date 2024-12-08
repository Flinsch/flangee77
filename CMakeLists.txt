cmake_minimum_required(VERSION 3.29)
project(flangee77)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
endif ()

set(CMAKE_CXX_STANDARD 20)

set(USE_UNICODE ON)

add_subdirectory(flangee77)
add_subdirectory(flangee77-tests)
add_subdirectory(HelloWorld)
