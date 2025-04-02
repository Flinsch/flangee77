#ifndef XL7_GRAPHICS_DEPTHSTENCILFORMAT_H
#define XL7_GRAPHICS_DEPTHSTENCILFORMAT_H

#include <CoreLabs/root.h>



namespace xl7::graphics {



enum struct DepthStencilFormat
{
    UNKNOWN,

    D16,
    D32,

    D15S1,
    D24S8,
    D24X8,
    D32S8X24,
};



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_DEPTHSTENCILFORMAT_H
