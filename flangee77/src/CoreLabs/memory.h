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
    cl7::string stringify_byte_amount_si(const unsigned long long bytes, const cl7::string_view& zero_string = TEXT("0 MB"));

    /**
     * 
     */
    cl7::string stringify_byte_amount_binary(const unsigned long long bytes, const cl7::string_view& zero_string = TEXT("0 MiB"));

    /**
     * 
     */
    unsigned get_reasonable_byte_amount(const unsigned long long bytes, unsigned base_factor = 1024, unsigned max_orders_of_magnitude = -1, unsigned* orders_of_magnitude_out = nullptr);



} // namespace memory
} // namespace cl7

#endif // CL7_MEMORY_H
