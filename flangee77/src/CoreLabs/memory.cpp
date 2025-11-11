#include "memory.h"

#include <CoreLabs/sstream.h>

#include <vector>



namespace cl7::memory {



    /**
     * Converts a byte count to a human-readable string using decimal SI units
     * (e.g., "kB", "MB", etc.). If the byte count is zero, the provided string is
     * returned instead.
     */
    cl7::u8string stringify_byte_amount_si(unsigned long long bytes, cl7::u8string_view zero_string)
    {
        if (!bytes)
            return cl7::u8string(zero_string);

        constexpr unsigned base_factor = 1000;

        constexpr cl7::u8string_view units[] = {
            u8"B", u8"kB", u8"MB", u8"GB", u8"TB", u8"PB", u8"EB", u8"ZB", u8"YB",
        };
        constexpr unsigned unit_count = std::size(units);
        constexpr unsigned max_orders_of_magnitude = unit_count - 1;

        unsigned orders_of_magnitude;
        const unsigned x = get_reasonable_byte_amount(bytes, base_factor, max_orders_of_magnitude, &orders_of_magnitude);
        assert(orders_of_magnitude <= max_orders_of_magnitude);
        assert(orders_of_magnitude < unit_count);

        cl7::u8osstream oss;
        oss << x << u8" " << units[orders_of_magnitude];

        return oss.str();
    }

    /**
     * Converts a byte count to a human-readable string using binary IEC units
     * (e.g., "KiB", "MiB", etc.). If the byte count is zero, the provided string is
     * returned instead.
     */
    cl7::u8string stringify_byte_amount_iec(unsigned long long bytes, cl7::u8string_view zero_string)
    {
        if (!bytes)
            return cl7::u8string(zero_string);

        constexpr unsigned base_factor = 1024;

        constexpr cl7::u8string_view units[] = {
            u8"B", u8"KiB", u8"MiB", u8"GiB", u8"TiB", u8"PiB", u8"EiB", u8"ZiB", u8"YiB",
        };
        constexpr unsigned unit_count = std::size(units);
        constexpr unsigned max_orders_of_magnitude = unit_count - 1;

        unsigned orders_of_magnitude;
        const unsigned x = get_reasonable_byte_amount(bytes, base_factor, max_orders_of_magnitude, &orders_of_magnitude);
        assert(orders_of_magnitude <= max_orders_of_magnitude);
        assert(orders_of_magnitude < unit_count);

        cl7::u8osstream oss;
        oss << x << u8" " << units[orders_of_magnitude];

        return oss.str();
    }

    /**
     * Calculates the largest reasonable unit for a byte count and scales the value
     * accordingly. You can specify the base factor for scaling (e.g., 1000 for
     * decimal SI, 1024 for binary IEC) and the maximum number of orders of
     * magnitude to consider. The actual number of orders of magnitude is optionally
     * stored via the corresponding output parameter.
     */
    unsigned get_reasonable_byte_amount(unsigned long long bytes, unsigned base_factor, unsigned max_orders_of_magnitude, unsigned* orders_of_magnitude_out)
    {
        constexpr unsigned long long max_amount = 9999;

        unsigned long long x = bytes;
        unsigned c = 0;
        while (base_factor > 1 && x > max_amount && c < max_orders_of_magnitude)
        {
            x /= base_factor;
            ++c;
        }

        if (orders_of_magnitude_out)
            *orders_of_magnitude_out = c;

        return static_cast<unsigned>(x);
    }



} // namespace cl7::memory
