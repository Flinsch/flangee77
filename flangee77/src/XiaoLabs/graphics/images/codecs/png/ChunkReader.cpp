#include "ChunkReader.h"

#include <CoreLabs/bits.h>

#define ZLIB_CONST
#define ZLIB_WINAPI
#include <zlib.h>



namespace xl7::graphics::images::codecs::png {



    bool ChunkReader::read_chunk_header(cl7::io::IReadable& readable, ChunkHeader& header)
    {
        if (readable.read({reinterpret_cast<std::byte*>(&header), sizeof(ChunkHeader)}) != sizeof(ChunkHeader))
            return false;

        header.length = cl7::bits::swap_bytes_unless_endian<std::endian::big>(header.length);
        return true;
    }

    bool ChunkReader::read_chunk_data(cl7::io::IReadable& readable, const ChunkHeader& header, cl7::byte_vector& data)
    {
        const size_t offset = data.size();

        if (header.length > 0)
        {
            data.resize(offset + header.length);
            if (readable.read(cl7::make_byte_span(data.data() + offset, header.length)) != header.length)
                return false;
        }

        uLong crc = ::crc32(0L, Z_NULL, 0);
        crc = ::crc32(crc, reinterpret_cast<const Bytef*>(header.type), 4);
        if (header.length > 0)
            crc = ::crc32(crc, reinterpret_cast<const Bytef*>(data.data() + offset), static_cast<uInt>(header.length));

        uint32_t stored_crc;
        if (readable.read({reinterpret_cast<std::byte*>(&stored_crc), 4}) != 4)
            return false;
        stored_crc = cl7::bits::swap_bytes_unless_endian<std::endian::big>(stored_crc);

        return static_cast<uint32_t>(crc) == stored_crc;
    }

    bool ChunkReader::skip_chunk_data(cl7::io::IReadable& readable, const ChunkHeader& header)
    {
        if (header.length > 0)
            readable.seek_read(header.length);

        uint32_t crc;
        return readable.read({reinterpret_cast<std::byte*>(&crc), 4}) == 4;
    }



} // namespace xl7::graphics::images::codecs::png
