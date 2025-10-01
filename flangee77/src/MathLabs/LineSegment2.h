#ifndef ML7_LINESEGMENT2_H
#define ML7_LINESEGMENT2_H

#include <MathLabs/Vector2.h>

#include "./detail/distance.h"

#include "./ops.h"



namespace ml7 {



template <std::floating_point T>
struct LineSegment2
{

    using scalar_type = T;
    using vector_type = Vector2<T>;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The starting point of the line segment. */
    Vector2<T> start;
    /** The ending point of the line segment. */
    Vector2<T> end;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line segment.
     */
    constexpr LineSegment2() noexcept
        : start(T{0}, T{0})
        , end(T{0}, T{0})
    {
    }

    /**
     * Explicit constructor with parameters for the starting point and ending point.
     */
    constexpr LineSegment2(const Vector2<T>& start, const Vector2<T>& end) noexcept
        : start(start)
        , end(end)
    {
    }

    /**
     * Swap operation.
     */
    void swap(LineSegment2& other) noexcept
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
    Vector2<T> point_at(T t) const
    {
        //return start + t * (end - start);
        return Vector2<T>::lerp(start, end, t);
    }

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this line segment is closest to the specified query point.
     */
    T parameter(const Vector2<T>& point) const
    {
        return detail::distance::point_line<ops::clamp01, detail::distance::PointResult_t<scalar_type>>(point, start, end - start).t;
    }

    /**
     * Finds the closest point on this line segment to the specified query point.
     */
    Vector2<T> closest_point(const Vector2<T>& point) const
    {
        return detail::distance::point_line<ops::clamp01, detail::distance::PointResult_point<vector_type>>(point, start, end - start).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line
     * segment.
     */
    T distance(const Vector2<T>& point) const
    {
        return std::sqrt(distance_squared(point));
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line segment.
     */
    T distance_squared(const Vector2<T>& point) const
    {
        return detail::distance::point_line<ops::clamp01, detail::distance::PointResult_distance_squared<scalar_type>>(point, start, end - start).distance_squared;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const LineSegment2& other) const noexcept = default;
    bool operator!=(const LineSegment2& other) const noexcept = default;

}; // struct LineSegment2



using LineSegment2f = LineSegment2<float>;
using LineSegment2d = LineSegment2<double>;
using LineSegment2ld = LineSegment2<long double>;



} // namespace ml7

#endif // ML7_LINESEGMENT2_H
