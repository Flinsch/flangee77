#include "errors.h"



namespace xl7::graphics::impl::shared::errors {



    cl7::string dxgi_result(long error_code)
    {
        return cl7::errors::system_result(static_cast<unsigned long>(error_code));
    }

    cl7::string dxgi_result(long error_code, cl7::string_view context)
    {
        return cl7::errors::with_context(dxgi_result(error_code), context);
    }

    cl7::string d3d_result(long error_code)
    {
        return cl7::errors::system_result(static_cast<unsigned long>(error_code));
    }

    cl7::string d3d_result(long error_code, cl7::string_view context)
    {
        return cl7::errors::with_context(d3d_result(error_code), context);
    }



} // namespace xl7::graphics::impl::shared::errors
