#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGE_H
#define XL7_GRAPHICS_IMAGES_IMAGE_H

#include "../PixelFormat.h"
#include "../ChannelOrder.h"

#include <CoreLabs/byte_vector.h>



namespace xl7 {
namespace graphics {
namespace images {



class ImageHandler;



class Image
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    Image(void);



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The pixel format.
     */
    PixelFormat _pixel_format;

    /**
     * The channel order.
     */
    ChannelOrder _channel_order;

    /**
     * The width of the image, in pixels.
     */
    unsigned _width;

    /**
     * The height of the image, in pixels.
     */
    unsigned _height;

    /**
     * The image data.
     */
    cl7::byte_vector _data;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the pixel format.
     */
    PixelFormat get_pixel_format() const { return _pixel_format; }

    /**
     * Returns the channel order.
     */
    ChannelOrder get_channel_order() const { return _channel_order; }

    /**
     * Returns the width of the image, in pixels.
     */
    unsigned get_width() const { return _width; }

    /**
     * Returns the height of the image, in pixels.
     */
    unsigned get_height() const { return _height; }

    /**
     * Returns the image data.
     */
    const cl7::byte_vector& get_data() const { return _data; }



    // #############################################################################
    // Methods
    // #############################################################################
public:


}; // class Image



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGE_H
