#ifndef ML7_OPS_H
#define ML7_OPS_H

#include <CoreLabs/root.h>

#include <cmath>



namespace ml7::ops {



    /**
     * Clamps the given value to lie within the range [min; max].
     */
    template <auto min, auto max>
    struct clamp
    {
        template <typename T>
            requires(std::is_arithmetic_v<T> && std::is_convertible_v<decltype(min), T> && std::is_convertible_v<decltype(max), T>)
        constexpr T operator()(T x)
        {
            if (x <= static_cast<T>(min)) return static_cast<T>(min);
            if (x >= static_cast<T>(max)) return static_cast<T>(max);
            return x;
        }
    };

    /**
     * Clamps the given value to lie within the range [0; 1].
     */
    struct clamp01
    {
        template <typename T>
            requires(std::is_arithmetic_v<T>)
        constexpr T operator()(T x)
        {
            if (x <= T(0)) return T(0);
            if (x >= T(1)) return T(1);
            return x;
        }
    };


    /**
     * General ramp function: customizable slope and starting point.
     */
    template <auto start = 0, auto slope = 1>
    struct ramp
    {
        template <typename T>
            requires(std::is_arithmetic_v<T> && std::is_convertible_v<decltype(start), T> && std::is_convertible_v<decltype(slope), T>)
        constexpr T operator()(T x)
        {
            if (x <= start)
                return T(0);
            return (x - start) * slope;
        }
    };

    /**
     * Unit ramp function: returns x if x >= 0, 0 otherwise.
     */
    template <>
    struct ramp<0, 1>
    {
        template <typename T>
            requires(std::is_arithmetic_v<T>)
        constexpr T operator()(T x)
        {
            if (x <= T(0))
                return T(0);
            return x;
        }
    };



} // namespace ml7::ops

#endif // ML7_OPS_H
