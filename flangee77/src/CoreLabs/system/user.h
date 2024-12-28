#pragma once
#ifndef CL7_SYSTEM_SECURITY_H
#define CL7_SYSTEM_SECURITY_H

#include <CoreLabs/string.h>



namespace cl7::system {



class user
{

public:
    enum class UserNameFormat
    {
        FullUserName,
        PureUserName,
    };



    /**
     * Retrieves the name of the system user.
     */
    static cl7::u8string get_user_name(UserNameFormat user_name_format);

}; // class user



} // namespace cl7::system

#endif // CL7_SYSTEM_SECURITY_H
