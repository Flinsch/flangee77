#ifndef ML7_ALGORITHM_FINDROOTSBYSIGNCHANGEANDNEWTON_H
#define ML7_ALGORITHM_FINDROOTSBYSIGNCHANGEANDNEWTON_H

#include "./merge_nearby.h"

#include <vector>
#include <cmath>



namespace ml7::algorithm {



    /**
     * Finds the real roots of a continuous, differentiable function within a
     * specified range.
     *
     * The algorithm first detects sign changes in the function over the given
     * interval by subdividing the range into equal segments. Then, for each
     * subinterval showing a sign change (indicating a likely root), it refines the
     * root estimate using the Newton-Raphson method.
     */
    template <typename Tfunction, typename Tderivative>
        requires(std::invocable<Tfunction, float> && std::invocable<Tderivative, float> && requires(const Tfunction& f, const Tderivative& df)
    {
        { f(0.0f) } -> std::convertible_to<float>;
        { df(0.0f) } -> std::convertible_to<float>;
    })
    std::vector<float> find_roots_by_sign_change_and_newton(const Tfunction& f, const Tderivative& df, float lower_bound, float upper_bound, unsigned subdivisions = 1000, unsigned max_iterations = 20, float convergence_tolerance = 1e-5f, float merge_tolerance = 1e-5f)
    {
        std::vector<float> roots;

        if (upper_bound <= lower_bound || subdivisions == 0)
            return roots;

        const float step = (upper_bound - lower_bound) / static_cast<float>(subdivisions);

        // Step 1: Sign change detection
        for (unsigned k = 0; k < subdivisions; ++k)
        {
            const float x0 = lower_bound + static_cast<float>(k) * step;
            const float x1 = x0 + step;
            const float f0 = f(x0);
            const float f1 = f(x1);

            float x; // This will be our root value, if found, later.

            // Sign change detected?
            const bool sign_change = std::signbit(f0) != std::signbit(f1);
            if (sign_change)
            {
                // Take the midpoint as initial guess.
                //x = (x0 + x1) * 0.5f;
                // No, interpret this subdivision as a
                // straight line and take its 0-intercept.
                // y = ax + b
                // a = (y2 - y1) / (x2 - x1)
                // b = y1 - a * x1
                // x = -b / a
                //   = x1 - y1 * (x2 - x1) / (y2 - y1)
                x = x0 - f0 * step / (f1 - f0);
                // It's actually not that much more expensive and is definitely
                // worth the increased (starting) precision we get from it.
            }

            // Otherwise, see if either point is close to 0
            // and then take that one as initial guess.
            else if (std::abs(f0) < std::numeric_limits<float>::epsilon())
                x = x0;
            else if (std::abs(f1) < std::numeric_limits<float>::epsilon())
                x = x1;

            // No? Then there is no candidate here.
            else
                continue;

            // Step 2: Newton-Raphson refinement
            bool converged = false;
            for (unsigned i = 0; i < max_iterations; ++i)
            {
                const float fx = f(x);
                const float dfx = df(x);

                // Avoid division by near-zero derivative.
                if (std::abs(dfx) < std::numeric_limits<float>::epsilon())
                {
                    // But wait! What if we found a root right here?
                    // But then we have to make sure that the subdivision is
                    // not a straight line that lies "exactly" on the x-axis.
                    // Otherwise we might get too many apparent roots
                    // for a whole series (possibly all) subdivisions.
                    if (std::abs(fx) <= std::numeric_limits<float>::min() && std::max(std::abs(df(x0)), std::abs(df(x1))) >= std::numeric_limits<float>::min())
                        converged = true;
                    break;
                }

                const float dx = fx / dfx;
                x -= dx;

                if (std::abs(dx) <= convergence_tolerance)
                {
                    converged = true;
                    break;
                }
            } // for each Newton-Raphson iteration

            // If there was a sign change,
            // we've definitely found a root.
            // Otherwise: converged?
            if (!sign_change && !converged)
                continue;

            // Out of bounds? Not a candidate.
            if (x < lower_bound || x > upper_bound)
                continue;

            // There we go.
            roots.push_back(x);

        } // for each subdivision

        // Merge nearby roots.
        merge_nearby(roots, merge_tolerance);

        return roots;
    }



} // namespace ml7::algorithm

#endif // ML7_ALGORITHM_FINDROOTSBYSIGNCHANGEANDNEWTON_H
