#ifndef CL7_BITS_H
#define CL7_BITS_H

#include <CoreLabs/root.h>

#include <bit>



namespace cl7::bits {



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



namespace detail {
    template <typename T, size_t size>
    struct SwapBytesImpl
    {
        static T swap_bytes(T value)
        {
            // For different implementations and discussions of these see, for example:
            // https://mklimenko.github.io/english/2018/08/22/robust-endian-swap
            // https://stackoverflow.com/questions/2182002/how-to-convert-big-endian-to-little-endian-in-c-without-using-library-functions
            // https://commandcenter.blogspot.com/2012/04/byte-order-fallacy.html
            // Or also related:
            // https://codereview.stackexchange.com/questions/149717/implementation-of-c-standard-library-function-ntohl
            static_assert(std::has_unique_object_representations_v<T>);
            static_assert(size == sizeof(T));
            union
            {
                T value;
                uint8_t bytes[size];
            } src, dst;
            src.value = value;
            for (size_t i = 0, j = size; j; )
                dst.bytes[--j] = src.bytes[i++];
            return dst.value;
        }
    };

    template <typename T>
    struct SwapBytesImpl<T, 1>
    {
        static T swap_bytes(T value)
        {
            return value;
        }
    };
} // namespace detail

    /**
     * Reverses the bytes in the given (integer) value.
     */
    template <typename T>
    T swap_bytes(T value)
    {
        return detail::SwapBytesImpl<T, sizeof(T)>::swap_bytes(value);
    }

    /**
     * Reverses the bytes in the given (integer) value if the specified endian
     * matches the endianness of the platform; the value remains unchanged otherwise.
     * The platform is assumed to use either little- or big-endian. We explicitly do
     * not want to address other exotic byte orders.
     */
    template <std::endian endian, typename T>
    T swap_bytes_if_endian(T value)
    {
        static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big);
        if (endian == std::endian::native)
            value = swap_bytes(value);
        return value;
    }

    /**
     * Reverses the bytes in the given (integer) value if the specified endian does
     * not match the endianness of the platform; the value remains unchanged otherwise.
     * The platform is assumed to use either little- or big-endian. We explicitly do
     * not want to address other exotic byte orders.
     */
    template <std::endian endian, typename T>
    T swap_bytes_unless_endian(T value)
    {
        static_assert(std::endian::native == std::endian::little || std::endian::native == std::endian::big);
        if (endian != std::endian::native)
            value = swap_bytes(value);
        return value;
    }



    /**
     * Converts a native 32-bit floating point number to a non-native 16-bit
     * floating point number.
     */
    int16_t float_to_half(float value);

    /**
     * Converts a non-native 16-bit floating point number to a native 32-bit
     * floating point number.
     */
    float half_to_float(int16_t value);



    /**
     * Converts a normalized floating point number in the range [0;1] to an unsigned
     * integer value of a certain bit depth (up to 32 bits), so that 1 is mapped to
     * the highest possible integer value of this bit depth.
     */
    unsigned norm_to_fixed(float value, unsigned bit_depth);

    /**
     * Converts an unsigned integer value of a certain bit depth (up to 32 bits) to
     * a normalized floating point number in the range [0;1], so that the highest
     * possible integer value of this bit depth is mapped to 1.
     */
    float fixed_to_norm(unsigned value, unsigned bit_depth);

    /**
     * Converts between two unsigned integer values of certain bit depths (up to 32
     * bits each). The value ranges are mapped in such a way that the highest values
     * correspond to each other.
     */
    unsigned fixed_to_fixed(unsigned value, unsigned src_bit_depth, unsigned dst_bit_depth);



} // namespace cl7::bits

#endif // CL7_BITS_H
