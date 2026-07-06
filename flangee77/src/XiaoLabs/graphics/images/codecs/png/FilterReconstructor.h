#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_FILTERRECONSTRUCTOR_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_FILTERRECONSTRUCTOR_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7::graphics::images::codecs::png {



struct FilterReconstructor
{

    /**
     * Reconstructs the original scanline data from the given filtered source buffer.
     * src must contain height * (1 + bytes_per_scanline) bytes, with a filter type
     * byte at the start of each scanline. dst receives height * bytes_per_scanline
     * bytes of reconstructed pixel data.
     */
    static bool reconstruct(cl7::byte_view src, cl7::byte_vector& dst, size_t bytes_per_pixel, size_t bytes_per_scanline, size_t height);

}; // struct FilterReconstructor



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_FILTERRECONSTRUCTOR_H