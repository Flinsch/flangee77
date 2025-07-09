#include "distance3.h"

#include "./detail/distance.h"

#include "./functions.h"



namespace ml7::distance3 {



    /**
     * Calculates the distance result of a point and a line in 3D.
     */
    PointResult point_line(const ml7::Vector3& point, const ml7::Line3& line)
    {
        return ml7::detail::distance::point_line<[](float x) { return x; }, PointResult>(point, line.point, line.direction);
    }

    /**
     * Calculates the distance result of a point and a line segment in 3D.
     */
    PointResult point_line_segment(const ml7::Vector3& point, const ml7::LineSegment3& line_segment)
    {
        return ml7::detail::distance::point_line<ml7::clamp01<float>, PointResult>(point, line_segment.start, line_segment.end - line_segment.start);
    }

    /**
     * Calculates the distance result of a point and a ray in 3D.
     */
    PointResult point_ray(const ml7::Vector3& point, const ml7::Ray3& ray)
    {
        return ml7::detail::distance::point_line<ml7::ramp<0.0f, 1.0f, float>, PointResult>(point, ray.origin, ray.direction);
    }



} // namespace ml7::distance3
