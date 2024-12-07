
include(../cmake/common.cmake)

add_executable(HelloWorld)

equip_windows_app(HelloWorld)

add_subdirectory(src)

target_include_directories(HelloWorld
    PRIVATE
    ../flangee77/src
)

target_link_libraries(HelloWorld
    PRIVATE
    flangee77
)

add_custom_command(TARGET HelloWorld
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $ENV{ZLIB_DIR}/bin/zlibwapi.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
