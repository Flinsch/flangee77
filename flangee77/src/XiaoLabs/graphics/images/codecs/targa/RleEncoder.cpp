#include "RleEncoder.h"



namespace xl7::graphics::images::codecs::targa {



    RleEncoder::Result RleEncoder::encode(cl7::io::IWritable& writable, unsigned pixel_depth, cl7::byte_view input)
    {
        if (pixel_depth == 0 || pixel_depth % 8 != 0)
            return Result::BadPixelDepth;

        const auto bytes_per_pixel = static_cast<size_t>(pixel_depth / 8);
        assert(bytes_per_pixel > 0);

        if (input.size() % bytes_per_pixel != 0)
            return Result::BadInputBufferSize;

        const auto total_pixel_count = input.size() / bytes_per_pixel;

        size_t pixel_index = 0;
        const std::byte* cursor = input.data();

        while (pixel_index < total_pixel_count)
        {
            size_t pixel_count;
            size_t chunk_size;

            const auto rle_count = _count_rle_run(input, pixel_index, bytes_per_pixel);

            if (rle_count < 2)
            {
                // Write "raw" packet chunk.
                pixel_count = _count_raw_run(input, pixel_index, bytes_per_pixel);
                chunk_size = pixel_count * bytes_per_pixel;

                if (writable.write(static_cast<std::byte>(pixel_count - 1)) != 1)
                    return Result::WriteError;
                if (writable.write({cursor, chunk_size}) != chunk_size)
                    return Result::WriteError;
            }
            else
            {
                // Write RLE packet chunk.
                pixel_count = rle_count;
                chunk_size = pixel_count * bytes_per_pixel;

                if (writable.write(static_cast<std::byte>((pixel_count - 1) | 0x80)) != 1)
                    return Result::WriteError;
                if (writable.write({cursor, bytes_per_pixel}) != bytes_per_pixel)
                    return Result::WriteError;
            }

            cursor += chunk_size;
            pixel_index += pixel_count;
        } // for each chunk of pixels

        assert(pixel_index == total_pixel_count);

        return Result::Success;
    }



    size_t RleEncoder::_count_raw_run(cl7::byte_view input, size_t start, size_t bytes_per_pixel)
    {
        assert(bytes_per_pixel > 0);
        assert(input.size() % bytes_per_pixel == 0);
        const auto total_pixel_count = input.size() / bytes_per_pixel;

        size_t count = 1;

        while (start + count < total_pixel_count && count < 128)
        {
            const auto rle_count = _count_rle_run(input, start + count, bytes_per_pixel);
            if (rle_count >= 2)
                break;

            ++count;
        }

        return count;
    }

    size_t RleEncoder::_count_rle_run(cl7::byte_view input, size_t start, size_t bytes_per_pixel)
    {
        assert(bytes_per_pixel > 0);
        assert(input.size() % bytes_per_pixel == 0);
        const auto total_pixel_count = input.size() / bytes_per_pixel;

        size_t count = 1;

        const auto* buf1 = input.data() + start * bytes_per_pixel;
        const auto* buf2 = buf1;

        while (start + count < total_pixel_count && count < 128)
        {
            buf2 += bytes_per_pixel;
            if (std::memcmp(buf1, buf2, bytes_per_pixel) != 0)
                break;

            ++count;
        }

        return count;
    }



} // namespace xl7::graphics::images::codecs::targa
