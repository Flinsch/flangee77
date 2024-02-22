#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_ERRORS_H
#define XL7_GRAPHICS_IMPL_D3D11_ERRORS_H

#include <CoreLabs/errors.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace errors {



    cl7::string dxgi_result(long error_code);
    cl7::string dxgi_result(long error_code, cl7::string_view context);

    cl7::string d3d11_result(long error_code);
    cl7::string d3d11_result(long error_code, cl7::string_view context);



} // namespace errors
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_ERRORS_H
