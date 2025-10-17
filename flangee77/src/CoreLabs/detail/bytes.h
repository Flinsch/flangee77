#ifndef CL7_DETAIL_BYTES_H
#define CL7_DETAIL_BYTES_H

#include <CoreLabs/root.h>



namespace cl7::detail {



    /**
     * Checks whether elements of a raw C-style array are laid out contiguously in
     * memory (no gaps between consecutive elements).
     *
     * This function is somewhat redundant, since traditional C-style arrays are
     * contiguous without any padding between elements in C++ anyway.
     */
    template <typename Telement>
        requires(!std::is_reference_v<Telement>)
    bool is_contiguous_without_gaps(const Telement* array, size_t count)
    {
        static_assert(std::is_trivially_copyable_v<Telement>);

        if (array == nullptr)
            return count == 0;

        if (count <= 1)
            return true;

        const auto* b0 = reinterpret_cast<const std::byte*>(&array[0]);
        const auto* bn = reinterpret_cast<const std::byte*>(&array[count - 1]);
        return bn == b0 + (count - 1) * sizeof(Telement);
    }

    /**
     * Checks whether elements of a standard contiguous container (e.g, vector,
     * array, span, etc.) are laid out contiguously in memory (no gaps between
     * consecutive elements).
     *
     * This function is somewhat redundant, since standard containers (like vector)
     * are guaranteed to be contiguous without any padding between elements anyway.
     * It is more useful for custom containers or spans.
     */
    template <std::ranges::contiguous_range Tcontainer>
    bool is_contiguous_without_gaps(const Tcontainer& container)
    {
        using Telement = typename Tcontainer::value_type;

        static_assert(std::is_trivially_copyable_v<Telement>);

        size_t count = container.size();

        if (count <= 1)
            return true;

        const auto* b0 = reinterpret_cast<const std::byte*>(&container[0]);
        const auto* bn = reinterpret_cast<const std::byte*>(&container[count - 1]);
        return bn == b0 + (count - 1) * sizeof(Telement);
    }



} // namespace cl7::detail

#endif // CL7_DETAIL_BYTES_H
