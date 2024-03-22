#pragma once
#ifndef ML7_UTILITIES_H
#define ML7_UTILITIES_H

#include <CoreLabs/root.h>



namespace ml7 {
namespace utilities {



    /**
     * Returns the less value of two given values.
     */
    template <typename T>
    constexpr T min2(T a, T b)
    {
        if ( a <= b )
            return a;
        return b;
    }

    /**
     * Returns the greater value of two given values.
     */
    template <typename T>
    constexpr T max2(T a, T b)
    {
        if ( a >= b )
            return a;
        return b;
    }

    /**
    * Returns the smallest value of three given values.
    */
    template <typename T>
    constexpr T min3(T a, T b, T c)
    {
        T m = a;
        if ( b < m ) m = b;
        if ( c < m ) m = c;
        return m;
    }

    /**
    * Returns the greatest value of three given values.
    */
    template <typename T>
    constexpr T max3(T a, T b, T c)
    {
        T m = a;
        if ( b > m ) m = b;
        if ( c > m ) m = c;
        return m;
    }


    /**
     * Clamps the given value to lie within the range [min; max].
     */
    template <typename T>
    constexpr T clamp(T x, T min, T max)
    {
        if ( x <= min ) return min;
        if ( x >= max ) return max;
        return x;
    }


    /**
     * Rounds the given value to the closest "integer" value.
     */
    float round(float x);

    /**
     * Rounds the given value to the closest "integer" value.
     */
    double round(double x);

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    float round(float x, unsigned num_decimals);

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    double round(double x, unsigned num_decimals);


    /**
     * Checks whether the given value is a power of two.
     */
    template <typename T>
        requires( std::is_integral_v<T> )
    constexpr bool is_power_of_2(T x)
    {
        return x > 0 && (x & (x - 1)) == 0;
    }

    /**
     * Returns the highest power of two value that is less than or equal to the
     * specified non-zero value. If the value is 0, 0 is returned.
     */
    template <typename T>
        requires( std::is_integral_v<T> )
    constexpr T prev_power_of_2(T x)
    {
        constexpr T max = T(1) << (sizeof(T) * 8 - 1);
        if ( x >= max )
            return max;
        unsigned y = 1;
        while ( y <= x )
            y <<= 1;
        return y >> 1;
    }

    /**
     * Returns the lowest power of two value that is greater than or equal to the
     * specified value. If such a value does not exist (regarding integer overflow),
     * 0 is returned.
     */
    template <typename T>
        requires( std::is_integral_v<T> )
    constexpr T next_power_of_2(T x)
    {
        constexpr T max = T(1) << (sizeof(T) * 8 - 1);
        if ( x > max )
            return 0;
        unsigned y = 1;
        while ( y < x )
            y <<= 1;
        return y;
    }



} // namespace utilities
} // namespace ml7

#endif // ML7_UTILITIES_H
