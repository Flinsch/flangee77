#pragma once
#ifndef XL7_GRAPHICS_IMAGES_RESAMPLINGMETHOD_H
#define XL7_GRAPHICS_IMAGES_RESAMPLINGMETHOD_H

#include <CoreLabs/root.h>



namespace xl7::graphics::images {



enum class ResamplingMethod
{
    NearestNeighbor,
    LinearInterpolation,
    //CubicConvolution,
};



} // namespace xl7::graphics::images

#endif // XL7_GRAPHICS_IMAGES_RESAMPLINGMETHOD_H
