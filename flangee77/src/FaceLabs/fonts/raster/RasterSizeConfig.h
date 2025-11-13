#ifndef FL7_FONTS_RASTER_RASTERSIZECONFIG_H
#define FL7_FONTS_RASTER_RASTERSIZECONFIG_H

#include <CoreLabs/root.h>



namespace fl7::fonts::raster {



/**
 * Configures the implicit size (width, height) of a rasterization result through
 * specifications of font size and optional padding.
 */
struct RasterSizeConfig
{
    /** The font size, in pixels, used to determine the scale of the rasterized glyph. */
    float font_size;
    /** Additional padding, in pixels, applied around the glyph to ensure proper spacing during rasterization. */
    unsigned padding = 0;
};



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_RASTERSIZECONFIG_H
