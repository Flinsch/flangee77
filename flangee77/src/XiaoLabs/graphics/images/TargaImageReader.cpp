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
        if (!_read_header(readable, source_name, header))
            return false;

        // Skip possible free-form image identification field.
        readable.seek_read(header.id_length);

        // Read color map, if provided.
        cl7::byte_vector color_map;
        if (header.color_map_type == 1)
        {
            const auto color_map_size = static_cast<size_t>(header.color_map_length) * static_cast<size_t>(header.color_map_depth / 8);
            color_map.resize(color_map_size);
            if (!_read_color_map(readable, source_name, color_map))
                return false;
        }

        // Prepare data buffer ...
        const size_t pixel_count = static_cast<size_t>(header.width) * static_cast<size_t>(header.height);
        cl7::byte_vector image_data(pixel_count * static_cast<size_t>(header.pixel_depth / 8));

        // ... and read image data.
        if (header.image_type >= 1 && header.image_type <= 3)
            if (!_read_uncompressed(readable, source_name, image_data))
                return false;
        if (header.image_type >= 9 && header.image_type <= 11)
            if (!_read_compressed(readable, source_name, header.pixel_depth, image_data))
                return false;

        // Assume the number of bits per pixel.
        auto effective_depth = header.pixel_depth;

        if (header.image_type == 1 || header.image_type == 9)
        {
            // In the case of color-mapped data, the actual pixel depth
            // is not that of the image data, but that of the color map.
            effective_depth = header.color_map_depth;

            cl7::byte_vector index_data = std::move(image_data);
            image_data = cl7::byte_vector(pixel_count * static_cast<size_t>(header.color_map_depth / 8));

            if (!_map_color_data(source_name, color_map, index_data, image_data))
                return false;
        }

        // If bottom-left origin (default), flip vertically.
        const auto vertical_origin = (header.image_descriptor >> 5) & 0x1;
        if (vertical_origin == 0)
            _flip_vertically(image_data, header.width, header.height);

        Image::Desc desc;
        desc.pixel_format = PixelFormat::UNKNOWN;
        if (effective_depth == 8)
            desc.pixel_format = PixelFormat::R8_UNORM;
        if (effective_depth == 16)
            desc.pixel_format = (header.image_descriptor & 0xf) == 0 ? PixelFormat::R5G5B5X1_UNORM : PixelFormat::R5G5B5A1_UNORM;
        if (effective_depth == 24)
            desc.pixel_format = PixelFormat::R8G8B8_UNORM;
        if (effective_depth == 32)
            desc.pixel_format = PixelFormat::R8G8B8A8_UNORM;
        assert(desc.pixel_format != PixelFormat::UNKNOWN);
        desc.channel_order = ChannelOrder::BGRA;
        desc.width = static_cast<unsigned>(header.width);
        desc.height = static_cast<unsigned>(header.height);
        desc.depth = 1;

        return image.init(desc, std::move(image_data));
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Reads and validates the Targa header.
     */
    bool TargaImageReader::_read_header(cl7::io::IReadable& readable, const cl7::u8string& source_name, Header& header)
    {
        if (readable.read({reinterpret_cast<std::byte*>(&header), sizeof(Header)}) != sizeof(Header))
            return _log_bad_header_error(source_name, u8"bad header length");

        header.width = cl7::bits::swap_bytes_unless_endian<std::endian::little>(header.width);
        header.height = cl7::bits::swap_bytes_unless_endian<std::endian::little>(header.height);

        header.color_map_start = cl7::bits::swap_bytes_unless_endian<std::endian::little>(header.color_map_start);
        header.color_map_length = cl7::bits::swap_bytes_unless_endian<std::endian::little>(header.color_map_length);

        if (header.color_map_type > 1)
            return _log_bad_header_error(source_name, u8"invalid color map type: " + cl7::to_string(header.color_map_type));
        if ((header.image_type < 1 || header.image_type > 3) && (header.image_type < 9 || header.image_type > 11) && (header.image_type < 32 || header.image_type > 33))
            return _log_bad_header_error(source_name, u8"invalid image type: " + cl7::to_string(header.image_type));
        if (header.width == 0 || header.height == 0)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");
        if (header.width > Image::MAX_SIZE || header.height > Image::MAX_SIZE)
            return _log_bad_header_error(source_name, u8"valid width and height not greater than " + cl7::to_string(Image::MAX_SIZE) + u8" expected");
        if (header.pixel_depth != 8 && header.pixel_depth != 16 && header.pixel_depth != 24 && header.pixel_depth != 32)
            return _log_bad_header_error(source_name, u8"invalid pixel depth: " + cl7::to_string(header.pixel_depth));

        // The effective pixel depth depends on the image type:
        // for color-mapped images, it is the color map depth;
        // otherwise, it is the regular pixel depth of the image data.
        const auto effective_depth = header.image_type == 1 || header.image_type == 9 ? header.color_map_depth : header.pixel_depth;

        if ((header.image_type == 1 || header.image_type == 9) && (header.color_map_type != 1 || header.color_map_length == 0))
            return _log_bad_header_error(source_name, u8"color map expected");

        if (header.color_map_type == 1 && header.color_map_start >= header.color_map_length)
            return _log_bad_header_error(source_name, u8"invalid color map entries specification");
        if (header.color_map_type == 1 && (header.pixel_depth != 8 || (header.color_map_depth != 16 && header.color_map_depth != 24 && header.color_map_depth != 32)))
            return _log_bad_header_error(source_name, u8"invalid color map bit depth specification");

        if (header.image_type == 32 || header.image_type == 33)
            return _log_unsupported_format_error(source_name, u8"Huffman-encoded images not supported");

        // Sometimes the attribute bits are set even though they are not relevant
        // (e.g., 8-bit alpha despite 24-bit indexed RGB without alpha),
        // so we simply ignore the bits if they are not really relevant anyway.
        const auto alpha_depth = header.image_descriptor & 0xf;
        if ((effective_depth == 16 && alpha_depth > 1) || (effective_depth == 32 && alpha_depth != 8)/* || (effective_depth != 16 && effective_depth != 32 && alpha_depth != 0)*/)
            return _log_bad_header_error(source_name, u8"bad image descriptor attribute bits");

        // Right origin is very uncommon. Almost no software generates this,
        // and it would be quite stupid, even considering historical matters.
        // Even though it would be relatively easy to handle, we disallow it.
        // Vertical flipping is a different story: Bottom-left origin is the
        // classic TGA default. We actually expect this and handle it accordingly
        // (and therefore do not prohibit it).
        const auto horizontal_origin = (header.image_descriptor >> 4) & 0x1;
        //const auto vertical_origin = (header.image_descriptor >> 5) & 0x1;
        if (horizontal_origin != 0)
            return _log_unsupported_format_error(source_name, u8"horizontal flip not supported");

        // Interleaving means scan lines are stored in separated groups.
        // This was designed for very old video hardware and frame grabbers.
        // Modern software almost never uses any scan-line interleaving,
        // and many TGA loaders ignore it completely, and so do we.
        const auto interleaving = (header.image_descriptor >> 6) & 0x3;
        if (interleaving != 0)
            return _log_unsupported_format_error(source_name, u8"interleaving not supported");

        return true;
    }

    /**
     * Reads the color map.
     */
    bool TargaImageReader::_read_color_map(cl7::io::IReadable& readable, const cl7::u8string& source_name, cl7::byte_span color_map)
    {
        if (readable.read(color_map) != color_map.size())
            return _log_bad_data_error(source_name, u8"bad color map data length");

        return true;
    }

    /**
     * Reads uncompressed image data.
     */
    bool TargaImageReader::_read_uncompressed(cl7::io::IReadable& readable, const cl7::u8string& source_name, cl7::byte_span image_data)
    {
        if (readable.read(image_data) != image_data.size())
            return _log_bad_data_error(source_name, u8"bad uncompressed image data length");

        return true;
    }

    /**
     * Reads RLE-decoded compressed image data.
     */
    bool TargaImageReader::_read_compressed(cl7::io::IReadable& readable, const cl7::u8string& source_name, unsigned pixel_depth, cl7::byte_span image_data)
    {
        assert(pixel_depth % 8 == 0);
        const auto bytes_per_pixel = static_cast<size_t>(pixel_depth / 8);
        assert(bytes_per_pixel > 0);
        assert(image_data.size() % bytes_per_pixel == 0);
        const auto total_pixel_count = image_data.size() / bytes_per_pixel;

        size_t pixel_index = 0;
        std::byte* cursor = image_data.data();

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
            return _log_bad_data_error(source_name, u8"bad compressed image data length");

        return true;
    }

    /**
     * Translates the color indices to actual color values from the color map.
     */
    bool TargaImageReader::_map_color_data(const cl7::u8string& source_name, cl7::byte_view color_map, cl7::byte_view index_data, cl7::byte_span color_data)
    {
        const auto pixel_count = index_data.size(); // 8 bits per pixel
        assert(pixel_count > 0);
        assert(color_data.size() % pixel_count == 0);
        const auto bytes_per_pixel = color_data.size() / pixel_count;
        assert(bytes_per_pixel == 2 || bytes_per_pixel == 3 || bytes_per_pixel == 4); // 16, 24, or 32 bits per pixel

        for (size_t i = 0; i < pixel_count; ++i)
        {
            const auto index = static_cast<size_t>(static_cast<uint8_t>(index_data[i]));
            if ((index + 1) * bytes_per_pixel > color_map.size())
                return _log_bad_data_error(source_name, u8"bad color map index data");
            for (size_t k = 0; k < bytes_per_pixel; ++k)
                color_data[i * bytes_per_pixel + k] = color_map[index * bytes_per_pixel + k];
        }

        return true;
    }

    /**
     * Flips the image vertically.
     */
    void TargaImageReader::_flip_vertically(cl7::byte_span image_data, unsigned width, unsigned height)
    {
        const size_t pixel_count = static_cast<size_t>(width) * static_cast<size_t>(height);
        assert(pixel_count > 0);
        assert(image_data.size() % pixel_count == 0);
        const size_t bytes_per_pixel = image_data.size() / pixel_count;
        assert(bytes_per_pixel > 0);

        const auto line_count = static_cast<size_t>(height);
        const size_t bytes_per_line = static_cast<size_t>(width) * bytes_per_pixel;

        cl7::byte_vector line(bytes_per_line);
        for (size_t i = 0; i < line_count / 2; ++i)
        {
            const size_t lo = bytes_per_line * i;
            const size_t hi = bytes_per_line * (line_count - i - 1);
            assert(lo < hi);
            std::memcpy(line.data(), &image_data[lo], bytes_per_line);
            std::memcpy(&image_data[lo], &image_data[hi], bytes_per_line);
            std::memcpy(&image_data[hi], line.data(), bytes_per_line);
        }
    }



} // namespace xl7::graphics::images
