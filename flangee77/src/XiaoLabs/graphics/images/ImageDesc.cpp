#include "ImageDesc.h"

#include "../PixelLayout.h"



namespace xl7::graphics::images {



    /** Returns the size of one pixel, in bytes. */
    unsigned ImageDesc::determine_pixel_stride() const
    {
        return PixelLayout::determine_stride(pixel_format);
    }

    /** Calculates the number of pixels of the image. */
    size_t ImageDesc::calculate_pixel_count() const
    {
        return
            static_cast<size_t>(width) *
            static_cast<size_t>(height) *
            static_cast<size_t>(depth);
    }

    /** Calculates the total size of the image data, in bytes. */
    size_t ImageDesc::calculate_data_size() const
    {
        return
            calculate_pixel_count() *
            determine_pixel_stride();
    }



} // namespace xl7::graphics::images
