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

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T> && std::is_trivially_copyable_v<T> && sizeof(T) == sizeof(std::byte))
    auto make_byte_view(const T* obj)
    {
        return byte_view(reinterpret_cast<const std::byte*>(obj), 1);
    }



    template <class T>
        requires(requires {
            typename T::value_type;
        } && requires (const T& v) {
            { v.data() } -> std::same_as<const typename T::value_type*>;
            { v.size() } -> std::convertible_to<size_t>;
        })
    auto make_byte_view_contiguous(const T& v)
    {
        if (v.size() > 1) {
            const auto* b0 = reinterpret_cast<const std::byte*>(&v[0]);
            const auto* b1 = reinterpret_cast<const std::byte*>(&v[1]);
            assert(b1 == b0 + sizeof(T));
        }
        return byte_view(reinterpret_cast<const std::byte*>(v.data()), v.size() * sizeof(typename T::value_type));
    }

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T>)
    auto make_byte_view_contiguous(const T* array, size_t count)
    {
        if (count > 1) {
            const auto* b0 = reinterpret_cast<const std::byte*>(&array[0]);
            const auto* b1 = reinterpret_cast<const std::byte*>(&array[1]);
            assert(b1 == b0 + sizeof(T));
        }
        return byte_view(reinterpret_cast<const std::byte*>(array), count * sizeof(T));
    }

    template <typename T>
        requires(std::is_same_v<std::remove_cvref_t<T>, T> && std::is_trivially_copyable_v<T>)
    auto make_byte_view_contiguous(const T* obj)
    {
        return byte_view(reinterpret_cast<const std::byte*>(obj), sizeof(T));
    }



} // namespace cl7

#endif // CL7_BYTEVIEW_H
