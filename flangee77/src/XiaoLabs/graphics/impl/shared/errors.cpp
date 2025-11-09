#include "errors.h"



namespace xl7::graphics::impl::shared::errors {



    cl7::u8string dxgi_result(long error_code)
    {
        return cl7::platform::errors::system_result(static_cast<int>(error_code));
    }

    cl7::u8string dxgi_result(long error_code, cl7::u8string_view context)
    {
        return cl7::platform::errors::with_context(dxgi_result(error_code), context);
    }

    cl7::u8string d3d_result(long error_code)
    {
        return cl7::platform::errors::system_result(static_cast<int>(error_code));
    }

    cl7::u8string d3d_result(long error_code, cl7::u8string_view context)
    {
        return cl7::platform::errors::with_context(d3d_result(error_code), context);
    }



} // namespace xl7::graphics::impl::shared::errors
