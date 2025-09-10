#ifndef ML7_FUNCTIONS_H
#define ML7_FUNCTIONS_H

#include <CoreLabs/root.h>

#include <cmath>



namespace ml7 {



    /**
     * Returns the value of base raised to the power of exp.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr T pow(T base, T exp)
    {
        return std::pow(base, exp);
    }

    /**
     * Returns the value of base raised to the power of exp.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    constexpr T pow(T base, int exp)
    {
        return std::pow(base, exp);
    }

    /**
     * Returns the value of base raised to the power of exp.
     */
    template <unsigned exp, typename T>
        requires(std::is_floating_point_v<T>)
    constexpr T pow(T base)
    {
        if constexpr (exp == 0)
            return T(1);
        else
            return base * pow<exp - 1, T>(base);
    }

    /**
     * Returns the value of base raised to the power of exp.
     */
    template <auto base, unsigned exp, typename T = decltype(base)>
        requires(std::is_arithmetic_v<T>)
    constexpr T pow()
    {
        if constexpr (exp == 0)
            return T(1);
        else
            return base * pow<base, exp - 1, T>();
    }

    /**
     * Returns the given value squared.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T square(T x)
    {
        return x * x;
    }

    /**
     * Returns the given value cubed.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T cube(T x)
    {
        return x * x * x;
    }

    /**
     * Returns the square root of the given value.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T sqrt(T x)
    {
        return std::sqrt(x);
    }

    /**
     * Returns the signed square root of the given value.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T ssqrt(T x)
    {
        if (x < T(0))
            return -std::sqrt(-x);
        return std::sqrt(x);
    }

    /**
     * Returns the cube root of the given value.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T cbrt(T x)
    {
        return std::cbrt(x);
    }



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
    constexpr T step(T x, T edge)
    {
        return x >= edge ? T(1) : T(0);
    }

    /**
     * Returns 1 if x >= edge, 0 otherwise.
     */
    template <auto edge, typename T>
        requires(std::is_arithmetic_v<T> && std::is_convertible_v<decltype(edge), T>)
    constexpr T step(T x)
    {
        return x >= static_cast<T>(edge) ? T(1) : T(0);
    }

    /**
     * Returns 1 if x >= 0, 0 otherwise.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T step(T x)
    {
        return x >= T(0) ? T(1) : T(0);
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
     * Returns 0 if x is less than min, 1 if x is greater than max, and a value
     * between 0 and 1 otherwise using a Hermite polynomial.
     */
    template <auto min, auto max, typename T>
        requires(std::is_arithmetic_v<T> && std::is_convertible_v<decltype(min), T> && std::is_convertible_v<decltype(max), T>)
    constexpr T smoothstep(T x)
    {
        if constexpr (static_cast<T>(min) == static_cast<T>(max))
            return step<min>(x);
        if (x <= static_cast<T>(min)) return T(0);
        if (x >= static_cast<T>(max)) return T(1);
        x = (x - static_cast<T>(min)) / (static_cast<T>(max) - static_cast<T>(min));
        return x * x * (3.0f - 2.0f * x);
    }

    /**
     * Returns 0 if x is less than 0, 1 if x is greater than 1, and a value between
     * 0 and 1 otherwise using a Hermite polynomial.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T smoothstep(T x)
    {
        if (x <= T(0)) return T(0);
        if (x >= T(1)) return T(1);
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
     * Clamps the given value to lie within the range [min; max].
     */
    template <auto min, auto max, typename T>
        requires(std::is_arithmetic_v<T> && std::is_convertible_v<decltype(min), T> && std::is_convertible_v<decltype(max), T>)
    constexpr T clamp(T x)
    {
        if (x <= static_cast<T>(min)) return static_cast<T>(min);
        if (x >= static_cast<T>(max)) return static_cast<T>(max);
        return x;
    }

    /**
     * Clamps the given value to lie within the range [0; 1].
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T clamp01(T x)
    {
        if (x <= T(0)) return T(0);
        if (x >= T(1)) return T(1);
        return x;
    }


    /**
     * General ramp function: customizable slope and starting point.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T ramp(T x, T start, T slope = T(1))
    {
        if (x <= start)
            return T(0);
        return (x - start) * slope;
    }

    /**
     * General ramp function: customizable slope and starting point.
     */
    template <auto start, auto slope = 1, typename T>
        requires(std::is_arithmetic_v<T> && std::is_convertible_v<decltype(start), T> && std::is_convertible_v<decltype(slope), T>)
    constexpr T ramp(T x)
    {
        if (x <= start)
            return T(0);
        return (x - start) * slope;
    }

    /**
     * Unit ramp function: returns x if x >= 0, 0 otherwise.
     */
    template <typename T>
        requires(std::is_arithmetic_v<T>)
    constexpr T ramp(T x)
    {
        if (x <= T(0))
            return T(0);
        return x;
    }


    /**
     * Rounds the given value to the specified number of decimal places.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    T round(T x, unsigned num_decimals)
    {
        const T p = std::pow(T(10.0), static_cast<T>(num_decimals));
        return std::floor(x * p + T(0.5)) / p;
    }

    /**
     * Rounds the given value to the specified number of decimal places.
     */
    template <unsigned num_decimals, typename T>
        requires(std::is_floating_point_v<T>)
    T round(T x)
    {
        constexpr T p = ml7::pow<T(10.0), num_decimals>();
        return std::floor(x * p + T(0.5)) / p;
    }

    /**
     * Rounds the given value to the closest integer value.
     */
    template <typename T>
        requires(std::is_floating_point_v<T>)
    T round(T x)
    {
        return std::floor(x + T(0.5));
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
    constexpr bool is_one_squared(T x, T epsilon = std::numeric_limits<T>::epsilon())
    {
        //  x  =  1+epsilon
        // x^2 = (1+epsilon)^2
        // x^2 = 1^2 + 2*epsilon + epsilon^2
        // x^2 =  1  + 2*epsilon
        // epsilon^2 is omitted because it is far below floating point precision.
        return std::abs(x - T(1)) <= T(2) * epsilon;
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
        requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
    constexpr bool is_power_of_two(T x)
    {
        return x > 0 && (x & (x - 1)) == 0;
    }

    /**
     * Returns the highest power of two value that is less than or equal to the
     * specified non-zero value. If the value is 0, 0 is returned.
     */
    template <typename T>
        requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
    constexpr T prev_power_of_two(T x)
    {
        constexpr T max = T(1) << (sizeof(T) * 8 - 1);
        if (x >= max)
            return max;
        T y = T(1);
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
        requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
    constexpr T next_power_of_two(T x)
    {
        constexpr T max = T(1) << (sizeof(T) * 8 - 1);
        if (x > max)
            return 0;
        T y = T(1);
        while (y < x)
            y <<= 1;
        return y;
    }

    /**
     * Determines the number of decimal digits of an unsigned integer value.
     */
    template <typename U = unsigned, typename T>
        requires(std::is_integral_v<T> && std::is_unsigned_v<T>)
    constexpr U decimal_digits(T x)
    {
        constexpr T _10 = T(10);
        U n = U(1);
        for (; x >= _10; ++n)
            x /= _10;
        return n;
    }



} // namespace ml7

#endif // ML7_FUNCTIONS_H
