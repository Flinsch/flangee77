#ifndef ML7_DISTANCE3_H
#define ML7_DISTANCE3_H

#include "./Vector3.h"
#include "./Line3.h"
#include "./LineSegment3.h"
#include "./Ray3.h"
#include "./QuadraticBezier3.h"
#include "./CubicBezier3.h"

#include "./detail/distance.h"

#include "./ops.h"



namespace ml7::distance3 {



template <std::floating_point T>
struct ResultBase
{
    /** The squared distance between two shapes. */
    T distance_squared;
    /** Calculates the distance between two shapes by taking the square root of the squared distance. */
    T distance() const { assert(distance_squared >= T{0}); return std::sqrt(distance_squared); }
};

template <std::floating_point T>
struct PointResult
    : public ResultBase<T>
{
    /** The closest point on the other shape. */
    Vector3<T> point;
    /** The parametric distance along the other shape. */
    T t;

    PointResult() noexcept = default;
    PointResult(T distance_squared, const Vector3<T>& point, T t) noexcept : ResultBase<T>(distance_squared), point(point), t(t) {}
};



    /**
     * Calculates the distance result of a point and a line in 3D.
     */
    template <std::floating_point T>
    PointResult<T> point_line(const Vector3<T>& point, const Line3<T>& line)
    {
        return detail::distance::point_line<std::identity, PointResult<T>>(point, line.point, line.direction);
    }

    /**
     * Calculates the distance result of a point and a line segment in 3D.
     */
    template <std::floating_point T>
    PointResult<T> point_line_segment(const Vector3<T>& point, const LineSegment3<T>& line_segment)
    {
        return detail::distance::point_line<ops::clamp01, PointResult<T>>(point, line_segment.start, line_segment.end - line_segment.start);
    }

    /**
     * Calculates the distance result of a point and a ray in 3D.
     */
    template <std::floating_point T>
    PointResult<T> point_ray(const Vector3<T>& point, const Ray3<T>& ray)
    {
        return detail::distance::point_line<ops::ramp<>, PointResult<T>>(point, ray.origin, ray.direction);
    }

    /**
     * Calculates the distance result of a point and a quadratic Bézier curve in 3D.
     */
    template <std::floating_point T>
    PointResult<T> point_quadratic_bezier(const Vector3<T>& point, const QuadraticBezier3<T>& bezier)
    {
        return detail::distance::point_bezier<PointResult<T>>(point, bezier);
    }

    /**
     * Calculates the distance result of a point and a cubic Bézier curve in 3D.
     */
    template <std::floating_point T>
    PointResult<T> point_cubic_bezier(const Vector3<T>& point, const CubicBezier3<T>& bezier)
    {
        return detail::distance::point_bezier<PointResult<T>>(point, bezier);
    }



} // namespace ml7::distance3

#endif // ML7_DISTANCE3_H
