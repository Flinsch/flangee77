#include "RleDecoder.h"



namespace xl7::graphics::images::codecs::targa {



    RleDecoder::Result RleDecoder::decode(cl7::io::IReadable& readable, unsigned pixel_depth, cl7::byte_span output)
    {
        if (pixel_depth == 0 || pixel_depth % 8 != 0)
            return Result::BadPixelDepth;

        const auto bytes_per_pixel = static_cast<size_t>(pixel_depth / 8);
        assert(bytes_per_pixel > 0);

        if (output.size() % bytes_per_pixel != 0)
            return Result::BadOutputBufferSize;

        const auto total_pixel_count = output.size() / bytes_per_pixel;

        size_t pixel_index = 0;
        std::byte* cursor = output.data();

        while (pixel_index < total_pixel_count)
        {
            unsigned char chunk_header;
            if (readable.read({reinterpret_cast<std::byte*>(&chunk_header), 1}) != 1)
                return Result::BadChunkHeaderLength;

            const size_t pixel_count = static_cast<size_t>(chunk_header & 0x7f) + 1;
            const size_t chunk_size = pixel_count * bytes_per_pixel;

            if (chunk_header <= 0x7f)
            {
                // Chunk is a "raw" packet.
                if (readable.read({cursor, chunk_size}) != chunk_size)
                    return Result::BadRawPacketChunkDataLength;
            }
            else
            {
                // Chunk is an RLE packet.
                if (readable.read({cursor, bytes_per_pixel}) != bytes_per_pixel)
                    return Result::BadRlePacketChunkDataLength;
                for (size_t i = 1; i < pixel_count; ++i)
                    std::memcpy(cursor + i * bytes_per_pixel, cursor, bytes_per_pixel);
            }

            cursor += chunk_size;
            pixel_index += pixel_count;
        } // for each chunk of pixels

        if (pixel_index != total_pixel_count)
            return Result::BadCompressedImageDataLength;

        return Result::Success;
    }



} // namespace xl7::graphics::images::codecs::targa
