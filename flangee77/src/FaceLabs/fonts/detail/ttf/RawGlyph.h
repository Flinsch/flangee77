#ifndef FL7_FONTS_DETAIL_TTF_RAWGLYPH_H
#define FL7_FONTS_DETAIL_TTF_RAWGLYPH_H

#include "../../Glyph.h"

#include <vector>



namespace fl7::fonts::detail::ttf {



/**
 * Contains the outline (described as quadratic Bézier curves) and metrics for a
 * single glyph character. The structure corresponds to the "raw" file format, e.g.
 * of a TrueType font (TTF).
 */
struct RawGlyph
{
    /**
     * Represents a single point in a glyph's outline.
     */
    struct Point
    {
        /** The x-coordinate of the point (in font units). */
        int16_t x;
        /** The y-coordinate of the point (in font units). */
        int16_t y;
        /** True if this point lies on the curve; false if it is a control point. */
        bool on_curve;

        ml7::Vector2f to_vector() const;
        static Point from_vector(ml7::Vector2f v, bool on_curve);
    };

    /** All points defining the glyph's shape. */
    std::vector<Point> contour_points;
    /** Each entry marks the last point of a contour, separating the glyph into subpaths. */
    std::vector<uint16_t> end_point_indices;

    /** The minimum x for coordinate data (in font units). */
    int16_t x_min = 0;
    /** The minimum y for coordinate data (in font units). */
    int16_t y_min = 0;
    /** The maximum x for coordinate data (in font units). */
    int16_t x_max = 0;
    /** The maximum y for coordinate data (in font units). */
    int16_t y_max = 0;

    /** The advance width (in font units). */
    uint16_t advance_width = 0;
    /** The left-side bearing (in font units). */
    int16_t left_side_bearing = 0;



    Glyph normalize(float em_per_unit) const;
};



} // namespace fl7::fonts::detail::ttf

#endif // FL7_FONTS_DETAIL_TTF_RAWGLYPH_H
