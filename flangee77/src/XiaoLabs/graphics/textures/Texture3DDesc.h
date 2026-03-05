#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE3DDESC_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE3DDESC_H
#include "./TextureDesc.h"



namespace xl7::graphics::textures {



struct Texture3DDesc
    : TextureDesc
{
    /** The depth of the texture, in pixels (or the number of 2D image slices, if you like). */
    unsigned depth;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE3DDESC_H
