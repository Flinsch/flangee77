cmake_minimum_required(VERSION 3.29)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

project(flangee77 LANGUAGES CXX)

enable_testing()

add_subdirectory(flangee77)
add_subdirectory(flangee77-tests)
add_subdirectory(HelloWorld)
