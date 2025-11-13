#ifndef FL7_FONTS_RASTER_PIXELOFFSET_H
#define FL7_FONTS_RASTER_PIXELOFFSET_H

#include <CoreLabs/root.h>



namespace fl7::fonts::raster {



/**
 * Represents the positioning offset of a glyph's pixel grid relative to its
 * typographic origin, in pixel space.
 */
struct PixelOffset
{
    /**
     * The horizontal offset of the glyph's pixel grid relative to the left edge
     * of its advance width (typographic origin), in pixel space. This can be
     * negative, depending on the padding used for rasterization or overshoots
     * of the glyph's outline.
     */
    int left;

    /**
     * The vertical offset of the glyph's pixel grid relative to the typographic
     * baseline, in pixel space. This is typically negative, as most glyphs
     * extend above the baseline (e.g., ascenders). The value may also be
     * influenced by rasterization padding or overshoots of the glyph's outline.
     */
    int top;
};



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_PIXELOFFSET_H
