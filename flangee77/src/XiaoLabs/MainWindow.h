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



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
private:
    /**
     * Default constructor.
     */
    MainWindow();

    /**
     * Destructor.
     */
    virtual ~MainWindow() override = default;

private:
    /** Copy constructor. */
    MainWindow(const MainWindow&) = delete;
    /** Copy assignment operator. */
    MainWindow& operator = (const MainWindow&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The handle of the window.
     */
    HWND _handle;

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
    cl7::string _title;

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



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the handle of the window.
     */
    const HWND get_handle() const { return _handle; }

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
    const cl7::string& get_title() const { return _title; }

    /**
     * Returns the handle of the icon (or NULL).
     */
    const HICON get_icon_handle() const { return _icon_handle; }

    /**
     * Returns the handle of the small icon (or NULL).
     */
    const HICON get_small_icon_handle() const { return _small_icon_handle; }

    /**
     * Returns the flag indicating whether the window is currently active.
     */
    bool is_active() const { return _active; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
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
    std::pair<bool, int> process_window_messages();



    // #############################################################################
    // Component Implementations
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init() override;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
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
    bool _unregister_window_class();



    // #############################################################################
    // Window Proc Callback
    // #############################################################################
private:
    /**
     * The callback function processing incoming window messages.
     */
    static LRESULT CALLBACK wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam);

}; // class MainWindow



} // namespace xl7

#endif // XL7_MAINWINDOW_H
