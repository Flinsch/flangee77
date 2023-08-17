#pragma once
#ifndef CL7_SYSTEM_SECURITY_H
#define CL7_SYSTEM_SECURITY_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace system {



class user
{

public:
    enum class UserNameFormat
    {
        FullUserName,
        PureUserName,
    }; // enum class UserNameFormat



    // #############################################################################
    // Static Methods
    // #############################################################################
public:
    /**
     * Retrieves the name of the system user.
     */
    static cl7::string get_user_name(UserNameFormat user_name_format);

}; // class user



} // namespace system
} // namespace cl7

#endif // CL7_SYSTEM_SECURITY_H
