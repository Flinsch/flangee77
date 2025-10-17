#ifndef CL7_BYTESPAN_H
#define CL7_BYTESPAN_H

#include "./detail/bytes.h"

#include <span>



namespace cl7 {



    using byte_span = std::span<std::byte>;



    template <std::ranges::contiguous_range Tcontainer>
        requires(std::is_trivially_copyable_v<typename Tcontainer::value_type>)
    auto make_byte_span(Tcontainer& container)
    {
        assert(detail::is_contiguous_without_gaps(container));
        return byte_span(reinterpret_cast<std::byte*>(container.data()), container.size() * sizeof(typename Tcontainer::value_type));
    }

    template <typename Telement>
        requires(!std::is_reference_v<Telement> && std::is_trivially_copyable_v<Telement>)
    auto make_byte_span(Telement* array, size_t count)
    {
        assert(detail::is_contiguous_without_gaps(array, count));
        return byte_span(reinterpret_cast<std::byte*>(array), count * sizeof(Telement));
    }

    template <typename Tobject>
        requires(!std::is_reference_v<Tobject> && std::is_trivially_copyable_v<Tobject>)
    auto make_byte_span(Tobject* object)
    {
        return byte_span(reinterpret_cast<std::byte*>(object), sizeof(Tobject));
    }



} // namespace cl7

#endif // CL7_BYTESPAN_H
