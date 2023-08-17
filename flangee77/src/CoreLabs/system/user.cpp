#include "user.h"

#include <CoreLabs/errors.h>
#include <CoreLabs/logging.h>

#include <windows.h>
#define SECURITY_WIN32
#include <security.h>



namespace cl7 {
namespace system {



    // #############################################################################
    // Static Methods
    // #############################################################################

    /**
     * Retrieves the name of the system user.
     */
    cl7::string user::get_user_name(UserNameFormat user_name_format)
    {
        const unsigned long MAX_NAME_LENGTH = 1024;
        cl7::char_type name[ MAX_NAME_LENGTH ];
        ::memset( name, 0, MAX_NAME_LENGTH * sizeof(cl7::char_type) );

        EXTENDED_NAME_FORMAT extended_name_format = NameSamCompatible;

        unsigned long name_length = MAX_NAME_LENGTH;
        if ( !::GetUserNameEx( extended_name_format, name, &name_length ) )
        {
            //LOG_ERROR( cl7::errors::system_result( ::GetLastError(), TEXT("::GetUserNameEx") ) );
        }

        cl7::string user_name( name );

        if ( user_name_format == UserNameFormat::PureUserName )
        {
            size_t p = user_name.find( TEXT('\\') );
            if ( p >= 0 )
                user_name = user_name.substr( p + 1 );
        }

        return user_name;
    }



} // namespace system
} // namespace cl7
