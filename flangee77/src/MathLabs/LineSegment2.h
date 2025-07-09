#ifndef ML7_LINESEGMENT2_H
#define ML7_LINESEGMENT2_H

#include <MathLabs/Vector2.h>



namespace ml7 {



struct LineSegment2
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /** The starting point of the line segment. */
    Vector2 start;
    /** The ending point of the line segment. */
    Vector2 end;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line segment.
     */
    constexpr LineSegment2() noexcept
        : start(0.0f, 0.0f)
        , end(0.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for the starting point and ending point.
     */
    constexpr LineSegment2(const Vector2& start, const Vector2& end) noexcept
        : start(start)
        , end(end)
    {
    }

    /**
     * Swap operation.
     */
    void swap(LineSegment2& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line segment at the specified distance/parameter.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 point_at(float t) const;

    /**
     * Calculate the line segment parameter (distance from the starting point to the
     * closest point) for the specified query point.
     */
    float parameter(const Vector2& point) const;

    /**
     * Finds the closest point on this line segment to the specified query point.
     */
    Vector2 closest_point(const Vector2& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this line
     * segment.
     */
    float distance(const Vector2& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line segment.
     */
    float distsqr(const Vector2& point) const;



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const LineSegment2& other) const noexcept = default;
    bool operator!=(const LineSegment2& other) const noexcept = default;

}; // struct LineSegment2



} // namespace ml7

#endif // ML7_LINESEGMENT2_H
