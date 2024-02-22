#include "TargaImageHandler.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/utilities.h>

#include <bit>



namespace xl7 {
namespace graphics {
namespace images {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any rom.
     */
    bool TargaImageHandler::_load_from(cl7::io::irom& rom, cl7::string_view rom_name, Image& image)
    {
        Header header;
        if ( rom.read( { (std::byte*)&header, sizeof(Header) } ) != sizeof(Header) )
            return _log_bad_format_error( rom_name );

        if ( header.id_length || header.color_map_type )
            return _log_bad_header_error( rom_name );
        if ( header.image_type != 2 && header.image_type != 10 )
            return _log_bad_header_error( rom_name );
        if ( header.map_start || header.map_length || header.map_depth )
            return _log_bad_header_error( rom_name );
        if ( header.pixel_depth != 24 && header.pixel_depth != 32 )
            return _log_bad_header_error( rom_name );

        Image::Desc desc;
        desc.pixel_format = PixelFormat::UNKNOWN;
        if ( header.pixel_depth == 24 )
            desc.pixel_format = PixelFormat::R8G8B8_UNORM;
        if ( header.pixel_depth == 32 )
            desc.pixel_format = PixelFormat::R8G8B8A8_UNORM;
        desc.channel_order = ChannelOrder::RGBA;
        desc.width = static_cast<unsigned>( cl7::utilities::swap_bytes_unless_endian( header.width, std::endian::little ) );
        desc.height = static_cast<unsigned>( cl7::utilities::swap_bytes_unless_endian( header.height, std::endian::little ) );

        cl7::byte_vector data( desc.calculate_data_size() );

        if ( header.image_type == 2 )
            if ( !_load_uncompressed( rom, rom_name, header, desc, data ) )
                return false;
        if ( header.image_type == 10 )
            if ( !_load_compressed( rom, rom_name, header, desc, data ) )
                return false;

        // Flip vertically.
        const size_t line_count = static_cast<size_t>( desc.height );
        const size_t bytes_per_line = static_cast<size_t>( desc.width ) * static_cast<size_t>( desc.determine_pixel_stride() );
        cl7::byte_vector line( bytes_per_line );
        for ( size_t i = 0; i < line_count / 2; ++i )
        {
            size_t lo = bytes_per_line * i;
            size_t hi = bytes_per_line * (line_count - i - 1);
            assert( lo < hi );
            ::memcpy( line.data(), &data[ lo ], bytes_per_line );
            ::memcpy( &data[ lo ], &data[ hi ], bytes_per_line );
            ::memcpy( &data[ hi ], line.data(), bytes_per_line );
        }

        return image.init( desc, std::move(data) );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Loads an uncompressed TGA.
     */
    bool TargaImageHandler::_load_uncompressed(cl7::io::irom& rom, cl7::string_view rom_name, const Header& header, const Image::Desc& desc, cl7::byte_span data)
    {
        assert( data.size() == desc.calculate_data_size() );

        if ( rom.read( data ) != data.size() )
            return _log_bad_format_error( rom_name );

        return true;
    }

    /**
     * Loads a compressed TGA.
     */
    bool TargaImageHandler::_load_compressed(cl7::io::irom& rom, cl7::string_view rom_name, const Header& header, const Image::Desc& desc, cl7::byte_span data)
    {
        assert( data.size() == desc.calculate_data_size() );

        const size_t bytes_per_pixel = static_cast<size_t>( desc.determine_pixel_stride() );
        const size_t total_pixel_count = static_cast<size_t>( desc.width ) * static_cast<size_t>( desc.height );

        assert( total_pixel_count * bytes_per_pixel == data.size() );

        size_t pixel_index = 0;
        std::byte* cursor = &data[0];

        while ( pixel_index < total_pixel_count )
        {
            unsigned char chunk_header;
            if ( rom.read( { (std::byte*)&chunk_header, 1 } ) != 1 )
                return _log_bad_format_error( rom_name );

            if ( chunk_header < 128 )
            {
                // Chunk is a "raw" chunk.
                // The number of raw pixels is
                // the chunk header plus 1.
                const size_t pixel_count = static_cast<size_t>( chunk_header ) + 1;
                const size_t chunk_size = bytes_per_pixel * pixel_count;
                if ( rom.read( { cursor, chunk_size } ) != chunk_size )
                    return _log_bad_format_error( rom_name );
                cursor += chunk_size;
                pixel_index += pixel_count;
            }
            else
            {
                // Chunk is an RLE header.
                // The number of RLE pixels is
                // the chunk header minus 127.
                const size_t pixel_count = static_cast<size_t>( chunk_header ) - 127;
                const size_t chunk_size = bytes_per_pixel * pixel_count;
                if ( rom.read( { cursor, bytes_per_pixel } ) != bytes_per_pixel )
                    return _log_bad_format_error( rom_name );
                for ( size_t i = 1; i < pixel_count; ++i )
                    ::memcpy( cursor, cursor + i, bytes_per_pixel );
                cursor += chunk_size;
                pixel_index += pixel_count;
            }
        } // for each chunk of pixels

        if ( pixel_index != total_pixel_count )
            return _log_bad_format_error( rom_name );

        return true;
    }



} // namespace images
} // namespace graphics
} // namespace xl7
