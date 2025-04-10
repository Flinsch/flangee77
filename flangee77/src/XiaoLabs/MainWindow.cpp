#include "MainWindow.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>



namespace xl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Shows the window (makes the window visible).
     * The function returns true, if the window was visible before.
     */
    bool MainWindow::show_window()
    {
        // Show the window and remember
        // previous visibility state.
        bool visible_before = ::ShowWindow(_handle, SW_SHOW) != 0;

        // Update the client area of the window.
        if (!::UpdateWindow(_handle))
            LOG_ERROR(cl7::errors::system_result(::GetLastError(), u8"::UpdateWindow"));

        return visible_before;
    }

    /**
     * Hides the window (makes the window unvisible).
     * The function returns true, if the window was visible before.
     */
    bool MainWindow::hide_window()
    {
        // Hide the window and remember
        // previous visibility state.
        bool visible_before = ::ShowWindow(_handle, SW_HIDE) != 0;

        return visible_before;
    }

    /**
     * Closes the window (and destroys it).
     */
    bool MainWindow::close()
    {
        // Send WM_CLOSE to the window. The window will
        // be destroyed from the window callback.
        ::SendMessage(_handle, WM_CLOSE, 0, 0);

        return true;
    }

    /**
     * Processes incoming window messages.
     * Returns a flag indicating a request to terminate the application (WM_QUIT
     * message) and, if so, the exit code (0 otherwise).
     */
    std::pair<bool, int> MainWindow::process_window_messages()
    {
        std::pair<bool, int> quit_flag_and_exit_code{false, 0};

        MSG msg;

        while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        } // while ...

        // Check for WM_QUIT message and, if present,
        // set the quit flag and the exit code.
        if (msg.message == WM_QUIT)
            quit_flag_and_exit_code = {true, static_cast<int>(msg.wParam)};

        return quit_flag_and_exit_code;
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
        if (!_register_window_class())
        {
            LOG_ERROR(u8"The window class for the flangee77 main window could not be registered.");
            return false;
        }

        if (!_create_window())
        {
            LOG_ERROR(u8"The flangee77 main window could not be created.");
            return false;
        }

        LOG_SUCCESS(u8"The flangee77 main window has been successfully initialized.");
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool MainWindow::_shutdown()
    {
        if (::IsWindow(_handle))
            _destroy_window();

        _unregister_window_class();

        LOG_SUCCESS(u8"The flangee77 main window has been closed and destroyed.");
        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Registers the window class.
     */
    bool MainWindow::_register_window_class()
    {
        // Determine the handles
        // of the desired icons.
        _icon_handle = nullptr;
        _small_icon_handle = nullptr;

        // Fill the data structure.
        WNDCLASSEXW wnd_class;
        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.style = CS_CLASSDC;
        wnd_class.lpfnWndProc = wnd_proc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = ::GetModuleHandleW(nullptr);
        wnd_class.hIcon = _icon_handle;
        wnd_class.hCursor = nullptr;
        wnd_class.hbrBackground = nullptr;
        wnd_class.lpszMenuName = nullptr;
        wnd_class.lpszClassName = L"flangee77_window_class";
        wnd_class.hIconSm = _small_icon_handle;

        // Register the window class.
        if (!::RegisterClassExW(&wnd_class))
        {
            LOG_ERROR(cl7::errors::system_result(::GetLastError(), u8"::RegisterClassEx"));
            return false;
        }

        return true;
    }

    /**
     * Creates the window.
     */
    bool MainWindow::_create_window()
    {
        // Fetch the desktop resolution...
        const int screen_width = ::GetSystemMetrics(SM_CXSCREEN);
        const int screen_height = ::GetSystemMetrics(SM_CYSCREEN);

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
        DWORD style;
        int x;
        int y;
        if (get_config().video.presentation_mode == Config::Video::PresentationMode::ExclusiveFullscreen)
        {
            // Fullscreen mode
            display_mode = DisplayMode::Fullscreen;
            style = WS_POPUP | WS_MAXIMIZE;
            x = y = 0;
        }
        else if (get_config().video.presentation_mode == Config::Video::PresentationMode::BorderlessFullscreen)
        {
            // Borderless mode
            display_mode = DisplayMode::Borderless;
            style = WS_POPUP | WS_MAXIMIZE;
            x = y = 0;
            // Force width and height to match the desktop size.
            width = screen_width;
            height = screen_height;
        }
        else
        {
            assert(get_config().video.presentation_mode == Config::Video::PresentationMode::Windowed);
            // Windowed mode
            display_mode = DisplayMode::Windowed;
            style = WS_OVERLAPPEDWINDOW;
            x = (screen_width - width) / 2;
            y = (screen_height - height) / 2;
        }

        const std::wstring window_name{cl7::strings::reinterpret_utf16(cl7::strings::to_utf16(_title))};

        // Create the window.
        _handle = ::CreateWindowExW(
            0,                              // dwExStyle
            L"flangee77_window_class",      // lpClassName
            window_name.c_str(),            // lpWindowName
            style,                          // dwStyle
            x,                              // X
            y,                              // Y
            width,                          // nWidth
            height,                         // nHeight
            nullptr,                        // hWndParent
            nullptr,                        // hMenu
            ::GetModuleHandleW(nullptr),     // hInstance
            nullptr);                       // lpParam

        if (!_handle)
        {
            LOG_ERROR(cl7::errors::system_result(::GetLastError(), u8"::CreateWindowEx"));
            return false;
        }

        _display_mode = display_mode;
        _width = static_cast<unsigned>(width);
        _height = static_cast<unsigned>(height);

        // Hide the hardware cursor?
        if (!get_config().general.use_hardware_cursor)
            ::ShowCursor(FALSE);

        return true;
    }

    /**
     * Destroys the window.
     */
    bool MainWindow::_destroy_window()
    {
        // The window will be destroyed in
        // the window callback for WM_CLOSE.
        if (!::CloseWindow(_handle))
        {
            LOG_WARNING(cl7::errors::system_result(::GetLastError(), u8"::CloseWindow"));
            return false;
        }

        return true;
    }

    /**
     * Unregisters the window class.
     */
    bool MainWindow::_unregister_window_class()
    {
        if (!::UnregisterClassW(L"flangee77_window_class", ::GetModuleHandleW(nullptr)))
        {
            LOG_WARNING(cl7::errors::system_result(::GetLastError(), u8"::UnregisterClass"));
            return false;
        }

        return true;
    }



    // #############################################################################
    // Window Proc Callback
    // #############################################################################

    /**
     * The callback function processing incoming window messages.
     */
    LRESULT CALLBACK MainWindow::wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam)
    {
        switch (msg)
        {

        case WM_ACTIVATE:
        {
            MainWindow::instance()._active = LOWORD(wparam) != WA_INACTIVE;
            break;
        }

        case WM_CLOSE:
        {
            ::DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY:
        {
            ::PostQuitMessage(0);
            break;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            if (MainWindow::instance()._active && wparam == MainWindow::instance().get_config().general.quit_key)
                ::PostQuitMessage(0);
            break;
        }

        default:
        {
            return ::DefWindowProc(hwnd, msg, wparam, lparam);
        }

        } // switch (msg)

        return 0;
    }



} // namespace xl7
