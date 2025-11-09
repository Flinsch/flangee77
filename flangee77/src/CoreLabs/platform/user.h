#ifndef CL7_PLATFORM_USER_H
#define CL7_PLATFORM_USER_H

#include <CoreLabs/string.h>



namespace cl7::platform::user {



    enum struct UserNameFormat
    {
        FullUserName,
        PureUserName,
    };



    /**
     * Retrieves the name of the current system user.
     */
    cl7::u8string get_user_name(UserNameFormat user_name_format);



    /**
     * Retrieves the width of the screen of the primary display monitor, in pixels.
     */
    int get_screen_width();

    /**
     * Retrieves the height of the screen of the primary display monitor, in pixels.
     */
    int get_screen_height();



    /**
     * Increments the internal display counter that determines whether the mouse
     * cursor should be displayed by one. The cursor is displayed if the display
     * counter is greater than or equal to 0. If a mouse is installed, the initial
     * display count is 0. If no mouse is installed, the display count is -1.
     * Returns the new display counter.
     */
    int show_cursor();

    /**
     * Decrements the internal display counter that determines whether the mouse
     * cursor should be displayed by one. The cursor is displayed if the display
     * counter is greater than or equal to 0. If a mouse is installed, the initial
     * display count is 0. If no mouse is installed, the display count is -1.
     * Returns the new display counter.
     */
    int hide_cursor();



} // namespace cl7::platform::user

#endif // CL7_PLATFORM_USER_H
