#ifndef F77_TESTS_XL7_GENERAL_H
#define F77_TESTS_XL7_GENERAL_H

#include <XiaoLabs/DriverResolver.h>

#include <TestLabs/TestSuite.h>

#include "./shared.h"



TESTLABS_CASE( u8"XiaoLabs:  DriverResolver::resolve_driver_name" )
{
    struct Entry
    {
        cl7::u8string config_name;
        cl7::u8string expected;
    } entry;

    const std::vector<Entry> container {
        { u8"", u8"Direct3D 11" },
        { u8"Direct3D 9", u8"Direct3D 9" },
        { u8"Direct3D 9.0c", u8"Direct3D 9" },
        { u8"direct3d9", u8"Direct3D 9" },
        { u8"D3D9", u8"Direct3D 9" },
        { u8"d3d9", u8"Direct3D 9" },
        { u8"Direct3D", u8"Direct3D 11" },
        { u8"direct3d", u8"Direct3D 11" },
        { u8"D3D", u8"Direct3D 11" },
        { u8"d3d", u8"Direct3D 11" },
        { u8"OpenGL", u8"OpenGL" },
        { u8"opengl", u8"OpenGL" },
        { u8"OGL", u8"OpenGL" },
        { u8"oGL", u8"OpenGL" },
        { u8"GL", u8"OpenGL" },
        { u8"gl", u8"OpenGL" },
        { u8"Open Graphics Library", u8"OpenGL" },
        { u8"OpenGL 4", u8"OpenGL" },
        { u8"OpenGL 4.6", u8"OpenGL" },
        { u8"default", u8"Direct3D 11" },
        { u8"unknown", u8"Direct3D 11" },
    };

    xl7::DriverResolver::DriverEntries driver_entries;
    driver_entries.push_back( { u8"Direct3D 11", { u8"D3D11", u8"D3D", u8"Direct3D" } } );
    driver_entries.push_back( { u8"Direct3D 9", { u8"D3D9", u8"D3D", u8"Direct3D" } } );
    driver_entries.push_back( { u8"OpenGL", { u8"OGL", u8"GL", u8"Open Graphics Library" } } );

    TESTLABS_SUBCASE_BATCH_WITH_DATA_STRING( u8"resolve_driver_name", container, entry, entry.config_name + u8" --> " + entry.expected )
    {
        const auto actual = xl7::DriverResolver::resolve_driver_name( driver_entries, entry.config_name );
        const auto& expected = entry.expected;

        TESTLABS_CHECK_EQ( actual, expected );
    }
}



#endif // F77_TESTS_XL7_GENERAL_H
