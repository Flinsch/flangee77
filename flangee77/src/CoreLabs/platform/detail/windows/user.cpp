#include "user.h"
#if F77_IS_WINDOWS

#include <CoreLabs/logging.h>
#include <CoreLabs/platform/errors.h>
#include <CoreLabs/text/codec.h>

#include <windows.h>
#define SECURITY_WIN32
#include <security.h>



namespace cl7::platform::detail::windows::user {



    cl7::u8string get_user_name(cl7::platform::user::UserNameFormat user_name_format)
    {
        constexpr unsigned long MAX_NAME_LENGTH = 1024;
        wchar_t name[MAX_NAME_LENGTH] = {};

        EXTENDED_NAME_FORMAT extended_name_format = NameSamCompatible;

        unsigned long name_length = MAX_NAME_LENGTH - 1;
        if (!::GetUserNameExW(extended_name_format, name, &name_length))
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::GetUserNameEx"));
        }

        cl7::u8string user_name = cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(name));

        if (user_name_format == cl7::platform::user::UserNameFormat::PureUserName)
        {
            size_t p = user_name.find(u8'\\');
            if (p != cl7::u8string::npos)
                user_name = user_name.substr(p + 1);
        }

        return user_name;
    }



    int get_screen_width()
    {
        return ::GetSystemMetrics(SM_CXSCREEN);
    }

    int get_screen_height()
    {
        return ::GetSystemMetrics(SM_CYSCREEN);
    }



    int show_cursor()
    {
        return ::ShowCursor(TRUE);
    }

    int hide_cursor()
    {
        return ::ShowCursor(FALSE);
    }



} // namespace cl7::platform::detail::windows::user

#endif // F77_IS_WINDOWS
