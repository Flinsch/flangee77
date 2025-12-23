#include "NetpbmImageReader.h"

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
        const bool is_ascii = magic2 >= '1' && magic2 <= '3';
        const bool is_binary = magic2 >= '4' && magic2 <= '6';
        const bool is_1bit = magic2 == '1' || magic2 == '4';
        const unsigned channel_count = magic2 == '3' || magic2 == '6' ? 3 : 1;
        assert((is_ascii || is_binary) && !(is_ascii && is_binary));

        Image::Desc desc;
        desc.pixel_format = channel_count == 1 ? PixelFormat::R8_UNORM : PixelFormat::R8G8B8_UNORM;
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
            return _log_bad_header_error(source_name, u8"space character expected");

        if (!desc.width || !desc.height)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");

        if (max_val == 0 || max_val > 65535)
            return _log_bad_header_error(source_name, u8"valid bit depth greater than 0 expected");
        if (max_val > 255)
            return _log_unsupported_format_error(source_name, u8"bit depths greater than 8 bits (and therefore maximum values greater than 255) are not supported.");

        cl7::byte_vector data;

        if (is_1bit && is_ascii)
            data = _read_1bit_ascii(readable, source_name, desc);
        else if (is_1bit)
            data = _read_1bit_binary(readable, source_name, desc, true);
        else if (is_ascii)
            data = _read_ascii(readable, source_name, desc);
        else
            data = _read_binary(readable, source_name, desc);

        if (data.size() != desc.calculate_data_size())
            return _log_bad_data_error(source_name);

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

        if (max_val == 0 || max_val > 65535)
            return _log_bad_header_error(source_name, u8"valid bit depth greater than 0 expected");
        if (max_val > 255)
            return _log_unsupported_format_error(source_name, u8"bit depths greater than 8 bits (and therefore maximum values greater than 255) are not supported.");

        cl7::byte_vector data;

        if (tuple_type == "BLACKANDWHITE")
        {
            if (max_val != 1)
                return _log_bad_header_error(source_name, u8"BLACKANDWHITE requires a maximum value of 1");
            if (channel_count < 1)
                return _log_bad_header_error(source_name, u8"BLACKANDWHITE requires a channel count/depth of 1");
            desc.pixel_format = PixelFormat::R8_UNORM;
            data = _read_1bit_binary(readable, source_name, desc, false);
        }
        else if (tuple_type == "GRAYSCALE")
        {
            if (channel_count < 1)
                return _log_bad_header_error(source_name, u8"GRAYSCALE requires a channel count/depth of 1");
            desc.pixel_format = PixelFormat::R8_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "RGB")
        {
            if (channel_count < 3)
                return _log_bad_header_error(source_name, u8"RGB requires a channel count/depth of 3");
            desc.pixel_format = PixelFormat::R8G8B8_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "BLACKANDWHITE_ALPHA")
        {
            // We deliberately do not support this.
            return _log_unsupported_format_error(source_name, u8"BLACKANDWHITE_ALPHA not supported");
        }
        else if (tuple_type == "GRAYSCALE_ALPHA")
        {
            if (channel_count < 2)
                return _log_bad_header_error(source_name, u8"GRAYSCALE_ALPHA requires a channel count/depth of 2");
            desc.pixel_format = PixelFormat::R8G8_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else if (tuple_type == "RGB_ALPHA")
        {
            if (channel_count < 4)
                return _log_bad_header_error(source_name, u8"RGB_ALPHA requires a channel count/depth of 4");
            desc.pixel_format = PixelFormat::R8G8B8A8_UNORM;
            data = _read_binary(readable, source_name, desc);
        }
        else
            return _log_unknown_format_error(source_name, cl7::text::codec::to_utf8(tuple_type));

        if (data.size() != desc.calculate_data_size())
            return _log_bad_data_error(source_name);

        return image.init(desc, std::move(data));
    }



    cl7::byte_vector NetpbmImageReader::_read_1bit_ascii(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        const size_t pixel_count = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.height);
        cl7::byte_vector data(pixel_count);

        for (size_t i = 0; i < pixel_count; )
        {
            const auto chr = ascii_reader.read_char();
            if (std::isspace(chr))
                continue;

            if (chr == '0')
                data[i++] = std::byte{0xff};
            else if (chr == '1')
                data[i++] = std::byte{0x00};
            else
            {
                return {};
            }
        }

        return data;
    }

    cl7::byte_vector NetpbmImageReader::_read_1bit_binary(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc, bool zero_is_white)
    {
        cl7::io::ByteReader byte_reader{&readable};

        const size_t pixel_count = static_cast<size_t>(desc.width) * static_cast<size_t>(desc.height);
        const size_t byte_count = (pixel_count + 7) / 8;

        auto bits =  byte_reader.read_all();

        if (bits.size() != byte_count)
        {
            return {};
        }

        cl7::byte_vector data(pixel_count);

        for (size_t i = 0; i < pixel_count; ++i)
        {
            const std::byte byte = bits[i / 8];
            const uint8_t bit = (static_cast<uint8_t>(byte) >> (7 - (i % 8))) & 0x1;
            const bool is_zero = bit == 0;
            data[i] = is_zero == zero_is_white ? std::byte{0xff} : std::byte{0x00};
        }

        return data;
    }

    cl7::byte_vector NetpbmImageReader::_read_ascii(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        const size_t byte_count = desc.calculate_data_size();
        cl7::byte_vector data(byte_count);

        for (size_t i = 0; i < byte_count; ++i)
        {
            const auto token = ascii_reader.read_token();
            if (token.empty() || token.length() > 3)
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

            if (value > 255)
            {
                return {};
            }

            data[i] = static_cast<std::byte>(value);
        }

        return data;
    }

    cl7::byte_vector NetpbmImageReader::_read_binary(cl7::io::IReadable& readable, const cl7::u8string& source_name, const Image::Desc& desc)
    {
        cl7::io::ByteReader byte_reader{&readable};

        const size_t data_size = desc.calculate_data_size();

        auto data = byte_reader.read_all();

        if (data.size() != data_size)
        {
            return {};
        }

        return data;
    }



    void NetpbmImageReader::_skip_comments(cl7::io::IReadable& readable)
    {
        cl7::io::AsciiReader ascii_reader{&readable};

        while (std::isspace(ascii_reader.peek_char()))
            ascii_reader.read_char();

        auto first_char = ascii_reader.peek_char();
        if (first_char != '#')
            return;

        ascii_reader.read_line();
    }



} // namespace xl7::graphics::images
