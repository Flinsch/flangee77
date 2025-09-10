#ifndef ML7_CUBICBEZIER3_H
#define ML7_CUBICBEZIER3_H

#include <MathLabs/Vector3.h>

#include <span>



namespace ml7 {



struct CubicBezier3
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /** The start point of the cubic Bézier curve. */
    Vector3 p0;
    /** The first control point of the cubic Bézier curve. */
    Vector3 p1;
    /** The second control point of the cubic Bézier curve. */
    Vector3 p2;
    /** The end point of the cubic Bézier curve. */
    Vector3 p3;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" Bézier curve.
     */
    constexpr CubicBezier3() noexcept
        : p0(0.0f, 0.0f, 0.0f)
        , p1(0.0f, 0.0f, 0.0f)
        , p2(0.0f, 0.0f, 0.0f)
        , p3(0.0f, 0.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for the start point, first and second
     * control points, and end point.
     */
    constexpr CubicBezier3(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3) noexcept
        : p0(p0)
        , p1(p1)
        , p2(p2)
        , p3(p3)
    {
    }

    /**
     * Swap operation.
     */
    void swap(CubicBezier3& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Returns the control points of this Bézier curve.
     */
    std::span<const Vector3> control_points() const { return {&p0, 4}; }

    /**
     * Calculates the position on this Bézier curve at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 point_at(float t) const;

    /**
     * Calculates the tangent of this Bézier curve at the specified parameter t.
     * The tangent vector is the first derivative of the curve with respect to t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 tangent_at(float t) const;

    /**
     * Calculates the second derivative of this Bézier curve at the specified
     * parameter t. The second derivative represents the rate of change of the
     * tangent vector, which is the first derivative of the curve.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 second_derivative_at(float t) const;

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this Bézier curve is closest to the specified query point.
     */
    float parameter(const Vector3& point) const;

    /**
     * Finds the closest point on this Bézier curve to the specified query point.
     */
    Vector3 closest_point(const Vector3& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this Bézier
     * curve.
     */
    float distance(const Vector3& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this Bézier curve.
     */
    float distsqr(const Vector3& point) const;



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Subdivides this Bézier curve at the specified parameter t and returns a pair
     * of new objects representing the two segments.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    std::pair<CubicBezier3, CubicBezier3> subdivide(float t) const;



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const CubicBezier3& other) const noexcept = default;
    bool operator!=(const CubicBezier3& other) const noexcept = default;

}; // struct CubicBezier3



} // namespace ml7

#endif // ML7_CUBICBEZIER3_H
