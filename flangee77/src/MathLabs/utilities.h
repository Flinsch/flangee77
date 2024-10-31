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
        requires( std::is_arithmetic_v<T> )
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
        requires( std::is_arithmetic_v<T> )
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
        requires( std::is_arithmetic_v<T> )
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
        requires( std::is_arithmetic_v<T> )
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
        requires( std::is_arithmetic_v<T> )
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
     * Checks whether the specified floating point value is (approximately) zero.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_zero(T x, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return std::abs( x ) <= epsilon;
    }

    /**
     * Checks whether the specified floating point value is (approximately) one.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_one(T x, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return std::abs( x - T(1) ) <= epsilon;
    }

    /**
     * Checks whether the specified (squared) floating point value is
     * (approximately) one squared.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_one_sqr(T sqr, T epsilon = std::numeric_limits<T>::epsilon())
    {
        //  x  =  1+epsilon
        // x^2 = (1+epsilon)^2
        // x^2 = 1^2 + 2*epsilon + epsilon^2
        // x^2 =  1  + 2*epsilon
        // epsilon^2 is omitted because it is far below floating point precision.
        return std::abs( sqr - T(1) ) <= T(2) * epsilon;
    }

    /**
     * Checks whether the specified floating point values are (approximately) equal.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return std::abs( a - b ) <= epsilon;
    }

    /**
     * Checks whether the value of a is less than the value of b.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_less(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return ( b - a ) > epsilon;
    }

    /**
     * Checks whether the value of a is less than or equal to the value of b.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_less_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        if ( a < b ) return true;
        return ( a - b ) <= epsilon;
    }

    /**
     * Checks whether the value of a is greater than the value of b.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_greater(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return is_less( b, a );
    }

    /**
     * Checks whether the value of a is greater than or equal to the value of b.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr bool is_greater_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return is_less_equal( b, a );
    }


    /**
     * Transforms a given angle in degrees to radians.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr T deg2rad(T degrees)
    {
        return degrees * T(0.01745329251);
    }

    /**
     * Transforms a given angle in radians to degrees.
     */
    template <typename T>
        requires( std::is_floating_point_v<T> )
    constexpr T rad2deg(T radians)
    {
        return radians * T(57.2957795131);
    }


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
