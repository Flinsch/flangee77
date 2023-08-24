#include "Window.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>



namespace xl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    Window::Window(void)
        : _handle( NULL )
        , _width( 0 )
        , _height( 0 )
        , _title( TEXT("flangee77") )
        , _icon_handle( NULL )
        , _small_icon_handle( NULL )
        , _active( false )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Shows the window (makes the window visible).
     * The function returns true, if the window was visible before.
     */
    bool Window::show_window()
    {
        // Show the window and remember
        // previous visibility state.
        bool visible_before = ::ShowWindow( _handle, SW_SHOW ) != 0;

        // Update the client area of the window.
        if ( !::UpdateWindow( _handle ) )
            LOG_ERROR( cl7::errors::system_result( ::GetLastError(), TEXT("::UpdateWindow") ) );

        return visible_before;
    }

    /**
     * Hides the window (makes the window unvisible).
     * The function returns true, if the window was visible before.
     */
    bool Window::hide_window()
    {
        // Hide the window and remember
        // previous visibility state.
        bool visible_before = ::ShowWindow( _handle, SW_HIDE ) != 0;

        return visible_before;
    }

    /**
     * Closes the window (and destroys it).
     */
    bool Window::close()
    {
        // Send WM_CLOSE to the window. The window will
        // be destroyed from the window callback.
        ::SendMessage( _handle, WM_CLOSE, 0, 0 );

        return true;
    }

    /**
     * Processes incoming window messages.
     * Returns a flag indicating a request to terminate the application (WM_QUIT
     * message) and, if so, the exit code (0 otherwise).
     */
    std::pair<bool, int> Window::process_window_messages()
    {
        std::pair<bool, int> quit_flag_and_exit_code{ false, 0 };

        MSG msg;

        while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
        {
            if ( msg.message == WM_QUIT )
                break;

            ::TranslateMessage( &msg );
            ::DispatchMessage( &msg );
        } // while ...

        // Check for WM_QUIT message and, if present,
        // set the quit flag and the exit code.
        if ( msg.message == WM_QUIT )
            quit_flag_and_exit_code = { true, static_cast<int>( msg.wParam ) };

        return quit_flag_and_exit_code;
    }



    // #############################################################################
    // Component Implementations
    // #############################################################################

    /**
     * Initializes the component.
     */
    bool Window::_init()
    {
        if ( !_register_window_class() )
        {
            LOG_ERROR( TEXT("The window class for the flangee77 main window could not be registered.") );
            return false;
        }

        if ( !_create_window() )
        {
            LOG_ERROR( TEXT("The flangee77 main window could not be created.") );
            return false;
        }

        LOG_SUCCESS( TEXT("The flangee77 main window has been successfully initialized.") );
        return true;
    }

    /**
     * De-initializes the component.
     */
    bool Window::_shutdown()
    {
        if ( ::IsWindow( _handle ) )
            _destroy_window();

        _unregister_window_class();

        LOG_SUCCESS( TEXT("The flangee77 main window has been closed.") );
        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Registers the window class.
     */
    bool Window::_register_window_class()
    {
        // Determine the handles
        // of the desired icons.
        _icon_handle = NULL;
        _small_icon_handle = NULL;

        // Fill the data structure.
        WNDCLASSEX wnd_class;
        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.style = CS_CLASSDC;
        wnd_class.lpfnWndProc = wnd_proc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = ::GetModuleHandle( NULL );
        wnd_class.hIcon = _icon_handle;
        wnd_class.hCursor = NULL;
        wnd_class.hbrBackground = NULL;
        wnd_class.lpszMenuName = NULL;
        wnd_class.lpszClassName = TEXT("flangee77_window_class");
        wnd_class.hIconSm = _small_icon_handle;

        // Register the window class.
        if ( !RegisterClassEx( &wnd_class ) )
        {
            LOG_ERROR( cl7::errors::system_result( ::GetLastError(), TEXT("::RegisterClassEx") ) );
            return false;
        }

        return true;
    }

    /**
     * Creates the window.
     */
    bool Window::_create_window()
    {
        // Fetch the desktop resolution...
        const int screen_width = ::GetSystemMetrics( SM_CXSCREEN );
        const int screen_height = ::GetSystemMetrics( SM_CYSCREEN );

        // ...and the desired window size.
        int width = static_cast<int>( get_config().video.display_mode.width );
        int height = static_cast<int>( get_config().video.display_mode.height );
        if ( width <= 0 || height <= 0 )
        {
            // If no legal size has been specified,
            // take the desktop size as default.
            width = screen_width;
            height = screen_height;
        }

        // Fetch the desired window title.
        _title = get_config().generic.window.title;

        // Define the window style and position,
        // depending on the display mode.
        DWORD style;
        int x, y;
        if ( get_config().video.display_mode.fullscreen )
        {
            // Fullscreen mode
            style = WS_POPUP | WS_MAXIMIZE;
            x = y = 0;
        }
        else if ( width == screen_width && height == screen_height )
        {
            // Borderless mode
            style = WS_POPUP | WS_MAXIMIZE;
            x = y = 0;
        }
        else
        {
            // Windowed mode
            style = WS_OVERLAPPEDWINDOW;
            x = (screen_width - width) / 2;
            y = (screen_height - height) / 2;
        }

        // Create the window.
        _handle = ::CreateWindowEx(
            0,                              // dwExStyle
            TEXT("flangee77_window_class"), // lpClassName
            _title.c_str(),                 // lpWindowName
            style,                          // dwStyle
            x,                              // X
            y,                              // Y
            width,                          // nWidth
            height,                         // nHeight
            NULL,                           // hWndParent
            NULL,                           // hMenu
            ::GetModuleHandle( NULL ),      // hInstance
            NULL );                         // lpParam

        if ( !_handle )
        {
            LOG_ERROR( cl7::errors::system_result( ::GetLastError(), TEXT("::CreateWindowEx") ) );
            return false;
        }

        _width = static_cast<unsigned>( width );
        _height = static_cast<unsigned>( height );

        // Hide the hardware cursor?
        if ( !get_config().generic.use_hardware_cursor )
            ::ShowCursor( FALSE );

        return true;
    }

    /**
     * Destroys the window.
     */
    bool Window::_destroy_window()
    {
        // The window will be destroyed in
        // the window callback for WM_CLOSE.
        if ( !::CloseWindow( _handle ) )
        {
            LOG_WARNING( cl7::errors::system_result( ::GetLastError(), TEXT("::CloseWindow") ) );
            return false;
        }

        return true;
    }

    /**
     * Unregisters the window class.
     */
    bool Window::_unregister_window_class()
    {
        if ( !::UnregisterClass( TEXT("flangee77_window_class"), ::GetModuleHandle( NULL ) ) )
        {
            LOG_WARNING( cl7::errors::system_result( ::GetLastError(), TEXT("::UnregisterClass") ) );
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
    LRESULT CALLBACK Window::wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam)
    {
        switch ( msg )
        {

        case WM_ACTIVATE:
        {
            Window::instance()._active = LOWORD( wparam ) != WA_INACTIVE;
            break;
        }

        case WM_CLOSE:
        {
            ::DestroyWindow( hwnd );
            break;
        }

        case WM_DESTROY:
        {
            ::PostQuitMessage( 0 );
            break;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            if ( Window::instance()._active && wparam == Window::instance().get_config().generic.quit_key )
                ::PostQuitMessage( 0 );
            break;
        }

        default:
        {
            return ::DefWindowProc( hwnd, msg, wparam, lparam );
        }

        } // switch ( msg )

        return 0;
    }



} // namespace xl7
