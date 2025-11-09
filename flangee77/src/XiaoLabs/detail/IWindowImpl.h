#ifndef XL7_DETAIL_IWINDOWIMPL_H
#define XL7_DETAIL_IWINDOWIMPL_H

#include "../DisplayMode.h"

#include <CoreLabs/string.h>



namespace xl7::detail {



class IWindowImpl
{
public:
    virtual ~IWindowImpl() = default;



    virtual bool create_window(cl7::u8string_view title, DisplayMode display_mode, int x, int y, int width, int height) = 0;
    virtual bool destroy_window() = 0;

    virtual void show_window() = 0;
    virtual void hide_window() = 0;

    virtual void close_window() = 0;

    virtual std::pair<bool, int> process_window_events() = 0;

    virtual bool is_active() = 0;

    virtual void* get_handle() = 0;

}; // IWindowImpl



} // namespace xl7::detail

#endif // XL7_DETAIL_IWINDOWIMPL_H
