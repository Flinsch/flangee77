#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREWRITE_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREWRITE_H

#include "../images/Image.h"

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::textures {



struct TextureWrite
{
    /** The provided source pixel data to write. */
    cl7::byte_view data;

    /** The destination x-offset, in pixels. */
    unsigned x = 0;
    /** The destination y-offset, in pixels. */
    unsigned y = 0;
    /** The destination z-offset, in pixels (if 3D texture, otherwise trivially 0). */
    unsigned z = 0;

    /** The width of the region to update, in pixels. */
    unsigned width;
    /** The height of the region to update, in pixels. */
    unsigned height;
    /** The depth of the region to update, in pixels (i.e., the number of 2D image slices, if 3D texture, otherwise trivially 1). */
    unsigned depth;

    /** The index of the texture layer to update (if texture array or cubemap, otherwise trivially 0). */
    unsigned layer;

    /** The number of bytes between consecutive rows in the source data. */
    unsigned row_pitch;
    /** The number of bytes between consecutive 2D image slices in the source data (if 3D texture). */
    unsigned slice_pitch;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREWRITE_H
