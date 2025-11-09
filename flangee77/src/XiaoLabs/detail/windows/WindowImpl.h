#ifndef XL7_DETAIL_WINDOWS_WINDOWIMPL_H
#define XL7_DETAIL_WINDOWS_WINDOWIMPL_H
#include "../IWindowImpl.h"
#if F77_IS_WINDOWS

#include <windows.h>



namespace xl7::detail::windows {



class WindowImpl
    : public xl7::detail::IWindowImpl
{
public:
    ~WindowImpl() override = default;



    bool create_window(cl7::u8string_view title, DisplayMode display_mode, int x, int y, int width, int height) override;
    bool destroy_window() override;

    void show_window() override;
    void hide_window() override;

    void close_window() override;

    std::pair<bool, int> process_window_events() override;

    bool is_active() override { return _is_active; }

    void* get_handle() override { return _handle; }



private:
    static bool _register_window_class();
    bool _create_window(cl7::u8string_view title, DisplayMode display_mode, int x, int y, int width, int height);
    bool _destroy_window();
    static bool _unregister_window_class();

    static LRESULT CALLBACK wnd_proc(
        HWND hwnd,
        UINT msg,
        WPARAM wparam,
        LPARAM lparam);

    HWND _handle = nullptr;

    bool _is_active = false;

}; // WindowImpl



} // namespace xl7::detail::windows

#endif // F77_IS_WINDOWS
#endif // XL7_DETAIL_WINDOWS_WINDOWIMPL_H
