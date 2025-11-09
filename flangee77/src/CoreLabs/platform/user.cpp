#include "user.h"

#if F77_IS_WINDOWS
#include "./detail/windows/user.h"
namespace {
namespace impl = cl7::platform::detail::windows::user;
}
#endif



namespace cl7::platform::user {



    /**
     * Retrieves the name of the system user.
     */
    cl7::u8string get_user_name(UserNameFormat user_name_format)
    {
        return impl::get_user_name(user_name_format);
    }



    /**
     * Retrieves the width of the screen of the primary display monitor, in pixels.
     */
    int get_screen_width()
    {
        return impl::get_screen_width();
    }

    /**
     * Retrieves the height of the screen of the primary display monitor, in pixels.
     */
    int get_screen_height()
    {
        return impl::get_screen_height();
    }



    /**
     * Increments the internal display counter that determines whether the mouse
     * cursor should be displayed by one. The cursor is displayed if the display
     * counter is greater than or equal to 0. If a mouse is installed, the initial
     * display count is 0. If no mouse is installed, the display count is -1.
     * Returns the new display counter.
     */
    int show_cursor()
    {
        return impl::show_cursor();
    }

    /**
     * Decrements the internal display counter that determines whether the mouse
     * cursor should be displayed by one. The cursor is displayed if the display
     * counter is greater than or equal to 0. If a mouse is installed, the initial
     * display count is 0. If no mouse is installed, the display count is -1.
     * Returns the new display counter.
     */
    int hide_cursor()
    {
        return impl::hide_cursor();
    }



} // namespace cl7::platform::user
