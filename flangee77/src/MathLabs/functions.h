#ifndef ML7_FUNCTIONS_H
#define ML7_FUNCTIONS_H

#include <CoreLabs/root.h>

#include <cmath>



namespace ml7 {



    /**
     * Returns the absolute amount of the given value.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T abs(T x)
    {
        return std::abs(x);
    }

    /**
     * Returns -1, +1, or 0 according to whether the sign of the given value is
     * negative, positive, or zero itself, respectively.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T sgn(T x)
    {
        if (x < T(0)) return T(-1);
        if (x > T(0)) return T(+1);
        return T(0);
    }

    /**
     * Returns 1 if x >= edge, 0 otherwise.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T step(T x, T edge = T(0))
    {
        return x >= edge ? T(1) : T(0);
    }

    /**
     * Returns 0 if x is less than min, 1 if x is greater than max, and a value
     * between 0 and 1 otherwise using a Hermite polynomial.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T smoothstep(T x, T min, T max)
    {
        if (min == max)
            return step(x, min);
        if (x <= min) return T(0);
        if (x >= max) return T(1);
        x = (x - min) / (max - min);
        return x * x * (3.0f - 2.0f * x);
    }


    /**
     * Returns the less value of two given values.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T min2(T a, T b)
    {
        return a <= b ? a : b;
    }

    /**
     * Returns the greater value of two given values.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T max2(T a, T b)
    {
        return a >= b ? a : b;
    }

    /**
    * Returns the smallest value of three given values.
    */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T min3(T a, T b, T c)
    {
        T m = a;
        if (b < m) m = b;
        if (c < m) m = c;
        return m;
    }

    /**
    * Returns the greatest value of three given values.
    */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T max3(T a, T b, T c)
    {
        T m = a;
        if (b > m) m = b;
        if (c > m) m = c;
        return m;
    }


    /**
     * Clamps the given value to lie within the range [min; max].
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T clamp(T x, T min, T max)
    {
        if (x <= min) return min;
        if (x >= max) return max;
        return x;
    }


    /**
     * Rounds the given value to the closest "integer" value.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    T round(T x)
    {
        return std::floor(x + T(0.5));
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    T round(T x, unsigned num_decimals)
    {
        const T p = std::pow(T(10.0), static_cast<T>(num_decimals));
        return round(x * p) / p;
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    template <typename T, unsigned num_decimals>
        requires(std::is_floating_point_v<T>)
    T round(T x)
    {
        constexpr T p = std::pow(T(10.0), static_cast<T>(num_decimals));
        constexpr T i = T(1.0) / p;
        return round(x * p) * i;
    }


    /**
     * Checks whether the specified floating point value is (approximately) zero.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_zero(T x, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return std::abs(x) <= epsilon;
    }

    /**
     * Checks whether the specified floating point value is (approximately) one.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_one(T x, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return std::abs(x - T(1)) <= epsilon;
    }

    /**
     * Checks whether the specified (squared) floating point value is
     * (approximately) one squared.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_one_sqr(T sqr, T epsilon = std::numeric_limits<T>::epsilon())
    {
        //  x  =  1+epsilon
        // x^2 = (1+epsilon)^2
        // x^2 = 1^2 + 2*epsilon + epsilon^2
        // x^2 =  1  + 2*epsilon
        // epsilon^2 is omitted because it is far below floating point precision.
        return std::abs(sqr - T(1)) <= T(2) * epsilon;
    }

    /**
     * Checks whether the specified floating point values are (approximately) equal.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return std::abs(a - b) <= epsilon;
    }

    /**
     * Checks whether the value of a is less than the value of b.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_less(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return (b - a) > epsilon;
    }

    /**
     * Checks whether the value of a is less than or equal to the value of b.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_less_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        if (a < b) return true;
        return (a - b) <= epsilon;
    }

    /**
     * Checks whether the value of a is greater than the value of b.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_greater(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return is_less(b, a, epsilon);
    }

    /**
     * Checks whether the value of a is greater than or equal to the value of b.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr bool is_greater_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
    {
        return is_less_equal(b, a, epsilon);
    }


    /**
     * Checks whether the specified floating point values are (approximately) equal.
     */
    bool is_equal_ulps(float a, float b, int ulps);

    /**
     * Checks whether the value of a is less than the value of b.
     */
    bool is_less_ulps(float a, float b, int ulps);

    /**
     * Checks whether the value of a is less than or equal to the value of b.
     */
    bool is_less_equal_ulps(float a, float b, int ulps);

    /**
     * Checks whether the value of a is less than the value of b.
     */
    inline bool is_greater_ulps(float a, float b, int ulps)
    {
        return is_less_ulps(b, a, ulps);
    }

    /**
     * Checks whether the value of a is less than or equal to the value of b.
     */
    inline bool is_greater_equal_ulps(float a, float b, int ulps)
    {
        return is_less_equal_ulps(b, a, ulps);
    }


    /**
     * Transforms a given angle in degrees to radians.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr T deg_to_rad(T degrees)
    {
        return degrees * T(0.01745329251);
    }

    /**
     * Transforms a given angle in radians to degrees.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr T rad_to_deg(T radians)
    {
        return radians * T(57.2957795131);
    }


    /**
     * Checks whether the given value is a power of two.
     */
    template <typename T>
        requires(std::is_integral_v<T>)
    constexpr bool is_power_of_2(T x)
    {
        return x > 0 && (x & (x - 1)) == 0;
    }

    /**
     * Returns the highest power of two value that is less than or equal to the
     * specified non-zero value. If the value is 0, 0 is returned.
     */
    template <typename T>
        requires(std::is_integral_v<T>)
    constexpr T prev_power_of_2(T x)
    {
        constexpr T max = T(1) << (sizeof(T) * 8 - 1);
        if (x >= max)
            return max;
        unsigned y = 1;
        while (y <= x)
            y <<= 1;
        return y >> 1;
    }

    /**
     * Returns the lowest power of two value that is greater than or equal to the
     * specified value. If such a value does not exist (regarding integer overflow),
     * 0 is returned.
     */
    template <typename T>
        requires(std::is_integral_v<T>)
    constexpr T next_power_of_2(T x)
    {
        constexpr T max = T(1) << (sizeof(T) * 8 - 1);
        if (x > max)
            return 0;
        unsigned y = 1;
        while (y < x)
            y <<= 1;
        return y;
    }



} // namespace ml7

#endif // ML7_FUNCTIONS_H
