#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE3DDESC_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE3DDESC_H
#include "./TextureDesc.h"

#include "./TextureExtent3D.h"



namespace xl7::graphics::textures {



struct Texture3DDesc
{
    /** Identifies how the texture is expected to be updated. */
    TextureUsage usage;

    /** The pixel format. */
    PixelFormat pixel_format;
    /** The preferred channel order. (The actual channel order may vary depending on hardware capabilities.) */
    ChannelOrder preferred_channel_order;

    /** The maximum number of mipmap levels. (Use 1 for a multisampled texture; or 0 to generate a full set of subtextures, down to 1 by 1). */
    unsigned mip_levels;

    /** The extent of the texture, in pixels. */
    TextureExtent3D extent;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE3DDESC_H
