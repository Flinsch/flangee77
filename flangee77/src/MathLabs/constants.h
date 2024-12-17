#pragma once
#ifndef ML7_CONSTANTS_H
#define ML7_CONSTANTS_H

#include <CoreLabs/root.h>

#include <numbers>



namespace ml7 {



template <typename T = float>
    requires(std::is_floating_point_v<T>)
struct constants_
{

    /**
     * The mathematical constant Pi, approximately equal to 3.14159.
     */
    static constexpr T pi = std::numbers::pi_v<T>;

    /**
     * The mathematical constant Pi times 2 (sometimes referred to as Tau).
     */
    static constexpr T pi2 = T(2.0f) * pi;

    /**
     * The mathematical constant Pi times 0.5.
     */
    static constexpr T pi05 = T(0.5f) * pi;

    /**
     * The mathematical constant e (Euler's number), approximately equal to 2.71828.
     */
    static constexpr T e = std::numbers::e_v<T>;

    /**
     * The natural logarithm of 2, approximately equal to 0.693147.
     */
    static constexpr T ln2 = std::numbers::ln2_v<T>;

    /**
     * The square root of 2 (also known as Pythagoras' constant).
     */
    static constexpr T sqrt2 = std::numbers::sqrt2_v<T>;

    /**
     * The square root of 3 (also known as Theodorus' constant).
     */
    static constexpr T sqrt3 = std::numbers::sqrt3_v<T>;

}; // struct constants



using constants = constants_<float>;



} // namespace ml7

#endif // ML7_CONSTANTS_H
