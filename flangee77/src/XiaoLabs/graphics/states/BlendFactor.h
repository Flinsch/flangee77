#ifndef XL7_GRAPHICS_STATES_BLENDFACTOR_H
#define XL7_GRAPHICS_STATES_BLENDFACTOR_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Specifies the factor used to scale source and destination colors during blending. */
enum struct BlendFactor
{
    /** Blend factor (0,0,0,0). */
    Zero = 1,
    /** Blend factor (1,1,1,1). */
    One,
    /** Use the color of the source pixel. */
    SrcColor,
    /** Use the inverse of the source color (1 - SrcColor). */
    InvSrcColor,
    /** Use the alpha value of the source pixel. */
    SrcAlpha,
    /** Use the inverse of the source alpha (1 - SrcAlpha). */
    InvSrcAlpha,
    /** Use the alpha value of the destination pixel. */
    DestAlpha,
    /** Use the inverse of the destination alpha (1 - DestAlpha). */
    InvDestAlpha,
    /** Use the color of the destination pixel. */
    DestColor,
    /** Use the inverse of the destination color (1 - DestColor). */
    InvDestColor,
    /** Use the minimum of SrcAlpha and (1 - DestAlpha). */
    SrcAlphaSat,
    /** Use the user-defined blend factor specified in the render state. */
    BlendFactor = 14, // Force 1-to-1 compatibility with Direct3D 9 and 11
    /** Use the inverse of the user-defined blend factor (1 - BlendFactor). */
    InvBlendFactor,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDFACTOR_H
