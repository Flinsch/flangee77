#include "QuadraticBezier3.h"

#include "./distance3.h"



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void QuadraticBezier3::swap(QuadraticBezier3& other) noexcept
    {
        p0.swap(other.p0);
        p1.swap(other.p1);
        p2.swap(other.p2);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this Bézier curve at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 QuadraticBezier3::point_at(float t) const
    {
        const float u = 1.0f - t;

        return u*u * p0 + 2.0f * u*t * p1 + t*t * p2;
    }

    /**
     * Calculates the tangent of this Bézier curve at the specified parameter t.
     * The tangent vector is the first derivative of the curve with respect to t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 QuadraticBezier3::tangent_at(float t) const
    {
        const float u = 1.0f - t;

        return 2.0f * (u * (p1 - p0) + t * (p2 - p1));
    }

    /**
     * Calculates the second derivative of this Bézier curve at the specified
     * parameter t. The second derivative represents the rate of change of the
     * tangent vector, which is the first derivative of the curve.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 QuadraticBezier3::second_derivative_at(float t) const
    {
        return 2.0f * (p2 - 2.0f * p1 + p0);
    }

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this Bézier curve is closest to the specified query point.
     */
    float QuadraticBezier3::parameter(const Vector3& point) const
    {
        return distance3::point_quadratic_bezier(point, *this).t;
    }

    /**
     * Finds the closest point on this Bézier curve to the specified query point.
     */
    Vector3 QuadraticBezier3::closest_point(const Vector3& point) const
    {
        return distance3::point_quadratic_bezier(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this Bézier
     * curve.
     */
    float QuadraticBezier3::distance(const Vector3& point) const
    {
        return distance3::point_quadratic_bezier(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this Bézier curve.
     */
    float QuadraticBezier3::distance_squared(const Vector3& point) const
    {
        return distance3::point_quadratic_bezier(point, *this).distance_squared;
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
    std::pair<QuadraticBezier3, QuadraticBezier3> QuadraticBezier3::subdivide(float t) const
    {
        const float u = 1.0f - t;

        const Vector3 p01 = u * p0 + t * p1;
        const Vector3 p12 = u * p1 + t * p2;
        const Vector3 p012 = u * p01 + t * p12;

        return {{p0, p01, p012}, {p012, p12, p2}};
    }



} // namespace ml7
