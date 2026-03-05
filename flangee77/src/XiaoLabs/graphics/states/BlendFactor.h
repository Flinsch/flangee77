#ifndef XL7_GRAPHICS_STATES_BLENDFACTOR_H
#define XL7_GRAPHICS_STATES_BLENDFACTOR_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



enum struct BlendFactor
{
    Zero = 1,
    One,
    SrcColor,
    InvSrcColor,
    SrcAlpha,
    InvSrcAlpha,
    DestAlpha,
    InvDestAlpha,
    DestColor,
    InvDestColor,
    SrcAlphaSat,
    BlendFactor = 14, // Force 1-to-1 compatibility with Direct3D 9 and 11
    InvBlendFactor,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDFACTOR_H
