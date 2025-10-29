#ifndef FL7_FONTS_GLYPHMETRICS_H
#define FL7_FONTS_GLYPHMETRICS_H

#include <CoreLabs/root.h>



namespace fl7::fonts {



/**
 * Provides glyph metrics, essentially a glyph's "advance" and its placement
 * relative to the origin.
 */
struct GlyphMetrics
{
    /** The horizontal advance after rendering this glyph. */
    float advance_width = 0.0f;
    /** The horizontal offset from the glyph origin to its left edge. */
    float left_side_bearing = 0.0f;
    /** The horizontal space between the rightmost edge of the glyph's shape and the advance width line: RSB = AW - LSB - width (because AW = LSB + width + RSB). */
    float right_side_bearing = 0.0f;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_GLYPHMETRICS_H
