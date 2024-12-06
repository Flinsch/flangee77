
include(../cmake/common.cmake)

add_library(flangee77 STATIC)

add_subdirectory(src)

target_include_directories(flangee77
    PRIVATE
    ./src
)
