#pragma once
#ifndef XL7_GRAPHICS_PIXELFORMAT_H
#define XL7_GRAPHICS_PIXELFORMAT_H

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {



enum class PixelFormat
{
    UNKNOWN,

    R8_UNORM,
    R8_SNORM,
    R8_UINT,
    R8_SINT,

    R16_UNORM,
    R16_SNORM,
    R16_UINT,
    R16_SINT,
    R16_FLOAT,

    R32_UINT,
    R32_SINT,
    R32_FLOAT,

    R8G8_UNORM,
    R8G8_SNORM,
    R8G8_UINT,
    R8G8_SINT,

    R16G16_UNORM,
    R16G16_SNORM,
    R16G16_UINT,
    R16G16_SINT,
    R16G16_FLOAT,

    R32G32_UINT,
    R32G32_SINT,
    R32G32_FLOAT,

    R4G4B4X4_UNORM,
    R5G5B5X1_UNORM,
    R5G6B5_UNORM,

    R8G8B8_UNORM,
    R8G8B8_SNORM,
    R8G8B8_UINT,
    R8G8B8_SINT,

    R8G8B8X8_UNORM,

    R11G11B10_FLOAT,

    // R16G16B16 explicitly not

    R32G32B32_UINT,
    R32G32B32_SINT,
    R32G32B32_FLOAT,

    R4G4B4A4_UNORM,
    R5G5B5A1_UNORM,

    R8G8B8A8_UNORM,
    R8G8B8A8_SNORM,
    R8G8B8A8_UINT,
    R8G8B8A8_SINT,

    R10G10B10A2_UNORM,
    R10G10B10A2_UINT,

    R16G16B16A16_UNORM,
    R16G16B16A16_SNORM,
    R16G16B16A16_UINT,
    R16G16B16A16_SINT,
    R16G16B16A16_FLOAT,

    R32G32B32A32_UINT,
    R32G32B32A32_SINT,
    R32G32B32A32_FLOAT,

    A8_UNORM,
};



} // namespace graphics
} // namespace xl7



namespace cl7 {
    cl7::string to_string(xl7::graphics::PixelFormat pixel_format);
}



#endif // XL7_GRAPHICS_PIXELFORMAT_H
