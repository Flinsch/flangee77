#ifndef FL7_FONTS_RASTER_DETAIL_GLYPH_GEOMETRY_H
#define FL7_FONTS_RASTER_DETAIL_GLYPH_GEOMETRY_H

#include "../../Glyph.h"

#include <MathLabs/QuadraticBezier2.h>
#include <MathLabs/Vector2.h>

#include <span>
#include <vector>



namespace fl7::fonts::raster::detail {



    /**
     * Transforms all of the glyph's contour segments from font-normalized (em, y-up)
     * space into pixel space (scaled by font_size, y-down), flattening them into a
     * single list of quadratic Bezier curves. Contour boundaries are not preserved;
     * use `transform_glyph_contours` if per-contour grouping is required.
     */
    std::vector<ml7::QuadraticBezier2f> transform_glyph_segments(const Glyph& glyph, float font_size);

    /**
     * Same as `transform_glyph_segments`, but keeps the segments grouped by contour.
     */
    std::vector<std::vector<ml7::QuadraticBezier2f>> transform_glyph_contours(const Glyph& glyph, float font_size);

    /**
     * Checks whether the given point lies inside the shape described by the given
     * (pixel-space) segments, using the nonzero winding rule via a horizontal ray cast.
     */
    bool is_point_inside(std::span<const ml7::QuadraticBezier2f> segments, ml7::Vector2f point);



} // namespace fl7::fonts::raster::detail

#endif // FL7_FONTS_RASTER_DETAIL_GLYPH_GEOMETRY_H
