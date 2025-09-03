#include "find_cubic_roots.h"

#include "./find_quadratic_roots.h"

#include <MathLabs/constants.h>

#include <cmath>
#include <limits>
#include <algorithm>



namespace ml7::algorithm {



    /**
     * Finds the real roots of a cubic equation: ax^3 + bx^2 + cx + d = 0
     * Uses Cardano's method for depressed cubic.
     * Returns 1, 2, or 3 values in ascending order.
     */
    cl7::static_vector<float, 3> find_cubic_roots(float a, float b, float c, float d)
    {
        if (std::abs(a) < std::numeric_limits<float>::epsilon())
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
        const float Q = (3.0f * c - b * b) / 9.0f;
        const float R = (b * (9.0f * c - 2.0f * b * b) - 27.0f * d) / 54.0f;

        const float discriminant = Q * Q * Q + R * R;

        if (std::abs(discriminant) < std::numeric_limits<float>::epsilon())
        {
            if (std::abs(R) < std::numeric_limits<float>::epsilon())
            {
                // One root.
                return {-b / 3.0f};
            }

            // Two roots.
            const float u = std::cbrt(R);
            if (u < 0.0f)
                return {2.0f * u - b / 3.0f, -u - b / 3.0f};
            return {-u - b / 3.0f, 2.0f * u - b / 3.0f};
        }

        if (discriminant > 0.0f)
        {
            // One root.
            const float sqrt_d = std::sqrt(discriminant);
            const float S = std::cbrt(R + sqrt_d);
            const float T = std::cbrt(R - sqrt_d);
            return {S + T - b / 3.0f};
        }

        // Three roots.
        assert(Q < 0.0f);
        const float theta = std::acos(R / std::sqrt(-Q * Q * Q));
        const float u = 2.0f * std::sqrt(-Q);
        cl7::static_vector<float, 3> roots = {
            u * std::cos(theta / 3.0f) - b / 3.0f,
            u * std::cos((theta + ml7::constants::pi2) / 3.0f) - b / 3.0f,
            u * std::cos((theta + 2.0f * ml7::constants::pi2) / 3.0f) - b / 3.0f,
        };
        std::ranges::sort(roots);
        return roots;
    }



} // namespace ml7::algorithm
