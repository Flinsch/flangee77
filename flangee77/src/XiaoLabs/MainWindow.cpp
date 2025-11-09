#include "MainWindow.h"

#if F77_IS_WINDOWS
#include "./detail/windows/WindowImpl.h"
namespace {
namespace impl = xl7::detail::windows;
}
#endif

#include <CoreLabs/platform/user.h>
#include <CoreLabs/logging.h>



namespace xl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Shows the window (makes the window visible).
     */
    void MainWindow::show_window()
    {
        assert(_pimpl);
        if (!_pimpl)
            return;
        _pimpl->show_window();
    }

    /**
     * Hides the window (makes the window invisible).
     */
    void MainWindow::hide_window()
    {
        assert(_pimpl);
        if (!_pimpl)
            return;
        _pimpl->hide_window();
    }

    /**
     * Activates the windows.
     */
    void MainWindow::activate_window()
    {
        _active = true;
    }

    /**
     * Deactivates the windows.
     */
    void MainWindow::deactivate_window()
    {
        _active = false;
    }

    /**
     * Closes the (main) window, after which the application will then terminate.
     */
    void MainWindow::close_window()
    {
        assert(_pimpl);
        if (!_pimpl)
            return;
        _pimpl->close_window();
    }

    /**
     * Processes incoming window messages/events.
     * Returns a flag indicating a request to terminate the application and, if so,
     * the (suggested) exit code (0 otherwise).
     */
    std::pair<bool, int> MainWindow::process_window_events()
    {
        assert(_pimpl);
        if (!_pimpl)
            return {false, 0};
        return _pimpl->process_window_events();
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    MainWindow::MainWindow()
        : _title(u8"flangee77")
    {
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool MainWindow::_init()
    {
        if (!_create_window())
            return false;

        LOG_SUCCESS(u8"The flangee77 main window has been successfully initialized.");
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool MainWindow::_shutdown()
    {
        if (!_destroy_window())
            return false;

        LOG_SUCCESS(u8"The flangee77 main window has been closed and destroyed.");
        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the window.
     */
    bool MainWindow::_create_window()
    {
        // Fetch the desktop resolution...
        const int screen_width = cl7::platform::user::get_screen_width();
        const int screen_height = ::cl7::platform::user::get_screen_height();

        // ...and the desired window size.
        int width = static_cast<int>(get_config().video.back_buffer_width);
        int height = static_cast<int>(get_config().video.back_buffer_height);
        if (width <= 0 || height <= 0)
        {
            // If no legal size has been specified,
            // take the desktop size as default.
            width = screen_width;
            height = screen_height;
        }

        // Fetch the desired window title.
        _title = get_config().general.title;

        // Define the window style and position,
        // depending on the display mode.
        DisplayMode display_mode = DisplayMode::Unknown;
        int x;
        int y;
        if (get_config().video.presentation_mode == Config::Video::PresentationMode::ExclusiveFullscreen)
        {
            display_mode = DisplayMode::Fullscreen;
            x = y = 0;
        }
        else if (get_config().video.presentation_mode == Config::Video::PresentationMode::BorderlessFullscreen)
        {
            display_mode = DisplayMode::Borderless;
            x = y = 0;
            // Force width and height to match the desktop size.
            width = screen_width;
            height = screen_height;
        }
        else
        {
            assert(get_config().video.presentation_mode == Config::Video::PresentationMode::Windowed);
            display_mode = DisplayMode::Windowed;
            x = (screen_width - width) / 2;
            y = (screen_height - height) / 2;
        }

        _pimpl = std::make_unique<impl::WindowImpl>();
        if (!_pimpl->create_window(_title, display_mode, x, y, width, height))
            return false;

        _display_mode = display_mode;
        _width = static_cast<unsigned>(width);
        _height = static_cast<unsigned>(height);

        // Hide the hardware cursor?
        if (!get_config().general.use_hardware_cursor)
            cl7::platform::user::hide_cursor();

        return true;
    }

    /**
     * Destroys the window.
     */
    bool MainWindow::_destroy_window()
    {
        assert(_pimpl);
        if (!_pimpl)
            return false;

        bool b = _pimpl->destroy_window();
        _pimpl.reset();

        return b;
    }



} // namespace xl7
