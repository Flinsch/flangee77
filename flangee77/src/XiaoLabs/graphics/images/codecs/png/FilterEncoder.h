#ifndef XL7_GRAPHICS_IMAGES_CODECS_PNG_FILTERENCODER_H
#define XL7_GRAPHICS_IMAGES_CODECS_PNG_FILTERENCODER_H

#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>



namespace xl7::graphics::images::codecs::png {



struct FilterEncoder
{
    enum struct FilterType
    {
        None,
        Sub,
        Up,
        Average,
        Paeth,
        Auto,
    };

    /**
     * Applies PNG scanline filters to the given raw pixel data. src must contain
     * height * bytes_per_scanline bytes of unfiltered pixel data. dst receives
     * height * (1 + bytes_per_scanline) bytes, with a filter type byte prepended
     * to each scanline.
     * FilterType::Auto selects the filter per scanline that minimizes the sum of
     * absolute values of the filtered bytes (adaptive filtering).
     */
    static bool encode(cl7::byte_view src, cl7::byte_vector& dst,
                       size_t bytes_per_pixel,
                       size_t bytes_per_scanline,
                       size_t height,
                       FilterType filter_type);

private:
    static uint8_t _best_filter(cl7::byte_view src, size_t scanline_offset, size_t row,
                                size_t bytes_per_pixel, size_t bytes_per_scanline);

}; // struct FilterEncoder



} // namespace xl7::graphics::images::codecs::png

#endif // XL7_GRAPHICS_IMAGES_CODECS_PNG_FILTERENCODER_H
