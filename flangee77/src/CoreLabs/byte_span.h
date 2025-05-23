#ifndef CL7_BYTESPAN_H
#define CL7_BYTESPAN_H

#include <CoreLabs/root.h>

#include <span>



namespace cl7 {



    using byte_span = std::span<std::byte>;



    template <class T>
        requires(requires {
            typename T::value_type;
        } && requires (T& v) {
            { v.data() } -> std::same_as<typename T::value_type*>;
            { v.size() } -> std::convertible_to<size_t>;
        } && sizeof(typename T::value_type) == sizeof(std::byte))
    auto make_byte_span(T& v)
    {
        return byte_span(reinterpret_cast<std::byte*>(v.data()), v.size());
    }

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T> && std::is_trivially_copyable_v<T>)
    auto make_byte_span(T* obj)
    {
        return byte_span(reinterpret_cast<std::byte*>(obj), sizeof(T));
    }

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T> && sizeof(T) == sizeof(std::byte))
    auto make_byte_span(T* data, size_t size)
    {
        return byte_span(reinterpret_cast<std::byte*>(data), size);
    }



} // namespace cl7

#endif // CL7_BYTESPAN_H
