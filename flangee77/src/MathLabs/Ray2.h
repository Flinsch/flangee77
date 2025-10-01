#ifndef ML7_RAY2_H
#define ML7_RAY2_H

#include "./Vector2.h"

#include "./detail/distance.h"

#include "./ops.h"



namespace ml7 {



template <std::floating_point T>
struct Ray2
{

    using scalar_type = T;
    using vector_type = Vector2<T>;

    static const Ray2 X;
    static const Ray2 Y;



    // #############################################################################
    // Attributes
    // #############################################################################

    /** The ray origin point. */
    Vector2<T> origin;
    /** The ray direction vector (preferably but not necessarily normalized). */
    Vector2<T> direction;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes the ray with zero origin and x-axis direction.
     */
    constexpr Ray2() noexcept
        : origin(T{0}, T{0})
        , direction(T{1}, T{0})
    {
    }

    /**
     * Explicit constructor with zero origin and a parameter for the direction.
     */
    explicit constexpr Ray2(const Vector2<T>& direction) noexcept
        : origin(T{0}, T{0})
        , direction(direction)
    {
    }

    /**
     * Explicit constructor with parameters for origin and direction.
     */
    constexpr Ray2(const Vector2<T>& origin, const Vector2<T>& direction) noexcept
        : origin(origin)
        , direction(direction)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Ray2& other) noexcept
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
    Vector2<T> point_at(T t) const
    {
        return origin + t * direction;
    }

    /**
     * Calculates the parameter t >= 0 such that the corresponding point on this ray
     * is closest to the specified query point.
     */
    T parameter(const Vector2<T>& point) const
    {
        return detail::distance::point_line<ops::ramp<>, detail::distance::PointResult_t<scalar_type>>(point, origin, direction).t;
    }

    /**
     * Finds the closest point on this ray to the specified query point.
     */
    Vector2<T> closest_point(const Vector2<T>& point) const
    {
        return detail::distance::point_line<ops::ramp<>, detail::distance::PointResult_point<vector_type>>(point, origin, direction).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this ray.
     */
    T distance(const Vector2<T>& point) const
    {
        return std::sqrt(distance_squared(point));
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this ray.
     */
    T distance_squared(const Vector2<T>& point) const
    {
        return detail::distance::point_line<ops::ramp<>, detail::distance::PointResult_distance_squared<scalar_type>>(point, origin, direction).distance_squared;
    }



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



    template <std::floating_point T>
    const Ray2<T> Ray2<T>::X =    {{T{0}, T{0}}, {T{1}, T{0}}};
    template <std::floating_point T>
    const Ray2<T> Ray2<T>::Y =    {{T{0}, T{0}}, {T{0}, T{1}}};



using Ray2f = Ray2<float>;
using Ray2d = Ray2<double>;
using Ray2ld = Ray2<long double>;



} // namespace ml7

#endif // ML7_RAY2_H
