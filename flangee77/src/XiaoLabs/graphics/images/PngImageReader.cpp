#include "PngImageReader.h"

#include <DataLabs/compression.h>

#include <CoreLabs/logging.h>
#include <CoreLabs/bits.h>



namespace xl7::graphics::images {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any readable object.
     */
    bool PngImageReader::_load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
    {
        Signature signature;
        if (readable.read({reinterpret_cast<std::byte*>(&signature), sizeof(Signature)}) != sizeof(Signature))
            return _log_bad_header_error(source_name, u8"bad signature length");

        if (signature._x89 != 0x89)
            return _log_bad_header_error(source_name, u8"bad signature");
        if (signature.png[0] != 'P' || signature.png[1] != 'N' || signature.png[2] != 'G')
            return _log_bad_header_error(source_name, u8"bad signature");
        if (signature.crlf[0] != 0x0d || signature.crlf[1] != 0x0a)
            return _log_bad_header_error(source_name, u8"bad signature");
        if (signature.eof != 0x1a)
            return _log_bad_header_error(source_name, u8"bad signature");
        if (signature.lf != 0x0a)
            return _log_bad_header_error(source_name, u8"bad signature");

        BitInfo bit_info = {};
        std::vector<PaletteEntry> palette;

        cl7::byte_vector temp;
        cl7::byte_vector temp2;

        if (!_process_chunks(readable, source_name, bit_info, palette, temp))
            return false; // An error message has already been logged.

        if (!_decompress(temp, temp2))
            return _log_bad_data_error(source_name, u8"decompression error");
        if (!_reconstruct(temp2, temp, bit_info))
            return _log_bad_data_error(source_name, u8"reconstruction error");

        assert(bit_info.channel_count > 0);
        assert(bit_info.bits_per_pixel % bit_info.channel_count == 0);
        const unsigned bits_per_channel = bit_info.bits_per_pixel / bit_info.channel_count;

        if (bits_per_channel != 8)
        {
            temp2.swap(temp);
            if (!_decode(std::move(temp2), temp, bit_info))
                return _log_bad_data_error(source_name, u8"decoding error");
        }

        cl7::byte_vector buffer = std::move(temp);

        //
        static constexpr PixelFormat _8BIT_PIXEL_FORMATS_BY_COLOR_TYPE[7] = {
            PixelFormat::R8_UNORM,          // CT 0: Grayscale
            PixelFormat::UNKNOWN,           // CT 1: (invalid)
            PixelFormat::R8G8B8_UNORM,      // CT 2: Truecolor
            PixelFormat::R8G8B8_UNORM,      // CT 3: Indexed-color
            PixelFormat::R8G8_UNORM,        // CT 4: Grayscale with Alpha
            PixelFormat::UNKNOWN,           // CT 5: (invalid)
            PixelFormat::R8G8B8A8_UNORM,    // CT 6: Truecolor with Alpha
        };

        //
        static constexpr PixelFormat _16BIT_PIXEL_FORMATS_BY_COLOR_TYPE[7] = {
            PixelFormat::R16_UNORM,         // CT 0: Grayscale
            PixelFormat::UNKNOWN,           // CT 1: (invalid)
            PixelFormat::R16G16B16_UNORM,   // CT 2: Truecolor
            PixelFormat::R16G16B16_UNORM,   // CT 3: Indexed-color
            PixelFormat::R16G16_UNORM,      // CT 4: Grayscale with Alpha
            PixelFormat::UNKNOWN,           // CT 5: (invalid)
            PixelFormat::R16G16B16A16_UNORM,// CT 6: Truecolor with Alpha
        };

        const auto* pixel_formats_by_color_type = bits_per_channel > 8
            ? _16BIT_PIXEL_FORMATS_BY_COLOR_TYPE
            : _8BIT_PIXEL_FORMATS_BY_COLOR_TYPE;

        Image::Desc desc;
        desc.pixel_format = pixel_formats_by_color_type[bit_info.color_type];
        desc.channel_order = ChannelOrder::RGBA;
        desc.width = bit_info.width;
        desc.height = bit_info.height;
        desc.depth = 1;

        cl7::byte_vector data(desc.calculate_data_size());

        const unsigned src_bytes_per_pixel = bit_info.ceil_bytes_per_pixel;
        const unsigned dst_bytes_per_pixel = desc.determine_pixel_stride();
        const size_t pixel_count = desc.calculate_pixel_count();
        assert(pixel_count * src_bytes_per_pixel == buffer.size());

        if (pixel_count * src_bytes_per_pixel != buffer.size())
            return _log_bad_data_error(source_name, u8"");

        if (bit_info.color_type == 3) // CT 3: Indexed-color
        {
            // If Indexed-color, iterate through the pixels
            // and get the color values from the palette.
            assert(src_bytes_per_pixel == 1);
            assert(dst_bytes_per_pixel == 3);
            for (size_t i = 0; i < pixel_count; ++i)
            {
                const size_t si = static_cast<uint8_t>(buffer[i]);
                const size_t di = i * dst_bytes_per_pixel;
                data[di + 0] = palette[si].r;
                data[di + 1] = palette[si].g;
                data[di + 2] = palette[si].b;
            }
        }
        else
        {
            // Otherwise, just "copy" the raw data.
            assert(dst_bytes_per_pixel == src_bytes_per_pixel);
            assert(data.size() == buffer.size());
            data.swap(buffer);
        }

        return image.init(desc, std::move(data));
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Processes the PNG chunks.
     */
    bool PngImageReader::_process_chunks(cl7::io::IReadable& readable, const cl7::u8string& source_name, BitInfo& bit_info, std::vector<PaletteEntry>& palette, cl7::byte_vector& data)
    {
        while (true)
        {
            ChunkInfo chunk_info;
            if (readable.read({reinterpret_cast<std::byte*>(&chunk_info), sizeof(ChunkInfo)}) != sizeof(ChunkInfo))
                return _log_bad_header_error(source_name, u8"unable to read chunk info");

            chunk_info.length = cl7::bits::swap_bytes_unless_endian<std::endian::big>(chunk_info.length);

            if (std::strncmp(chunk_info.type, "IHDR", 4) == 0)
            {
                if (!_process_IHDR_chunk(readable, source_name, chunk_info.length, bit_info))
                    return false;
            }
            else if (std::strncmp(chunk_info.type, "PLTE", 4) == 0)
            {
                if (!_process_PLTE_chunk(readable, source_name, chunk_info.length, palette))
                    return false;
            }
            else if (std::strncmp(chunk_info.type, "IDAT", 4) == 0)
            {
                if (!_process_IDAT_chunk(readable, source_name, chunk_info.length, data))
                    return false;
            }
            else if (std::strncmp(chunk_info.type, "IEND", 4) == 0)
            {
                break;
            }
            else
            {
                // This is just a (temporary?) workaround:
                // we skip <chunk_length> bytes of data.
                readable.seek_read(chunk_info.length);
            }

            uint32_t crc;
            if (readable.read({reinterpret_cast<std::byte*>(&crc), 4}) != 4)
                return _log_bad_data_error(source_name, u8"CRC mismatch");
            // Should we also specifically verify
            // the content of the check value?
        } // for each chunk

        return true;
    }

    /**
     * Processes the image header chunk, "IHDR".
     */
    bool PngImageReader::_process_IHDR_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, uint32_t chunk_length, BitInfo& bit_info)
    {
        if (chunk_length != sizeof(Header))
            return _log_bad_header_error(source_name, u8"bad IHDR chunk length");

        Header header;
        if (readable.read({reinterpret_cast<std::byte*>(&header), sizeof(Header)}) != sizeof(Header))
            return _log_bad_data_error(source_name, u8"bad IHDR chunk data");

        header.width = cl7::bits::swap_bytes_unless_endian<std::endian::big>(header.width);
        header.height = cl7::bits::swap_bytes_unless_endian<std::endian::big>(header.height);

        if (header.width == 0 || header.height == 0)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");
        if (header.width > Image::MAX_SIZE || header.height > Image::MAX_SIZE)
            return _log_bad_header_error(source_name, u8"valid width and height not greater than " + cl7::to_string(Image::MAX_SIZE) + u8" expected");
        if (std::popcount(header.bit_depth) != 1 || header.bit_depth > 16)
            return _log_bad_header_error(source_name, u8"invalid bit depth: " + cl7::to_string(header.bit_depth));
        if (header.color_type == 1 || header.color_type == 5 || header.color_type >= 7)
            return _log_bad_header_error(source_name, u8"invalid color type: " + cl7::to_string(header.color_type));
        if (header.compression_method != 0)
            return _log_unsupported_format_error(source_name, u8"unsupported compression method: " + cl7::to_string(header.compression_method));
        if (header.filter_method != 0)
            return _log_unsupported_format_error(source_name, u8"unsupported filter method: " + cl7::to_string(header.filter_method));
        if (header.interlace_method > 1)
            return _log_unsupported_format_error(source_name, u8"unsupported interlace method: " + cl7::to_string(header.interlace_method));

        if (header.color_type == 0) // CT 0: Grayscale
        {
            // Allowed bit depths: 1, 2, 4, 8, 16 (already checked, see above)
        }
        else if (header.color_type == 3) // CT 3: Indexed-color
        {
            // Allowed bit depths: 1, 2, 4, 8
            if (header.bit_depth > 8)
                return _log_bad_header_error(source_name, u8"invalid bit depth of " + cl7::to_string(header.bit_depth) + u8" for color type " + cl7::to_string(header.color_type));
        }
        else // CT 2: Truecolor, CT 4: Grayscale with Alpha, or CT 6: Truecolor with Alpha
        {
            assert(header.color_type == 2 || header.color_type == 4 || header.color_type == 6);
            // Allowed bit depths: 8, 16
            if (header.bit_depth != 8 && header.bit_depth != 16)
                return _log_bad_header_error(source_name, u8"invalid bit depth of " + cl7::to_string(header.bit_depth) + u8" for color type " + cl7::to_string(header.color_type));
        }

        //
        static constexpr unsigned CHANNEL_COUNTS_BY_COLOR_TYPE[7] = {
            1, // CT 0: Grayscale
            0, // CT 1: (invalid)
            3, // CT 2: Truecolor
            1, // CT 3: Indexed-color
            2, // CT 4: Grayscale with Alpha
            0, // CT 5: (invalid)
            4, // CT 6: Truecolor with Alpha
        };

        bit_info.color_type = static_cast<unsigned>(header.color_type);
        bit_info.channel_count = CHANNEL_COUNTS_BY_COLOR_TYPE[header.color_type];
        bit_info.bits_per_pixel = header.bit_depth * bit_info.channel_count;
        bit_info.ceil_bytes_per_pixel = (bit_info.bits_per_pixel + 7) / 8;
        bit_info.bytes_per_scanline = (header.width * bit_info.bits_per_pixel + 7) / 8;
        bit_info.width = header.width;
        bit_info.height = header.height;

        assert(bit_info.channel_count > 0);

        return true;
    }

    /**
     * Processes the palette chunk, "PLTE".
     */
    bool PngImageReader::_process_PLTE_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, uint32_t chunk_length, std::vector<PaletteEntry>& palette)
    {
        if (chunk_length % 3 != 0)
            return _log_bad_header_error(source_name, u8"bad PLTE chunk length");

        static_assert(sizeof(PaletteEntry) == 3);
        const size_t number_of_entries = chunk_length / 3;

        palette.resize(number_of_entries);

        if (readable.read({reinterpret_cast<std::byte*>(palette.data()), number_of_entries * 3}) != static_cast<size_t>(chunk_length))
            return _log_bad_data_error(source_name, u8"bad PLTE chunk data");

        return true;
    }

    /**
     * Processes the image data chunk, "IDAT".
     */
    bool PngImageReader::_process_IDAT_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, uint32_t chunk_length, cl7::byte_vector& data)
    {
        if (chunk_length == 0)
            return true;

        const size_t data_offset = data.size();
        const size_t data_length = chunk_length;

        data.resize(data_offset + data_length);

        if (readable.read(cl7::make_byte_span(data.data() + data_offset, data_length)) != data_length)
            return _log_bad_data_error(source_name, u8"bad IDAT chunk data");

        return true;
    }

    /**
     * Decompresses the given source data.
     */
    bool PngImageReader::_decompress(cl7::byte_view src, cl7::byte_vector& dst)
    {
        dst.clear();

        return dl7::compression::Deflate::decompress(src, dst);
    }

    /**
     * Reconstructs the target data from the given filtered source data.
     */
    bool PngImageReader::_reconstruct(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info)
    {
        const auto bytes_per_pixel = static_cast<size_t>(bit_info.ceil_bytes_per_pixel);
        const auto bytes_per_scanline = static_cast<size_t>(bit_info.bytes_per_scanline);
        const auto height = static_cast<size_t>(bit_info.height);

        const size_t expected_src_size = height * (bytes_per_scanline + 1);
        assert(src.size() == expected_src_size);
        if (src.size() != expected_src_size)
            return false;

        const size_t dst_size = height * bytes_per_scanline;
        dst.clear();
        dst.resize(dst_size);

        size_t si = 0;
        size_t di = 0;

        for (size_t row = 0; row < height; ++row)
        {
            assert(si < src.size());

            // Read and "skip" filter type byte.
            const auto filter_type = static_cast<uint8_t>(src[si++]);

            assert(si + bytes_per_scanline <= src.size());

            for (size_t col = 0; col < bytes_per_scanline; ++col)
            {
                const auto fx = static_cast<uint8_t>(src[si + col]);

                uint8_t a = 0; // sub/left
                uint8_t b = 0; // up
                uint8_t c = 0; // average up-left

                if (col >= bytes_per_pixel)
                    a = static_cast<uint8_t>(dst[di + col - bytes_per_pixel]);

                if (row > 0)
                    b = static_cast<uint8_t>(dst[di + col - bytes_per_scanline]);

                if (row > 0 && col >= bytes_per_pixel)
                    c = static_cast<uint8_t>(dst[di + col - bytes_per_scanline - bytes_per_pixel]);

                uint8_t result = fx;

                switch (filter_type)
                {
                case 1:
                    // Filter type "Sub" (1): the Sub filter transmits the difference between each
                    // byte and the value of the corresponding byte of the prior pixel.
                    result += a; // = fx + a
                    break;
                case 2:
                    // Filter type "Up" (2): the Up filter transmits the difference between each
                    // byte and the value of the corresponding byte of the prior scanline.
                    result += b; // = fx + b
                    break;
                case 3:
                {
                    // Filter type "Average" (3): the Average filter uses the average of the two
                    // neighboring pixels (left and above) to predict the value of a pixel.
                    // The sum shall be performed without overflow (using at least nine-bit arithmetic).
                    const unsigned sum = static_cast<unsigned>(a) + static_cast<unsigned>(b);
                    const unsigned average = sum >> 1;
                    result += static_cast<uint8_t>(average); // = fx + (a+b)/2
                    break;
                }
                case 4:
                    // Filter type "Paeth" (4): the Paeth filter computes a simple linear function
                    // of the three neighboring pixels (left, above, upper left), then chooses as
                    // predictor the neighboring pixel closest to the computed value.
                    result += _paeth(a, b, c); // = fx + PaethPredictor(a, b, c)
                    break;
                default:
                    // Filter type "None" (0): the scanline is transmitted unmodified.
                    // Nothing to do here.
                    break;
                } // switch filter type

                dst[di + col] = std::byte{result};
            } // for each byte in scanline

            si += bit_info.bytes_per_scanline;
            di += bit_info.bytes_per_scanline;
        } // for each scanline

        return true;
    }

    /**
     * Decodes the "normalized" data from the given reconstructed data.
     * No decoding/normalization is required for data with a bit depth of 8 bits per
     * channel.
     */
    bool PngImageReader::_decode(cl7::byte_vector&& src, cl7::byte_vector& dst, const BitInfo& bit_info)
    {
        const size_t pixel_count = static_cast<size_t>(bit_info.width) * static_cast<size_t>(bit_info.height);
        const size_t element_count = pixel_count * bit_info.channel_count;

        assert(bit_info.channel_count > 0);
        assert(bit_info.bits_per_pixel % bit_info.channel_count == 0);
        const unsigned bits_per_channel = bit_info.bits_per_pixel / bit_info.channel_count;

        const size_t bytes_per_row = (static_cast<size_t>(bit_info.width) * bit_info.bits_per_pixel + 7) / 8;

        const size_t src_byte_count = bytes_per_row * static_cast<size_t>(bit_info.height);
        if (src.size() != src_byte_count)
            return false;

        if (bits_per_channel == 8)
        {
            assert(src.size() == element_count);
            dst = std::move(src);
            return true;
        }

        if (bits_per_channel == 16)
        {
            assert(src.size() == element_count * 2);
            dst = std::move(src);
            for (size_t i = 0; i < element_count; ++i)
            {
                uint16_t value = reinterpret_cast<uint16_t*>(dst.data())[i];
                value = cl7::bits::swap_bytes_unless_endian<std::endian::big>(value);
                reinterpret_cast<uint16_t*>(dst.data())[i] = value;
            }
            return true;
        }

        assert(bits_per_channel == 1 || bits_per_channel == 2 || bits_per_channel == 4);
        assert(bit_info.color_type == 0 || bit_info.color_type == 3); // CT 0: Grayscale or CT 3: Indexed-color
        assert(bits_per_channel == bit_info.bits_per_pixel);
        assert(element_count == pixel_count);

        assert(8 % bits_per_channel == 0);
        const unsigned elements_per_pixel = 8 / bits_per_channel;
        const unsigned bit_mask = (1ul << bits_per_channel) - 1;

        dst.clear();
        dst.resize(element_count);

        size_t i = 0;
        for (unsigned y = 0; y < bit_info.height; ++y)
        {
            for (unsigned x = 0; x < bit_info.width; ++x)
            {
                const std::byte byte = src[y * bytes_per_row + x / elements_per_pixel];
                const unsigned bit_offset = (x % elements_per_pixel) * bits_per_channel;
                const unsigned bit_shift = 8 - bits_per_channel - bit_offset;
                uint8_t value = (static_cast<uint8_t>(byte) >> bit_shift) & bit_mask;
                dst[i] = std::byte{value};
                ++i;
            }
        }
        assert(i == element_count);

        if (bit_info.color_type == 0) // CT 0: Grayscale
        {
            const unsigned max_val = (1ul << bits_per_channel) - 1;
            for (i = 0; i < element_count; ++i)
            {
                auto value = static_cast<unsigned>(static_cast<uint8_t>(dst[i]));
                value *= 255;
                value /= max_val;
                dst[i] = std::byte{static_cast<uint8_t>(value)};
            }
        }

        return true;
    }

    /**
     * The Paeth filter function computes a simple linear function of the three
     * neighboring pixels (left, above, upper left), then chooses as predictor the
     * neighboring pixel closest to the computed value.
     */
    uint8_t PngImageReader::_paeth(uint8_t a, uint8_t b, uint8_t c)
    {
        const auto A = static_cast<signed>(a);
        const auto B = static_cast<signed>(b);
        const auto C = static_cast<signed>(c);

        const signed p = A + B - C;
        const signed pA = std::abs(p - A);
        const signed pB = std::abs(p - B);
        const signed pC = std::abs(p - C);

        uint8_t pr;

        if (pA <= pB && pA <= pC)
            pr = a;
        else if (pB <= pC)
            pr = b;
        else
            pr = c;

        return pr;
    }



} // namespace xl7::graphics::images
