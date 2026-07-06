#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_CHUNKREADER_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_CHUNKREADER_H

#include <CoreLabs/io/IReadable.h>
#include <CoreLabs/byte_vector.h>

#include <cstdint>



namespace xl7::graphics::images::codecs::png {



struct ChunkReader
{

    struct ChunkHeader
    {
        uint32_t length;
        char type[4];
    };
    static_assert(sizeof(ChunkHeader) == 8);

    /**
     * Reads a PNG chunk header from the readable: 4-byte big-endian length and
     * 4-byte chunk type. After this call, the readable is positioned at the start
     * of the chunk data.
     */
    static bool read_chunk_header(cl7::io::IReadable& readable, ChunkHeader& header);

    /**
     * Reads the chunk data for a previously read header, appending it to the given
     * byte vector, then reads and verifies the 4-byte CRC-32 over type and data.
     */
    static bool read_chunk_data(cl7::io::IReadable& readable, const ChunkHeader& header, cl7::byte_vector& data);

    /**
     * Skips the chunk data for a previously read header (seeking past the data bytes)
     * and discards the 4-byte CRC without verification.
     */
    static bool skip_chunk_data(cl7::io::IReadable& readable, const ChunkHeader& header);

}; // struct ChunkReader



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_CHUNKREADER_H
