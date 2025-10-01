#ifndef ML7_LINESEGMENT3_H
#define ML7_LINESEGMENT3_H

#include <MathLabs/Vector3.h>

#include "./detail/distance.h"

#include "./ops.h"



namespace ml7 {



template <std::floating_point T>
struct LineSegment3
{

    using scalar_type = T;
    using vector_type = Vector3<T>;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The starting point of the line segment. */
    Vector3<T> start;
    /** The ending point of the line segment. */
    Vector3<T> end;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line segment.
     */
    constexpr LineSegment3() noexcept
        : start(T{0}, T{0}, T{0})
        , end(T{0}, T{0}, T{0})
    {
    }

    /**
     * Explicit constructor with parameters for the starting point and ending point.
     */
    constexpr LineSegment3(const Vector3<T>& start, const Vector3<T>& end) noexcept
        : start(start)
        , end(end)
    {
    }

    /**
     * Swap operation.
     */
    void swap(LineSegment3& other) noexcept
    {
        start.swap(other.start);
        end.swap(other.end);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line segment at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3<T> point_at(T t) const
    {
        //return start + t * (end - start);
        return Vector3<T>::lerp(start, end, t);
    }

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this line segment is closest to the specified query point.
     */
    T parameter(const Vector3<T>& point) const
    {
        return detail::distance::point_line<ops::clamp01, detail::distance::PointResult_t<scalar_type>>(point, start, end - start).t;
    }

    /**
     * Finds the closest point on this line segment to the specified query point.
     */
    Vector3<T> closest_point(const Vector3<T>& point) const
    {
        return detail::distance::point_line<ops::clamp01, detail::distance::PointResult_point<vector_type>>(point, start, end - start).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line
     * segment.
     */
    T distance(const Vector3<T>& point) const
    {
        return std::sqrt(distance_squared(point));
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line segment.
     */
    T distance_squared(const Vector3<T>& point) const
    {
        return detail::distance::point_line<ops::clamp01, detail::distance::PointResult_distance_squared<scalar_type>>(point, start, end - start).distance_squared;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const LineSegment3& other) const noexcept = default;
    bool operator!=(const LineSegment3& other) const noexcept = default;

}; // struct LineSegment3



using LineSegment3f = LineSegment3<float>;
using LineSegment3d = LineSegment3<double>;
using LineSegment3ld = LineSegment3<long double>;



} // namespace ml7

#endif // ML7_LINESEGMENT3_H
