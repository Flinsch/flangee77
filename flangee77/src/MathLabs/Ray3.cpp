#include "Ray3.h"

#include "./distance3.h"



namespace ml7 {



    const Ray3 Ray3::X =    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}};
    const Ray3 Ray3::Y =    {{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}};
    const Ray3 Ray3::Z =    {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Ray3::swap(Ray3& other) noexcept
    {
        origin.swap(other.origin);
        direction.swap(other.direction);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this ray at the specified distance parameter.
     */
    Vector3 Ray3::point_at(float t) const
    {
        return origin + t * direction;
    }

    /**
     * Calculate the ray parameter (distance from origin to the closest point) for
     * the specified query point.
     */
    float Ray3::parameter(const Vector3& point) const
    {
        return distance3::point_ray(point, *this).t;
    }

    /**
     * Finds the closest point on this ray to the specified query point.
     */
    Vector3 Ray3::closest_point(const Vector3& point) const
    {
        return distance3::point_ray(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this ray.
     */
    float Ray3::distance(const Vector3& point) const
    {
        return distance3::point_ray(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this ray.
     */
    float Ray3::distsqr(const Vector3& point) const
    {
        return distance3::point_ray(point, *this).distsqr;
    }



} // namespace ml7
