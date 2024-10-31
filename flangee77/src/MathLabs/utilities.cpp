#include "utilities.h"

#include <cmath>



namespace ml7 {
namespace utilities {



    /**
     * Rounds the given value to the closest "integer" value.
     */
    float round(float x)
    {
        return std::floorf( x + 0.5f );
    }

    /**
     * Rounds the given value to the closest "integer" value.
     */
    double round(double x)
    {
        return std::floor( x + 0.5 );
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    float round(float x, unsigned num_decimals)
    {
        const auto p = std::powf( 10.0f, static_cast<float>( num_decimals ) );
        return round( x * p ) / p;
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    double round(double x, unsigned num_decimals)
    {
        const auto p = std::pow( 10.0, static_cast<double>( num_decimals ) );
        return round( x * p ) / p;
    }
    
    /**
     * Checks whether the specified floating point values are (approximately) equal.
     */
    bool is_equal_ulps(float a, float b, int ulps)
    {
        // "Inspired" by:
        // http://www.cygnus-software.com/papers/comparingfloats/Comparing%20floating%20point%20numbers.htm#_Toc135149455
        // https://randomascii.wordpress.com/2012/02/25/comparing-floating-point-numbers-2012-edition/
        static_assert( sizeof(float) == sizeof(long) );
        assert( ulps >= 0 && ulps < 4 * 1024 * 1024 );
        long aint = *(long*)&a;
        long bint = *(long*)&b;
        if ( (aint < 0) != (bint < 0) ) return a == b; // Check for -0 == +0 (or vice versa)
        int diff = static_cast<int>( std::abs( aint - bint ) );
        return diff <= ulps;
    }

    /**
     * Checks whether the value of a is less than the value of b.
     */
    bool is_less_ulps(float a, float b, int ulps)
    {
        if ( is_equal_ulps( a, b, ulps ) ) return false;
        return a < b;
    }

    /**
     * Checks whether the value of a is less than or equal to the value of b.
     */
    bool is_less_equal_ulps(float a, float b, int ulps)
    {
        if ( is_equal_ulps( a, b, ulps ) ) return true;
        return a < b;
    }



} // namespace utilities
} // namespace ml7
