#ifndef ML7_QUADRATICBEZIER2_H
#define ML7_QUADRATICBEZIER2_H

#include <MathLabs/Vector2.h>

#include "./detail/distance.h"
#include "./detail/subdivide.h"

#include <span>



namespace ml7 {



template <std::floating_point T>
struct QuadraticBezier2
{

    using scalar_type = T;
    using vector_type = Vector2<T>;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The start point of the quadratic Bézier curve. */
    Vector2<T> p0;
    /** The control point of the quadratic Bézier curve. */
    Vector2<T> p1;
    /** The end point of the quadratic Bézier curve. */
    Vector2<T> p2;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" Bézier curve.
     */
    constexpr QuadraticBezier2() noexcept
        : p0(T{0}, T{0})
        , p1(T{0}, T{0})
        , p2(T{0}, T{0})
    {
    }

    /**
     * Explicit constructor with parameters for the start point, control point, and
     * end point.
     */
    constexpr QuadraticBezier2(const Vector2<T>& p0, const Vector2<T>& p1, const Vector2<T>& p2) noexcept
        : p0(p0)
        , p1(p1)
        , p2(p2)
    {
    }

    /**
     * Swap operation.
     */
    void swap(QuadraticBezier2& other) noexcept
    {
        p0.swap(other.p0);
        p1.swap(other.p1);
        p2.swap(other.p2);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Returns the control points of this Bézier curve.
     */
    std::span<const Vector2<T>> control_points() const { return {&p0, 3}; }

    /**
     * Calculates the position on this Bézier curve at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2<T> point_at(T t) const
    {
        const T u = T{1} - t;

        return u*u * p0 + T{2} * u*t * p1 + t*t * p2;
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

        return T{2} * (u * (p1 - p0) + t * (p2 - p1));
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
        return T{2} * (p2 - T{2} * p1 + p0);
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
    std::pair<QuadraticBezier2, QuadraticBezier2> subdivide(T t) const
    {
        return detail::subdivide::subdivide_quadratic_bezier(*this, t);
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const QuadraticBezier2& other) const noexcept = default;
    bool operator!=(const QuadraticBezier2& other) const noexcept = default;

}; // struct QuadraticBezier2



using QuadraticBezier2f = QuadraticBezier2<float>;
using QuadraticBezier2d = QuadraticBezier2<double>;
using QuadraticBezier2ld = QuadraticBezier2<long double>;



} // namespace ml7

#endif // ML7_QUADRATICBEZIER2_H
