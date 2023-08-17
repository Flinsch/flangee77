#pragma once
#ifndef XL7_CONFIG_H
#define XL7_CONFIG_H

#include <CoreLabs/string.h>

#include <windows.h>



namespace xl7 {



struct Config
{

    /**
     * The generic presets.
     */
    struct Generic
    {
        struct
        {
            /** The main window title. */
            cl7::string         title;
            /** The main window icon handle. */
            HICON               icon_handle;
            /** The main window small icon handle. */
            HICON               small_icon_handle;
        }                   window;

        /** The index of the key to toggle the console. */
        unsigned            console_key;
        /** The index of the key to quit the application. */
        unsigned            quit_key;

        /** The flag indicating whether to use the hardware cursor or not. */
        bool                use_hardware_cursor;
    } generic;



    /**
     * The presets for the Direct3D component.
     */
    struct D3D
    {
        struct
        {
            /** The flag indicating fullscreen or windowed mode. */
            bool                fullscreen;
            /** The screen width, in pixels (0: use default/desktop width). */
            unsigned            width;
            /** The screen height, in pixels (0: use default/desktop height). */
            unsigned            height;
        }                   display_mode;
    } d3d;



    /** Default constructor. */
    Config(void);

}; // struct Config



} // namespace xl7

#endif // XL7_CONFIG_H
