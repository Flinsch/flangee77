#ifndef CL7_BYTEVIEW_H
#define CL7_BYTEVIEW_H

#include "./detail/bytes.h"

#include <span>



namespace cl7 {



    using byte_view = std::span<const std::byte>;



    template <std::ranges::contiguous_range Tcontainer>
        requires(std::is_trivially_copyable_v<typename Tcontainer::value_type>)
    auto make_byte_view(const Tcontainer& container)
    {
        assert(detail::is_contiguous_without_gaps(container));
        return byte_view(reinterpret_cast<const std::byte*>(container.data()), container.size() * sizeof(typename Tcontainer::value_type));
    }

    template <typename Telement>
        requires(!std::is_reference_v<Telement> && std::is_trivially_copyable_v<Telement>)
    auto make_byte_view(const Telement* array, size_t count)
    {
        assert(detail::is_contiguous_without_gaps(array, count));
        return byte_view(reinterpret_cast<const std::byte*>(array), count * sizeof(Telement));
    }

    template <typename Tobject>
        requires(!std::is_reference_v<Tobject> && std::is_trivially_copyable_v<Tobject>)
    auto make_byte_view(const Tobject* object)
    {
        return byte_view(reinterpret_cast<const std::byte*>(object), sizeof(Tobject));
    }



} // namespace cl7

#endif // CL7_BYTEVIEW_H
