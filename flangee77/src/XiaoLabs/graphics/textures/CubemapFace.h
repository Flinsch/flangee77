#ifndef XL7_GRAPHICS_TEXTURES_CUBEMAPFACE_H
#define XL7_GRAPHICS_TEXTURES_CUBEMAPFACE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::textures {



/** Defines the faces of a cubemap. */
enum struct CubemapFace
{
    /** The positive x-face of the cubemap. */
    PositiveX,
    /** The negative x-face of the cubemap. */
    NegativeX,
    /** The positive y-face of the cubemap. */
    PositiveY,
    /** The negative y-face of the cubemap. */
    NegativeY,
    /** The positive z-face of the cubemap. */
    PositiveZ,
    /** The negative z-face of the cubemap. */
    NegativeZ,
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_CUBEMAPFACE_H
