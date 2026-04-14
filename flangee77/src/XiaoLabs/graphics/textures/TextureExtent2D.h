#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT2D_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT2D_H

#include "./TextureExtent.h"



namespace xl7::graphics::textures {



struct TextureExtent2D
{
    /** The width of the texture, in pixels. */
    unsigned width;
    /** The height of the texture, in pixels. */
    unsigned height;



    auto volume() const
    {
        return width * height;
    }

    TextureExtent generalize() const
    {
        return {
            .width = width,
            .height = height,
            .depth = 1,
        };
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT2D_H
