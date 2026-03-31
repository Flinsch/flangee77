#ifndef XL7_GRAPHICS_TEXTURES_CUBEMAPWRITE_H
#define XL7_GRAPHICS_TEXTURES_CUBEMAPWRITE_H

#include "./CubemapFace.h"

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::textures {



struct CubemapWrite
{
    /** The provided source pixel data to write. */
    cl7::byte_view data;

    /** The destination x-offset, in pixels. */
    unsigned x = 0;
    /** The destination y-offset, in pixels. */
    unsigned y = 0;

    /** The width of the region to update, in pixels. */
    unsigned width;
    /** The height of the region to update, in pixels. */
    unsigned height;

    /** The cubemap face to update. */
    CubemapFace face;

    /** The number of bytes between consecutive rows in the source data. */
    unsigned row_pitch;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_CUBEMAPWRITE_H
