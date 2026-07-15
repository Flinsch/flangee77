
include(../cmake/common.cmake)

add_library(flangee77 STATIC)

add_subdirectory(src)

target_include_directories(flangee77
    PUBLIC
    ./src
)

copy_assets_post_build(flangee77
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
