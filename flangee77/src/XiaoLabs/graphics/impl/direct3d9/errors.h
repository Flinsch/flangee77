#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_ERRORS_H
#define XL7_GRAPHICS_IMPL_D3D9_ERRORS_H

#include <CoreLabs/errors.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace errors {



    cl7::string d3d9_result(long error_code);
    cl7::string d3d9_result(long error_code, const cl7::string_view& context);



} // namespace errors
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_ERRORS_H
