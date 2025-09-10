#ifndef ML7_QUADRATICBEZIER2_H
#define ML7_QUADRATICBEZIER2_H

#include <MathLabs/Vector2.h>

#include <span>



namespace ml7 {



struct QuadraticBezier2
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /** The start point of the quadratic Bézier curve. */
    Vector2 p0;
    /** The control point of the quadratic Bézier curve. */
    Vector2 p1;
    /** The end point of the quadratic Bézier curve. */
    Vector2 p2;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" Bézier curve.
     */
    constexpr QuadraticBezier2() noexcept
        : p0(0.0f, 0.0f)
        , p1(0.0f, 0.0f)
        , p2(0.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for the start point, control point, and
     * end point.
     */
    constexpr QuadraticBezier2(const Vector2& p0, const Vector2& p1, const Vector2& p2) noexcept
        : p0(p0)
        , p1(p1)
        , p2(p2)
    {
    }

    /**
     * Swap operation.
     */
    void swap(QuadraticBezier2& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Returns the control points of this Bézier curve.
     */
    std::span<const Vector2> control_points() const { return {&p0, 3}; }

    /**
     * Calculates the position on this Bézier curve at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 point_at(float t) const;

    /**
     * Calculates the tangent of this Bézier curve at the specified parameter t.
     * The tangent vector is the first derivative of the curve with respect to t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 tangent_at(float t) const;

    /**
     * Calculates the second derivative of this Bézier curve at the specified
     * parameter t. The second derivative represents the rate of change of the
     * tangent vector, which is the first derivative of the curve.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 second_derivative_at(float t) const;

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this Bézier curve is closest to the specified query point.
     */
    float parameter(const Vector2& point) const;

    /**
     * Finds the closest point on this Bézier curve to the specified query point.
     */
    Vector2 closest_point(const Vector2& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this Bézier
     * curve.
     */
    float distance(const Vector2& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this Bézier curve.
     */
    float distance_squared(const Vector2& point) const;



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Subdivides this Bézier curve at the specified parameter t and returns a pair
     * of new objects representing the two segments.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    std::pair<QuadraticBezier2, QuadraticBezier2> subdivide(float t) const;



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const QuadraticBezier2& other) const noexcept = default;
    bool operator!=(const QuadraticBezier2& other) const noexcept = default;

}; // struct QuadraticBezier2



} // namespace ml7

#endif // ML7_QUADRATICBEZIER2_H
