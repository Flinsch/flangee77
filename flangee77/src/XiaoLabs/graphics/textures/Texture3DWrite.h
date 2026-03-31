#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE3DWRITE_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE3DWRITE_H

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::textures {



struct Texture3DWrite
{
    /** The provided source pixel data to write. */
    cl7::byte_view data;

    /** The destination x-offset, in pixels. */
    unsigned x = 0;
    /** The destination y-offset, in pixels. */
    unsigned y = 0;
    /** The destination z-offset, in pixels. */
    unsigned z = 0;

    /** The width of the region to update, in pixels. */
    unsigned width;
    /** The height of the region to update, in pixels. */
    unsigned height;
    /** The depth of the region to update, in pixels (i.e., the number of 2D image slices). */
    unsigned depth;

    /** The number of bytes between consecutive pixel rows in the source data. */
    unsigned row_pitch;
    /** The number of bytes between consecutive 2D image slices in the source data. */
    unsigned slice_pitch;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE3DWRITE_H
