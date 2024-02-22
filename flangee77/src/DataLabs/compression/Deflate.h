#pragma once
#ifndef DL7_COMPRESSION_DEFLATE_H
#define DL7_COMPRESSION_DEFLATE_H

#include <CoreLabs/root.h>

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace dl7 {
namespace compression {



class Deflate
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Compresses the specified source data using the Deflate algorithm and stores
     * the compressed data in the specified destination buffer.
     */
    bool compress(cl7::byte_view src, cl7::byte_vector& dst);

    /**
     * Decompresses the specified source data using the Inflate algorithm and stores
     * the reconstructed data in the specified destination buffer.
     */
    bool decompress(cl7::byte_view src, cl7::byte_vector& dst);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Logs an error message corresponding to the given return code (if not Z_OK)
     * and returns false (if not Z_OK) or true (if Z_OK).
     */
    bool _log_and_return(int z_return_code);

}; // class Deflate



} // namespace compression
} // namespace dl7

#endif // DL7_COMPRESSION_DEFLATE_H
