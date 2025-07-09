#include "distance2.h"

#include "./detail/distance.h"

#include "./ops.h"



namespace ml7::distance2 {



    /**
     * Calculates the distance result of a point and a line in 2D.
     */
    PointResult point_line(const ml7::Vector2& point, const ml7::Line2& line)
    {
        return ml7::detail::distance::point_line<std::identity, PointResult>(point, line.point, line.direction);
    }

    /**
     * Calculates the distance result of a point and a line segment in 2D.
     */
    PointResult point_line_segment(const ml7::Vector2& point, const ml7::LineSegment2& line_segment)
    {
        return ml7::detail::distance::point_line<ml7::ops::clamp01, PointResult>(point, line_segment.start, line_segment.end - line_segment.start);
    }

    /**
     * Calculates the distance result of a point and a ray in 2D.
     */
    PointResult point_ray(const ml7::Vector2& point, const ml7::Ray2& ray)
    {
        return ml7::detail::distance::point_line<ml7::ops::ramp<>, PointResult>(point, ray.origin, ray.direction);
    }



} // namespace ml7::distance2
