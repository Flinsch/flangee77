#ifndef CL7_PLATFORM_DETAIL_WINDOWS_USER_H
#define CL7_PLATFORM_DETAIL_WINDOWS_USER_H
#include <CoreLabs/platform/user.h>
#if F77_IS_WINDOWS



namespace cl7::platform::detail::windows::user {



    cl7::u8string get_user_name(cl7::platform::user::UserNameFormat user_name_format);

    int get_screen_width();
    int get_screen_height();

    int show_cursor();
    int hide_cursor();



} // namespace cl7::platform::detail::windows::user

#endif // F77_IS_WINDOWS
#endif // CL7_PLATFORM_DETAIL_WINDOWS_USER_H
