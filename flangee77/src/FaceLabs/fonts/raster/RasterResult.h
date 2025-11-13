#ifndef FL7_FONTS_RASTER_RASTERRESULT_H
#define FL7_FONTS_RASTER_RASTERRESULT_H

#include "./PixelOffset.h"

#include <XiaoLabs/graphics/images/Image.h>



namespace fl7::fonts::raster {



/**
 * Encapsulates the output of a glyph rasterization operation.
 */
struct RasterResult
{
    /** The rasterized image of the glyph, containing pixel data. */
    xl7::graphics::images::Image glyph_image;
    /** The positioning offset of the glyph's pixel grid relative to its typographic origin, in pixel space. */
    PixelOffset pixel_offset;
};



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_RASTERRESULT_H
