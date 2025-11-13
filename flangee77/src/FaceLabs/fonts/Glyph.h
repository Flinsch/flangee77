#ifndef FL7_FONTS_GLYPH_H
#define FL7_FONTS_GLYPH_H

#include "./BoundingBox.h"
#include "./Contour.h"
#include "./GlyphMetrics.h"

#include <CoreLabs/text/codec/codepoint.h>

#include <vector>



namespace fl7::fonts {



/**
 * Contains the outline (described as quadratic Bézier curves) and metrics for a
 * single glyph character.
 */
struct Glyph
{
    /**
     * The (Unicode) code point whose visual shape this glyph represents.
     */
    cl7::text::codec::codepoint codepoint = {0};

    /**
     * All contours defining the glyph's outline/shape.
     *
     * If the points are arranged clockwise, the contour is filled. If the points
     * are arranged counter-clockwise, they describe a "hole".
     */
    std::vector<Contour> contours;

    /** The bounding box with lower left corner, upper right corner, and size (width and height) of the glyph. */
    BoundingBox bounding_box;

    /** The glyph metrics, essentially a glyph's "advance" and its placement relative to the origin. */
    GlyphMetrics metrics;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_GLYPH_H
