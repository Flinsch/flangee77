#ifndef XL7_GRAPHICS_IMAGES_IMAGEDESC_H
#define XL7_GRAPHICS_IMAGES_IMAGEDESC_H

#include "../PixelFormat.h"
#include "../ChannelOrder.h"



namespace xl7::graphics::images {



struct ImageDesc
{
    /** The pixel format. */
    PixelFormat pixel_format;
    /** The channel order. */
    ChannelOrder channel_order;

    /** The width of the image, in pixels. */
    unsigned width;
    /** The height of the image, in pixels. */
    unsigned height;
    /** The depth of the image, in pixels (if 3D image, otherwise trivially 1). */
    unsigned depth = 1;

    /** Returns the size of one pixel, in bytes. */
    unsigned determine_bytes_per_pixel() const;
    /** Calculates the number of pixels of the image. */
    size_t calculate_pixel_count() const;
    /** Calculates the total size of the image data, in bytes. */
    size_t calculate_data_size() const;
};



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEDESC_H
