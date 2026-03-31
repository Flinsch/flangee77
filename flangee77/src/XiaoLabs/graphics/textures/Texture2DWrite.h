#ifndef XL7_GRAPHICS_TEXTURES_TEXTURE2DWRITE_H
#define XL7_GRAPHICS_TEXTURES_TEXTURE2DWRITE_H

#include "../images/Image.h"

#include <CoreLabs/byte_view.h>



namespace xl7::graphics::textures {



struct Texture2DWrite
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

    /** The number of bytes between consecutive rows in the source data. */
    unsigned row_pitch;



    static Texture2DWrite from_image(const images::Image& image)
    {
        return from_image_data(image.get_desc(), image.get_data());
    }

    static Texture2DWrite from_image_data(const images::ImageDesc& image_desc, cl7::byte_view data)
    {
        return {
            .data = data,
            .width = image_desc.width,
            .height = image_desc.height,
            .row_pitch = image_desc.width * image_desc.determine_bytes_per_pixel(),
        };
    }
};



} // namespace xl7::graphics::textures

#endif // XL7_GRAPHICS_TEXTURES_TEXTURE2DWRITE_H
