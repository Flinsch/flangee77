#ifndef ML7_ALGORITHM_MERGENEARBY_H
#define ML7_ALGORITHM_MERGENEARBY_H

#include <CoreLabs/root.h>

#include <vector>
#include <limits>
#include <algorithm>



namespace ml7::algorithm {



    /**
     * Merges nearby floating-point values in a vector based on a tolerance. The
     * values are successively merged based on the smallest distance between pairs
     * until no pair meets the tolerance criterion.
     */
    inline
    void merge_nearby(std::vector<float>& values, float tolerance)
    {
        if (values.size() <= 1 || tolerance < 0.0f)
            return;

        std::ranges::sort(values);

        while (values.size() > 1)
        {
            constexpr float max_distance = std::numeric_limits<float>::max();
            float smallest_distance = max_distance;
            size_t base_index = values.size();

            for (size_t i = 0; i + 1 < values.size(); ++i)
            {
                const float distance = values[i + 1] - values[i]; // std::abs not necessary since the values are sorted in ascending order.
                assert(distance >= 0.0f);
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
            values[base_index] *= 0.5f;
            values.erase(values.begin() + static_cast<ptrdiff_t>(base_index) + 1);
        } // while
    }



} // namespace ml7::algorithm

#endif // ML7_ALGORITHM_MERGENEARBY_H
