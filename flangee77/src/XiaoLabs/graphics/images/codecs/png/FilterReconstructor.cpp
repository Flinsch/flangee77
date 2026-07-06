#include "FilterReconstructor.h"

#include "PaethPredictor.h"

#include <cassert>



namespace xl7::graphics::images::codecs::png {



    /**
     * Reconstructs the original scanline data from the given filtered source buffer.
     * src must contain height * (1 + bytes_per_scanline) bytes, with a filter type
     * byte at the start of each scanline. dst receives height * bytes_per_scanline
     * bytes of reconstructed pixel data.
     */
    bool FilterReconstructor::reconstruct(cl7::byte_view src, cl7::byte_vector& dst, size_t bytes_per_pixel, size_t bytes_per_scanline, size_t height)
    {
        const size_t expected_src_size = height * (1 + bytes_per_scanline);
        if (src.size() != expected_src_size)
            return false;

        dst.clear();
        dst.resize(height * bytes_per_scanline);

        size_t si = 0;
        size_t di = 0;

        for (size_t row = 0; row < height; ++row)
        {
            assert(si < src.size());
            const auto filter_type = static_cast<uint8_t>(src[si++]);

            assert(si + bytes_per_scanline <= src.size());

            for (size_t col = 0; col < bytes_per_scanline; ++col)
            {
                const auto fx = static_cast<uint8_t>(src[si + col]);

                uint8_t a = 0; // left
                uint8_t b = 0; // above
                uint8_t c = 0; // upper-left

                if (col >= bytes_per_pixel)
                    a = static_cast<uint8_t>(dst[di + col - bytes_per_pixel]);
                if (row > 0)
                    b = static_cast<uint8_t>(dst[di + col - bytes_per_scanline]);
                if (row > 0 && col >= bytes_per_pixel)
                    c = static_cast<uint8_t>(dst[di + col - bytes_per_scanline - bytes_per_pixel]);

                uint8_t result = fx;

                switch (filter_type)
                {
                case 1: // Sub
                    result += a;
                    break;
                case 2: // Up
                    result += b;
                    break;
                case 3: // Average
                    result += static_cast<uint8_t>((static_cast<unsigned>(a) + static_cast<unsigned>(b)) >> 1);
                    break;
                case 4: // Paeth
                    result += PaethPredictor::predict(a, b, c);
                    break;
                default: // None (0)
                    break;
                }

                dst[di + col] = std::byte{result};
            }

            si += bytes_per_scanline;
            di += bytes_per_scanline;
        }

        return true;
    }



} // namespace xl7::graphics::images::codecs::png
