#pragma once
#ifndef XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
#define XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
#include "./ImageProcessor.h"



namespace xl7 {
namespace graphics {
namespace images {



class ImageResizer
    : public ImageProcessor
{

public:
    enum class ResamplingMethod
    {
        NearestNeighbor,
        LinearInterpolation,
        //CubicConvolution,
    };



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Resizes an image. Pixel format and channel order do not change.
     */
    static Image resize_image(const Image& source_image, unsigned width, unsigned height, unsigned depth, ResamplingMethod resampling_method);

}; // class ImageResizer



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_IMAGERESIZER_H
