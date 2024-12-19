#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
#define XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
#include "./ImageProcessor.h"

#include "./ResamplingMethod.h"



namespace xl7::graphics::images {



struct ImageResizer
    : public ImageProcessor
{

    /**
     * Resizes an image. Pixel format and channel order do not change.
     */
    static Image resize_image(const Image& source_image, unsigned width, unsigned height, unsigned depth, ResamplingMethod resampling_method);

    /**
     * Creates a mipmap from the specified image.
     */
    static Image create_mipmap(const Image& source_image, ResamplingMethod resampling_method = ResamplingMethod::LinearInterpolation);

}; // struct ImageResizer



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
