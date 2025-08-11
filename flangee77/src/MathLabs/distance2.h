#ifndef ML7_DISTANCE2_H
#define ML7_DISTANCE2_H

#include "./Vector2.h"
#include "./Line2.h"
#include "./LineSegment2.h"
#include "./Ray2.h"



namespace ml7::distance2 {



struct ResultBase
{
    /** The squared distance between two shapes. */
    float distsqr;
    /** Calculates the distance between two shapes by taking the square root of the squared distance. */
    float distance() const { assert(distsqr >= 0.0f); return std::sqrt(distsqr); }
};

struct PointResult
    : public ResultBase
{
    /** The closest point on the other shape. */
    ml7::Vector2 point;
    /** The parametric distance along the other shape. */
    float t;
};



    /**
     * Calculates the distance result of a point and a line in 2D.
     */
    PointResult point_line(const ml7::Vector2& point, const ml7::Line2& line);

    /**
     * Calculates the distance result of a point and a line segment in 2D.
     */
    PointResult point_line_segment(const ml7::Vector2& point, const ml7::LineSegment2& line_segment);

    /**
     * Calculates the distance result of a point and a ray in 2D.
     */
    PointResult point_ray(const ml7::Vector2& point, const ml7::Ray2& ray);



} // namespace ml7::distance2

#endif // ML7_DISTANCE2_H
