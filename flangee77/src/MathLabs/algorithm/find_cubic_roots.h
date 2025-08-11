#ifndef ML7_ALGORITHM_FINDCUBICROOTS_H
#define ML7_ALGORITHM_FINDCUBICROOTS_H

#include <CoreLabs/static_vector.h>



namespace ml7::algorithm {



    /**
     * Finds the real roots of a cubic equation: ax^3 + bx^2 + cx + d = 0
     * Uses Cardano's method for depressed cubic.
     * Returns 1, 2, or 3 values in ascending order.
     */
    cl7::static_vector<float, 3> find_cubic_roots(float a, float b, float c, float d);



} // namespace ml7::algorithm

#endif // ML7_ALGORITHM_FINDQUADRATICROOTS_H
