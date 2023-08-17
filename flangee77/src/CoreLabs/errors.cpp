#include "errors.h"

#include <windows.h>



namespace cl7 {
namespace errors {



    cl7::string with_context(const cl7::string& message, const cl7::string& context)
    {
        return message + TEXT(" - ") + context;
    }

    cl7::string system_result(unsigned long error_code)
    {
        // Extract from microsoft.com for the FormatMessage function:
        // "This buffer cannot be larger than 64K bytes."
        constexpr unsigned buffer_size = 65536 / sizeof(cl7::char_type);
        static cl7::char_type buffer[ buffer_size ];

        ::FormatMessage(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            error_code,
            MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ),
            buffer,
            buffer_size,
            NULL );

        return cl7::to_string( error_code ) + TEXT(" - ") + buffer;
    }

    cl7::string system_result(unsigned long error_code, const cl7::string& context)
    {
        return with_context( system_result( error_code ), context );
    }



} // namespace errors
} // namespace cl7
