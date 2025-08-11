#include "find_quadratic_roots.h"

#include <cmath>
#include <limits>



namespace ml7::algorithm {



    /**
     * Finds the real roots of a quadratic equation: ax^2 + bx + c = 0
     * Returns 0, 1, or 2 values in ascending order.
     */
    cl7::static_vector<float, 2> find_quadratic_roots(float a, float b, float c)
    {
        if (std::abs(a) < std::numeric_limits<float>::epsilon())
        {
            // Degenerate to linear.
            if (std::abs(b) < std::numeric_limits<float>::epsilon())
                return {}; // No roots.

            // One root.
            return {-c / b};
        }

        const float _2a = 2.0f * a;
        const float discriminant = b * b - 2.0f * _2a * c;

        if (std::abs(discriminant) < std::numeric_limits<float>::epsilon())
        {
            // One root.
            return {-b / _2a};
        }

        if (discriminant > 0.0f)
        {
            // Two roots.
            float sqrt_d = std::sqrt(discriminant);
            if (_2a < 0.0f) sqrt_d = -sqrt_d; // Just to maintain an ascending order.
            return {
                (-b - sqrt_d) / _2a,
                (-b + sqrt_d) / _2a,
            };
        }

        // No roots.
        return {};
    }



} // namespace ml7::algorithm
