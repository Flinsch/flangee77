#pragma once
#ifndef CL7_MEMORY_H
#define CL7_MEMORY_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace memory {



    // #############################################################################
    // Utility Functions
    // #############################################################################

    /**
     * 
     */
    cl7::string stringify_byte_amount(const unsigned long long bytes, const cl7::string& zero_string = TEXT("0 MB"));

    /**
     * 
     */
    unsigned get_reasonable_byte_amount(const unsigned long long bytes, cl7::string* unit);



} // namespace memory
} // namespace cl7

#endif // CL7_MEMORY_H
