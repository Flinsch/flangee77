#ifndef FL7_FONTS_GLYPH_H
#define FL7_FONTS_GLYPH_H

#include "./Point.h"

#include <vector>



namespace fl7::fonts {



/**
 * Contains the outline (described as quadratic Bézier curves) and metrics for a
 * single glyph character.
 */
struct Glyph
{
    /** All points defining the glyph's shape. */
    std::vector<Point> contour_points;
    /** Each entry marks the last point of a contour, separating the glyph into subpaths. */
    std::vector<unsigned> end_point_indices;
    /** The horizontal advance after rendering this glyph (in font units). */
    int advance_width;
    /** The horizontal offset from the glyph origin to its left edge (in font units). */
    int left_side_bearing;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_GLYPH_H
