
include(../cmake/common.cmake)

add_executable(flangee77-tests)

add_subdirectory(src)

target_link_libraries(flangee77-tests
    PRIVATE
    flangee77
)

equip_console_app(flangee77-tests)

add_custom_command(TARGET flangee77-tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $ENV{ZLIB_DIR}/bin/zlibwapi.dll ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_test(NAME flangee77-tests
    COMMAND $<TARGET_FILE:flangee77-tests>
)
