#include "NetpbmImageReader.h"

#include "../PixelLayout.h"

#include <CoreLabs/io/AsciiReader.h>
#include <CoreLabs/io/ByteReader.h>

#include <CoreLabs/text/format.h>
#include <CoreLabs/logging.h>



namespace xl7::graphics::images {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any readable object.
     */
    bool NetpbmImageReader::_load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        if (ascii_reader.read_char() != 'P')
            return _log_bad_header_error(source_name, u8"'P' expected");

        const auto magic2 = ascii_reader.read_char();
        if (!std::isdigit(magic2))
            return _log_bad_header_error(source_name, u8"digit 1-7 expected");

        if (magic2 >= '1' && magic2 <= '6')
        {
            if (!_load_pnm(readable, source_name, image))
                return false;
        }
        else if (magic2 == '7')
        {
            if (!_load_pam(readable, source_name, image))
                return false;
        }
        else
            return _log_unknown_format_error(source_name, u8"bad magic number: P" + magic2);

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Parses any PBM, PGM, or PPM format.
     */
    bool NetpbmImageReader::_load_pnm(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        readable.seek_read(-1);
        const auto magic2 = ascii_reader.read_char();
        assert(magic2 >= '1' && magic2 <= '6');

        const bool is_ascii = magic2 >= '1' && magic2 <= '3';
        const bool is_binary = magic2 >= '4' && magic2 <= '6';
        const bool is_1bit = magic2 == '1' || magic2 == '4';
        const unsigned channel_count = magic2 == '3' || magic2 == '6' ? 3 : 1;
        assert((is_ascii || is_binary) && !(is_ascii && is_binary));

        Image::Desc desc;
        desc.pixel_format = PixelFormat::UNKNOWN;
        desc.channel_order = ChannelOrder::RGBA;
        desc.width = 0;
        desc.height = 0;
        desc.depth = 1;

        unsigned max_val = 0;

        _skip_comments(readable);
        desc.width = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
        _skip_comments(readable);
        desc.height = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
        if (is_1bit)
            max_val = 1;
        else
        {
            _skip_comments(readable);
            max_val = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
        }

        if (!std::isspace(ascii_reader.read_char()))
            return _log_bad_header_error(source_name, u8"whitespace character expected");

        if (!desc.width || !desc.height)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");
        if (desc.width > Image::MAX_SIZE || desc.height > Image::MAX_SIZE)
            return _log_bad_header_error(source_name, u8"valid width and height not greater than " + cl7::to_string(Image::MAX_SIZE) + u8" expected");

        if (max_val == 0 || max_val > 65535)
            return _log_bad_header_error(source_name, u8"valid bit depth of 8 or 16 expected (maximum value greater than 0 and less than 65536)");

        cl7::byte_vector data;

        if (channel_count == 1)
            desc.pixel_format = max_val <= 255 ? PixelFormat::R8_UNORM : PixelFormat::R16_UNORM;
        else // => channel_count == 3
            desc.pixel_format = max_val <= 255 ? PixelFormat::R8G8B8_UNORM : PixelFormat::R16G16B16_UNORM;

        if (is_1bit && is_ascii)
            data = _read_1bit_ascii(readable, source_name, desc);
        else if (is_1bit)
            data = _read_1bit_binary(readable, source_name, desc);
        else if (is_ascii)
            data = _read_ascii(readable, source_name, desc);
        else
            data = _read_binary(readable, source_name, desc);

        if (data.size() != desc.calculate_data_size())
            return _log_bad_data_error(source_name);

        _apply_max_val(desc, data, max_val);

        return image.init(desc, std::move(data));
    }

    /**
     * Parses the PAM format.
     */
    bool NetpbmImageReader::_load_pam(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        Image::Desc desc;
        desc.pixel_format = PixelFormat::UNKNOWN;
        desc.channel_order = ChannelOrder::RGBA;
        desc.width = 0;
        desc.height = 0;
        desc.depth = 1;

        unsigned channel_count = 0;
        unsigned max_val = 0;
        cl7::astring tuple_type;

        while (true)
        {
            _skip_comments(readable);

            const auto token = ascii_reader.read_token();
            if (token == "ENDHDR")
                break;

            if (token == "WIDTH")
                desc.width = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
            else if (token == "HEIGHT")
                desc.height = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
            else if (token == "DEPTH")
                channel_count = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
            else if (token == "MAXVAL")
                max_val = cl7::text::format::parse_int<unsigned>(ascii_reader.read_token());
            else if (token == "TUPLTYPE")
                tuple_type.append(ascii_reader.read_token());
        } // for each header line

        if (ascii_reader.read_char() != '\n')
            return _log_bad_header_error(source_name, u8"newline character \\n expected");

        if (!desc.width || !desc.height)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");
        if (desc.width > Image::MAX_SIZE || desc.height > Image::MAX_SIZE)
            return _log_bad_header_error(source_name, u8"valid width and height not greater than " + cl7::to_string(Image::MAX_SIZE) + u8" expected");

        if (max_val == 0 || max_val > 65535)
            return _log_bad_header_error(source_name, u8"valid bit depth of 8 or 16 expected (maximum value greater than 0 and less than 65536)");

        cl7::byte_vector data;

        if (tuple_type == "BLACKANDWHITE")
        {
            if (max_val != 1)
                return _log_bad_header_error(source_name, u8"BLACKANDWHITE requires a maximum value of 1");
            if (channel_count < 1)
                return _log_bad_header_error(source_name, u8"BLACKANDWHITE requires a channel count (depth) of 1");
            desc.pixel_format = PixelFormat::R8_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "GRAYSCALE")
        {
            if (channel_count < 1)
                return _log_bad_header_error(source_name, u8"GRAYSCALE requires a channel count (depth) of 1");
            desc.pixel_format = max_val <= 255 ? PixelFormat::R8_UNORM : PixelFormat::R16_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "RGB")
        {
            if (channel_count < 3)
                return _log_bad_header_error(source_name, u8"RGB requires a channel count (depth) of 3");
            desc.pixel_format = max_val <= 255 ? PixelFormat::R8G8B8_UNORM : PixelFormat::R16G16B16_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "BLACKANDWHITE_ALPHA")
        {
            // We deliberately do not support this.
            return _log_unsupported_format_error(source_name, u8"BLACKANDWHITE_ALPHA is not supported");
        }
        else if (tuple_type == "GRAYSCALE_ALPHA")
        {
            if (channel_count < 2)
                return _log_bad_header_error(source_name, u8"GRAYSCALE_ALPHA requires a channel count (depth) of 2");
            desc.pixel_format = max_val <= 255 ? PixelFormat::R8G8_UNORM : PixelFormat::R16G16_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "RGB_ALPHA")
        {
            if (channel_count < 4)
                return _log_bad_header_error(source_name, u8"RGB_ALPHA requires a channel count (depth) of 4");
            desc.pixel_format = max_val <= 255 ? PixelFormat::R8G8B8A8_UNORM : PixelFormat::R16G16B16A16_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else
            return _log_unknown_format_error(source_name, cl7::text::codec::to_utf8(tuple_type));

        if (data.size() != desc.calculate_data_size())
            return _log_bad_data_error(source_name);

        _apply_max_val(desc, data, max_val);

        return image.init(desc, std::move(data));
    }



    cl7::byte_vector NetpbmImageReader::_read_1bit_ascii(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        const size_t pixel_count = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.height);
        cl7::byte_vector data(pixel_count);

        cl7::io::AsciiReader ascii_reader{&readable};

        for (size_t i = 0; i < pixel_count; )
        {
            const auto chr = ascii_reader.read_char();
            if (std::isspace(chr))
                continue;

            // 1 is black, 0 is white.
            if (chr == '0')
                data[i++] = std::byte{1};
            else if (chr == '1')
                data[i++] = std::byte{0};
            else
            {
                return {};
            }
        } // for each pixel

        return data;
    }

    cl7::byte_vector NetpbmImageReader::_read_1bit_binary(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        static constexpr bool zero_is_white = true;

        const auto width = static_cast<size_t>(desc.width);
        const auto height = static_cast<size_t>(desc.height);
        const size_t pixel_count = width * height;
        const size_t bytes_per_row = (width + 7) / 8;
        const size_t byte_count = bytes_per_row * height;

        cl7::io::ByteReader byte_reader{&readable};

        auto bits =  byte_reader.read_all();

        if (bits.size() != byte_count)
        {
            return {};
        }

        cl7::byte_vector data(pixel_count);

        for (size_t i = 0; i < pixel_count; ++i)
        {
            const size_t col = i % width;
            const size_t row = i / width;

            const std::byte byte = bits[row * bytes_per_row + col / 8];
            uint8_t bit = (static_cast<uint8_t>(byte) >> (7 - (col % 8))) & 0x1;
            if constexpr (zero_is_white)
                bit = 1 - bit;
            data[i] = std::byte{bit};
        } // for each pixel

        return data;
    }

    cl7::byte_vector NetpbmImageReader::_read_ascii(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        PixelLayout pixel_layout{desc.pixel_format, desc.channel_order};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count == 1 || channel_count == 3);
        assert(bit_depth == 8 || bit_depth == 16);

        const unsigned max_val = bit_depth == 8 ? 255 : 65535;
        const unsigned max_val_length = bit_depth == 8 ? 3 : 5;

        const size_t pixel_count = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.height);
        const size_t data_size = pixel_count * channel_count * bit_depth / 8;
        assert(data_size == desc.calculate_data_size());
        cl7::byte_vector data(data_size);

        cl7::io::AsciiReader ascii_reader{&readable};

        for (size_t i = 0; i < pixel_count; ++i)
        {
            for (size_t k = 0; k < channel_count; ++k)
            {
                const unsigned semantic_index = pixel_layout.semantic_involved[k];
                const auto& channel = pixel_layout.channels[semantic_index];
                const unsigned logical_index = channel.index;
                assert(logical_index == k);

                const auto token = ascii_reader.read_token();
                if (token.empty() || token.length() > static_cast<size_t>(max_val_length))
                {
                    return {};
                }

                unsigned value = 0;
                for (auto chr : token)
                {
                    if (!std::isdigit(chr))
                    {
                        return {};
                    }

                    value *= 10;
                    value += static_cast<unsigned>(chr - '0');
                }

                if (value > max_val)
                {
                    return {};
                }

                if (bit_depth == 8)
                    reinterpret_cast<uint8_t*>(data.data())[i * channel_count + logical_index] = value;
                else // => bit_depth == 16
                    reinterpret_cast<uint16_t*>(data.data())[i * channel_count + logical_index] = value;
            } // for each channel
        } // for each pixel

        return data;
    }

    cl7::byte_vector NetpbmImageReader::_read_binary(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        PixelLayout pixel_layout{desc.pixel_format, desc.channel_order};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count >= 1 && channel_count <= 4);
        assert(bit_depth == 8 || bit_depth == 16);

        const size_t pixel_count = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.height);
        const size_t data_size = pixel_count * channel_count * bit_depth / 8;
        assert(data_size == desc.calculate_data_size());
        cl7::byte_vector data(data_size);

        cl7::io::ByteReader byte_reader{&readable};

        for (size_t i = 0; i < pixel_count; ++i)
        {
            for (size_t k = 0; k < channel_count; ++k)
            {
                const unsigned semantic_index = pixel_layout.semantic_involved[k];
                const auto& channel = pixel_layout.channels[semantic_index];
                const unsigned logical_index = channel.index;
                assert(logical_index == k);

                if (bit_depth == 8)
                {
                    uint8_t value;
                    if (byte_reader.read_scalar<std::endian::big>(value) != sizeof(value))
                        return {};
                    reinterpret_cast<uint8_t*>(data.data())[i * channel_count + logical_index] = value;
                }
                else // => bit_depth == 16
                {
                    uint16_t value;
                    if (byte_reader.read_scalar<std::endian::big>(value) != sizeof(value))
                        return {};
                    reinterpret_cast<uint16_t*>(data.data())[i * channel_count + logical_index] = value;
                }
            } // for each channel
        } // for each pixel

        return data;
    }



    void NetpbmImageReader::_apply_max_val(const Image::Desc& desc, cl7::byte_vector& data, unsigned max_val)
    {
        PixelLayout pixel_layout{desc.pixel_format, desc.channel_order};
        const unsigned channel_count = pixel_layout.channel_count;
        const unsigned bit_depth = pixel_layout.uniform_depth;

        assert(channel_count >= 1 && channel_count <= 4);
        assert(bit_depth == 8 || bit_depth == 16);

        const size_t pixel_count = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.height);
        assert(data.size() == pixel_count * channel_count * bit_depth / 8);

        const size_t element_count = pixel_count * channel_count;
        assert(data.size() == element_count * bit_depth / 8);

        if (bit_depth == 8)
        {
            assert(max_val > 0 && max_val < 256);
            for (size_t j = 0; j < element_count; ++j)
            {
                auto value = static_cast<unsigned>(reinterpret_cast<const uint8_t*>(data.data())[j]);
                value *= 255;
                value /= max_val;
                reinterpret_cast<uint8_t*>(data.data())[j] = static_cast<uint8_t>(value);
            } // for each element
        }
        else // => bit_depth == 16
        {
            assert(max_val > 0 && max_val < 65536);
            for (size_t j = 0; j < element_count; ++j)
            {
                auto value = static_cast<unsigned>(reinterpret_cast<const uint16_t*>(data.data())[j]);
                value *= 65535;
                value /= max_val;
                reinterpret_cast<uint16_t*>(data.data())[j] = static_cast<uint16_t>(value);
            } // for each element
        } // bit_depth
    }



    void NetpbmImageReader::_skip_comments(cl7::io::IReadable& readable)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        while (true)
        {
            while (std::isspace(ascii_reader.peek_char()))
                ascii_reader.read_char();

            auto first_char = ascii_reader.peek_char();
            if (first_char != '#')
                return;

            ascii_reader.read_line();
        }
    }



} // namespace xl7::graphics::images
