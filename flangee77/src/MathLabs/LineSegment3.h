#ifndef ML7_LINESEGMENT3_H
#define ML7_LINESEGMENT3_H

#include <MathLabs/Vector3.h>



namespace ml7 {



struct LineSegment3
{

    // #############################################################################
    // Attributes
    // #############################################################################

    /** The starting point of the line segment. */
    Vector3 start;
    /** The ending point of the line segment. */
    Vector3 end;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor. Initializes an "empty" line segment.
     */
    constexpr LineSegment3() noexcept
        : start(0.0f, 0.0f, 0.0f)
        , end(0.0f, 0.0f, 0.0f)
    {
    }

    /**
     * Explicit constructor with parameters for the starting point and ending point.
     */
    constexpr LineSegment3(const Vector3& start, const Vector3& end) noexcept
        : start(start)
        , end(end)
    {
    }

    /**
     * Swap operation.
     */
    void swap(LineSegment3& other) noexcept;



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line segment at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 point_at(float t) const;

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this line segment is closest to the specified query point.
     */
    float parameter(const Vector3& point) const;

    /**
     * Finds the closest point on this line segment to the specified query point.
     */
    Vector3 closest_point(const Vector3& point) const;

    /**
     * Calculates the minimum distance from the specified query point to this line
     * segment.
     */
    float distance(const Vector3& point) const;

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line segment.
     */
    float distsqr(const Vector3& point) const;



    // #############################################################################
    // Comparison Operators
    // #############################################################################

    bool operator==(const LineSegment3& other) const noexcept = default;
    bool operator!=(const LineSegment3& other) const noexcept = default;

}; // struct LineSegment3



} // namespace ml7

#endif // ML7_LINESEGMENT3_H
