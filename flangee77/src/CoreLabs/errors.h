#pragma once
#ifndef CL7_ERRORS_H
#define CL7_ERRORS_H

#include <CoreLabs/string.h>



namespace cl7::errors {



    cl7::string with_context(cl7::string_view message, cl7::string_view context);

    cl7::string system_result(unsigned long error_code);
    cl7::string system_result(unsigned long error_code, cl7::string_view context);



} // namespace cl7::errors

#endif // CL7_ERRORS_H
