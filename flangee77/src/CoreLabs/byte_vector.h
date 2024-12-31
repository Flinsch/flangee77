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



} // namespace cl7

#endif // CL7_BYTEVECTOR_H
