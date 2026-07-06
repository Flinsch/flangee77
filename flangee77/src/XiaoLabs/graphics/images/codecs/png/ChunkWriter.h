#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_CHUNKWRITER_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_CHUNKWRITER_H

#include <CoreLabs/io/IWritable.h>
#include <CoreLabs/byte_view.h>



namespace xl7::graphics::images::codecs::png {



struct ChunkWriter
{
    /**
     * Writes a complete PNG chunk to the given writable: 4-byte big-endian data
     * length, 4-byte chunk type, data bytes, and 4-byte big-endian CRC-32 computed
     * over type and data.
     */
    static bool write_chunk(cl7::io::IWritable& writable, const char (&type)[5], cl7::byte_view data);

}; // struct ChunkWriter



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_CHUNKWRITER_H
