#ifndef ML7_RAY2_H
#define ML7_RAY2_H

#include <MathLabs/Vector2.h>



namespace ml7 {



struct Ray2
{

    static const Ray2 X;
    static const Ray2 Y;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The ray origin point. */
    Vector2 origin;
    /** The ray direction vector (preferably but not necessarily normalized). */
    Vector2 direction;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the ray with zero origin and x-axis direction.
     */
    constexpr Ray2() noexcept
        : origin(0.0f, 0.0f)
        , direction(1.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with zero origin and a parameter for the direction.
     */
    explicit constexpr Ray2(const Vector2& direction) noexcept
        : origin(0.0f, 0.0f)
        , direction(direction)
    {
    }

    /**
     * Explicit constructor with parameters for origin and direction.
     */
    constexpr Ray2(const Vector2& origin, const Vector2& direction) noexcept
        : origin(origin)
        , direction(direction)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Ray2& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this ray at the specified parameter t.
     * The function does not limit the parameter if it is less than 0.
     */
    Vector2 point_at(float t) const;

    /**
     * Calculates the parameter t >= 0 such that the corresponding point on this ray
     * is closest to the specified query point.
     */
    float parameter(const Vector2& point) const;

    /**
     * Finds the closest point on this ray to the specified query point.
     */
    Vector2 closest_point(const Vector2& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this ray.
     */
    float distance(const Vector2& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this ray.
     */
    float distsqr(const Vector2& point) const;



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this ray normalized such that its direction vector has a
     * magnitude of 1.
     */
    Ray2 normalized() const
    {
        return {origin, direction.normalized()};
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this ray that its direction vector has a magnitude of 1.
     */
    Ray2& normalize()
    {
        direction.normalize();
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Ray2& other) const noexcept = default;
    bool operator!=(const Ray2& other) const noexcept = default;

}; // struct Ray2



} // namespace ml7

#endif // ML7_RAY2_H
