#ifndef ML7_CUBICBEZIER2_H
#define ML7_CUBICBEZIER2_H

#include <MathLabs/Vector2.h>

#include "./detail/distance.h"
#include "./detail/subdivide.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct CubicBezier2
{

    using scalar_type = T;
    using vector_type = Vector2<T>;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The start point of the cubic Bézier curve. */
    Vector2<T> p0;
    /** The first control point of the cubic Bézier curve. */
    Vector2<T> p1;
    /** The second control point of the cubic Bézier curve. */
    Vector2<T> p2;
    /** The end point of the cubic Bézier curve. */
    Vector2<T> p3;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" Bézier curve.
     */
    constexpr CubicBezier2() noexcept
        : p0(T{0}, T{0})
        , p1(T{0}, T{0})
        , p2(T{0}, T{0})
        , p3(T{0}, T{0})
    {
    }

    /**
     * Explicit constructor with parameters for the start point, first and second
     * control points, and end point.
     */
    constexpr CubicBezier2(const Vector2<T>& p0, const Vector2<T>& p1, const Vector2<T>& p2, const Vector2<T>& p3) noexcept
        : p0(p0)
        , p1(p1)
        , p2(p2)
        , p3(p3)
    {
    }

    /**
     * Swap operation.
     */
    void swap(CubicBezier2& other) noexcept
    {
        p0.swap(other.p0);
        p1.swap(other.p1);
        p2.swap(other.p2);
        p3.swap(other.p3);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Returns the control points of this Bézier curve.
     */
    std::span<const Vector2<T>> control_points() const { return {&p0, 4}; }

    /**
     * Calculates the position on this Bézier curve at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2<T> point_at(T t) const
    {
        const T u = T{1} - t;
        const T uu = u * u;
        const T tt = t * t;

        return uu*u * p0 + T{3} * (uu*t * p1 + u*tt * p2) + tt*t * p3;
    }

    /**
     * Calculates the tangent of this Bézier curve at the specified parameter t.
     * The tangent vector is the first derivative of the curve with respect to t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2<T> tangent_at(T t) const
    {
        const T u = T{1} - t;

        return T{3} * (u*u * (p1 - p0) + T{2} * u*t * (p2 - p1) + t*t * (p3 - p2));
    }

    /**
     * Calculates the second derivative of this Bézier curve at the specified
     * parameter t. The second derivative represents the rate of change of the
     * tangent vector, which is the first derivative of the curve.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2<T> second_derivative_at(T t) const
    {
        const T u = T{1} - t;

        return T{6} * (u * (p2 - T{2} * p1 + p0) + t * (p3 - T{2} * p2 + p1));
    }

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this Bézier curve is closest to the specified query point.
     */
    T parameter(const Vector2<T>& point) const
    {
        return detail::distance::point_bezier<detail::distance::PointResult_t<scalar_type>>(point, *this).t;
    }

    /**
     * Finds the closest point on this Bézier curve to the specified query point.
     */
    Vector2<T> closest_point(const Vector2<T>& point) const
    {
        return detail::distance::point_bezier<detail::distance::PointResult_point<vector_type>>(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this Bézier
     * curve.
     */
    T distance(const Vector2<T>& point) const
    {
        return std::sqrt(distance_squared(point));
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this Bézier curve.
     */
    T distance_squared(const Vector2<T>& point) const
    {
        return detail::distance::point_bezier<detail::distance::PointResult_distance_squared<scalar_type>>(point, *this).distance_squared;
    }



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Subdivides this Bézier curve at the specified parameter t and returns a pair
     * of new objects representing the two segments.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    std::pair<CubicBezier2, CubicBezier2> subdivide(T t) const
    {
        return detail::subdivide::subdivide_cubic_bezier(*this, t);
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const CubicBezier2& other) const noexcept = default;
    bool operator!=(const CubicBezier2& other) const noexcept = default;

}; // struct CubicBezier2



using CubicBezier2f = CubicBezier2<float>;
using CubicBezier2d = CubicBezier2<double>;
using CubicBezier2ld = CubicBezier2<long double>;



} // namespace ml7

#endif // ML7_CUBICBEZIER2_H
