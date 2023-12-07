#include "memory.h"

#include <CoreLabs/sstream.h>

#include <vector>



namespace cl7 {
namespace memory {



    // #############################################################################
    // Utility Functions
    // #############################################################################

    /**
     * 
     */
    cl7::string stringify_byte_amount(const unsigned long long bytes, const cl7::string& zero_string)
    {
        if ( !bytes )
            return zero_string;

        cl7::string unit;
        const unsigned x = get_reasonable_byte_amount( bytes, &unit );

        cl7::osstream oss;
        oss << x << TEXT(" ") << unit;

        return oss.str();
    }

    /**
     * 
     */
    unsigned get_reasonable_byte_amount(const unsigned long long bytes, cl7::string* unit)
    {
        constexpr unsigned long long max = 9999;

        const cl7::char_type* units[] = {
            TEXT("B"), TEXT("KB"), TEXT("MB"), TEXT("GB"), TEXT("TB"), TEXT("PB"), TEXT("EB"), TEXT("ZB"), TEXT("YB"),
        };
        const unsigned unit_count = sizeof(units) / sizeof(cl7::char_type*);

        unsigned long long x = bytes;
        unsigned c = 0;
        while ( x > max )
        {
            x /= 1024;
            if ( ++c + 1 == unit_count )
                break;
        } // while ( x > max )

        if ( unit )
        {
            assert( c < unit_count );
            *unit = units[ c ];
        }

        return static_cast<unsigned>( x );
    }



} // namespace memory
} // namespace cl7
