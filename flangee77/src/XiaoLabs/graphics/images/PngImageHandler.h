#pragma once
#ifndef XL7_GRAPHICS_IMAGES_PNGIMAGEHANDLER_H
#define XL7_GRAPHICS_IMAGES_PNGIMAGEHANDLER_H
#include "./ImageHandler.h"



namespace xl7 {
namespace graphics {
namespace images {



class PngImageHandler
    : public ImageHandler
{

private:
    struct Signature
    {
        uint8_t     _x89;
        char        png[3];
        char        crlf[2];
        char        eof;
        char        lf;
    }; // struct Signature
    static_assert( sizeof(Signature) == 8 );

    struct ChunkInfo
    {
        uint32_t length;
        char type[4];
    }; // struct ChunkInfo
    static_assert( sizeof(ChunkInfo) == 8 );

#pragma pack( push, 1 )
    struct Header
    {
        uint32_t    width;
        uint32_t    height;
        uint8_t     bit_depth;
        uint8_t     color_type;
        uint8_t     compression_method;
        uint8_t     filter_method;
        uint8_t     interlace_method;
    }; // struct Header
#pragma pack( pop )
    static_assert( sizeof(Header) == 13 );

#pragma pack( push, 1 )
    struct PaletteEntry
    {
        std::byte r, g, b;
    }; // struct PaletteEntry
#pragma pack( pop )
    static_assert( sizeof(PaletteEntry) == 3 );

    struct BitInfo
    {
        unsigned color_type;
        unsigned channel_count;
        unsigned bits_per_pixel;
        unsigned max_bytes_per_pixel;
        unsigned bytes_per_scanline;
        unsigned width;
        unsigned height;
    }; // struct BitInfo

    static constexpr unsigned char CT_PALETTE_USED = 1;
    static constexpr unsigned char CT_COLOR_USED = 2;
    static constexpr unsigned char CT_ALPHA_USED = 4;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Destructor.
     */
    virtual ~PngImageHandler() = default;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Loads an image from any rom.
     */
    virtual bool _load_from(cl7::io::irom& rom, const cl7::string& rom_name, Image& image) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Processes the PNG chunks.
     */
    bool _process_chunks(cl7::io::irom& rom, const cl7::string& rom_name, BitInfo& bit_info, std::vector<PaletteEntry>& palette, cl7::byte_vector& data);

    /**
     * Processes the image header chunk, "IHDR".
     */
    bool _process_IHDR_chunk(cl7::io::irom& rom, const cl7::string& rom_name, uint32_t chunk_length, BitInfo& bit_info);

    /**
     * Processes the palette chunk, "PLTE".
     */
    bool _process_PLTE_chunk(cl7::io::irom& rom, const cl7::string& rom_name, uint32_t chunk_length, std::vector<PaletteEntry>& palette);

    /**
     * Processes the image data chunk, "IDAT".
     */
    bool _process_IDAT_chunk(cl7::io::irom& rom, const cl7::string& rom_name, uint32_t chunk_length, cl7::byte_vector& data);

    /**
     * Decompresses the given source data.
     */
    bool _decompress(cl7::byte_view src, cl7::byte_vector& dst);

    /**
     * Reconstructs the target data from the given filtered source data.
     */
    bool _reconstruct(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info);

    /**
     * Filter type "None" (0): the scanline is transmitted unmodified.
     */
    bool _reconstruct_scanline_filter0_none(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di);

    /**
     * Filter type "Sub" (1): the Sub filter transmits the difference between each
     * byte and the value of the corresponding byte of the prior pixel.
     */
    bool _reconstruct_scanline_filter1_sub(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di);

    /**
     * Filter type "Up" (2): the Up filter transmits the difference between each
     * byte and the value of the corresponding byte of the prior scanline.
     */
    bool _reconstruct_scanline_filter2_up(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di);

    /**
     * Filter type "Average" (3): the Average filter uses the average of the two
     * neighboring pixels (left and above) to predict the value of a pixel.
     */
    bool _reconstruct_scanline_filter3_average(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di);

    /**
     * Filter type "Paeth" (4): the Paeth filter computes a single linear function
     * of the three neighboring pixel (left, above, upper left), then chooses as
     * predictor the neighboring pixel closest to the computed value.
     */
    bool _reconstruct_scanline_filter4_paeth(cl7::byte_view src, cl7::byte_vector& dst, const BitInfo& bit_info, unsigned si, unsigned di);

}; // class PngImageHandler



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_PNGIMAGEHANDLER_H
