#include "errors.h"

#include "../shared/errors.h"



namespace xl7::graphics::impl::direct3d11::errors {



    cl7::string dxgi_result(long error_code)
    {
        return shared::errors::dxgi_result(error_code);
    }

    cl7::string dxgi_result(long error_code, cl7::string_view context)
    {
        return cl7::errors::with_context(dxgi_result(error_code), context);
    }

    cl7::string d3d11_result(long error_code)
    {
        return shared::errors::d3d_result(error_code);
    }

    cl7::string d3d11_result(long error_code, cl7::string_view context)
    {
        return cl7::errors::with_context(d3d11_result(error_code), context);
    }



} // namespace xl7::graphics::impl::direct3d11::errors
