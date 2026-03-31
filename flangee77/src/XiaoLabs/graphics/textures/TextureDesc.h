#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREDESC_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREDESC_H

#include "./TextureUsage.h"

#include "../PixelFormat.h"
#include "../ChannelOrder.h"



namespace xl7::graphics::textures {



struct TextureDesc
{
    /** Identifies how the texture is expected to be updated. */
    TextureUsage usage;

    /** The pixel format. */
    PixelFormat pixel_format;
    /** The preferred channel order. (The actual channel order may vary depending on hardware capabilities.) */
    ChannelOrder preferred_channel_order;

    /** The maximum number of mipmap levels. (Use 1 for a multisampled texture; or 0 to generate a full set of subtextures, down to 1 by 1). */
    unsigned mip_levels;

    /** The width of the texture, in pixels. */
    unsigned width;
    /** The height of the texture, in pixels. */
    unsigned height;
    /** The depth of the texture, in pixels (i.e., the number of 2D image slices, if 3D texture, otherwise trivially 1). */
    unsigned depth;

    /** The number of texture layers (if texture array or cubemap, otherwise trivially 1). */
    unsigned layer_count;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREDESC_H
