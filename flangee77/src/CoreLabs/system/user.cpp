#include "user.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/strings.h>

#include <windows.h>
#define SECURITY_WIN32
#include <security.h>



namespace cl7::system {



    /**
     * Retrieves the name of the system user.
     */
    cl7::u8string user::get_user_name(UserNameFormat user_name_format)
    {
        constexpr unsigned long MAX_NAME_LENGTH = 1024;
        wchar_t name[MAX_NAME_LENGTH];
        ::memset(name, 0, MAX_NAME_LENGTH * sizeof(wchar_t));

        EXTENDED_NAME_FORMAT extended_name_format = NameSamCompatible;

        unsigned long name_length = MAX_NAME_LENGTH - 1;
        if (!::GetUserNameExW(extended_name_format, name, &name_length))
        {
            LOG_ERROR(cl7::errors::system_result(::GetLastError(), u8"::GetUserNameEx"));
        }

        cl7::u8string user_name = cl7::strings::to_utf8(cl7::strings::reinterpret_utfx(name));

        if (user_name_format == UserNameFormat::PureUserName)
        {
            size_t p = user_name.find(u8'\\');
            if (p != cl7::u8string::npos)
                user_name = user_name.substr(p + 1);
        }

        return user_name;
    }



} // namespace cl7::system
