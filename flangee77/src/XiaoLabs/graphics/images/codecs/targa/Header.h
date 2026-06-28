#ifndef XL7_GRAPHICS_IMAGES_CODECS_TARGA_HEADER_H
#define XL7_GRAPHICS_IMAGES_CODECS_TARGA_HEADER_H

#include <CoreLabs/root.h>



namespace xl7::graphics::images::codecs::targa {



#pragma pack(push, 1)
struct Header
{
    /** Length of possible free-form image identification field, in bytes. */
    uint8_t id_length;

    /** 0: unmapped grayscale or RGB image; 1: color-mapped image. */
    uint8_t color_map_type;

    /**
     *  1: uncompressed color-mapped image
     *  2: uncompressed RGB image
     *  3: uncompressed grayscale image
     *  9: run-length encoded color-mapped image
     * 10: run-length encoded RGB image
     * 11: run-length encoded grayscale image
     * 32: Huffman-delta-run-length encoded color-mapped image (not supported)
     * 33: Huffman-delta-run-length-4-pass-quadtree-type process encoded color-mapped image (not supported)
     */
    uint8_t image_type;

    /** Index of the first color map entry. */
    uint16_t color_map_start;
    /** Number of entries in the color map. */
    uint16_t color_map_length;
    /** Number of bits per color map entry. */
    uint8_t color_map_depth;

    /** Absolute x-coordinate of the lower-left corner of the image. */
    uint16_t x_origin;
    /** Absolute y-coordinate of the lower-left corner of the image. */
    uint16_t y_origin;
    /** Width of the image, in pixels. */
    uint16_t width;
    /** Height of the image, in pixels. */
    uint16_t height;
    /** Number of bits per pixel in the image data. */
    uint8_t pixel_depth;

    /**
     * - Bits 0-3:  number of attribute bits associated with each pixel (i.e., alpha channel depth)
     * - Bit 4:     right-to-left pixel ordering if set (not supported, must be set to 0: left-to-right)
     * - Bit 5:     top-to-bottom scanline ordering if set (bottom-to-top is the classic TGA default)
     * - Bits 6-7:  data storage interleaving mode (not supported, must be set to 0: non-interleaved)
     */
    uint8_t image_descriptor;
};
#pragma pack(pop)
static_assert(sizeof(Header) == 18);



} // namespace xl7::graphics::images::codecs::targa

#endif // XL7_GRAPHICS_IMAGES_CODECS_TARGA_HEADER_H
