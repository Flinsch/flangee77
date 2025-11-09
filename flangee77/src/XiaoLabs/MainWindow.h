#ifndef XL7_MAINWINDOW_H
#define XL7_MAINWINDOW_H
#include "./Component.h"

#include "./detail/IWindowImpl.h"

#include <CoreLabs/string.h>



namespace xl7 {



class MainWindow final
    : public Component<MainWindow>
{
    friend class cl7::creational::Singleton<MainWindow>;



public:
    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(MainWindow&&) = delete;
    MainWindow& operator=(MainWindow&&) = delete;



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the raw handle of the window.
     */
    void* get_handle() const { assert(_pimpl); if (!_pimpl) return nullptr; return _pimpl->get_handle(); }

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
     * Returns the flag indicating whether the window is currently active.
     */
    bool is_active() const { return _active; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Shows the window (makes the window visible).
     */
    void show_window();

    /**
     * Hides the window (makes the window invisible).
     */
    void hide_window();

    /**
     * Activates the windows.
     */
    void activate_window();

    /**
     * Deactivates the windows.
     */
    void deactivate_window();

    /**
     * Closes the (main) window, after which the application will then terminate.
     */
    void close_window();

    /**
     * Processes incoming window messages/events.
     * Returns a flag indicating a request to terminate the application and, if so,
     * the (suggested) exit code (0 otherwise).
     */
    std::pair<bool, int> process_window_events();



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
     * Creates the window.
     */
    bool _create_window();

    /**
     * Destroys the window.
     */
    bool _destroy_window();



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The display mode (fullscreen, borderless, or windowed).
     */
    DisplayMode _display_mode = DisplayMode::Unknown;

    /**
     * The window width, in pixels.
     */
    unsigned _width = 0;

    /**
     * The window height, in pixels.
     */
    unsigned _height = 0;

    /**
     * The title of the window.
     */
    cl7::u8string _title;

    /**
     * The flag indicating whether the window is currently active.
     */
    bool _active = false;

    /**
     * Our "pointer to implementation" of the platform-dependent operations.
     */
    std::unique_ptr<detail::IWindowImpl> _pimpl;

}; // class MainWindow



    inline MainWindow& main_window() { return MainWindow::instance(); }



} // namespace xl7

#endif // XL7_MAINWINDOW_H
