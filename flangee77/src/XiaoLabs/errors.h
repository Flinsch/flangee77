#pragma once
#ifndef XL7_ERRORS_H
#define XL7_ERRORS_H

#include <CoreLabs/errors.h>



namespace xl7 {
namespace errors {



    cl7::string directx_result(long error_code);
    cl7::string directx_result(long error_code, const cl7::string_view& context);



} // namespace errors
} // namespace xl7

#endif // XL7_ERRORS_H
