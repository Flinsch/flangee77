#ifndef ML7_LINE2_H
#define ML7_LINE2_H

#include <MathLabs/Vector2.h>

#include "./detail/distance.h"



namespace ml7 {



template <std::floating_point T>
struct Line2
{

    using scalar_type = T;
    using vector_type = Vector2<T>;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** A point on the line. */
    Vector2<T> point;
    /** A direction vector (not necessarily normalized). */
    Vector2<T> direction;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line.
     */
    constexpr Line2() noexcept
        : point(T{0}, T{0})
        , direction(T{0}, T{0})
    {
    }

    /**
     * Explicit constructor with parameters for a point on the line and a direction.
     */
    constexpr Line2(const Vector2<T>& point, const Vector2<T>& direction) noexcept
        : point(point)
        , direction(direction)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Line2& other) noexcept
    {
        point.swap(other.point);
        direction.swap(other.direction);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line at the specified parameter t.
     */
    Vector2<T> point_at(T t) const
    {
        return point + t * direction;
    }

    /**
     * Calculates the parameter t such that the corresponding point on this line is
     * closest to the specified query point.
     */
    T parameter(const Vector2<T>& point) const
    {
        return detail::distance::point_line<std::identity, detail::distance::PointResult_t<scalar_type>>(point, this->point, direction).t;
    }

    /**
     * Finds the closest point on this line to the specified query point.
     */
    Vector2<T> closest_point(const Vector2<T>& point) const
    {
        return detail::distance::point_line<std::identity, detail::distance::PointResult_point<vector_type>>(point, this->point, direction).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line.
     */
    T distance(const Vector2<T>& point) const
    {
        return std::sqrt(distance_squared(point));
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line.
     */
    T distance_squared(const Vector2<T>& point) const
    {
        return detail::distance::point_line<std::identity, detail::distance::PointResult_distance_squared<scalar_type>>(point, this->point, direction).distance_squared;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this line normalized such that its direction vector has a
     * magnitude of 1.
     */
    Line2 normalized() const
    {
        return {point, direction.normalized()};
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this line that its direction vector has a magnitude of 1.
     */
    Line2& normalize()
    {
        direction.normalize();
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Line2& other) const noexcept = default;
    bool operator!=(const Line2& other) const noexcept = default;



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a line from a point on the line and a direction vector.
     */
    static constexpr Line2 from_point_and_direction(const Vector2<T>& point, const Vector2<T>& direction)
    {
        return {point, direction};
    }

    /**
     * Initializes a line from two points on the line.
     */
    static constexpr Line2 from_two_points(const Vector2<T>& point1, const Vector2<T>& point2)
    {
        return {point1, point2 - point1};
    }

}; // struct Line2



using Line2f = Line2<float>;
using Line2d = Line2<double>;
using Line2ld = Line2<long double>;



} // namespace ml7

#endif // ML7_LINE2_H
