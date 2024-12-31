#ifndef XL7_GRAPHICS_IMPL_SHARED_ERRORS_H
#define XL7_GRAPHICS_IMPL_SHARED_ERRORS_H

#include <CoreLabs/errors.h>



namespace xl7::graphics::impl::shared::errors {



    cl7::u8string dxgi_result(long error_code);
    cl7::u8string dxgi_result(long error_code, cl7::u8string_view context);

    cl7::u8string d3d_result(long error_code);
    cl7::u8string d3d_result(long error_code, cl7::u8string_view context);



} // namespace xl7::graphics::impl::shared::errors

#endif // XL7_GRAPHICS_IMPL_SHARED_ERRORS_H
