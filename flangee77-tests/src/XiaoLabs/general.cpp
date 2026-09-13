
#include <TestLabs/TestSuite.h>

#include <XiaoLabs/DriverResolver.h>
#include <XiaoLabs/ConfigBinding.h>

#include <DataLabs/config/Binder.h>
#include <DataLabs/config/json.h>
#include <DataLabs/json/JsonReader.h>
#include <DataLabs/json/JsonWriter.h>

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



TESTLABS_CASE( u8"XiaoLabs:  ConfigBinding:  load from JSON" )
{
    // A configuration file states only what differs, everything
    // else keeps the defaults `Config` sets up for itself.
    const auto json = dl7::json::JsonReader{}.parse( u8R"({
        "general": { "title": "Example" },
        "video": {
            "presentation_mode": "exclusive_fullscreen",
            "back_buffer_width": 1920,
            "vsync_enabled": true
        }
    })" );

    const xl7::Config defaults;
    xl7::Config config;

    dl7::config::Binder binder;
    binder.load( dl7::config::from_json( json ), config );

    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );

    TESTLABS_CHECK_EQ( config.general.title, u8"Example" );
    TESTLABS_CHECK( config.video.presentation_mode == xl7::Config::Video::PresentationMode::ExclusiveFullscreen );
    TESTLABS_CHECK_EQ( config.video.back_buffer_width, 1920 );
    TESTLABS_CHECK( config.video.vsync_enabled );

    // What the file does not mention is left alone.
    TESTLABS_CHECK_EQ( config.general.quit_key, defaults.general.quit_key );
    TESTLABS_CHECK_EQ( config.video.driver_name, defaults.video.driver_name );
    TESTLABS_CHECK_EQ( config.video.back_buffer_count, defaults.video.back_buffer_count );
    TESTLABS_CHECK( config.video.allow_tearing == defaults.video.allow_tearing );
    TESTLABS_CHECK_EQ( config.input.game_controller_driver_name, defaults.input.game_controller_driver_name );
}

TESTLABS_CASE( u8"XiaoLabs:  ConfigBinding:  round trip through JSON (every field)" )
{
    // Every field differs from its default, so a field the binding forgot
    // to list would come back as its default and give itself away.
    xl7::Config config;
    config.general.title = u8"Round Trip";
    config.general.console_key = 0x71;
    config.general.quit_key = 0x7A;
    config.general.use_hardware_cursor = true;
    config.video.driver_name = u8"OpenGL";
    config.video.presentation_mode = xl7::Config::Video::PresentationMode::Windowed;
    config.video.back_buffer_width = 1920;
    config.video.back_buffer_height = 1080;
    config.video.back_buffer_count = 3;
    config.video.refresh_rate = 144;
    config.video.vsync_enabled = true;
    config.video.allow_tearing = false;
    config.input.keyboard_mouse_driver_name = u8"DirectInput";
    config.input.game_controller_driver_name = u8"SDL";

    dl7::config::Binder binder;
    const auto string = dl7::json::JsonWriter{}.to_string( dl7::config::to_json( binder.save( config ) ) );

    xl7::Config reloaded;
    binder.load( dl7::config::from_json( dl7::json::JsonReader{}.parse( string ) ), reloaded );

    TESTLABS_CHECK_EQ( binder.get_diagnostics().get_count(), 0 );

    TESTLABS_CHECK_EQ( reloaded.general.title, u8"Round Trip" );
    TESTLABS_CHECK_EQ( reloaded.general.console_key, 0x71 );
    TESTLABS_CHECK_EQ( reloaded.general.quit_key, 0x7A );
    TESTLABS_CHECK( reloaded.general.use_hardware_cursor );
    TESTLABS_CHECK_EQ( reloaded.video.driver_name, u8"OpenGL" );
    TESTLABS_CHECK( reloaded.video.presentation_mode == xl7::Config::Video::PresentationMode::Windowed );
    TESTLABS_CHECK_EQ( reloaded.video.back_buffer_width, 1920 );
    TESTLABS_CHECK_EQ( reloaded.video.back_buffer_height, 1080 );
    TESTLABS_CHECK_EQ( reloaded.video.back_buffer_count, 3 );
    TESTLABS_CHECK_EQ( reloaded.video.refresh_rate, 144 );
    TESTLABS_CHECK( reloaded.video.vsync_enabled );
    TESTLABS_CHECK( !reloaded.video.allow_tearing );
    TESTLABS_CHECK_EQ( reloaded.input.keyboard_mouse_driver_name, u8"DirectInput" );
    TESTLABS_CHECK_EQ( reloaded.input.game_controller_driver_name, u8"SDL" );
}
