#ifndef CL7_TEXT_METRICS_H
#define CL7_TEXT_METRICS_H

#include <CoreLabs/string.h>

#include <algorithm>



namespace cl7::text::metrics {



    /**
     * Calculates the Levenshtein distance between two strings. The difference is
     * calculated in terms of characters (code units), not in terms of (Unicode)
     * code points.
     */
    template <class Tstring_or_view1, class Tstring_or_view2>
        requires(is_any_string_or_view_v<Tstring_or_view1> && is_any_string_or_view_v<Tstring_or_view2>)
    size_t levenshtein(Tstring_or_view1&& s1, Tstring_or_view2&& s2)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view1>::value_type> sv1{std::forward<Tstring_or_view1>(s1)};
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view2>::value_type> sv2{std::forward<Tstring_or_view2>(s2)};

        const size_t size1 = sv1.size();
        const size_t size2 = sv2.size();
        if (size1 == 0) return size2;
        if (size2 == 0) return size1;

        const size_t len = size2 + 1;
        const size_t total_size = (size1 + 1) * (size2 + 1);

        auto mat = std::make_unique_for_overwrite<size_t[]>(total_size);

        for (size_t i = 0; i <= size1; ++i)
            mat[i * len + 0] = i;
        for (size_t j = 0; j <= size2; ++j)
            mat[0 * len + j] = j;

        for (size_t i = 1; i <= size1; ++i)
        {
            for (size_t j = 1; j <= size2; ++j)
            {
                const size_t cost = sv1[i - 1] == sv2[j - 1] ? 0 : 1;
                mat[i * len + j] = (std::min)({
                    mat[(i - 1) * len + (j)] + 1,
                    mat[(i) * len + (j - 1)] + 1,
                    mat[(i - 1) * len + (j - 1)] + cost,
                });
            } // for ... j
        } // for ... i

        return mat[size1 * len + size2];
    }

    /**
     * Calculates a normalized Levenshtein distance between two strings on a single
     * scale from 0 ("identical") to 1 ("nothing in common"). The difference is
     * calculated in terms of characters (code units), not in terms of (Unicode)
     * code points.
     */
    template <std::floating_point Tfloat = float, class Tstring_or_view1, class Tstring_or_view2>
        requires(is_any_string_or_view_v<Tstring_or_view1> && is_any_string_or_view_v<Tstring_or_view2>)
    Tfloat levenshtein_normalized(Tstring_or_view1&& s1, Tstring_or_view2&& s2)
    {
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view1>::value_type> sv1{std::forward<Tstring_or_view1>(s1)};
        std::basic_string_view<typename std::remove_cvref_t<Tstring_or_view2>::value_type> sv2{std::forward<Tstring_or_view2>(s2)};

        const size_t size = (std::max)(sv1.size(), sv2.size());
        if (size == 0) return 0.0f;
        return static_cast<Tfloat>(levenshtein(sv1, sv2)) / static_cast<Tfloat>(size);
    }



} // namespace cl7::text::metrics

#endif // CL7_TEXT_METRICS_H
