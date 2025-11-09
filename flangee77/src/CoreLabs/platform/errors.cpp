#include "errors.h"

#if F77_IS_WINDOWS
#include "./detail/windows/errors.h"
namespace {
namespace impl = cl7::platform::detail::windows::errors;
}
#endif

#include <CoreLabs/sstream.h>



namespace cl7::platform::errors {



    cl7::u8string with_context(cl7::u8string_view message, cl7::u8string_view context)
    {
        cl7::u8osstream oss;
        oss << message << u8" " << context;
        return oss.str();
    }

    cl7::u8string system_result(int error_code)
    {
        return impl::system_result(static_cast<unsigned long>(error_code));
    }

    cl7::u8string system_result(int error_code, cl7::u8string_view context)
    {
        return with_context(system_result(error_code), context);
    }



} // namespace cl7::platform::errors
