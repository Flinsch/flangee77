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

        cl7::byte_vector data;

        if (!_process_chunks(readable, source_name, bit_info, palette, data))
            return false; // An error message has already been logged.

        cl7::byte_vector buffer;
        cl7::byte_vector temp;

        if (!_decompress(data, temp))
            return _log_bad_data_error(source_name, u8"decompression error");
        if (!_reconstruct(temp, buffer, bit_info))
            return _log_bad_data_error(source_name, u8"reconstruction error");

        Image::Desc desc;
        if (bit_info.color_type & CT_ALPHA_USED)
            desc.pixel_format = PixelFormat::R8G8B8A8_UNORM;
        else
            desc.pixel_format = PixelFormat::R8G8B8_UNORM;
        desc.channel_order = ChannelOrder::RGBA;
        desc.width = bit_info.width;
        desc.height = bit_info.height;
        desc.depth = 1;

        data.resize(desc.calculate_data_size());

        unsigned src_bytes_per_pixel = bit_info.max_bytes_per_pixel;
        unsigned dst_bytes_per_pixel = desc.determine_pixel_stride();
        size_t N = desc.calculate_pixel_count();
        assert(N * src_bytes_per_pixel == buffer.size());

        // If Indexed-color ...
        if (bit_info.color_type & CT_PALETTE_USED)
        {
            // ... iterate through pixels and get
            // the RGB values from the palette.
            assert(src_bytes_per_pixel == 1);
            assert(dst_bytes_per_pixel == 3);
            for (size_t i = 0; i < N; ++i)
            {
                const size_t si = static_cast<uint8_t>(buffer[i]);
                const size_t di = i * dst_bytes_per_pixel;
                data[di + 0] = palette[si].r;
                data[di + 1] = palette[si].g;
                data[di + 2] = palette[si].b;
            }
        }
        // If "true" Truecolor (with or w/o Alpha) ...
        else if (bit_info.color_type & CT_COLOR_USED)
        {
            // ... just "copy" the raw data.
            assert(src_bytes_per_pixel == 3 || src_bytes_per_pixel == 4);
            assert(dst_bytes_per_pixel == 3 || dst_bytes_per_pixel == 4);
            assert(dst_bytes_per_pixel == src_bytes_per_pixel);
            assert(data.size() == buffer.size());
            data.swap(buffer);
        }
        // If 8-bit Grayscale (with or w/o Alpha) ...
        else // => !(bit_info.color_type & CT_PALETTE_USED) && !(bit_info.color_type & CT_COLOR_USED)
        {
            // ... iterate through pixels and set the RGB
            // values to the source grayscale value.
            assert(src_bytes_per_pixel == 1 || src_bytes_per_pixel == 2);
            assert(dst_bytes_per_pixel == 3 || dst_bytes_per_pixel == 4);
            for (size_t i = 0; i < N; ++i)
            {
                const size_t si = i * src_bytes_per_pixel;
                const size_t di = i * dst_bytes_per_pixel;
                data[di + 0] = data[di + 1] = data[di + 2] = buffer[si];
                // If alpha channel given, copy the alpha
                // value to the corresponding slot.
                if (bit_info.color_type & CT_ALPHA_USED)
                    data[di + 3] = buffer[si + 1];
            }
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

        if (header.color_type == 0)
        {
            // Allowed bit depths: 1, 2, 4, 8, 16 (already checked, see above)
        }
        else if (header.color_type == 3)
        {
            // Allowed bit depths: 1, 2, 4, 8
            if (header.bit_depth > 8)
                return _log_unsupported_format_error(source_name, u8"invalid bit depth of " + cl7::to_string(header.bit_depth) + u8" for color type " + cl7::to_string(header.color_type));
        }
        else
        {
            // Allowed bit depths: 8, 16
            if (header.bit_depth != 8 && header.bit_depth != 16)
                return _log_unsupported_format_error(source_name, u8"invalid bit depth of " + cl7::to_string(header.bit_depth) + u8" for color type " + cl7::to_string(header.color_type));
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
        bit_info.max_bytes_per_pixel = (bit_info.bits_per_pixel + 7) / 8;
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
        dst.clear();

        unsigned si = 0;
        unsigned di = 0;

        for (unsigned v = 0; v < bit_info.height; ++v)
        {
            assert(si == di + v);
            assert(di % bit_info.bytes_per_scanline == 0);
            assert(si % (bit_info.bytes_per_scanline+1) == 0);

            const auto filter_type = static_cast<unsigned char>(src[si++]); // Advance source index by one byte.

            switch (filter_type)
            {
            case 0: if (!_reconstruct_scanline_filter0_none(src, dst, bit_info, si, di)) return false; break;
            case 1: if (!_reconstruct_scanline_filter1_sub(src, dst, bit_info, si, di)) return false; break;
            case 2: if (!_reconstruct_scanline_filter2_up(src, dst, bit_info, si, di)) return false; break;
            case 3: if (!_reconstruct_scanline_filter3_average(src, dst, bit_info, si, di)) return false; break;
            case 4: if (!_reconstruct_scanline_filter4_paeth(src, dst, bit_info, si, di)) return false; break;
            default: return false;
            };

            si += bit_info.bytes_per_scanline;
            di += bit_info.bytes_per_scanline;
        } // for each scanline

        return true;
    }

    /**
     * Filter type "None" (0): the scanline is transmitted unmodified.
     */
    bool PngImageReader::_reconstruct_scanline_filter0_none(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di)
    {
        assert(di == dst.size());

        dst.insert(
            dst.end(),
            src.begin() + si,
            src.begin() + si + bit_info.bytes_per_scanline);

        return true;
    }

    /**
     * Filter type "Sub" (1): the Sub filter transmits the difference between each
     * byte and the value of the corresponding byte of the prior pixel.
     */
    bool PngImageReader::_reconstruct_scanline_filter1_sub(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di)
    {
        // Run through the all
        // bytes (not pixels).
        for (unsigned h = 0; h < bit_info.bytes_per_scanline; ++h)
        {
            const auto fx = static_cast<unsigned char>(src[si++]);

            unsigned char a = 0;
            if (h >= bit_info.max_bytes_per_pixel)
                a = static_cast<unsigned char>(dst[di + h - bit_info.max_bytes_per_pixel]);

            dst.push_back(static_cast<std::byte>(fx + a));
        } // for each byte

        return true;
    }

    /**
     * Filter type "Up" (2): the Up filter transmits the difference between each
     * byte and the value of the corresponding byte of the prior scanline.
     */
    bool PngImageReader::_reconstruct_scanline_filter2_up(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di)
    {
        const unsigned v = di / bit_info.bytes_per_scanline;

        // Run through the all
        // bytes (not pixels).
        for (unsigned h = 0; h < bit_info.bytes_per_scanline; ++h)
        {
            const auto fx = static_cast<unsigned char>(src[si++]);

            unsigned char b = 0;
            if (v >= 1)
                b = static_cast<unsigned char>(dst[di + h - bit_info.bytes_per_scanline]);

            dst.push_back(static_cast<std::byte>(fx + b));
        } // for each byte

        return true;
    }

    /**
     * Filter type "Average" (3): the Average filter uses the average of the two
     * neighboring pixels (left and above) to predict the value of a pixel.
     */
    bool PngImageReader::_reconstruct_scanline_filter3_average(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di)
    {
        const unsigned v = di / bit_info.bytes_per_scanline;

        // Run through the all
        // bytes (not pixels).
        for (unsigned h = 0; h < bit_info.bytes_per_scanline; ++h)
        {
            const auto fx = static_cast<unsigned char>(src[si++]);

            unsigned char a = 0;
            if (h >= bit_info.max_bytes_per_pixel)
                a = static_cast<unsigned char>(dst[di + h - bit_info.max_bytes_per_pixel]);

            unsigned char b = 0;
            if (v >= 1)
                b = static_cast<unsigned char>(dst[di + h - bit_info.bytes_per_scanline]);

            dst.push_back(static_cast<std::byte>(fx + (a+b)/2));
        } // for each byte

        return true;
    }

    /**
     * Filter type "Paeth" (4): the Paeth filter computes a single linear function
     * of the three neighboring pixel (left, above, upper left), then chooses as
     * predictor the neighboring pixel closest to the computed value.
     */
    bool PngImageReader::_reconstruct_scanline_filter4_paeth(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di)
    {
        const unsigned v = di / bit_info.bytes_per_scanline;

        // Run through the all
        // bytes (not pixels).
        for (unsigned h = 0; h < bit_info.bytes_per_scanline; ++h)
        {
            const auto fx = static_cast<unsigned char>(src[si++]);

            unsigned char a = 0;
            if (h >= bit_info.max_bytes_per_pixel)
                a = static_cast<unsigned char>(dst[di + h - bit_info.max_bytes_per_pixel]);

            unsigned char b = 0;
            if (v >= 1)
                b = static_cast<unsigned char>(dst[di + h - bit_info.bytes_per_scanline]);

            unsigned char c = 0;
            if (v >= 1 && h >= bit_info.max_bytes_per_pixel)
                c = static_cast<unsigned char>(dst[di + h - bit_info.bytes_per_scanline - bit_info.max_bytes_per_pixel]);

            const auto A = static_cast<signed>(a);
            const auto B = static_cast<signed>(b);
            const auto C = static_cast<signed>(c);
            signed p = A+B-C;
            signed pa = p - A; if (pa < 0) pa = -pa;
            signed pb = p - B; if (pb < 0) pb = -pb;
            signed pc = p - C; if (pc < 0) pc = -pc;
            if (pa <= pb && pa <= pc)   p = a;
            else if (pb <= pc)          p = b;
            else                        p = c;
            const auto paeth = static_cast<unsigned char>(static_cast<unsigned>(p) & 0xff);

            dst.push_back(static_cast<std::byte>(fx + paeth));
        } // for each byte

        return true;
    }



} // namespace xl7::graphics::images
