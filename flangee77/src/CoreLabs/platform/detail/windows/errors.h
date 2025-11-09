#ifndef CL7_PLATFORM_DETAIL_WINDOWS_ERRORS_H
#define CL7_PLATFORM_DETAIL_WINDOWS_ERRORS_H
#include <CoreLabs/platform/errors.h>
#if F77_IS_WINDOWS



namespace cl7::platform::detail::windows::errors {



    cl7::u8string system_result(unsigned long error_code);



} // namespace cl7::platform::detail::windows::errors

#endif // F77_IS_WINDOWS
#endif // CL7_PLATFORM_DETAIL_WINDOWS_ERRORS_H
