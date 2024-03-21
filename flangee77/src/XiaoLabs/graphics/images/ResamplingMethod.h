#pragma once
#ifndef XL7_GRAPHICS_IMAGES_RESAMPLINGMETHOD_H
#define XL7_GRAPHICS_IMAGES_RESAMPLINGMETHOD_H

#include <CoreLabs/root.h>



namespace xl7 {
namespace graphics {
namespace images {



enum class ResamplingMethod
{
    NearestNeighbor,
    LinearInterpolation,
    //CubicConvolution,
};



} // namespace images
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMAGES_RESAMPLINGMETHOD_H
