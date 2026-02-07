#ifndef CL7_BYTEVECTOR_H
#define CL7_BYTEVECTOR_H

#include <CoreLabs/root.h>

#include <vector>



namespace cl7 {



    using byte_vector = std::vector<std::byte>;



    template <typename... T>
    byte_vector make_bytes(T&&... args) noexcept
    {
        return {static_cast<std::byte>(std::forward<T>(args))...};
    }

    template <std::ranges::contiguous_range Tcontainer>
        requires(std::is_trivially_copyable_v<typename Tcontainer::value_type> && sizeof(typename Tcontainer::value_type) == sizeof(std::byte))
    byte_vector to_bytes(const Tcontainer& container)
    {
        return byte_vector(container.begin(), container.end());
    }



} // namespace cl7

#endif // CL7_BYTEVECTOR_H
