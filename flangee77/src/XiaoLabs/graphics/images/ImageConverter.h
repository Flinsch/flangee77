#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#define XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H

#include "./Image.h"

#include "../Color.h"

#include <CoreLabs/byte_span.h>
#include <CoreLabs/byte_view.h>



namespace xl7 {
namespace graphics {
namespace images {



class ImageConverter
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Copies pixel data from one image to another, possibly converting the data to
     * the specified pixel format and/or channel order. The image size does not
     * change.
     */
    static Image convert_image(const Image& source_image, PixelFormat pixel_format, ChannelOrder channel_order);

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

}; // class ImageConverter



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
