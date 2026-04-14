#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT_H

#include <CoreLabs/root.h>



namespace xl7::graphics::textures {



struct TextureExtent
{
    /** The width of the texture, in pixels. */
    unsigned width;
    /** The height of the texture, in pixels. */
    unsigned height;
    /** The depth of the texture, in pixels (i.e., the number of 2D image slices, if 3D texture, otherwise trivially 1). */
    unsigned depth = 1;



    auto volume() const
    {
        return width * height * depth;
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREEXTENT_H
