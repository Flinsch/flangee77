#ifndef ML7_RAY3_H
#define ML7_RAY3_H

#include <MathLabs/Vector3.h>



namespace ml7 {



struct Ray3
{

    static const Ray3 X;
    static const Ray3 Y;
    static const Ray3 Z;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The ray origin point. */
    Vector3 origin;
    /** The ray direction vector (preferably but not necessarily normalized). */
    Vector3 direction;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the ray with zero origin and z-axis direction.
     */
    constexpr Ray3() noexcept
        : origin(0.0f, 0.0f, 0.0f)
        , direction(0.0f, 0.0f, 1.0f)
    {
    }

    /**
     * Explicit constructor with zero origin and a parameter for the direction.
     */
    explicit constexpr Ray3(const Vector3& direction) noexcept
        : origin(0.0f, 0.0f, 0.0f)
        , direction(direction)
    {
    }

    /**
     * Explicit constructor with parameters for origin and direction.
     */
    constexpr Ray3(const Vector3& origin, const Vector3& direction) noexcept
        : origin(origin)
        , direction(direction)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Ray3& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this ray at the specified distance/parameter.
     */
    Vector3 point_at(float t) const;

    /**
     * Calculate the ray parameter (distance from origin to the closest point) for
     * the specified query point.
     */
    float parameter(const Vector3& point) const;

    /**
     * Finds the closest point on this ray to the specified query point.
     */
    Vector3 closest_point(const Vector3& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this ray.
     */
    float distance(const Vector3& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this ray.
     */
    float distsqr(const Vector3& point) const;



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this ray normalized such that its direction vector has a
     * magnitude of 1.
     */
    Ray3 normalized() const
    {
        return {origin, direction.normalized()};
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this ray that its direction vector has a magnitude of 1.
     */
    Ray3& normalize()
    {
        direction.normalize();
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Ray3& other) const noexcept = default;
    bool operator!=(const Ray3& other) const noexcept = default;

}; // struct Ray3



} // namespace ml7

#endif // ML7_RAY3_H
