#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGESTACK_H
#define XL7_GRAPHICS_IMAGES_IMAGESTACK_H

#include "./Image.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7 {
namespace graphics {
namespace images {



class ImageStack
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    ImageStack();

    /**
     * Explicit constructor.
     */
    ImageStack(const Image::Desc& desc);

    /**
     * Explicit constructor.
     */
    ImageStack(const Image& image);



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The descriptor of the images.
     */
    Image::Desc _desc;

    /**
     * The consecutive data of the contained images.
     */
    cl7::byte_vector _data;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the images.
     */
    const Image::Desc& get_image_desc() const { return _desc; }

    /**
     * Returns the pixel format.
     */
    PixelFormat get_pixel_format() const { return _desc.pixel_format; }

    /**
     * Returns the channel order.
     */
    ChannelOrder get_channel_order() const { return _desc.channel_order; }

    /**
     * Returns the respective width of the images, in pixels.
     */
    unsigned get_image_width() const { return _desc.width; }

    /**
     * Returns the respective height of the images, in pixels.
     */
    unsigned get_image_height() const { return _desc.height; }

    /**
     * Returns the number of contained images.
     */
    unsigned get_image_count() const;

    /**
     * Returns the consecutive data of the contained images.
     */
    const cl7::byte_vector& get_data() const { return _data; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * (Re)initializes the image stack.
     */
    bool init(const Image::Desc& desc);

    /**
     * (Re)initializes the image stack.
     */
    bool init(const Image& image);

    /**
     * Adds the specified image to this image stack by copying its data.
     */
    bool add_image(const Image& image);

}; // class ImageStack



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGESTACK_H
