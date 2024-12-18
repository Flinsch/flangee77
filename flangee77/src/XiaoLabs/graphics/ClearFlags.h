#pragma once
#ifndef XL7_GRAPHICS_CLEARFLAGS_H
#define XL7_GRAPHICS_CLEARFLAGS_H

#include <CoreLabs/root.h>



namespace xl7::graphics {



enum class ClearFlags
{
    ColorBuffer = 0x1,
    DepthBuffer = 0x2,
    StencilBuffer = 0x4,

    DepthStencilBuffer = 0x6,
    All = 0x7,
};



inline ClearFlags operator | (ClearFlags a, ClearFlags b)
{
    return static_cast<ClearFlags>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline ClearFlags operator & (ClearFlags a, ClearFlags b)
{
    return static_cast<ClearFlags>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_CLEARFLAGS_H
