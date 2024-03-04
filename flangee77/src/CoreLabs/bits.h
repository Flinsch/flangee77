#pragma once
#ifndef CL7_BITS_H
#define CL7_BITS_H

#include <CoreLabs/root.h>

#include <bit>



namespace cl7 {
namespace bits {



    /**
     * Combines two hash values.
     */
    inline void hash_combine(size_t& hash, size_t value)
    {
        // Implementation "borrowed" from the Boost library.
        // I don't want to say whether the implementation is really good or bad.
        // There are enough discussions about it, for example here:
        // https://stackoverflow.com/questions/35985960/c-why-is-boosthash-combine-the-best-way-to-combine-hash-values
        // But I think it's a good start, at least as a general implementation.
        // For specific cases, individual logic should then be introduced where
        // appropriate; there's nothing wrong with that.
        hash ^= value + 0x9e3779b9 + (hash << 6) + (hash >> 2);
    }



    /**
     * Reverses the bytes in the given (integer) value.
     */
    template <typename T>
    T swap_bytes(T value)
    {
        // For different implementations and discussions of these see, for example:
        // https://mklimenko.github.io/english/2018/08/22/robust-endian-swap
        // https://stackoverflow.com/questions/2182002/how-to-convert-big-endian-to-little-endian-in-c-without-using-library-functions
        // https://commandcenter.blogspot.com/2012/04/byte-order-fallacy.html
        static_assert( std::has_unique_object_representations_v<T> );
        constexpr size_t size = sizeof(T);
        union
        {
            T value;
            uint8_t bytes[ size ];
        } src, dst;
        src.value = value;
        for ( size_t i = 0, j = size; j; )
            dst.bytes[ --j ] = src.bytes[ i++ ];
        return dst.value;
    }

    /**
     * Reverses the bytes in the given (integer) value if the specified endian
     * matches the endianness of the platform; the value remains unchanged otherwise.
     * The platform is assumed to use either little- or big-endian. We explicitly do
     * not want to address other exotic byte orders.
     */
    template <typename T>
    T swap_bytes_if_endian(T value, std::endian endian)
    {
        static_assert( std::endian::native == std::endian::little || std::endian::native == std::endian::big );
        if ( endian == std::endian::native )
            value = swap_bytes( value );
        return value;
    }

    /**
     * Reverses the bytes in the given (integer) value if the specified endian does
     * not match the endianness of the platform; the value remains unchanged otherwise.
     * The platform is assumed to use either little- or big-endian. We explicitly do
     * not want to address other exotic byte orders.
     */
    template <typename T>
    T swap_bytes_unless_endian(T value, std::endian endian)
    {
        static_assert( std::endian::native == std::endian::little || std::endian::native == std::endian::big );
        if ( endian != std::endian::native )
            value = swap_bytes( value );
        return value;
    }



} // namespace bits
} // namespace cl7

#endif // CL7_BITS_H
