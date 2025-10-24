
include(../cmake/common.cmake)

add_library(flangee77 STATIC)

add_subdirectory(src)

target_include_directories(flangee77
    PUBLIC
    ./src
)

add_custom_command(TARGET flangee77
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
