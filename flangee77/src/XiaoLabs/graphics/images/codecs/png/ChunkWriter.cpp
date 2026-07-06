#include "ChunkWriter.h"

#include <CoreLabs/bits.h>

#define ZLIB_CONST
#define ZLIB_WINAPI
#include <zlib.h>



namespace xl7::graphics::images::codecs::png {



    bool ChunkWriter::write_chunk(cl7::io::IWritable& writable, const char (&type)[5], cl7::byte_view data)
    {
        auto length = static_cast<uint32_t>(data.size());
        length = cl7::bits::swap_bytes_unless_endian<std::endian::big>(length);
        if (writable.write(cl7::make_byte_view(length)) != 4)
            return false;

        if (writable.write(cl7::make_byte_view(type, 4)) != 4)
            return false;

        if (!data.empty() && writable.write(data) != data.size())
            return false;

        uLong crc = ::crc32(0L, Z_NULL, 0);
        crc = ::crc32(crc, reinterpret_cast<const Bytef*>(type), 4);
        if (!data.empty())
            crc = ::crc32(crc, reinterpret_cast<const Bytef*>(data.data()), static_cast<uInt>(data.size()));

        auto crc32_val = static_cast<uint32_t>(crc);
        crc32_val = cl7::bits::swap_bytes_unless_endian<std::endian::big>(crc32_val);
        if (writable.write(cl7::make_byte_view(crc32_val)) != 4)
            return false;

        return true;
    }



} // namespace xl7::graphics::images::codecs::png
