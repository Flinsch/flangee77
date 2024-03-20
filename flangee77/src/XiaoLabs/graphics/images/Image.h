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
        /** The depth of the image, in pixels (if 3D image, otherwise trivially 1). */
        unsigned depth;

        /** Returns the size of one pixel, in bytes. */
        unsigned determine_pixel_stride() const;
        /** Calculates the number of pixels of the image. */
        size_t calculate_pixel_count() const;
        /** Calculates the total size of the image data, in bytes. */
        size_t calculate_data_size() const;
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor. Initializes an "empty" image.
     */
    Image();

    /**
     * Explicit constructor. Initializes an "empty" image.
     */
    explicit Image(const Desc& desc);

    /**
     * Explicit constructor. If view_only is set to true, no data will be duplicated;
     * instead, the image's data view will point to the specified data view, which
     * accordingly should persist beyond the lifetime of the image.
     */
    Image(const Desc& desc, cl7::byte_view data, bool view_only = false);

    /**
     * Explicit constructor.
     */
    Image(const Desc& desc, cl7::byte_vector&& data);

    /**
     * Copy constructor. Creates a buffer and duplicates the data regardless of
     * whether the other image is "view only".
     */
    Image(const Image& rhs);

    /**
     * Copy assignment operator. Creates a buffer and duplicates the data regardless
     * of whether the other image is "view only".
     */
    Image& operator = (const Image& rhs);

    /**
     * Swap operation.
     */
    void swap(Image& rhs);

    /**
     * Special swap operation. The image's data is essentially "exported" and then
     * remains undefined.
     */
    void swap(cl7::byte_vector& data);



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The descriptor of the image.
     */
    Desc _desc;

    /**
     * The image data view.
     */
    cl7::byte_view _data_view;

    /**
     * The image data buffer (or empty if "view only").
     */
    cl7::byte_vector _data_buffer;



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
     * Returns the depth of the image, in pixels (if 3D image, otherwise trivially 1).
     */
    unsigned get_depth() const { return _desc.depth; }

    /**
     * Returns the image data.
     */
    cl7::byte_view get_data() const { return _data_view; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * (Re)initializes an "empty" image.
     */
    bool init(const Desc& desc);

    /**
     * (Re)initializes the image based on the given data. If view_only is set to
     * true, no data will be duplicated; instead, the image's data view will point
     * to the specified data view, which accordingly should persist beyond the
     * lifetime of the image.
     */
    bool init(const Desc& desc, cl7::byte_view data, bool view_only = false);

    /**
     * (Re)initializes the image based on the given data.
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
