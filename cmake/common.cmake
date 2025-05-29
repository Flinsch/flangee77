
if (NOT DEFINED ENV{ZLIB_DIR})
    message(FATAL_ERROR "ZLIB_DIR is not defined. Please set it in your environment.")
endif()

include_directories(
    $ENV{ZLIB_DIR}/include
)

link_directories(
    $ENV{ZLIB_DIR}/lib
)

link_libraries(
    zlibwapi.lib
    Secur32.lib
    dxgi.lib
    d3dcompiler.lib
    d3d9.lib
)
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    link_libraries(
        d3d11.lib
        dxguid.lib
    )
endif()

add_compile_definitions(
    WIN32_LEAN_AND_MEAN
    NOMINMAX
)

add_compile_definitions(UNICODE _UNICODE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/utf-8)
endif ()

function(equip_windows_app target)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(${target} PRIVATE -mwindows)
        target_link_options(${target} PRIVATE -municode)
    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set_target_properties(${target} PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
    endif()
endfunction()

function(equip_console_app target)
endfunction()
