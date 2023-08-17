#pragma once
#ifndef XL7_ERRORS_H
#define XL7_ERRORS_H

#include <CoreLabs/string.h>



namespace xl7 {
namespace errors {



    cl7::string with_context(const cl7::string& message, const cl7::string& context);

    cl7::string windows_result(unsigned long error_code);
    cl7::string windows_result(unsigned long error_code, const cl7::string& context);

    /*cl7::string directx_result(long error_code);
    cl7::string directx_result(long error_code, const cl7::string& context);*/



} // namespace errors
} // namespace xl7

#endif // XL7_ERRORS_H
