#include "Writer.h"

#include "ChunkWriter.h"
#include "FilterEncoder.h"

#include "../../ImageConverter.h"

#include "../../../PixelLayout.h"

#include <DataLabs/compression.h>

#include <CoreLabs/bits.h>

#include <cstring>



namespace xl7::graphics::images::codecs::png {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * "Dumps" an image to any writable object.
     */
    bool Writer::_dump_to(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name)
    {
        const auto& image_desc = image.get_desc();
        const PixelLayout pixel_layout{image_desc.pixel_format, image_desc.channel_order};

        if (pixel_layout.data_type != PixelLayout::DataType::UNORM)
            return _log_unsupported_format_error(target_name, u8"only UNORM pixel formats are supported");

        const unsigned channel_count = pixel_layout.channel_count;
        if (channel_count == 0 || channel_count > 4)
            return _log_unsupported_format_error(target_name, u8"only 1-4 channel pixel formats are supported");

        const unsigned bit_depth = pixel_layout.uniform_depth;
        if (bit_depth != 8 && bit_depth != 16)
            return _log_unsupported_format_error(target_name, u8"only 8-bit and 16-bit channel depths are supported");

        const uint8_t color_type = _color_type_for(channel_count);

        // Convert to RGBA channel order if needed (for multi-channel images).
        cl7::byte_view image_data = image.get_data();
        cl7::byte_vector channel_buffer;
        if (channel_count > 1 && pixel_layout.channel_order != ChannelOrder::RGBA)
        {
            channel_buffer = ImageConverter::convert_image_data(image_desc, image_data, image_desc.pixel_format, ChannelOrder::RGBA);
            image_data = channel_buffer;
        }

        // PNG stores 16-bit values in big-endian byte order.
        cl7::byte_vector endian_buffer;
        if (bit_depth == 16)
        {
            endian_buffer.assign(image_data.begin(), image_data.end());
            const size_t element_count = endian_buffer.size() / 2;
            for (size_t i = 0; i < element_count; ++i)
            {
                uint16_t value;
                std::memcpy(&value, endian_buffer.data() + i * 2, 2);
                value = cl7::bits::swap_bytes_unless_endian<std::endian::big>(value);
                std::memcpy(endian_buffer.data() + i * 2, &value, 2);
            }
            image_data = endian_buffer;
        }

        // Apply PNG scanline filters.
        const size_t bytes_per_pixel = pixel_layout.bytes_per_pixel;
        const size_t bytes_per_scanline = static_cast<size_t>(image_desc.width) * bytes_per_pixel;
        cl7::byte_vector filtered;
        if (!FilterEncoder::encode(image_data, filtered, bytes_per_pixel, bytes_per_scanline, image_desc.height, _options.filter_type))
            return _log_bad_data_error(target_name, u8"filter encoding error");

        // Compress filtered data.
        cl7::byte_vector compressed;
        if (!dl7::compression::Deflate::compress(filtered, compressed))
            return _log_bad_data_error(target_name, u8"compression error");

        // Write PNG signature.
        static constexpr uint8_t SIGNATURE[8] = {0x89, 'P', 'N', 'G', '\r', '\n', 0x1a, '\n'};
        if (writable.write(cl7::make_byte_view(SIGNATURE)) != 8)
            return _log_write_error(target_name);

        // Write IHDR chunk.
        IhdrData ihdr;
        ihdr.width = cl7::bits::swap_bytes_unless_endian<std::endian::big>(image_desc.width);
        ihdr.height = cl7::bits::swap_bytes_unless_endian<std::endian::big>(image_desc.height);
        ihdr.bit_depth = static_cast<uint8_t>(bit_depth);
        ihdr.color_type = color_type;
        ihdr.compression_method = 0;
        ihdr.filter_method = 0;
        ihdr.interlace_method = 0;
        if (!ChunkWriter::write_chunk(writable, "IHDR", cl7::make_byte_view(ihdr)))
            return _log_write_error(target_name);

        // Write IDAT chunk.
        if (!ChunkWriter::write_chunk(writable, "IDAT", compressed))
            return _log_write_error(target_name);

        // Write IEND chunk.
        if (!ChunkWriter::write_chunk(writable, "IEND", {}))
            return _log_write_error(target_name);

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    uint8_t Writer::_color_type_for(unsigned channel_count)
    {
        static constexpr uint8_t COLOR_TYPES[5] = {
            0, // 0 channels: invalid (unused)
            0, // 1 channel:  Grayscale
            4, // 2 channels: Grayscale with Alpha
            2, // 3 channels: Truecolor
            6, // 4 channels: Truecolor with Alpha
        };
        return COLOR_TYPES[channel_count];
    }



} // namespace xl7::graphics::images::codecs::png
