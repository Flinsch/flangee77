#ifndef AL7_MATH_FINDCUBICROOTS_H
#define AL7_MATH_FINDCUBICROOTS_H

#include "./find_quadratic_roots.h"

#include <CoreLabs/static_vector.h>

#include <cmath>
#include <limits>
#include <numbers>
#include <algorithm>



namespace al7::math {



    /**
     * Finds the real roots of a cubic equation: ax^3 + bx^2 + cx + d = 0
     * Uses Cardano's method for depressed cubic.
     * Returns 1, 2, or 3 values in ascending order.
     */
    template <std::floating_point T>
    cl7::static_vector<T, 3> find_cubic_roots(T a, T b, T c, T d)
    {
        // Relative (not absolute) tolerance, for the same reason as in
        // find_quadratic_roots: `a`/`b`/`c`/`d` can be of any magnitude depending on
        // the caller's coordinate scale, so a fixed absolute epsilon is meaningless.
        // This matters even more here, since a near-zero `a` that slips through would
        // then be divided into b/c/d below, corrupting every coefficient used from
        // that point on (not just one division, as in the quadratic case).
        if (std::abs(a) <= std::numeric_limits<T>::epsilon() * std::abs(b))
        {
            // Degenerate to quadratic.
            const auto roots = find_quadratic_roots(b, c, d);
            return {roots.begin(), roots.end()};
        }

        // Normalize coefficients.
        b /= a;
        c /= a;
        d /= a;

        // Depress the cubic equation.
        const T Q = (T{3} * c - b * b) / T{9};
        const T R = (b * (T{9} * c - T{2} * b * b) - T{27} * d) / T{54};

        const T discriminant = Q * Q * Q + R * R;

        // Same relative-tolerance reasoning as above: scale by the magnitude of the
        // terms making up `discriminant`/`R` themselves, rather than a fixed absolute
        // epsilon that's meaningless once Q/R are outside the unit range.
        if (std::abs(discriminant) <= std::numeric_limits<T>::epsilon() * (R * R + std::abs(Q * Q * Q)))
        {
            if (std::abs(R) <= std::numeric_limits<T>::epsilon() * std::abs(b))
            {
                // One root.
                return {-b / T{3}};
            }

            // Two roots.
            const T u = std::cbrt(R);
            if (u < T{0})
                return {T{2} * u - b / T{3}, -u - b / T{3}};
            return {-u - b / T{3}, T{2} * u - b / T{3}};
        }

        if (discriminant > T{0})
        {
            // One root.
            const T sqrt_d = std::sqrt(discriminant);
            const T s = std::cbrt(R + sqrt_d);
            const T t = std::cbrt(R - sqrt_d);
            return {s + t - b / T{3}};
        }

        // Three roots.
        assert(Q < T{0});
        const T theta = std::acos(R / std::sqrt(-Q * Q * Q));
        const T u = T{2} * std::sqrt(-Q);
        cl7::static_vector<T, 3> roots = {
            u * std::cos(theta / T{3}) - b / T{3},
            u * std::cos((theta + T{2} * std::numbers::pi_v<T>) / T{3}) - b / T{3},
            u * std::cos((theta + T{4} * std::numbers::pi_v<T>) / T{3}) - b / T{3},
        };
        std::ranges::sort(roots);
        return roots;
    }



    //extern template cl7::static_vector<float, 3> find_cubic_roots(float a, float b, float c, float d);
    //extern template cl7::static_vector<double, 3> find_cubic_roots(double a, double b, double c, double d);



} // namespace al7::math

#endif // AL7_MATH_FINDQUADRATICROOTS_H
