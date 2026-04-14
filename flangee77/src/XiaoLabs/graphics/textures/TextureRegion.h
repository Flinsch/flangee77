#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREREGION_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREREGION_H

#include "./TextureExtent.h"



namespace xl7::graphics::textures {



struct TextureRegion
{
    /** The x-offset of the texture region, in pixels. */
    unsigned x = 0;
    /** The y-offset of the texture region, in pixels. */
    unsigned y = 0;
    /** The z-offset of the texture region, in pixels (if 3D texture, otherwise trivially 0). */
    unsigned z = 0;

    /** The width of the texture region, in pixels. */
    unsigned width;
    /** The height of the texture region, in pixels. */
    unsigned height;
    /** The depth of the texture region, in pixels (i.e., the number of 2D image slices, if 3D texture, otherwise trivially 1). */
    unsigned depth = 1;



    static TextureRegion zero()
    {
        return {
            .x = 0,
            .y = 0,
            .z = 0,
            .width = 0,
            .height = 0,
            .depth = 0,
        };
    }

    static TextureRegion full(const TextureExtent& extent)
    {
        return {
            .x = 0,
            .y = 0,
            .z = 0,
            .width = extent.width,
            .height = extent.height,
            .depth = extent.depth,
        };
    }

    TextureExtent to_extent() const
    {
        return {
            .width = width,
            .height = height,
            .depth = depth,
        };
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREREGION_H
