#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_READER_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_READER_H
#include "../../ImageReader.h"

#include "ChunkReader.h"
#include "FilterReconstructor.h"



namespace xl7::graphics::images::codecs::png {



class Reader
    : public ImageReader
{

public:
    ~Reader() override = default;



private:

    struct Signature
    {
        uint8_t     _x89;
        char        png[3];
        char        crlf[2];
        char        eof;
        char        lf;
    };
    static_assert(sizeof(Signature) == 8);

#pragma pack(push, 1)
    struct Header
    {
        uint32_t    width;
        uint32_t    height;
        uint8_t     bit_depth;
        uint8_t     color_type;
        uint8_t     compression_method;
        uint8_t     filter_method;
        uint8_t     interlace_method;
    };
#pragma pack(pop)
    static_assert(sizeof(Header) == 13);

#pragma pack(push, 1)
    struct PaletteEntry
    {
        std::byte r, g, b;
    };
#pragma pack(pop)
    static_assert(sizeof(PaletteEntry) == 3);

    struct BitInfo
    {
        unsigned color_type;
        unsigned channel_count;
        unsigned bits_per_pixel;
        unsigned ceil_bytes_per_pixel;
        unsigned bytes_per_scanline;
        unsigned width;
        unsigned height;
    };



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Loads an image from any readable object.
     */
    bool _load_from(cl7::io::IReadable& readable, const cl7::u8string& source_name, Image& image) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Processes the PNG chunks.
     */
    static bool _process_chunks(cl7::io::IReadable& readable, const cl7::u8string& source_name, BitInfo& bit_info, std::vector<PaletteEntry>& palette, cl7::byte_vector& data);

    /**
     * Processes the image header chunk, "IHDR".
     */
    static bool _process_IHDR_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, const ChunkReader::ChunkHeader& chunk_header, BitInfo& bit_info);

    /**
     * Processes the palette chunk, "PLTE".
     */
    static bool _process_PLTE_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, const ChunkReader::ChunkHeader& chunk_header, std::vector<PaletteEntry>& palette);

    /**
     * Processes the image data chunk, "IDAT".
     */
    static bool _process_IDAT_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, const ChunkReader::ChunkHeader& chunk_header, cl7::byte_vector& data);

    /**
     * Decompresses the given source data.
     */
    static bool _decompress(cl7::byte_view src, cl7::byte_vector& dst);

    /**
     * Decodes the "normalized" data from the given reconstructed data.
     * No decoding/normalization is required for data with a bit depth of 8 bits per
     * channel.
     */
    static bool _decode(cl7::byte_vector&& src, cl7::byte_vector& dst, const BitInfo& bit_info);

}; // class Reader



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_READER_H
