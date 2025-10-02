#include "bits.h"



namespace cl7::bits {



    /**
     * Converts a native 32-bit floating-point number to a non-native 16-bit
     * floating-point number.
     */
    int16_t float_to_half(float value)
    {
        static_assert(sizeof(float) == sizeof(uint32_t));
        const uint32_t f = *reinterpret_cast<uint32_t*>(&value);

        uint32_t h;

        if ((f & 0x7fffffff) >= 0x7f800000) // exponent all ones?
        {
            if ((f & 0x007fffff) != 0) // mantissa non-zero?
                h = ((f >> 16) & 0x8000) | 0x7fff; // NaN
            else
                h = ((f >> 16) & 0x8000) | 0x7c00; // infinity
        }
        else // => "valid" (can still overflow/underflow)
        {
            const uint32_t E = (f >> 23) & 0xff; // exponent (biased)
            if (E >= 0x1f + 127 - 15) // E = [0x8f;0xfe] = [143;254] => e > 15
                h = ((f >> 16) & 0x8000) | 0x7c00; // overflow => infinity
            else if (E <= 127 - 15) // E = [0x00;0x70] = [0;112] => e < -14
            {
                if (E < 127 - 15 - 10) // E = [0x00;0x65] = [0;101] => e < -25
                    h = ((f >> 16) & 0x8000); // underflow => zero
                else // E = [0x66;0x70] = [102;112] => e = [-25;-13]
                    h = ((f >> 16) & 0x8000) | ((((f | 0x00800000) >> (127 - 15 + 1 - E)) >> 13) & 0x03ff); // denormalized/zero
            }
            else // E = [0x71;0x8e] = [113;142] => e = [-14;15]
                h = ((f >> 16) & 0x8000) | ((E + 15 - 127) << 10) | ((f >> 13) & 0x03ff); // normalized
        }

        union
        {
            uint32_t h32;
            struct
            {
                int16_t h16;
                int16_t pad;
            };
        } _;
        _.h32 = h;
        return _.h16;
    }

    /**
     * Converts a non-native 16-bit floating-point number to a native 32-bit
     * floating-point number.
     */
    float half_to_float(int16_t value)
    {
        union
        {
            struct
            {
                int16_t h16;
                int16_t pad;
            };
            uint32_t h32;
        } _;
        _.h16 = value;
        const uint32_t h = _.h32;

        uint32_t f;

        if ((h & 0x7fff) >= 0x7c00) // exponent all ones?
        {
            if ((h & 0x03ff) != 0) // mantissa non-zero?
                f = ((h << 16) & 0x80000000) | 0x7fffffff; // NaN
            else
                f = ((h << 16) & 0x80000000) | 0x7f800000; // infinity
        }
        else // => "valid"
        {
            const uint32_t E = (h >> 10) & 0x1f; // exponent (biased)
            if (E == 0) // E = 0 => e < -14 (theoretically)
                f = ((h << 16) & 0x80000000) | ((h << 13) & 0x007fffff); // denormalized/zero
            else // E = [0x01;0x1e] = [1;30] => e = [-14;15]
                f = ((h << 16) & 0x80000000) | ((E + 127 - 15) << 23) | ((h << 13) & 0x007fffff); // normalized
        }

        static_assert(sizeof(float) == sizeof(uint32_t));
        return *reinterpret_cast<float*>(&f);
    }



    /**
     * Converts a normalized floating-point number in the range [0;1] to an unsigned
     * integer value of a certain bit depth (up to 32 bits), so that 1 is mapped to
     * the highest possible integer value of this bit depth.
     */
    unsigned norm_to_fixed(float value, unsigned bit_depth)
    {
        assert(bit_depth <= 32);
        //return static_cast<unsigned>(value * static_cast<float>((1ULL << bit_depth) - 1));
        if (value <= 0.0f) return 0;
        if (value >= 1.0f) return static_cast<unsigned>((1ULL << bit_depth) - 1);
        return static_cast<unsigned>(value * static_cast<float>(1ULL << bit_depth));
    }

    /**
     * Converts an unsigned integer value of a certain bit depth (up to 32 bits) to
     * a normalized floating-point number in the range [0;1], so that the highest
     * possible integer value of this bit depth is mapped to 1.
     */
    float fixed_to_norm(unsigned value, unsigned bit_depth)
    {
        assert(bit_depth <= 32);
        return static_cast<float>(value) / static_cast<float>((1ULL << bit_depth) - 1);
    }

    /**
     * Converts between two unsigned integer values of certain bit depths (up to 32
     * bits each). The value ranges are mapped in such a way that the highest values
     * correspond to each other.
     */
    unsigned fixed_to_fixed(unsigned value, unsigned src_bit_depth, unsigned dst_bit_depth)
    {
        assert(src_bit_depth <= 32);
        assert(dst_bit_depth <= 32);

        if (src_bit_depth > dst_bit_depth)
        {
            value >>= src_bit_depth - dst_bit_depth;
        }
        else if (src_bit_depth < dst_bit_depth)
        {
            if (value >= static_cast<unsigned>((1ULL << src_bit_depth) - 1))
                value = static_cast<unsigned>((1ULL << dst_bit_depth) - 1);
            else if (value > 0)
                value = static_cast<unsigned>(static_cast<uint64_t>(value) * (1ULL << dst_bit_depth) / ((1ULL << src_bit_depth) - 1));
        }

        return value;
    }



} // namespace cl7::bits
