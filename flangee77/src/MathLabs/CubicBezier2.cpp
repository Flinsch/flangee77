#include "CubicBezier2.h"

#include "./distance2.h"



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void CubicBezier2::swap(CubicBezier2& other) noexcept
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
     * Calculates the position on this Bézier curve at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 CubicBezier2::point_at(float t) const
    {
        const float u = 1.0f - t;
        const float uu = u * u;
        const float tt = t * t;

        return uu*u * p0 + 3.0f * (uu*t * p1 + u*tt * p2) + tt*t * p3;
    }

    /**
     * Calculates the tangent of this Bézier curve at the specified parameter t.
     * The tangent vector is the first derivative of the curve with respect to t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 CubicBezier2::tangent_at(float t) const
    {
        const float u = 1.0f - t;

        return 3.0f * (u*u * (p1 - p0) + 2.0f * u*t * (p2 - p1) + t*t * (p3 - p2));
    }

    /**
     * Calculates the second derivative of this Bézier curve at the specified
     * parameter t. The second derivative represents the rate of change of the
     * tangent vector, which is the first derivative of the curve.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 CubicBezier2::second_derivative_at(float t) const
    {
        const float u = 1.0f - t;

        return 6.0f * (u * (p2 - 2.0f * p1 + p0) + t * (p3 - 2.0f * p2 + p1));
    }

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this Bézier curve is closest to the specified query point.
     */
    float CubicBezier2::parameter(const Vector2& point) const
    {
        return distance2::point_cubic_bezier(point, *this).t;
    }

    /**
     * Finds the closest point on this Bézier curve to the specified query point.
     */
    Vector2 CubicBezier2::closest_point(const Vector2& point) const
    {
        return distance2::point_cubic_bezier(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this Bézier
     * curve.
     */
    float CubicBezier2::distance(const Vector2& point) const
    {
        return distance2::point_cubic_bezier(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this Bézier curve.
     */
    float CubicBezier2::distance_squared(const Vector2& point) const
    {
        return distance2::point_cubic_bezier(point, *this).distance_squared;
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
    std::pair<CubicBezier2, CubicBezier2> CubicBezier2::subdivide(float t) const
    {
        const float u = 1.0f - t;

        const Vector2 p01 = u * p0 + t * p1;
        const Vector2 p12 = u * p1 + t * p2;
        const Vector2 p23 = u * p2 + t * p3;
        const Vector2 p012 = u * p01 + t * p12;
        const Vector2 p123 = u * p12 + t * p23;
        const Vector2 p0123 = u * p012 + t * p123;

        return {{p0, p01, p012, p0123}, {p0123, p123, p23, p3}};
    }



} // namespace ml7
