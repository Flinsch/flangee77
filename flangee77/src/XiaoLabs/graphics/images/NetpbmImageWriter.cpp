#include "NetpbmImageWriter.h"

#include "../PixelLayout.h"

#include <CoreLabs/io/AsciiWriter.h>
#include <CoreLabs/io/ByteWriter.h>

#include <CoreLabs/text/format.h>



namespace xl7::graphics::images {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * "Dumps" an image to any writable object.
     */
    bool NetpbmImageWriter::_dump_to(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name)
    {
        if (_format == Format::Pam)
            return _write_pam(image, writable, target_name);
        return _write_pnm(image, writable, target_name, _format);
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Writes any PBM, PGM, or PPM format.
     */
    bool NetpbmImageWriter::_write_pnm(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name, Format format)
    {
        const auto magic2 = static_cast<cl7::achar_t>('0' + static_cast<int>(format));
        assert(magic2 >= '1' && magic2 <= '6');

        const bool is_ascii = magic2 >= '1' && magic2 <= '3';
        const bool is_binary = magic2 >= '4' && magic2 <= '6';
        const bool is_1bit = magic2 == '1' || magic2 == '4';
        const unsigned channel_count = magic2 == '3' || magic2 == '6' ? 3 : 1;
        assert((is_ascii || is_binary) && !(is_ascii && is_binary));

        PixelLayout pixel_layout{image.get_pixel_format(), image.get_channel_order()};
        const unsigned bit_depth = pixel_layout.uniform_depth;

        if (pixel_layout.channel_count != channel_count)
        {
            _log_unsupported_format_error(target_name, u8"The desired PNM format does not support the image's channel count of " + cl7::text::format::to_string<cl7::u8string>(pixel_layout.channel_count) + u8".");
            return _log_unsupported_format_error(target_name, u8"The PNM format only supports channel counts of 1 (PBM, PGM) or 3 (PPM).");
        }

        if (pixel_layout.data_type != PixelLayout::DataType::UNORM && pixel_layout.data_type != PixelLayout::DataType::UINT)
            return _log_unsupported_format_error(target_name, u8"The Netpbm/PNM format only supports unsigned integer data types (UNORM or UINT).");

        if (is_1bit && bit_depth != 8)
            return _log_unsupported_format_error(target_name, u8"The PBM format only supports a bit depth of 8.");
        if (bit_depth != 8 && bit_depth != 16)
            return _log_unsupported_format_error(target_name, u8"The Netpbm/PNM format only supports uniform bit depths of 8 or 16 (PGM/PPM).");

        cl7::io::AsciiWriter ascii_writer{&writable};

        ascii_writer.write_char('P');
        ascii_writer.write_char(magic2);
        ascii_writer.write_line("");

        ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>(image.get_width()));
        ascii_writer.write_char(' ');
        ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>(image.get_height()));
        ascii_writer.write_line("");

        if (!is_1bit)
        {
            ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>((1 << bit_depth) - 1));
            ascii_writer.write_line("");
        }

        size_t data_size = 0;

        if (is_1bit && is_ascii)
            data_size = _write_1bit_ascii(writable, target_name, image);
        else if (is_1bit)
            data_size = _write_1bit_binary(writable, target_name, image);
        else if (is_ascii)
            data_size = _write_ascii(writable, target_name, image);
        else
            data_size = _write_binary(writable, target_name, image);

        if (data_size != image.get_desc().calculate_data_size())
            return _log_bad_data_error(target_name);

        return true;
    }

    /**
     * Writes the PAM format.
     */
    bool NetpbmImageWriter::_write_pam(const Image& image, cl7::io::IWritable& writable, const cl7::u8string& target_name)
    {
        PixelLayout pixel_layout{image.get_pixel_format(), image.get_channel_order()};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count >= 1 && channel_count <= 4);

        if (pixel_layout.data_type != PixelLayout::DataType::UNORM && pixel_layout.data_type != PixelLayout::DataType::UINT)
            return _log_unsupported_format_error(target_name, u8"The Netpbm/PAM format only supports unsigned integer data types (UNORM or UINT).");

        if (bit_depth != 8 && bit_depth != 16)
            return _log_unsupported_format_error(target_name, u8"The Netpbm/PAM format only supports uniform bit depths of 8 or 16.");

        cl7::io::AsciiWriter ascii_writer{&writable};

        ascii_writer.write_line("P7");

        ascii_writer.write("WIDTH");
        ascii_writer.write_char(' ');
        ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>(image.get_width()));
        ascii_writer.write_line("");

        ascii_writer.write("HEIGHT");
        ascii_writer.write_char(' ');
        ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>(image.get_height()));
        ascii_writer.write_line("");

        ascii_writer.write("DEPTH");
        ascii_writer.write_char(' ');
        ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>(channel_count));
        ascii_writer.write_line("");

        ascii_writer.write("MAXVAL");
        ascii_writer.write_char(' ');
        ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>((1 << bit_depth) - 1));
        ascii_writer.write_line("");

        ascii_writer.write("TUPLTYPE");
        ascii_writer.write_char(' ');
        switch (channel_count)
        {
            // We deliberately do not support BLACKANDWHITE/BLACKANDWHITE_ALPHA.
            case 1: ascii_writer.write("GRAYSCALE"); break;
            case 2: ascii_writer.write("GRAYSCALE_ALPHA"); break;
            case 3: ascii_writer.write("RGB"); break;
            case 4: ascii_writer.write("RGB_ALPHA"); break;
            default: assert(false);
        }
        ascii_writer.write_line("");

        ascii_writer.write_line("ENDHDR");

        const size_t data_size = _write_binary(writable, target_name, image);

        if (data_size != image.get_desc().calculate_data_size())
            return _log_bad_data_error(target_name);

        return true;
    }



    size_t NetpbmImageWriter::_write_1bit_ascii(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image)
    {
        const size_t pixel_count = static_cast<size_t>(image.get_width()) * static_cast<size_t>(image.get_height());
        cl7::byte_view data = image.get_data();
        assert(data.size() == pixel_count);

        const size_t line_pixel_count = std::min(image.get_width(), 70u);
        const bool insert_spaces = line_pixel_count <= 35;

        cl7::io::AsciiWriter ascii_writer{&writable};
        size_t written_count = 0;

        size_t k = 0;
        for (size_t i = 0; i < pixel_count; ++i)
        {
            const auto byte = data[i];
            const auto chr = static_cast<uint8_t>(byte) > 0 ? '0' : '1'; // 1 is black, 0 is white.
            written_count += ascii_writer.write_char(chr);

            if (++k >= line_pixel_count)
            {
                k = 0;
                ascii_writer.write_line("");
            }
            else if (insert_spaces)
            {
                ascii_writer.write_char(' ');
            }
        } // for each pixel

        return written_count;
    }

    size_t NetpbmImageWriter::_write_1bit_binary(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image)
    {
        static constexpr bool zero_is_white = true;

        const auto width = static_cast<size_t>(image.get_width());
        const auto height = static_cast<size_t>(image.get_height());
        const size_t pixel_count = width * height;
        cl7::byte_view data = image.get_data();
        assert(data.size() == pixel_count);

        const size_t bytes_per_row = (width + 7) / 8;
        const size_t byte_count = bytes_per_row * height;
        cl7::byte_vector bits(byte_count);

        uint8_t buf = 0;
        for (size_t i = 0; i < pixel_count; ++i)
        {
            const size_t col = i % width;
            const size_t row = i / width;

            const auto byte = data[i];
            uint8_t bit = static_cast<uint8_t>(byte) > 0 ? 1 : 0;
            if constexpr (zero_is_white)
                bit = 1 - bit;
            buf |= static_cast<uint8_t>(bit << (7 - (col % 8)));

            if (col + 1 == width || i + 1 == pixel_count)
            {
                bits[row * bytes_per_row + col / 8] = std::byte{buf};
                buf = 0;
            }
        } // for each pixel

        cl7::io::ByteWriter byte_writer{&writable};

        const size_t written_count = byte_writer.write_bytes(bits);

        assert(written_count == byte_count || written_count * 8 <= pixel_count);
        return written_count == byte_count ? pixel_count : written_count * 8;
    }

    size_t NetpbmImageWriter::_write_ascii(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image)
    {
        PixelLayout pixel_layout{image.get_pixel_format(), image.get_channel_order()};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count == 1 || channel_count == 3);
        assert(bit_depth == 8 || bit_depth == 16);

        const size_t pixel_count = static_cast<size_t>(image.get_width()) * static_cast<size_t>(image.get_height());
        cl7::byte_view data = image.get_data();
        assert(data.size() == pixel_count * channel_count * bit_depth / 8);

        const unsigned digit_count = bit_depth == 8 ? 3 : 5; // maxval is 255 or 65535

        cl7::io::AsciiWriter ascii_writer{&writable};
        size_t written_count = 0;

        for (size_t i = 0; i < pixel_count; ++i)
        {
            for (size_t k = 0; k < channel_count; ++k)
            {
                const unsigned semantic_index = pixel_layout.semantic_involved[k];
                const auto& channel = pixel_layout.channels[semantic_index];
                const unsigned logical_index = channel.index;
                assert(logical_index < channel_count);

                if (k > 0)
                    ascii_writer.write_char(' ');

                unsigned value;
                if (bit_depth == 8)
                    value = static_cast<unsigned>(reinterpret_cast<const uint8_t*>(data.data())[i * channel_count + logical_index]);
                else // => bit_depth == 16
                    value = static_cast<unsigned>(reinterpret_cast<const uint16_t*>(data.data())[i * channel_count + logical_index]);
                written_count += ascii_writer.write(cl7::text::format::to_string<cl7::astring_view>(value, digit_count));
            } // for each channel

            ascii_writer.write_line("");
        } // for each pixel

        return (written_count / digit_count) * (bit_depth / 8);
    }

    size_t NetpbmImageWriter::_write_binary(cl7::io::IWritable& writable, const cl7::u8string& target_name, const Image& image)
    {
        PixelLayout pixel_layout{image.get_pixel_format(), image.get_channel_order()};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count >= 1 && channel_count <= 4);
        assert(bit_depth == 8 || bit_depth == 16);

        const size_t pixel_count = static_cast<size_t>(image.get_width()) * static_cast<size_t>(image.get_height());
        cl7::byte_view data = image.get_data();
        assert(data.size() == pixel_count * channel_count * bit_depth / 8);

        cl7::io::ByteWriter byte_writer{&writable};
        size_t written_count = 0;

        for (size_t i = 0; i < pixel_count; ++i)
        {
            for (size_t k = 0; k < channel_count; ++k)
            {
                const unsigned semantic_index = pixel_layout.semantic_involved[k];
                const auto& channel = pixel_layout.channels[semantic_index];
                const unsigned logical_index = channel.index;
                assert(logical_index < channel_count);

                if (bit_depth == 8)
                {
                    auto value = reinterpret_cast<const uint8_t*>(data.data())[i * channel_count + logical_index];
                    written_count += byte_writer.write_scalar<std::endian::big>(value);
                }
                else // => bit_depth == 16
                {
                    auto value = reinterpret_cast<const uint16_t*>(data.data())[i * channel_count + logical_index];
                    written_count += byte_writer.write_scalar<std::endian::big>(value);
                }
            } // for each channel
        } // for each pixel

        return written_count;
    }



} // namespace xl7::graphics::images
