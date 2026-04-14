#ifndef XL7_GRAPHICS_TEXTURES_TEXTURERECT_H
#define XL7_GRAPHICS_TEXTURES_TEXTURERECT_H

#include "./TextureExtent2D.h"
#include "./TextureRegion.h"



namespace xl7::graphics::textures {



struct TextureRect
{
    /** The x-offset of the texture region, in pixels. */
    unsigned x = 0;
    /** The y-offset of the texture region, in pixels. */
    unsigned y = 0;

    /** The width of the texture region, in pixels. */
    unsigned width;
    /** The height of the texture region, in pixels. */
    unsigned height;



    static TextureRect zero()
    {
        return {
            .x = 0,
            .y = 0,
            .width = 0,
            .height = 0,
        };
    }

    static TextureRect full(const TextureExtent2D& extent)
    {
        return {
            .x = 0,
            .y = 0,
            .width = extent.width,
            .height = extent.height,
        };
    }

    TextureExtent2D to_extent() const
    {
        return {
            .width = width,
            .height = height,
        };
    }

    TextureRegion generalize() const
    {
        return {
            .x = x,
            .y = y,
            .z = 0,
            .width = width,
            .height = height,
            .depth = 1,
        };
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURERECT_H
