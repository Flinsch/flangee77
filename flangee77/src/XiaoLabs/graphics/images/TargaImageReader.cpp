#include "TargaImageReader.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/bits.h>



namespace xl7::graphics::images {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any readable object.
     */
    bool TargaImageReader::_load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
    {
        Header header;
        if (readable.read({reinterpret_cast<std::byte*>(&header), sizeof(Header)}) != sizeof(Header))
            return _log_bad_header_error(source_name, u8"bad header length");

        if (header.color_map_type > 1)
            return _log_bad_header_error(source_name, u8"invalid color map type: " + cl7::to_string(header.color_map_type));
        if ((header.image_type < 1 || header.image_type > 3) && (header.image_type < 9 || header.image_type > 11) && (header.image_type < 32 || header.image_type > 33))
            return _log_bad_header_error(source_name, u8"invalid image type: " + cl7::to_string(header.image_type));
        if (header.width == 0 || header.height == 0)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");
        if (header.pixel_depth != 16 && header.pixel_depth != 24 && header.pixel_depth != 32)
            return _log_bad_header_error(source_name, u8"invalid pixel depth: " + cl7::to_string(header.pixel_depth));

        if (header.color_map_type != 0)
            return _log_unsupported_format_error(source_name, u8"only unmapped images supported");
        if (header.image_type != 2 && header.image_type != 10)
            return _log_unsupported_format_error(source_name, u8"only unmapped RGB images supported (uncompressed or runlength encoded)");
        if (header.pixel_depth != 24 && header.pixel_depth != 32)
            return _log_unsupported_format_error(source_name, u8"only 24 or 32 bit images supported");

        Image::Desc desc;
        desc.pixel_format = PixelFormat::UNKNOWN;
        if (header.pixel_depth == 24)
            desc.pixel_format = PixelFormat::R8G8B8_UNORM;
        if (header.pixel_depth == 32)
            desc.pixel_format = PixelFormat::R8G8B8A8_UNORM;
        desc.channel_order = ChannelOrder::BGRA;
        desc.width = static_cast<unsigned>(cl7::bits::swap_bytes_unless_endian<std::endian::little>(header.width));
        desc.height = static_cast<unsigned>(cl7::bits::swap_bytes_unless_endian<std::endian::little>(header.height));
        desc.depth = 1;

        cl7::byte_vector data(desc.calculate_data_size());

        if (header.image_type == 2)
            if (!_load_uncompressed(readable, source_name, header, desc, data))
                return false;
        if (header.image_type == 10)
            if (!_load_compressed(readable, source_name, header, desc, data))
                return false;

        // Flip vertically.
        const auto line_count = static_cast<size_t>(desc.height);
        const auto bytes_per_line = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.determine_pixel_stride());
        cl7::byte_vector line(bytes_per_line);
        for (size_t i = 0; i < line_count / 2; ++i)
        {
            size_t lo = bytes_per_line * i;
            size_t hi = bytes_per_line * (line_count - i - 1);
            assert(lo < hi);
            std::memcpy(line.data(), &data[lo], bytes_per_line);
            std::memcpy(&data[lo], &data[hi], bytes_per_line);
            std::memcpy(&data[hi], line.data(), bytes_per_line);
        }

        return image.init(desc, std::move(data));
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Loads an uncompressed TGA.
     */
    bool TargaImageReader::_load_uncompressed(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Header& header, const Image::Desc& desc, cl7::byte_span data)
    {
        assert(header.image_type == 2);
        assert(data.size() == desc.calculate_data_size());

        if (readable.read(data) != data.size())
            return _log_bad_data_error(source_name, u8"bad uncompressed data length");

        return true;
    }

    /**
     * Loads a compressed TGA.
     */
    bool TargaImageReader::_load_compressed(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Header& header, const Image::Desc& desc, cl7::byte_span data)
    {
        assert(header.image_type == 10);
        assert(data.size() == desc.calculate_data_size());

        const auto bytes_per_pixel = static_cast<size_t>(desc.determine_pixel_stride());
        const auto total_pixel_count = desc.calculate_pixel_count();

        assert(total_pixel_count * bytes_per_pixel == data.size());

        size_t pixel_index = 0;
        std::byte* cursor = data.data();

        while (pixel_index < total_pixel_count)
        {
            unsigned char chunk_header;
            if (readable.read({reinterpret_cast<std::byte*>(&chunk_header), 1}) != 1)
                return _log_bad_header_error(source_name, u8"bad chunk header length");

            const size_t pixel_count = static_cast<size_t>(chunk_header & 0x7f) + 1;
            const size_t chunk_size = pixel_count * bytes_per_pixel;

            if (chunk_header <= 0x7f)
            {
                // Chunk is a "raw" packet.
                if (readable.read({cursor, chunk_size}) != chunk_size)
                    return _log_bad_data_error(source_name, u8"bad \"raw\"-packet chunk data length");
            }
            else
            {
                // Chunk is an RLE packet.
                if (readable.read({cursor, bytes_per_pixel}) != bytes_per_pixel)
                    return _log_bad_data_error(source_name, u8"bad RLE-packet chunk data length");
                for (size_t i = 1; i < pixel_count; ++i)
                    std::memcpy(cursor + i * bytes_per_pixel, cursor, bytes_per_pixel);
            }

            cursor += chunk_size;
            pixel_index += pixel_count;
        } // for each chunk of pixels

        if (pixel_index != total_pixel_count)
            return _log_bad_data_error(source_name, u8"bad compressed data length");

        return true;
    }



} // namespace xl7::graphics::images
