#ifndef XL7_GRAPHICS_IMAGES_PNGIMAGEREADER_H
#define XL7_GRAPHICS_IMAGES_PNGIMAGEREADER_H
#include "./ImageReader.h"



namespace xl7::graphics::images {



class PngImageReader
    : public ImageReader
{

public:
    ~PngImageReader() override = default;



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

    struct ChunkInfo
    {
        uint32_t length;
        char type[4];
    };
    static_assert(sizeof(ChunkInfo) == 8);

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
    static bool _process_IHDR_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, uint32_t chunk_length, BitInfo& bit_info);

    /**
     * Processes the palette chunk, "PLTE".
     */
    static bool _process_PLTE_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, uint32_t chunk_length, std::vector<PaletteEntry>& palette);

    /**
     * Processes the image data chunk, "IDAT".
     */
    static bool _process_IDAT_chunk(cl7::io::IReadable& readable, const cl7::u8string& source_name, uint32_t chunk_length, cl7::byte_vector& data);

    /**
     * Decompresses the given source data.
     */
    static bool _decompress(cl7::byte_view src, cl7::byte_vector& dst);

    /**
     * Reconstructs the target data from the given filtered source data.
     */
    static bool _reconstruct(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info);

    /**
     * Decodes the "normalized" data from the given reconstructed data.
     * No decoding/normalization is required for data with a bit depth of 8 bits per
     * channel.
     */
    static bool _decode(cl7::byte_vector&& src, cl7::byte_vector& dst, const BitInfo& bit_info);

    /**
     * The Paeth filter function computes a simple linear function of the three
     * neighboring pixels (left, above, upper left), then chooses as predictor the
     * neighboring pixel closest to the computed value.
     */
    static uint8_t _paeth(uint8_t a, uint8_t b, uint8_t c);

}; // class PngImageReader



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_PNGIMAGEREADER_H
