#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT3D_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT3D_H

#include "./TextureExtent.h"



namespace xl7::graphics::textures {



struct TextureExtent3D
{
    /** The width of the texture, in pixels. */
    unsigned width;
    /** The height of the texture, in pixels. */
    unsigned height;
    /** The depth of the texture, in pixels (i.e., the number of 2D image slices). */
    unsigned depth;



    auto volume() const
    {
        return width * height * depth;
    }

    TextureExtent generalize() const
    {
        return {
            .width = width,
            .height = height,
            .depth = depth,
        };
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT3D_H
