#ifndef CL7_CHECKSUM_H
#define CL7_CHECKSUM_H

#include "./byte_view.h"



namespace cl7::checksum {



    /**
     * Computes the CRC-32 checksum of the given data (the same well-known
     * algorithm/polynomial as used by zlib, PNG, gzip, etc.). This is a
     * self-contained implementation with no dependency on zlib, kept deliberately
     * independent so it can be used (and controlled) anywhere, without pulling in a
     * compression library just for a checksum. Code that deals with an actual
     * ZIP/PNG/gzip stream should keep using zlib's own `crc32` there instead, since
     * that's a hard interop requirement of those formats.
     */
    uint32_t crc32(cl7::byte_view data);



} // namespace cl7::checksum

#endif // CL7_CHECKSUM_H
