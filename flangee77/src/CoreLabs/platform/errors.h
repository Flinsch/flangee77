#ifndef CL7_PLATFORM_ERRORS_H
#define CL7_PLATFORM_ERRORS_H

#include <CoreLabs/string.h>



namespace cl7::platform::errors {



    cl7::u8string with_context(cl7::u8string_view message, cl7::u8string_view context);

    cl7::u8string system_result(int error_code);
    cl7::u8string system_result(int error_code, cl7::u8string_view context);



} // namespace cl7::platform::errors

#endif // CL7_PLATFORM_ERRORS_H
