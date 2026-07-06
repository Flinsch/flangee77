#include "Reader.h"

#include <DataLabs/compression.h>

#include <CoreLabs/logging.h>
#include <CoreLabs/bits.h>



namespace xl7::graphics::images::codecs::png {



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any readable object.
     */
    bool Reader::_load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image)
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
        if (!FilterReconstructor::reconstruct(temp2, temp, bit_info.ceil_bytes_per_pixel, bit_info.bytes_per_scanline, bit_info.height))
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

        ImageDesc desc;
        desc.pixel_format = pixel_formats_by_color_type[bit_info.color_type];
        desc.channel_order = ChannelOrder::RGBA;
        desc.width = bit_info.width;
        desc.height = bit_info.height;
        desc.depth = 1;

        cl7::byte_vector data(desc.calculate_data_size());

        const unsigned src_bytes_per_pixel = bit_info.ceil_bytes_per_pixel;
        const unsigned dst_bytes_per_pixel = desc.determine_bytes_per_pixel();
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
    bool Reader::_process_chunks(cl7::io::IReadable& readable, const cl7::u8string& source_name, BitInfo& bit_info, std::vector<PaletteEntry>& palette, cl7::byte_vector& data)
    {
        while (true)
        {
            ChunkReader::ChunkHeader chunk_header;
            if (!ChunkReader::read_chunk_header(readable, chunk_header))
                return _log_bad_header_error(source_name, u8"unable to read chunk header");

            if (std::strncmp(chunk_header.type, "IHDR", 4) == 0)
            {
                if (!_process_IHDR_chunk(readable, source_name, chunk_header, bit_info))
                    return false;
            }
            else if (std::strncmp(chunk_header.type, "PLTE", 4) == 0)
            {
                if (!_process_PLTE_chunk(readable, source_name, chunk_header, palette))
                    return false;
            }
            else if (std::strncmp(chunk_header.type, "IDAT", 4) == 0)
            {
                if (!_process_IDAT_chunk(readable, source_name, chunk_header, data))
                    return false;
            }
            else if (std::strncmp(chunk_header.type, "IEND", 4) == 0)
            {
                if (!ChunkReader::skip_chunk_data(readable, chunk_header))
                    return _log_bad_data_error(source_name, u8"unable to read IEND chunk");
                break;
            }
            else
            {
                if (!ChunkReader::skip_chunk_data(readable, chunk_header))
                    return _log_bad_data_error(source_name, u8"unable to skip unknown chunk");
            }
        } // for each chunk

        return true;
    }

    /**
     * Processes the image header chunk, "IHDR".
     */
    bool Reader::_process_IHDR_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, const ChunkReader::ChunkHeader& chunk_header, BitInfo& bit_info)
    {
        cl7::byte_vector data;
        if (!ChunkReader::read_chunk_data(readable, chunk_header, data))
            return _log_bad_data_error(source_name, u8"bad IHDR chunk data");

        if (data.size() != sizeof(Header))
            return _log_bad_header_error(source_name, u8"bad IHDR chunk length");

        const auto& header = *reinterpret_cast<const Header*>(data.data());
        const uint32_t width = cl7::bits::swap_bytes_unless_endian<std::endian::big>(header.width);
        const uint32_t height = cl7::bits::swap_bytes_unless_endian<std::endian::big>(header.height);

        if (width == 0 || height == 0)
            return _log_bad_header_error(source_name, u8"valid width and height greater than 0 expected");
        if (width > Image::MAX_SIZE || height > Image::MAX_SIZE)
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
        bit_info.bytes_per_scanline = (width * bit_info.bits_per_pixel + 7) / 8;
        bit_info.width = width;
        bit_info.height = height;

        assert(bit_info.channel_count > 0);

        return true;
    }

    /**
     * Processes the palette chunk, "PLTE".
     */
    bool Reader::_process_PLTE_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, const ChunkReader::ChunkHeader& chunk_header, std::vector<PaletteEntry>& palette)
    {
        cl7::byte_vector data;
        if (!ChunkReader::read_chunk_data(readable, chunk_header, data))
            return _log_bad_data_error(source_name, u8"bad PLTE chunk data");

        if (data.size() % 3 != 0)
            return _log_bad_header_error(source_name, u8"bad PLTE chunk length");

        static_assert(sizeof(PaletteEntry) == 3);
        const size_t number_of_entries = data.size() / 3;

        palette.assign(
            reinterpret_cast<const PaletteEntry*>(data.data()),
            reinterpret_cast<const PaletteEntry*>(data.data()) + number_of_entries);

        return true;
    }

    /**
     * Processes the image data chunk, "IDAT".
     */
    bool Reader::_process_IDAT_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, const ChunkReader::ChunkHeader& chunk_header, cl7::byte_vector& data)
    {
        if (!ChunkReader::read_chunk_data(readable, chunk_header, data))
            return _log_bad_data_error(source_name, u8"bad IDAT chunk data");

        return true;
    }

    /**
     * Decompresses the given source data.
     */
    bool Reader::_decompress(cl7::byte_view src, cl7::byte_vector& dst)
    {
        dst.clear();

        return dl7::compression::Deflate::decompress(src, dst);
    }

    /**
     * Decodes the "normalized" data from the given reconstructed data.
     * No decoding/normalization is required for data with a bit depth of 8 bits per
     * channel.
     */
    bool Reader::_decode(cl7::byte_vector&& src, cl7::byte_vector& dst, const BitInfo& bit_info)
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



} // namespace xl7::graphics::images::codecs::png
