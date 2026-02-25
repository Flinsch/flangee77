cmake_minimum_required(VERSION 3.29)

project(flangee77 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(flangee77)
add_subdirectory(flangee77-tests)
add_subdirectory(HelloWorld)
