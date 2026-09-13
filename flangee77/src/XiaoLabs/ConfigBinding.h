#ifndef XL7_CONFIGBINDING_H
#define XL7_CONFIGBINDING_H

#include "./Config.h"

#include <DataLabs/config/Binding.h>

#include <tuple>



/**
 * Describes how the "X" pre-config structure maps onto a configuration value, so it
 * can be read from and written to any format `dl7::config` converts. Kept apart
 * from `Config.h`, which knows nothing of any of this. The keys are the member
 * names, the enumeration names their values in snake case.
 */
namespace dl7::config {



    template <>
    struct EnumNames<xl7::Config::Video::PresentationMode>
    {
        static constexpr auto names = std::make_tuple(
            enum_value(u8"windowed", xl7::Config::Video::PresentationMode::Windowed),
            enum_value(u8"borderless_fullscreen", xl7::Config::Video::PresentationMode::BorderlessFullscreen),
            enum_value(u8"exclusive_fullscreen", xl7::Config::Video::PresentationMode::ExclusiveFullscreen));
    };



    template <>
    struct Binding<xl7::Config::General>
    {
        static constexpr auto fields = std::make_tuple(
            field(u8"title", &xl7::Config::General::title),
            field(u8"console_key", &xl7::Config::General::console_key),
            field(u8"quit_key", &xl7::Config::General::quit_key),
            field(u8"use_hardware_cursor", &xl7::Config::General::use_hardware_cursor));
    };

    template <>
    struct Binding<xl7::Config::Video>
    {
        static constexpr auto fields = std::make_tuple(
            field(u8"driver_name", &xl7::Config::Video::driver_name),
            field(u8"presentation_mode", &xl7::Config::Video::presentation_mode),
            field(u8"back_buffer_width", &xl7::Config::Video::back_buffer_width),
            field(u8"back_buffer_height", &xl7::Config::Video::back_buffer_height),
            field(u8"back_buffer_count", &xl7::Config::Video::back_buffer_count),
            field(u8"refresh_rate", &xl7::Config::Video::refresh_rate),
            field(u8"vsync_enabled", &xl7::Config::Video::vsync_enabled),
            field(u8"allow_tearing", &xl7::Config::Video::allow_tearing));
    };

    template <>
    struct Binding<xl7::Config::Input>
    {
        static constexpr auto fields = std::make_tuple(
            field(u8"keyboard_mouse_driver_name", &xl7::Config::Input::keyboard_mouse_driver_name),
            field(u8"game_controller_driver_name", &xl7::Config::Input::game_controller_driver_name));
    };

    template <>
    struct Binding<xl7::Config>
    {
        static constexpr auto fields = std::make_tuple(
            field(u8"general", &xl7::Config::general),
            field(u8"video", &xl7::Config::video),
            field(u8"input", &xl7::Config::input));
    };



} // namespace dl7::config

#endif // XL7_CONFIGBINDING_H
