#ifndef CL7_UTILITIES_H
#define CL7_UTILITIES_H

#include <CoreLabs/root.h>

#include <functional>
#include <ranges>
#include <vector>



namespace cl7 {



    /**
     * Returns the first parameter that, when interpreted as a boolean, evaluates to
     * a "true-ish" value. Otherwise, the last value is returned.
     */
    template <typename T1, typename T2>
        requires(std::same_as<std::decay_t<T1>, std::decay_t<T2>> && std::is_convertible_v<T1, bool>)
    constexpr auto coalesce(T1&& a, T2&& b)
    {
        using T = std::decay_t<T1>;
        return static_cast<bool>(a) ? static_cast<T>(std::forward<T1>(a)) : static_cast<T>(std::forward<T2>(b));
    }



    /**
     * Returns a pair of values, flipping their order if the boolean flag is true.
     * This is the logical inverse of `flip_unless`.
     */
    template <typename T1, typename T2>
        requires(std::same_as<std::decay_t<T1>, std::decay_t<T2>>)
    constexpr auto flip_if(bool flip_if, T1&& a, T2&& b)
    {
        return flip_if
            ? std::pair<std::decay_t<T2>, std::decay_t<T1>>(std::forward<T2>(b), std::forward<T1>(a))
            : std::pair<std::decay_t<T1>, std::decay_t<T2>>(std::forward<T1>(a), std::forward<T2>(b))
        ;
    }

    /**
     * Returns a pair of values, flipping their order if the boolean flag is false.
     * This is the logical inverse of `flip_if`.
     */
    template <typename T1, typename T2>
        requires(std::same_as<std::decay_t<T1>, std::decay_t<T2>>)
    constexpr auto flip_unless(bool flip_unless, T1&& a, T2&& b)
    {
        return flip_unless
            ? std::pair<std::decay_t<T1>, std::decay_t<T2>>(std::forward<T1>(a), std::forward<T2>(b))
            : std::pair<std::decay_t<T2>, std::decay_t<T1>>(std::forward<T2>(b), std::forward<T1>(a))
        ;
    }



    /**
     * Checks whether two containers contain equal elements, regardless of order.
     * Runs in O(N²) time and uses O(N) auxiliary memory (as a dynamic bitset, so
     * the memory footprint is kept within limits).
     */
    template <std::ranges::range R1, std::ranges::range R2, typename Tpred = std::ranges::equal_to, typename Tproj1 = std::identity, typename Tproj2 = std::identity>
        requires(std::indirectly_comparable<std::ranges::iterator_t<R1>, std::ranges::iterator_t<R2>, Tpred, Tproj1, Tproj2>)
    bool unordered_equal(const R1& a, const R2& b, Tpred pred = {}, Tproj1 proj1 = {}, Tproj2 proj2 = {})
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

            const auto proj_x = std::invoke(proj1, x);

            for (auto it_b = it_b_begin; it_b != it_b_end; ++it_b, ++i)
            {
                if (!matched[i] && std::invoke(pred, proj_x, std::invoke(proj2, *it_b)))
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
