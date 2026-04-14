#ifndef XL7_GRAPHICS_TEXTURES_TEXTUREWRITE_H
#define XL7_GRAPHICS_TEXTURES_TEXTUREWRITE_H

#include "./TextureRegion.h"

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::textures {



struct TextureWrite
{
    /** The provided source pixel data to write. */
    cl7::byte_view data;

    /** The destination region to update, in pixels. */
    TextureRegion region;

    /** The index of the texture layer to update (if texture array or cubemap, otherwise trivially 0). */
    unsigned layer;

    /** The number of bytes between consecutive rows in the source data. */
    unsigned row_pitch;
    /** The number of bytes between consecutive 2D image slices in the source data (if 3D texture). */
    unsigned slice_pitch;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTUREWRITE_H
