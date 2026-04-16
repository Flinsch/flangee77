#ifndef XL7_GRAPHICS_IMAGES_IMAGEARRAY_H
#define XL7_GRAPHICS_IMAGES_IMAGEARRAY_H

#include "./Image.h"

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7::graphics::images {



/**
 * Represents an ordered array of equally sized and equally formatted images.
 * Suitable for texture arrays, frame sequences, or layered image data.
 */
class ImageArray
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ImageArray();
    explicit ImageArray(const ImageDesc& desc);
    explicit ImageArray(const Image& image);



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the descriptor of the images.
     */
    const ImageDesc& get_image_desc() const { return _desc; }

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
     * Returns the respective depth of the images, in pixels (if 3D images, otherwise trivially 1).
     */
    unsigned get_image_depth() const { return _desc.depth; }

    /**
     * Returns the number of contained images.
     */
    unsigned get_image_count() const;

    /**
     * Returns the consecutive data of all contained images.
     */
    cl7::byte_view get_data() const { return _data; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)initializes the image array.
     */
    void init(const ImageDesc& desc);

    /**
     * (Re)initializes the image array.
     */
    void init(const Image& image);

    /**
     * Adds the specified image to this image array by copying its data.
     */
    bool add_image(const Image& image);

    /**
     * Adds the specified image data to this image array by copying the data.
     */
    bool add_image_data(cl7::byte_view image_data);

    /**
     * Returns the data of the specified image.
     */
    cl7::byte_view get_image_data(unsigned image_index) const;

    /**
     * Returns the specified image as a "view" of the image data.
     */
    Image get_image(unsigned image_index) const;

    /**
     * Extracts the specified image by creating a new one and copying the data.
     */
    Image extract_image(unsigned image_index) const;



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The descriptor of the images.
     */
    ImageDesc _desc;

    /**
     * The consecutive data of the contained images.
     */
    cl7::byte_vector _data;

}; // class ImageArray



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGEARRAY_H
