
include(../cmake/common.cmake)

add_executable(flangee77_tests)

equip_console_app(flangee77_tests)

add_subdirectory(src)

target_include_directories(flangee77_tests
    PRIVATE
    ../flangee77/src
)

target_link_libraries(flangee77_tests
    PRIVATE
    flangee77
)

add_custom_command(TARGET flangee77_tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $ENV{ZLIB_DIR}/bin/zlibwapi.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

enable_testing()

add_test(NAME flangee77_tests
    COMMAND $<TARGET_FILE:flangee77_tests>
)
