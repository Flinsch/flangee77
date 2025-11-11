#ifndef CL7_MEMORY_H
#define CL7_MEMORY_H

#include <CoreLabs/string.h>



namespace cl7::memory {



    /**
     * Converts a byte count to a human-readable string using decimal SI units
     * (e.g., "kB", "MB", etc.). If the byte count is zero, the provided string is
     * returned instead.
     */
    cl7::u8string stringify_byte_amount_si(unsigned long long bytes, cl7::u8string_view zero_string = u8"0 MB");

    /**
     * Converts a byte count to a human-readable string using binary IEC units
     * (e.g., "KiB", "MiB", etc.). If the byte count is zero, the provided string is
     * returned instead.
     */
    cl7::u8string stringify_byte_amount_iec(unsigned long long bytes, cl7::u8string_view zero_string = u8"0 MiB");

    /**
     * Calculates the largest reasonable unit for a byte count and scales the value
     * accordingly. You can specify the base factor for scaling (e.g., 1000 for
     * decimal SI units, 1024 for binary IEC units) and the maximum number of orders
     * of magnitude to consider. The actual number of orders of magnitude is
     * optionally stored via the corresponding output parameter.
     */
    unsigned get_reasonable_byte_amount(unsigned long long bytes, unsigned base_factor = 1024, unsigned max_orders_of_magnitude = -1, unsigned* orders_of_magnitude_out = nullptr);



} // namespace cl7::memory

#endif // CL7_MEMORY_H
