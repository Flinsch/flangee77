#ifndef FL7_FONTS_GLYPH_H
#define FL7_FONTS_GLYPH_H

#include "./BoundingBox.h"
#include "./Contour.h"

#include <vector>



namespace fl7::fonts {



/**
 * Contains the outline (described as quadratic Bézier curves) and metrics for a
 * single glyph character.
 */
struct Glyph
{
    /**
     * All contours defining the glyph's outline/shape.
     *
     * If the points are arranged clockwise, the contour is filled. If the points
     * are arranged counter-clockwise, they describe a "hole".
     */
    std::vector<Contour> contours;

    /** The bounding box with lower left corner, upper right corner, and size (width and height) of the glyph. */
    BoundingBox bounding_box;

    struct
    {
        /** The horizontal advance after rendering this glyph. */
        float advance_width = 0.0f;
        /** The horizontal offset from the glyph origin to its left edge. */
        float left_side_bearing = 0.0f;

        // There is no `right_side_bearing` here because it would be redundant
        // information. That alone is not a valid argument, but the value is rarely
        // used anyway. If you need it, for whatever reason/purpose, it's easy to
        // calculate: RSB = AW - LSB - width (because AW = LSB + width + RSB).
    } typography;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_GLYPH_H
