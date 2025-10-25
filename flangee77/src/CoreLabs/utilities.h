#ifndef CL7_UTILITIES_H
#define CL7_UTILITIES_H

#include <CoreLabs/root.h>

#include <functional>
#include <ranges>
#include <vector>



namespace cl7 {



    /**
     * Returns the first parameter that, when interpreted as a Boolean, evaluates to
     * a "true-ish" value. Otherwise, the last value is returned.
     */
    template <typename T1, typename T2>
        requires(std::same_as<std::decay_t<T1>, std::decay_t<T2>> && std::is_convertible_v<T1, bool>)
    auto coalesce(T1&& a, T2&& b)
    {
        using T = std::decay_t<T1>;
        return static_cast<bool>(a) ? static_cast<T>(std::forward<T1>(a)) : static_cast<T>(std::forward<T2>(b));
    }


    /**
     * Checks whether two containers contain equal elements, regardless of order.
     * Runs in O(N²) time and uses O(N) auxiliary memory (as a dynamic bitset, so
     * the memory footprint is kept within limits).
     */
    template <std::ranges::range R1, std::ranges::range R2, typename Tproj = std::identity>
        requires(
            std::same_as<
                std::invoke_result_t<Tproj&, std::ranges::range_value_t<R1>>,
                std::invoke_result_t<Tproj&, std::ranges::range_value_t<R2>>> &&
            std::equality_comparable<
                std::invoke_result_t<Tproj&, std::ranges::range_value_t<R1>>>)
    bool unordered_equal(const R1& a, const R2& b, Tproj proj = {})
    {
        if (std::ranges::size(a) != std::ranges::size(b))
            return false;

        const auto size = std::ranges::size(b);
        std::vector<bool> matched(size, false);

        const auto it_b_begin = std::ranges::begin(b);
        const auto it_b_end = std::ranges::end(b);

        for (const auto& x : a)
        {
            bool found = false;
            size_t i = 0;

            const auto proj_x = std::invoke(proj, x);

            for (auto it_b = it_b_begin; it_b != it_b_end; ++it_b, ++i)
            {
                if (!matched[i] && proj_x == std::invoke(proj, *it_b))
                {
                    matched[i] = true;
                    found = true;
                    break;
                }
            }

            if (!found)
                return false;
        }

        return true;
    }



} // namespace cl7

#endif // CL7_UTILITIES_H
