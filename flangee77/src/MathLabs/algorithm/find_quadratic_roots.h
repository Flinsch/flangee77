#ifndef ML7_ALGORITHM_FINDQUADRATICROOTS_H
#define ML7_ALGORITHM_FINDQUADRATICROOTS_H

#include <CoreLabs/static_vector.h>

#include <cmath>
#include <limits>



namespace ml7::algorithm {



    /**
     * Finds the real roots of a quadratic equation: ax^2 + bx + c = 0
     * Returns 0, 1, or 2 values in ascending order.
     */
    template <std::floating_point T>
    cl7::static_vector<T, 2> find_quadratic_roots(T a, T b, T c)
    {
        if (std::abs(a) < std::numeric_limits<T>::epsilon())
        {
            // Degenerate to linear.
            if (std::abs(b) < std::numeric_limits<T>::epsilon())
                return {}; // No roots.

            // One root.
            return {-c / b};
        }

        const T _2a = T{2} * a;
        const T discriminant = b * b - T{2} * _2a * c;

        if (std::abs(discriminant) < std::numeric_limits<T>::epsilon())
        {
            // One root.
            return {-b / _2a};
        }

        if (discriminant > T{0})
        {
            // Two roots.
            T sqrt_d = std::sqrt(discriminant);
            if (_2a < T{0}) sqrt_d = -sqrt_d; // Just to maintain an ascending order.
            return {
                (-b - sqrt_d) / _2a,
                (-b + sqrt_d) / _2a,
            };
        }

        // No roots.
        return {};
    }



    //extern template cl7::static_vector<float, 2> find_quadratic_roots(float a, float b, float c);
    //extern template cl7::static_vector<double, 2> find_quadratic_roots(double a, double b, double c);



} // namespace ml7::algorithm

#endif // ML7_ALGORITHM_FINDQUADRATICROOTS_H
