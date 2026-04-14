#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYWRITE_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYWRITE_H

#include "./TextureRect.h"

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::textures {



struct Texture2DArrayWrite
{
    /** The provided source pixel data to write. */
    cl7::byte_view data;

    /** The destination region to update, in pixels. */
    TextureRect region;

    /** The index of the array layer to update. */
    unsigned layer;

    /** The number of bytes between consecutive rows in the source data. */
    unsigned row_pitch;
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DARRAYWRITE_H
