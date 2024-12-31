#ifndef XL7_GRAPHICS_IMPL_D3D11_ERRORS_H
#define XL7_GRAPHICS_IMPL_D3D11_ERRORS_H

#include <CoreLabs/errors.h>



namespace xl7::graphics::impl::direct3d11::errors {



    cl7::u8string dxgi_result(long error_code);
    cl7::u8string dxgi_result(long error_code, cl7::u8string_view context);

    cl7::u8string d3d11_result(long error_code);
    cl7::u8string d3d11_result(long error_code, cl7::u8string_view context);



} // namespace xl7::graphics::impl::direct3d11::errors

#endif // XL7_GRAPHICS_IMPL_D3D11_ERRORS_H
