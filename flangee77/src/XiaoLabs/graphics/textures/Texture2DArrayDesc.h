#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYDESC_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYDESC_H
#include "./TextureDesc.h"



namespace xl7::graphics::textures {



struct Texture2DArrayDesc
{
    /** Identifies how the texture is expected to be updated (frequency of update is a key factor). */
    TextureUsage usage;

    /** The pixel format. */
    PixelFormat pixel_format;
    /** The preferred channel order. (The actual channel order may vary depending on hardware capabilities.) */
    ChannelOrder preferred_channel_order;

    /** The maximum number of mipmap levels. (Use 1 for a multisampled texture; or 0 to generate a full set of subtextures, down to 1 by 1). */
    unsigned mip_levels;

    /** The width of each texture, in pixels. */
    unsigned width;
    /** The height of each texture, in pixels. */
    unsigned height;

    /** The number of textures/layers in the texture array. */
    unsigned texture_count;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYDESC_H
