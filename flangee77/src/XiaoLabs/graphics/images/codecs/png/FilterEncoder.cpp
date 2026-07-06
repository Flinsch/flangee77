#include "FilterEncoder.h"

#include "PaethPredictor.h"

#include <cassert>
#include <cstdlib>



namespace xl7::graphics::images::codecs::png {



    bool FilterEncoder::encode(cl7::byte_view src, cl7::byte_vector& dst,
                               size_t bytes_per_pixel,
                               size_t bytes_per_scanline,
                               size_t height,
                               FilterType filter_type)
    {
        const size_t expected_src_size = height * bytes_per_scanline;
        assert(src.size() == expected_src_size);
        if (src.size() != expected_src_size)
            return false;

        dst.clear();
        dst.resize(height * (1 + bytes_per_scanline));

        size_t si = 0;
        size_t di = 0;

        for (size_t row = 0; row < height; ++row)
        {
            const uint8_t ft = filter_type == FilterType::Auto
                ? _best_filter(src, si, row, bytes_per_pixel, bytes_per_scanline)
                : static_cast<uint8_t>(filter_type);

            dst[di++] = std::byte{ft};

            for (size_t col = 0; col < bytes_per_scanline; ++col)
            {
                const auto x = static_cast<uint8_t>(src[si + col]);

                uint8_t a = 0; // left
                uint8_t b = 0; // above
                uint8_t c = 0; // upper-left

                if (col >= bytes_per_pixel)
                    a = static_cast<uint8_t>(src[si + col - bytes_per_pixel]);
                if (row > 0)
                    b = static_cast<uint8_t>(src[si + col - bytes_per_scanline]);
                if (row > 0 && col >= bytes_per_pixel)
                    c = static_cast<uint8_t>(src[si + col - bytes_per_scanline - bytes_per_pixel]);

                uint8_t result;
                switch (ft)
                {
                case 1: // Sub
                    result = x - a;
                    break;
                case 2: // Up
                    result = x - b;
                    break;
                case 3: // Average
                    result = x - static_cast<uint8_t>((static_cast<unsigned>(a) + static_cast<unsigned>(b)) >> 1);
                    break;
                case 4: // Paeth
                    result = x - PaethPredictor::predict(a, b, c);
                    break;
                default: // None (0)
                    result = x;
                    break;
                }

                dst[di++] = std::byte{result};
            }

            si += bytes_per_scanline;
        }

        return true;
    }



    uint8_t FilterEncoder::_best_filter(cl7::byte_view src, size_t si, size_t row,
                                        size_t bytes_per_pixel, size_t bytes_per_scanline)
    {
        uint8_t best_ft = 0;
        uint64_t best_sum = UINT64_MAX;

        for (uint8_t ft = 0; ft <= 4; ++ft)
        {
            uint64_t sum = 0;

            for (size_t col = 0; col < bytes_per_scanline; ++col)
            {
                const auto x = static_cast<uint8_t>(src[si + col]);

                uint8_t a = 0, b = 0, c = 0;
                if (col >= bytes_per_pixel)
                    a = static_cast<uint8_t>(src[si + col - bytes_per_pixel]);
                if (row > 0)
                    b = static_cast<uint8_t>(src[si + col - bytes_per_scanline]);
                if (row > 0 && col >= bytes_per_pixel)
                    c = static_cast<uint8_t>(src[si + col - bytes_per_scanline - bytes_per_pixel]);

                uint8_t filt;
                switch (ft)
                {
                case 1: filt = x - a; break;
                case 2: filt = x - b; break;
                case 3: filt = x - static_cast<uint8_t>((static_cast<unsigned>(a) + static_cast<unsigned>(b)) >> 1); break;
                case 4: filt = x - PaethPredictor::predict(a, b, c); break;
                default: filt = x; break;
                }

                sum += static_cast<uint64_t>(std::abs(static_cast<int8_t>(filt)));
            }

            if (sum < best_sum)
            {
                best_sum = sum;
                best_ft = ft;
            }
        }

        return best_ft;
    }



} // namespace xl7::graphics::images::codecs::png
