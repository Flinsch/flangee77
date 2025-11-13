#ifndef FL7_FONTS_CONTOUR_H
#define FL7_FONTS_CONTOUR_H

#include <MathLabs/QuadraticBezier2.h>

#include <vector>



namespace fl7::fonts {



/**
 * Contains a single closed contour (described as quadratic Bézier curves) of a
 * glyph character.
 */
struct Contour
{
    /**
     * The points of the quadratic Bézier curves defining the contour.
     *
     * A single quadratic Bézier curve consists of three points (you don't say): an
     * on-curve start point, an off-curve control point, and an on-curve end point;
     * with each of two consecutive curves sharing one on-curve point. The first and
     * last Bézier curves also share an on-curve point to close the contour. The
     * shared points are not stored twice, except for the shared point of the first
     * and last Bézier curves, which is stored identically twice, as the very first
     * and very last point. On-curve and off-curve points thus alternate, and the
     * total number of points is always odd, while the number of resulting Bézier
     * curves is (n - 1) / 2, where n is the number of points.
     *
     * If the points are arranged clockwise, the contour is filled. If the points
     * are arranged counter-clockwise, they describe a "hole".
     */
    std::vector<ml7::Vector2f> points;

    /** Returns the effective number of Bézier curves. */
    size_t segment_count() const;

    /** Returns the quadratic Bézier curve identified by the given index. */
    const ml7::QuadraticBezier2f& segment_at(size_t index) const;
};



} // namespace fl7::fonts

#endif // FL7_FONTS_CONTOUR_H
