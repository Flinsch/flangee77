#ifndef CL7_MEMORY_H
#define CL7_MEMORY_H

#include <CoreLabs/string.h>



namespace cl7::memory {



    /**
     *
     */
    cl7::u8string stringify_byte_amount_si(unsigned long long bytes, cl7::u8string_view zero_string = u8"0 MB");

    /**
     *
     */
    cl7::u8string stringify_byte_amount_binary(unsigned long long bytes, cl7::u8string_view zero_string = u8"0 MiB");

    /**
     *
     */
    unsigned get_reasonable_byte_amount(unsigned long long bytes, unsigned base_factor = 1024, unsigned max_orders_of_magnitude = -1, unsigned* orders_of_magnitude_out = nullptr);



} // namespace cl7::memory

#endif // CL7_MEMORY_H
