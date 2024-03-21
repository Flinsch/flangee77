#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
#define XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
#include "./ImageProcessor.h"

#include "./ResamplingMethod.h"



namespace xl7 {
namespace graphics {
namespace images {



class ImageResizer
    : public ImageProcessor
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Resizes an image. Pixel format and channel order do not change.
     */
    static Image resize_image(const Image& source_image, unsigned width, unsigned height, unsigned depth, ResamplingMethod resampling_method);

    /**
     * Creates a mipmap from the specified image.
     */
    static Image create_mipmap(const Image& source_image, ResamplingMethod resampling_method = ResamplingMethod::LinearInterpolation);

}; // class ImageResizer



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
