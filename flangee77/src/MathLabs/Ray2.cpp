#include "Ray2.h"

#include "./distance2.h"



namespace ml7 {



    const Ray2 Ray2::X =    {{0.0f, 0.0f}, {1.0f, 0.0f}};
    const Ray2 Ray2::Y =    {{0.0f, 0.0f}, {0.0f, 1.0f}};



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Ray2::swap(Ray2& other) noexcept
    {
        origin.swap(other.origin);
        direction.swap(other.direction);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this ray at the specified parameter t.
     * The function does not limit the parameter if it is less than 0.
     */
    Vector2 Ray2::point_at(float t) const
    {
        return origin + t * direction;
    }

    /**
     * Calculates the parameter t >= 0 such that the corresponding point on this ray
     * is closest to the specified query point.
     */
    float Ray2::parameter(const Vector2& point) const
    {
        return distance2::point_ray(point, *this).t;
    }

    /**
     * Finds the closest point on this ray to the specified query point.
     */
    Vector2 Ray2::closest_point(const Vector2& point) const
    {
        return distance2::point_ray(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this ray.
     */
    float Ray2::distance(const Vector2& point) const
    {
        return distance2::point_ray(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this ray.
     */
    float Ray2::distsqr(const Vector2& point) const
    {
        return distance2::point_ray(point, *this).distsqr;
    }



} // namespace ml7
