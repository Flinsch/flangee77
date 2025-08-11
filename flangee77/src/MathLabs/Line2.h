#ifndef ML7_LINE2_H
#define ML7_LINE2_H

#include <MathLabs/Vector2.h>



namespace ml7 {



struct Line2
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /** A point on the line. */
    Vector2 point;
    /** A direction vector (not necessarily normalized). */
    Vector2 direction;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line.
     */
    constexpr Line2() noexcept
        : point(0.0f, 0.0f)
        , direction(0.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for a point on the line and a direction.
     */
    constexpr Line2(const Vector2& point, const Vector2& direction) noexcept
        : point(point)
        , direction(direction)
    {
    }

    /**
     * Swap operation.
     */
    void swap(Line2& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line at the specified parameter t.
     */
    Vector2 point_at(float t) const;

    /**
     * Calculates the parameter t such that the corresponding point on this line is
     * closest to the specified query point.
     */
    float parameter(const Vector2& point) const;

    /**
     * Finds the closest point on this line to the specified query point.
     */
    Vector2 closest_point(const Vector2& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this line.
     */
    float distance(const Vector2& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line.
     */
    float distsqr(const Vector2& point) const;



    // #############################################################################
    // Transformations
    // #############################################################################

    /**
     * Returns a copy of this line normalized such that its direction vector has a
     * magnitude of 1.
     */
    Line2 normalized() const
    {
        return {point, direction.normalized()};
    }



    // #############################################################################
    // Manipulations
    // #############################################################################

    /**
     * Normalizes this line that its direction vector has a magnitude of 1.
     */
    Line2& normalize()
    {
        direction.normalize();
        return *this;
    }



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const Line2& other) const noexcept = default;
    bool operator!=(const Line2& other) const noexcept = default;



    // #############################################################################
    // Static Functions
    // #############################################################################

    /**
     * Initializes a line from a point on the line and a direction vector.
     */
    static constexpr Line2 from_point_and_direction(const Vector2& point, const Vector2& direction)
    {
        return {point, direction};
    }

    /**
     * Initializes a line from two points on the line.
     */
    static constexpr Line2 from_two_points(const Vector2& point1, const Vector2& point2)
    {
        return {point1, point2 - point1};
    }

}; // struct Line2



} // namespace ml7

#endif // ML7_LINE2_H
