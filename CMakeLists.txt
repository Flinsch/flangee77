cmake_minimum_required(VERSION 3.29)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Generate compile_commands.json")

project(flangee77)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(flangee77)
add_subdirectory(flangee77-tests)
add_subdirectory(HelloWorld)
