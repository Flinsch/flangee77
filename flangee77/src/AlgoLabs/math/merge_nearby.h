#ifndef AL7_MATH_MERGENEARBY_H
#define AL7_MATH_MERGENEARBY_H

#include <CoreLabs/root.h>

#include <vector>
#include <limits>
#include <algorithm>



namespace al7::math {



    /**
     * Merges nearby floating-point values in a vector based on a tolerance. The
     * values are successively merged based on the smallest distance between pairs
     * until no pair meets the tolerance criterion.
     */
    template <std::floating_point T>
    void merge_nearby(std::vector<T>& values, T tolerance)
    {
        if (values.size() <= 1 || tolerance < T{0})
            return;

        std::ranges::sort(values);

        while (values.size() > 1)
        {
            constexpr T max_distance = std::numeric_limits<T>::max();
            T smallest_distance = max_distance;
            size_t base_index = values.size();

            for (size_t i = 0; i + 1 < values.size(); ++i)
            {
                const T distance = values[i + 1] - values[i]; // std::abs not necessary since the values are sorted in ascending order.
                assert(distance >= T{0});
                if (distance < smallest_distance)
                {
                    smallest_distance = distance;
                    base_index = i;
                }
            }

            if (smallest_distance > tolerance)
                break;

            assert(base_index < values.size());
            values[base_index] += values[base_index + 1];
            values[base_index] *= T{0.5};
            values.erase(values.begin() + static_cast<ptrdiff_t>(base_index) + 1);
        } // while
    }



    //extern template void merge_nearby(std::vector<float>& values, float tolerance);
    //extern template void merge_nearby(std::vector<double>& values, double tolerance);



} // namespace al7::math

#endif // AL7_MATH_MERGENEARBY_H
