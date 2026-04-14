#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREBOX_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREBOX_H

#include "./TextureExtent3D.h"
#include "./TextureRegion.h"



namespace xl7::graphics::textures {



struct TextureBox
{
    /** The x-offset of the texture region, in pixels. */
    unsigned x = 0;
    /** The y-offset of the texture region, in pixels. */
    unsigned y = 0;
    /** The z-offset of the texture region, in pixels. */
    unsigned z = 0;

    /** The width of the texture region, in pixels. */
    unsigned width;
    /** The height of the texture region, in pixels. */
    unsigned height;
    /** The depth of the texture region, in pixels (i.e., the number of 2D image slices). */
    unsigned depth;



    static TextureBox zero()
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

    static TextureBox full(const TextureExtent3D& extent)
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

    TextureExtent3D to_extent() const
    {
        return {
            .width = width,
            .height = height,
            .depth = depth,
        };
    }

    TextureRegion generalize() const
    {
        return {
            .x = x,
            .y = y,
            .z = z,
            .width = width,
            .height = height,
            .depth = depth,
        };
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREBOX_H
