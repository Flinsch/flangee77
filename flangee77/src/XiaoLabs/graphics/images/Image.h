#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGE_H
#define XL7_GRAPHICS_IMAGES_IMAGE_H

#include "../PixelFormat.h"
#include "../ChannelOrder.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7 {
namespace graphics {
namespace images {



class ImageHandler;



class Image
{

public:
    struct Desc
    {
        /** The pixel format. */
        PixelFormat pixel_format;
        /** The channel order. */
        ChannelOrder channel_order;
        /** The width of the image, in pixels. */
        unsigned width;
        /** The height of the image, in pixels. */
        unsigned height;

        /** Returns the size of one pixel, in bytes. */
        unsigned determine_pixel_stride() const;
        /** Calculates the total size of the image data, in bytes. */
        size_t calculate_data_size() const;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    Image();

    /**
     * Explicit constructor.
     */
    Image(const Desc& desc, cl7::byte_view data);

    /**
     * Explicit constructor.
     */
    Image(const Desc& desc, cl7::byte_vector&& data);



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The descriptor of the image.
     */
    Desc _desc;

    /**
     * The image data.
     */
    cl7::byte_vector _data;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the image.
     */
    const Desc& get_desc() const { return _desc; }

    /**
     * Returns the pixel format.
     */
    PixelFormat get_pixel_format() const { return _desc.pixel_format; }

    /**
     * Returns the channel order.
     */
    ChannelOrder get_channel_order() const { return _desc.channel_order; }

    /**
     * Returns the width of the image, in pixels.
     */
    unsigned get_width() const { return _desc.width; }

    /**
     * Returns the height of the image, in pixels.
     */
    unsigned get_height() const { return _desc.height; }

    /**
     * Returns the image data.
     */
    const cl7::byte_vector& get_data() const { return _data; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * (Re)initializes the image.
     */
    bool init(const Desc& desc, cl7::byte_view data);

    /**
     * (Re)initializes the image.
     */
    bool init(const Desc& desc, cl7::byte_vector&& data);



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Validates the initialization data (not in terms of content, but only roughly
     * with regard to technical aspects).
     */
    bool _validate(const Desc& desc, cl7::byte_view data);

}; // class Image



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGE_H
