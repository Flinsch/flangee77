#include "WindowImpl.h"
#if F77_IS_WINDOWS

#include <XiaoLabs/MainWindow.h>

#include <CoreLabs/platform/errors.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/text/codec.h>



namespace xl7::detail::windows {



    bool WindowImpl::create_window(cl7::u8string_view title, DisplayMode display_mode, int x, int y, int width, int height)
    {
        if (!_register_window_class())
        {
            LOG_ERROR(u8"The window class could not be registered.");
            return false;
        }

        if (!_create_window(title, display_mode, x, y, width, height))
        {
            LOG_ERROR(u8"The window could not be created.");
            return false;
        }

        return true;
    }

    bool WindowImpl::destroy_window()
    {
        if (::IsWindow(_handle))
            _destroy_window();

        _unregister_window_class();

        return true;
    }



    void WindowImpl::show_window()
    {
        // Show the window and remember
        // previous visibility state.
        ::ShowWindow(_handle, SW_SHOW);

        // Update the client area of the window.
        if (!::UpdateWindow(_handle))
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::UpdateWindow"));
        }
    }

    void WindowImpl::hide_window()
    {
        // Hide the window and remember
        // previous visibility state.
        ::ShowWindow(_handle, SW_HIDE);
    }



    void WindowImpl::close_window()
    {
        // Send WM_CLOSE to the window. The window will be
        // destroyed from the window callback, which in turn
        // triggers WM_QUIT to terminate the application.
        ::SendMessage(_handle, WM_CLOSE, 0, 0);
    }



    std::pair<bool, int> WindowImpl::process_window_events()
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



    bool WindowImpl::_register_window_class()
    {
        // Determine the handles
        // of the desired icons.
        HICON icon_handle = nullptr;
        HICON small_icon_handle = nullptr;

        // Fill the data structure.
        WNDCLASSEXW wnd_class;
        wnd_class.cbSize = sizeof(wnd_class);
        wnd_class.style = CS_CLASSDC;
        wnd_class.lpfnWndProc = wnd_proc;
        wnd_class.cbClsExtra = 0;
        wnd_class.cbWndExtra = 0;
        wnd_class.hInstance = ::GetModuleHandleW(nullptr);
        wnd_class.hIcon = icon_handle;
        wnd_class.hCursor = nullptr;
        wnd_class.hbrBackground = nullptr;
        wnd_class.lpszMenuName = nullptr;
        wnd_class.lpszClassName = L"flangee77_window_class";
        wnd_class.hIconSm = small_icon_handle;

        // Register the window class.
        if (!::RegisterClassExW(&wnd_class))
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::RegisterClassEx"));
            return false;
        }

        return true;
    }

    bool WindowImpl::_create_window(cl7::u8string_view title, DisplayMode display_mode, int x, int y, int width, int height)
    {
        const std::wstring window_name{cl7::text::codec::reinterpret_utf16(cl7::text::codec::to_utf16(title))};

        DWORD style;
        if (display_mode == DisplayMode::Fullscreen)
        {
            style = WS_POPUP | WS_MAXIMIZE;
        }
        else if (display_mode == DisplayMode::Borderless)
        {
            style = WS_POPUP | WS_MAXIMIZE;
        }
        else
        {
            assert(display_mode == DisplayMode::Windowed);
            style = WS_OVERLAPPEDWINDOW;
        }

        // Create the window.
        HWND handle = ::CreateWindowExW(
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
            ::GetModuleHandleW(nullptr),    // hInstance
            this);                          // lpParam (gets delivered to the WndProc via the CREATESTRUCT's lpCreateParams)

        if (!handle)
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::CreateWindowEx"));
            return false;
        }

        _handle = handle;
        return true;
    }

    bool WindowImpl::_destroy_window()
    {
        // The window will be destroyed in
        // the window callback for WM_CLOSE.
        if (!::CloseWindow(_handle))
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::CloseWindow"));
            return false;
        }

        return true;
    }

    bool WindowImpl::_unregister_window_class()
    {
        if (!::UnregisterClassW(L"flangee77_window_class", ::GetModuleHandleW(nullptr)))
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::UnregisterClass"));
            return false;
        }

        return true;
    }



    LRESULT CALLBACK WindowImpl::wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam)
    {
        WindowImpl* self = nullptr;

        if (msg == WM_NCCREATE)
        {
            auto* create_struct = std::bit_cast<CREATESTRUCT*>(lparam);
            self = static_cast<WindowImpl*>(create_struct->lpCreateParams);
            assert(self);
            ::SetWindowLongPtrW(hwnd, GWLP_USERDATA, std::bit_cast<LONG_PTR>(self));
        }
        else
        {
            self = std::bit_cast<WindowImpl*>(::GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        }

        switch (msg)
        {

        case WM_ACTIVATE:
        {
            if (self)
                self->_is_active = LOWORD(wparam) != WA_INACTIVE;
            break;
        }

        case WM_CLOSE:
        {
            // The window has been closed to terminate the application.
            // Destroy the window to trigger WM_QUIT (see below).
            ::DestroyWindow(hwnd);
            break;
        }

        case WM_DESTROY:
        {
            // The window has been destroyed. Post WM_QUIT to terminate
            // the application (if that doesn't already happen anyhow).
            ::PostQuitMessage(0);
            break;
        }

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            if (self && self->_is_active && wparam == MainWindow::instance().get_config().general.quit_key)
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



} // namespace xl7::detail::windows

#endif // F77_IS_WINDOWS
