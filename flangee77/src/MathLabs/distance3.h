#ifndef ML7_DISTANCE3_H
#define ML7_DISTANCE3_H

#include "./Vector3.h"
#include "./Line3.h"
#include "./LineSegment3.h"
#include "./Ray3.h"
#include "./QuadraticBezier3.h"
#include "./CubicBezier3.h"



namespace ml7::distance3 {



struct ResultBase
{
    /** The squared distance between two shapes. */
    float distance_squared;
    /** Calculates the distance between two shapes by taking the square root of the squared distance. */
    float distance() const { assert(distance_squared >= 0.0f); return std::sqrt(distance_squared); }
};

struct PointResult
    : public ResultBase
{
    /** The closest point on the other shape. */
    ml7::Vector3 point;
    /** The parametric distance along the other shape. */
    float t;
};



    /**
     * Calculates the distance result of a point and a line in 3D.
     */
    PointResult point_line(const ml7::Vector3& point, const ml7::Line3& line);

    /**
     * Calculates the distance result of a point and a line segment in 3D.
     */
    PointResult point_line_segment(const ml7::Vector3& point, const ml7::LineSegment3& line_segment);

    /**
     * Calculates the distance result of a point and a ray in 3D.
     */
    PointResult point_ray(const ml7::Vector3& point, const ml7::Ray3& ray);

    /**
     * Calculates the distance result of a point and a quadratic Bézier curve in 3D.
     */
    PointResult point_quadratic_bezier(const ml7::Vector3& point, const ml7::QuadraticBezier3& bezier);

    /**
     * Calculates the distance result of a point and a cubic Bézier curve in 3D.
     */
    PointResult point_cubic_bezier(const ml7::Vector3& point, const ml7::CubicBezier3& bezier);



} // namespace ml7::distance3

#endif // ML7_DISTANCE3_H
