#include "memory.h"

#include <CoreLabs/sstream.h>

#include <vector>



namespace cl7::memory {



    /**
     * 
     */
    cl7::string stringify_byte_amount_si(unsigned long long bytes, cl7::string_view zero_string)
    {
        if (!bytes)
            return cl7::string(zero_string);

        constexpr unsigned base_factor = 1000;

        constexpr cl7::string_view units[] = {
            //TEXT("B"), TEXT("kB"), TEXT("MB"), TEXT("GB"), TEXT("TB"), TEXT("PB"), TEXT("EB"), TEXT("ZB"), TEXT("YB"),
            TEXT("B"), TEXT("KB"), TEXT("MB"), TEXT("GB"), TEXT("TB"), TEXT("PB"), TEXT("EB"), TEXT("ZB"), TEXT("YB"),
        };
        constexpr unsigned unit_count = std::size(units);
        constexpr unsigned max_orders_of_magnitude = unit_count - 1;

        unsigned orders_of_magnitude;
        const unsigned x = get_reasonable_byte_amount(bytes, base_factor, max_orders_of_magnitude, &orders_of_magnitude);
        assert(orders_of_magnitude <= max_orders_of_magnitude);
        assert(orders_of_magnitude < unit_count);

        cl7::osstream oss;
        oss << x << TEXT(" ") << units[orders_of_magnitude];

        return oss.str();
    }

    /**
     * 
     */
    cl7::string stringify_byte_amount_binary(unsigned long long bytes, cl7::string_view zero_string)
    {
        if (!bytes)
            return cl7::string(zero_string);

        constexpr unsigned base_factor = 1024;

        constexpr cl7::string_view units[] = {
            TEXT("B"), TEXT("KiB"), TEXT("MiB"), TEXT("GiB"), TEXT("TiB"), TEXT("PiB"), TEXT("EiB"), TEXT("ZiB"), TEXT("YiB"),
        };
        constexpr unsigned unit_count = std::size(units);
        constexpr unsigned max_orders_of_magnitude = unit_count - 1;

        unsigned orders_of_magnitude;
        const unsigned x = get_reasonable_byte_amount(bytes, base_factor, max_orders_of_magnitude, &orders_of_magnitude);
        assert(orders_of_magnitude <= max_orders_of_magnitude);
        assert(orders_of_magnitude < unit_count);

        cl7::osstream oss;
        oss << x << TEXT(" ") << units[orders_of_magnitude];

        return oss.str();
    }

    /**
     * 
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
