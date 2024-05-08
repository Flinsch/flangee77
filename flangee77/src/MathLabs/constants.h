#pragma once
#ifndef ML7_CONSTANTS_H
#define ML7_CONSTANTS_H

#include <CoreLabs/root.h>



namespace ml7 {



template <typename T = float>
    requires( std::is_floating_point_v<T> )
struct constants
{

    /**
     * The mathematical constant Pi approximately equal to 3.14159.
     */
    static constexpr T pi = T(3.141592653589793);

    /**
     * The mathematical constant Pi times 2 (sometimes referred to as Tau).
     */
    static constexpr T pi2 = T(6.283185307179586);

    /**
     * The mathematical constant Pi times 0.5.
     */
    static constexpr T pi05 = T(1.5707963267948966);

    /**
     * The mathematical constant e (Euler's number) approximately equal to 2.71828.
     */
    static constexpr T e = T(2.718281828459045);

    /**
     * The natural logarithm of 2, approximately equal to 0.693147.
     */
    static constexpr T ln2 = T(0.6931471805599453);

    /**
     * The square root of 2 (also known as Pythagoras' constant).
     */
    static constexpr T sqrt2 = T(1.4142135623730950);

    /**
     * The square root of 3 (also known as Theodorus' constant).
     */
    static constexpr T sqrt3 = T(1.7320508075688773);

}; // struct constants



} // namespace ml7

#endif // ML7_CONSTANTS_H
