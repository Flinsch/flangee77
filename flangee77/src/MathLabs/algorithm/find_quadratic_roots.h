#ifndef ML7_ALGORITHM_FINDQUADRATICROOTS_H
#define ML7_ALGORITHM_FINDQUADRATICROOTS_H

#include <CoreLabs/static_vector.h>



namespace ml7::algorithm {



    /**
     * Finds the real roots of a quadratic equation: ax^2 + bx + c = 0
     * Returns 0, 1, or 2 values in ascending order.
     */
    cl7::static_vector<float, 2> find_quadratic_roots(float a, float b, float c);



} // namespace ml7::algorithm

#endif // ML7_ALGORITHM_FINDQUADRATICROOTS_H
