#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYDESC_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYDESC_H
#include "./TextureDesc.h"



namespace xl7::graphics::textures {



struct Texture2DArrayDesc
    : TextureDesc
{
    /** The number of textures/layers in the texture array. */
    unsigned count;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYDESC_H
