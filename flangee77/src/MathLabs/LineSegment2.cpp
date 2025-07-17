#include "LineSegment2.h"

#include "./distance2.h"



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void LineSegment2::swap(LineSegment2& other) noexcept
    {
        start.swap(other.start);
        end.swap(other.end);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line segment at the specified parameter t.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector2 LineSegment2::point_at(float t) const
    {
        //return start + t * (end - start);
        return ml7::Vector2::lerp(start, end, t);
    }

    /**
     * Calculates the parameter 0 <= t <= 1 such that the corresponding point on
     * this line segment is closest to the specified query point.
     */
    float LineSegment2::parameter(const Vector2& point) const
    {
        return distance2::point_line_segment(point, *this).t;
    }

    /**
     * Finds the closest point on this line segment to the specified query point.
     */
    Vector2 LineSegment2::closest_point(const Vector2& point) const
    {
        return distance2::point_line_segment(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line
     * segment.
     */
    float LineSegment2::distance(const Vector2& point) const
    {
        return distance2::point_line_segment(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line segment.
     */
    float LineSegment2::distsqr(const Vector2& point) const
    {
        return distance2::point_line_segment(point, *this).distsqr;
    }



} // namespace ml7
