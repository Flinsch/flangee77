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



} // namespace utilities
} // namespace ml7

#endif // ML7_UTILITIES_H
