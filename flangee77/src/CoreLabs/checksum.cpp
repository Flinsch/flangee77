#include "checksum.h"

#include <array>



namespace cl7::checksum {



namespace {

    // Standard CRC-32 (IEEE 802.3) polynomial, reflected form. Same one used by
    // zlib/PNG/gzip, so results are bit-compatible with those, even though this
    // implementation shares no code with them.
    constexpr uint32_t CRC32_POLYNOMIAL = 0xedb88320u;

    constexpr std::array<uint32_t, 256> _make_crc32_table()
    {
        std::array<uint32_t, 256> table{};
        for (uint32_t i = 0; i < 256; ++i)
        {
            uint32_t value = i;
            for (int bit = 0; bit < 8; ++bit)
                value = (value & 1) ? (CRC32_POLYNOMIAL ^ (value >> 1)) : (value >> 1);
            table[i] = value;
        }
        return table;
    }

    constexpr std::array<uint32_t, 256> CRC32_TABLE = _make_crc32_table();

} // namespace



    /**
     * Computes the CRC-32 checksum of the given data (the same well-known
     * algorithm/polynomial as used by zlib, PNG, gzip, etc.). This is a
     * self-contained implementation with no dependency on zlib, kept deliberately
     * independent so it can be used (and controlled) anywhere, without pulling in a
     * compression library just for a checksum. Code that deals with an actual
     * ZIP/PNG/gzip stream should keep using zlib's own `crc32` there instead, since
     * that's a hard interop requirement of those formats.
     */
    uint32_t crc32(cl7::byte_view data)
    {
        uint32_t crc = 0xffffffffu;
        for (std::byte b : data)
            crc = CRC32_TABLE[(crc ^ static_cast<uint8_t>(b)) & 0xff] ^ (crc >> 8);
        return crc ^ 0xffffffffu;
    }



} // namespace cl7::checksum
