#include "errors.h"

#include <CoreLabs/strings.h>
#include <CoreLabs/sstream.h>

#include <windows.h>



namespace cl7 {
namespace errors {



    cl7::string with_context(cl7::string_view message, cl7::string_view context)
    {
        cl7::osstream oss;
        oss << message << TEXT(" ") << cl7::string( context );
        return oss.str();
    }

    cl7::string system_result(unsigned long error_code)
    {
        // Extract from microsoft.com for the FormatMessage function:
        // "This buffer cannot be larger than 64K bytes."
        constexpr unsigned buffer_size = 65536 / sizeof(cl7::char_type);
        static cl7::char_type buffer[ buffer_size ];

        unsigned length = ::FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error_code,
            0,
            buffer,
            buffer_size,
            NULL );

        cl7::osstream oss;
        oss << std::hex << TEXT("0x") << error_code;
        oss << TEXT(" ");
        if ( length )
            oss << cl7::strings::trimmed( cl7::string( buffer ) );
        else
            oss << TEXT("An unknown error occurred.");
        return oss.str();
    }

    cl7::string system_result(unsigned long error_code, cl7::string_view context)
    {
        return with_context( system_result( error_code ), context );
    }



} // namespace errors
} // namespace cl7
