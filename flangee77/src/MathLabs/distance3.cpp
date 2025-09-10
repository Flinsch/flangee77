#include "distance3.h"

#include "./detail/distance.h"

#include "./ops.h"



namespace ml7::distance3 {



    /**
     * Calculates the distance result of a point and a line in 3D.
     */
    PointResult point_line(const ml7::Vector3& point, const ml7::Line3& line)
    {
        return ml7::detail::distance::point_line<std::identity, PointResult>(point, line.point, line.direction);
    }

    /**
     * Calculates the distance result of a point and a line segment in 3D.
     */
    PointResult point_line_segment(const ml7::Vector3& point, const ml7::LineSegment3& line_segment)
    {
        return ml7::detail::distance::point_line<ml7::ops::clamp01, PointResult>(point, line_segment.start, line_segment.end - line_segment.start);
    }

    /**
     * Calculates the distance result of a point and a ray in 3D.
     */
    PointResult point_ray(const ml7::Vector3& point, const ml7::Ray3& ray)
    {
        return ml7::detail::distance::point_line<ml7::ops::ramp<>, PointResult>(point, ray.origin, ray.direction);
    }

    /**
     * Calculates the distance result of a point and a quadratic Bézier curve in 3D.
     */
    PointResult point_quadratic_bezier(const ml7::Vector3& point, const ml7::QuadraticBezier3& bezier)
    {
        return ml7::detail::distance::point_bezier<PointResult>(point, bezier);
    }

    /**
     * Calculates the distance result of a point and a cubic Bézier curve in 3D.
     */
    PointResult point_cubic_bezier(const ml7::Vector3& point, const ml7::CubicBezier3& bezier)
    {
        return ml7::detail::distance::point_bezier<PointResult>(point, bezier);
    }



} // namespace ml7::distance3
