#ifndef XL7_GRAPHICS_IMAGES_IMAGEPROCESSOR_H
#define XL7_GRAPHICS_IMAGES_IMAGEPROCESSOR_H

#include "./Image.h"

#include "../Color.h"
#include "../PixelBitKit.h"

#include <CoreLabs/byte_span.h>
#include <CoreLabs/byte_view.h>



namespace xl7::graphics::images {



struct ImageProcessor
{

    /**
     * Transforms the given color into the specified pixel format and channel order.
     * The receiving data buffer is required to have at least the corresponding byte
     * size.
     */
    static void pack_color(const Color& color, PixelFormat pixel_format, ChannelOrder channel_order, cl7::byte_span packed_data);

    /**
     * Extracts a color that is in the specified pixel format and channel order.
     * The source data buffer is required to have at least the corresponding byte
     * size.
     */
    static Color unpack_color(cl7::byte_view packed_data, PixelFormat pixel_format, ChannelOrder channel_order);



    /**
     * Transforms the given color into the specified pixel format and channel order.
     * The receiving data buffer is required to have at least the corresponding byte
     * size.
     */
    static void _pack_color(const Color& color, const PixelBitKit& pbk, cl7::byte_span packed_data);

    /**
     * Extracts a color that is in the specified pixel format and channel order.
     * The source data buffer is required to have at least the corresponding byte
     * size.
     */
    static Color _unpack_color(cl7::byte_view packed_data, const PixelBitKit& pbk);

}; // struct ImageProcessor



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEPROCESSOR_H
