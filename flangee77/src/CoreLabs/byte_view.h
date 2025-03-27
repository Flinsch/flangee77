#ifndef CL7_BYTEVIEW_H
#define CL7_BYTEVIEW_H

#include <CoreLabs/root.h>

#include <span>



namespace cl7 {



    using byte_view = std::span<const std::byte>;



    template <class T>
        requires(requires {
            typename T::value_type;
        } && requires (const T& v) {
            { v.data() } -> std::same_as<const typename T::value_type*>;
            { v.size() } -> std::convertible_to<size_t>;
        } && sizeof(typename T::value_type) == sizeof(std::byte))
    auto make_byte_view(const T& v)
    {
        return byte_view(reinterpret_cast<const std::byte*>(v.data()), v.size());
    }

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T> && sizeof(T) == sizeof(std::byte))
    auto make_byte_view(const T* data, size_t size)
    {
        return byte_view(reinterpret_cast<const std::byte*>(data), size);
    }



} // namespace cl7

#endif // CL7_BYTEVIEW_H
