#pragma once
#ifndef XL7_MAINWINDOW_H
#define XL7_MAINWINDOW_H
#include "./Component.h"

#include <CoreLabs/string.h>

#include <windows.h>



namespace xl7 {



class MainWindow final
    : public Component<MainWindow>
{
    friend class cl7::creational::Singleton<MainWindow>;



public:
    enum class DisplayMode
    {
        Unknown,
        Fullscreen,
        Borderless,
        Windowed,
    };



    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the handle of the window.
     */
    HWND get_handle() const { return _handle; }

    /**
     * Returns the display mode.
     */
    DisplayMode get_display_mode() const { return _display_mode; }

    /**
     * Returns the window width, in pixels.
     */
    unsigned get_width() const { return _width; }

    /**
     * Returns the window height, in pixels.
     */
    unsigned get_height() const { return _height; }

    /**
     * Returns the title of the window.
     */
    const cl7::u8string& get_title() const { return _title; }

    /**
     * Returns the handle of the icon (or NULL).
     */
    HICON get_icon_handle() const { return _icon_handle; }

    /**
     * Returns the handle of the small icon (or NULL).
     */
    HICON get_small_icon_handle() const { return _small_icon_handle; }

    /**
     * Returns the flag indicating whether the window is currently active.
     */
    bool is_active() const { return _active; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Shows the window (makes the window visible).
     * The function returns true, if the window was visible before.
     */
    bool show_window();

    /**
     * Hides the window (makes the window unvisible).
     * The function returns true, if the window was visible before.
     */
    bool hide_window();

    /**
     * Closes the window (and destroys it).
     */
    bool close();

    /**
     * Processes incoming window messages.
     * Returns a flag indicating a request to terminate the application (WM_QUIT
     * message) and, if so, the exit code (0 otherwise).
     */
    static std::pair<bool, int> process_window_messages();



private:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    MainWindow();

    /**
     * Destructor.
     */
    ~MainWindow() override = default;



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool _init() override;

    /**
     * De-initializes the component.
     */
    bool _shutdown() override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Registers the window class.
     */
    bool _register_window_class();

    /**
     * Creates the window.
     */
    bool _create_window();

    /**
     * Destroys the window.
     */
    bool _destroy_window();

    /**
     * Unregisters the window class.
     */
    static bool _unregister_window_class();



    // #############################################################################
    // Window Proc Callback
    // #############################################################################

    /**
     * The callback function processing incoming window messages.
     */
    static LRESULT CALLBACK wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The handle of the window.
     */
    HWND _handle;

    /**
     * The display mode (fullscreen, borderless, or windowed).
     */
    DisplayMode _display_mode;

    /**
     * The window width, in pixels.
     */
    unsigned _width;

    /**
     * The window height, in pixels.
     */
    unsigned _height;

    /**
     * The title of the window.
     */
    cl7::u8string _title;

    /**
     * The handle of the icon (or NULL).
     */
    HICON _icon_handle;

    /**
     * The handle of the small icon (or NULL).
     */
    HICON _small_icon_handle;

    /**
     * The flag indicating whether the window is currently active.
     */
    bool _active;

}; // class MainWindow



    inline MainWindow& main_window() { return MainWindow::instance(); }



} // namespace xl7

#endif // XL7_MAINWINDOW_H
