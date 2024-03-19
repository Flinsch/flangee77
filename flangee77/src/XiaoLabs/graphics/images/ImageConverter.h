#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#define XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
#include "./ImageProcessor.h"



namespace xl7 {
namespace graphics {
namespace images {



class ImageConverter
    : public ImageProcessor
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

}; // class ImageConverter



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGECONVERTER_H
