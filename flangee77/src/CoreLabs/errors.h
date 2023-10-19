#pragma once
#ifndef CL7_ERRORS_H
#define CL7_ERRORS_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace errors {



    cl7::string with_context(const cl7::string_view& message, const cl7::string_view& context);

    cl7::string system_result(unsigned long error_code);
    cl7::string system_result(unsigned long error_code, const cl7::string_view& context);



} // namespace errors
} // namespace cl7

#endif // CL7_ERRORS_H
