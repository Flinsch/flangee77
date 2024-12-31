#ifndef XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#define XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#include "./ImageProcessor.h"



namespace xl7::graphics::images {



struct ImageConverter
    : public ImageProcessor
{

    /**
     * Copies pixel data from one image to another, possibly converting the data to
     * the specified pixel format and/or channel order. The image size does not
     * change.
     */
    static Image convert_image(const Image& source_image, PixelFormat pixel_format, ChannelOrder channel_order);

}; // struct ImageConverter



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
