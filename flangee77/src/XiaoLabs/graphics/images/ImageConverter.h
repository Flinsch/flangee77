#ifndef XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#define XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#include "./ImageProcessor.h"



namespace xl7::graphics::images {



struct ImageConverter
    : ImageProcessor
{

    /**
     * Copies pixel data from one image to another, possibly converting the data to
     * the specified pixel format and/or channel order. The image size (width and
     * height) does not change.
     */
    static Image convert_image(const Image& source_image, PixelFormat pixel_format, ChannelOrder channel_order);

    /**
     * Copies pixel data from one buffer to another, possibly converting the data to
     * the specified pixel format and/or channel order. The image size (width and
     * height) does not change.
     */
    static cl7::byte_vector convert_image_data(const ImageDesc& source_desc, cl7::byte_view source_data, PixelFormat pixel_format, ChannelOrder channel_order);

}; // struct ImageConverter



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
