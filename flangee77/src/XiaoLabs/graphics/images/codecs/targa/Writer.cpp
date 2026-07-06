#include "Writer.h"

#include "./Header.h"
#include "./RleEncoder.h"

#include "../../ImageConverter.h"

#include "../../../PixelLayout.h"

#include <CoreLabs/io/AsciiWriter.h>
#include <CoreLabs/io/ByteWriter.h>



namespace xl7::graphics::images::codecs::targa {



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
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count >= 1 && channel_count <= 4);

        if (pixel_layout.data_type != PixelLayout::DataType::UNORM && pixel_layout.data_type != PixelLayout::DataType::UINT)
            return _log_unsupported_format_error(target_name, u8"The Targa format supports only unsigned integer data types (UNORM or UINT).");

        if (channel_count != 1 && channel_count != 3 && channel_count != 4)
            return _log_unsupported_format_error(target_name, u8"The Targa format supports only grayscale images or RGB images with an optional alpha channel.");

        if (bit_depth != 8 && image_desc.pixel_format != PixelFormat::R5G5B5X1_UNORM && image_desc.pixel_format != PixelFormat::R5G5B5A1_UNORM)
            return _log_unsupported_format_error(target_name, u8"The Targa format supports only uniform 8-bit depths or the special 16-bit format with 5 bits per RGB channel plus optional 1-bit alpha.");

        // After completing the compatibility checks,
        // write the header.
        if (!_write_header(writable, target_name, image_desc))
            return false;

        // If there is more than one channel, we make sure
        // that the correct channel order is given (RGBA).
        cl7::byte_view image_data = image.get_data();
        cl7::byte_vector aux_buffer;
        if (channel_count > 1 && pixel_layout.channel_order != ChannelOrder::RGBA)
        {
            aux_buffer = ImageConverter::convert_image_data(image_desc, image_data, image_desc.pixel_format, ChannelOrder::RGBA);
            image_data = aux_buffer;
        }

        // Write the image data.
        if (_options.compression)
        {
            if (!_write_compressed(writable, target_name, pixel_layout.bytes_per_pixel * 8, image_data))
                return false;
        }
        else
        {
            if (!_write_uncompressed(writable, target_name, image_data))
                return false;
        }

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes the Targa header.
     */
    bool Writer::_write_header(cl7::io::IWritable& writable, const cl7::u8string& target_name, const ImageDesc& image_desc)
    {
        const PixelLayout pixel_layout{image_desc.pixel_format, image_desc.channel_order};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count == 1 || channel_count == 3 || channel_count == 4);
        assert(bit_depth == 8 || image_desc.pixel_format == PixelFormat::R5G5B5X1_UNORM || image_desc.pixel_format == PixelFormat::R5G5B5A1_UNORM);

        Header header = {};

        header.id_length = 0;
        header.color_map_type = 0;
        header.image_type = (channel_count == 1 ? 3 : 2) + (_options.compression ? 8 : 0);

        header.width = cl7::bits::swap_bytes_unless_endian<std::endian::little>(static_cast<uint16_t>(image_desc.width));
        header.height = cl7::bits::swap_bytes_unless_endian<std::endian::little>(static_cast<uint16_t>(image_desc.height));

        header.pixel_depth = bit_depth == 8 ? channel_count * 8 : 16; // 16 bits when R5G5B5X1 or R5G5B5A1

        header.image_descriptor = 0;
        if (channel_count == 4)
            header.image_descriptor |= bit_depth == 8 ? 8 : 1; // R8G8B8A8 or R5G5B5A1
        header.image_descriptor |= 0x1 << 5; // top-to-bottom scanline ordering

        if (writable.write({reinterpret_cast<const std::byte*>(&header), sizeof(Header)}) != sizeof(Header))
            return _log_write_error(target_name, u8"Could not write header.");

        return true;
    }

    /**
     * Writes uncompressed image data.
     */
    bool Writer::_write_uncompressed(cl7::io::IWritable& writable, const cl7::u8string& target_name, cl7::byte_view image_data)
    {
        if (writable.write(image_data) != image_data.size())
            return _log_write_error(target_name, u8"Could not write uncompressed image data.");

        return true;
    }

    /**
     * Writes RLE-compressed image data.
     */
    bool Writer::_write_compressed(cl7::io::IWritable& writable, const cl7::u8string& target_name, unsigned pixel_depth, cl7::byte_view image_data)
    {
        const auto result = RleEncoder::encode(writable, pixel_depth, image_data);

        switch (result)
        {
        case RleEncoder::Result::Success:
            return true;

        case RleEncoder::Result::WriteError:
            return _log_write_error(target_name, u8"Could not write compressed image data.");

        default:
            assert(false);
            return _log_bad_data_error(target_name, u8"unknown error writing RLE-encoded image data");
        }
    }



} // namespace xl7::graphics::images::codecs::targa
