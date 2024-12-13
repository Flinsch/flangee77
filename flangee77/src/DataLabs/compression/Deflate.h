#pragma once
#ifndef DL7_COMPRESSION_DEFLATE_H
#define DL7_COMPRESSION_DEFLATE_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace dl7::compression {



struct Deflate
{

    /**
     * Compresses the specified source data using the Deflate algorithm and stores
     * the compressed data in the specified destination buffer.
     */
    static bool compress(cl7::byte_view src, cl7::byte_vector& dst);

    /**
     * Decompresses the specified source data using the Inflate algorithm and stores
     * the reconstructed data in the specified destination buffer.
     */
    static bool decompress(cl7::byte_view src, cl7::byte_vector& dst);

}; // struct Deflate



} // namespace dl7::compression

#endif // DL7_COMPRESSION_DEFLATE_H
