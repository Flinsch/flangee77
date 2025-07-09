#ifndef ML7_LINE3_H
#define ML7_LINE3_H

#include <MathLabs/Vector3.h>



namespace ml7 {



struct Line3
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /** A point on the line. */
    Vector3 point;
    /** A direction vector (not necessarily normalized). */
    Vector3 direction;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line.
     */
    constexpr Line3() noexcept
        : point(0.0f, 0.0f, 0.0f)
        , direction(0.0f, 0.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for a point on the line and a direction.
     */
    constexpr Line3(const Vector3& point, const Vector3& direction) noexcept
        : point(point)
        , direction(direction)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Line3& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line at the specified distance/parameter.
     */
    Vector3 point_at(float t) const;

    /**
     * Calculate the line parameter (distance from the anchor point to the closest
     * point) for the specified query point.
     */
    float parameter(const Vector3& point) const;

    /**
     * Finds the closest point on this line to the specified query point.
     */
    Vector3 closest_point(const Vector3& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this line.
     */
    float distance(const Vector3& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line.
     */
    float distsqr(const Vector3& point) const;



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this line normalized such that its direction vector has a
     * magnitude of 1.
     */
    Line3 normalized() const
    {
        return {point, direction.normalized()};
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this line that its direction vector has a magnitude of 1.
     */
    Line3& normalize()
    {
        direction.normalize();
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Line3& other) const noexcept = default;
    bool operator!=(const Line3& other) const noexcept = default;



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a line from a point on the line and a direction vector.
     */
    static constexpr Line3 from_point_and_direction(const Vector3& point, const Vector3& direction)
    {
        return {point, direction};
    }

    /**
     * Initializes a line from two points on the line.
     */
    static constexpr Line3 from_two_points(const Vector3& point1, const Vector3& point2)
    {
        return {point1, point2 - point1};
    }

}; // struct Line3



} // namespace ml7

#endif // ML7_LINE3_H
