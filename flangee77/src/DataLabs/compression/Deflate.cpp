#include "Deflate.h"

#include <CoreLabs/logging.h>

#define ZLIB_CONST
#define ZLIB_WINAPI
#include <zlib.h>



namespace dl7 {
namespace compression {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Compresses the specified source data using the Deflate algorithm and stores
     * the compressed data in the specified destination buffer.
     */
    bool Deflate::compress(cl7::byte_view src, cl7::byte_vector& dst)
    {
        static const size_t BLOCK_SIZE = 262144;
        auto buffer = std::make_unique_for_overwrite<unsigned char[]>( BLOCK_SIZE );
        const int z_level = Z_DEFAULT_COMPRESSION;

        ::z_stream z_strm;

        // Initialize Deflate state.
        z_strm.zalloc = Z_NULL;
        z_strm.zfree = Z_NULL;
        z_strm.opaque = Z_NULL;
        int z_return_code = ::deflateInit( &z_strm, z_level );
        if ( z_return_code != Z_OK )
            return _log_and_return( z_return_code );

        // Compress until end of source.
        // This outer loop seems to be unnecessary since we may set "avail_in" to
        // the full size of the complete source data buffer. But, anyway, we stay
        // with it to make it more consistent with the example code from the zlib
        // homepage.
        int z_flush;
        size_t src_index = 0;
        do {
            const size_t src_block_size = src.size() - src_index;

            assert( src_index + src_block_size <= src.size() );
            const bool end_of_src = src_index + src_block_size == src.size();
            z_flush = end_of_src ? Z_FINISH : Z_NO_FLUSH;

            z_strm.avail_in = static_cast<unsigned>( src_block_size );
            z_strm.next_in = (const unsigned char*)&src[ src_index ];

            // Run deflate() on input until output buffer not full. This would
            // indicate that there is not enough input data left to fill the output
            // buffer. So, continue with the next iteration grabbing some new input
            // data.
            do {
                z_strm.avail_out = static_cast<unsigned>( BLOCK_SIZE );
                z_strm.next_out = buffer.get();

                // Deflate!
                z_return_code = ::deflate( &z_strm, z_flush );
                if ( z_return_code != Z_OK && z_return_code != Z_STREAM_END )
                {
                    ::deflateEnd( &z_strm );
                    return _log_and_return( z_return_code );
                }

                const size_t dst_block_size = BLOCK_SIZE - static_cast<size_t>( z_strm.avail_out );

                assert( dst_block_size > 0 );

                // Copy data from temporary buffer
                // to the destination buffer.
                cl7::byte_view buffer_view{ (std::byte*)buffer.get(), dst_block_size };
                dst.insert( dst.end(), buffer_view.begin(), buffer_view.end() );

            } while ( z_strm.avail_out == 0 );
            assert( z_strm.avail_in == 0 );

            src_index += src_block_size;
        } while ( z_return_code != Z_STREAM_END );
        assert( z_flush == Z_FINISH );
        assert( src_index == src.size() );

        // Clean up and return.
        ::deflateEnd( &z_strm );
        return true;
    }

    /**
     * Decompresses the specified source data using the Inflate algorithm and stores
     * the reconstructed data in the specified destination buffer.
     */
    bool Deflate::decompress(cl7::byte_view src, cl7::byte_vector& dst)
    {
        static const size_t BLOCK_SIZE = 262144;
        auto buffer = std::make_unique_for_overwrite<unsigned char[]>( BLOCK_SIZE );

        ::z_stream z_strm;

        // Initialize Inflate state.
        z_strm.zalloc = Z_NULL;
        z_strm.zfree = Z_NULL;
        z_strm.opaque = Z_NULL;
        z_strm.avail_in = 0;
        z_strm.next_in = Z_NULL;
        int z_return_code = ::inflateInit( &z_strm );
        if ( z_return_code != Z_OK )
            return _log_and_return( z_return_code );

        // Decompress until end of source (or Deflate stream ends).
        // This outer loop seems to be unnecessary since we may set "avail_in" to
        // the full size of the complete source data vector. But, anyway, we stay
        // with it to make it more consistent with the example code from the zlib
        // homepage.
        size_t src_index = 0;
        do {
            const size_t src_block_size = src.size() - src_index;

            assert( src_index + src_block_size <= src.size() );

            z_strm.avail_in = static_cast<unsigned>( src_block_size );
            z_strm.next_in = (const unsigned char*)&src[ src_index ];

            // Run inflate() on input until output buffer not full. This would
            // indicate that there is not enough input data left to fill the output
            // buffer. So, continue with the next iteration grabbing some new input
            // data.
            do {
                z_strm.avail_out = static_cast<unsigned>( BLOCK_SIZE );
                z_strm.next_out = buffer.get();

                // Inflate!
                z_return_code = ::inflate( &z_strm, Z_NO_FLUSH );
                if ( z_return_code != Z_OK && z_return_code != Z_STREAM_END )
                {
                    if ( z_return_code == Z_NEED_DICT )
                        z_return_code = Z_DATA_ERROR;
                    ::inflateEnd( &z_strm );
                    return _log_and_return( z_return_code );
                }

                const size_t dst_block_size = BLOCK_SIZE - z_strm.avail_out;

                assert( dst_block_size > 0 );

                // Copy data from temporary buffer
                // to the destination vector.
                cl7::byte_view buffer_view{ (std::byte*)buffer.get(), dst_block_size };
                dst.insert( dst.end(), buffer_view.begin(), buffer_view.end() );

            } while ( z_strm.avail_out == 0 );
            //assert( z_strm.avail_in == 0 );

            src_index += src_block_size;
        } while ( z_return_code != Z_STREAM_END );
        assert( src_index == src.size() );

        // Clean up and return.
        ::inflateEnd( &z_strm );
        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Logs an error message corresponding to the given return code (if not Z_OK)
     * and returns false (if not Z_OK) or true (if Z_OK).
     */
    bool Deflate::_log_and_return(int z_return_code)
    {
        if ( z_return_code == Z_OK )
            return true;

        switch ( z_return_code )
        {
        case Z_STREAM_ERROR:
            LOG_ERROR( TEXT("zlib: invalid compression level.") );
            break;
        case Z_DATA_ERROR:
            LOG_ERROR( TEXT("zlib: corrupted or incomplete data stream.") );
            break;
        case Z_MEM_ERROR:
            LOG_ERROR( TEXT("zlib: not enough memory.") );
            break;
        case Z_BUF_ERROR:
            LOG_ERROR( TEXT("zlib: output buffer too small.") );
            break;
        case Z_VERSION_ERROR:
            LOG_ERROR( TEXT("Invalid zlib version.") );
            break;
        default:
            LOG_ERROR( TEXT("zlib caused an unknown error.") );
        }

        return false;
    }



} // namespace compression
} // namespace dl7
