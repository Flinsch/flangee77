#ifndef XL7_CONFIG_H
#define XL7_CONFIG_H

#include <CoreLabs/string.h>



namespace xl7 {



struct Config
{

    /**
     * The general presets.
     */
    struct General
    {
        /** The application's main window title. */
        cl7::u8string title;

        /** The index of the key to toggle the console. */
        unsigned console_key;
        /** The index of the key to quit the application. */
        unsigned quit_key;

        /** The flag indicating whether to use the hardware cursor or not. */
        bool use_hardware_cursor;
    } general;



    /**
     * The presets for the graphics component.
     */
    struct Video
    {
        enum struct PresentationMode
        {
            /** Regular window with borders and title bar. Movable (and maybe resizable) by the user. */
            Windowed,
            /** Covers the entire screen without borders or title bar, but is not an exclusive fullscreen mode. Often used for seamless alt-tabbing and matching desktop resolution. */
            BorderlessFullscreen,
            /** Exclusive fullscreen mode. Takes full control of the display and may change resolution. Can offer better performance or lower latency, but slower to alt-tab. */
            ExclusiveFullscreen,
        };

        /** The name of the driver/implementation of the graphics component. */
        cl7::u8string driver_name;

        /** The presentation mode: windowed, borderless fullscreen, exclusive fullscreen. */
        PresentationMode presentation_mode;
        /** The width of the back buffer(s), in pixels (0: use default/desktop width). */
        unsigned back_buffer_width;
        /** The height of the back buffer(s), in pixels (0: use default/desktop height). */
        unsigned back_buffer_height;
        /** The number of swap chain back buffers (2 = double buffering, 3 = triple buffering). Values other than 2 or 3 may not be supported on all platforms. */
        unsigned back_buffer_count;
        /** The target display refresh rate, in Hz (0: adapter default). */
        unsigned refresh_rate;
        /** If true, synchronizes the frame presentation with the display's refresh rate to avoid screen tearing. */
        bool vsync_enabled;
        /** If true, allows tearing when vsync is disabled (depends on platform support). */
        bool allow_tearing;
    } video;



    Config();

}; // struct Config



} // namespace xl7

#endif // XL7_CONFIG_H
