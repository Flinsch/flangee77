#include "LineSegment3.h"

#include "./distance3.h"



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void LineSegment3::swap(LineSegment3& other) noexcept
    {
        start.swap(other.start);
        end.swap(other.end);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line segment at the specified distance parameter.
     * The function does not limit the parameter if it is less than 0 or greater
     * than 1.
     */
    Vector3 LineSegment3::point_at(float t) const
    {
        //return start + t * (end - start);
        return ml7::Vector3::lerp(start, end, t);
    }

    /**
     * Calculate the line segment parameter (distance from the starting point to the
     * closest point) for the specified query point.
     */
    float LineSegment3::parameter(const Vector3& point) const
    {
        return distance3::point_line_segment(point, *this).t;
    }

    /**
     * Finds the closest point on this line segment to the specified query point.
     */
    Vector3 LineSegment3::closest_point(const Vector3& point) const
    {
        return distance3::point_line_segment(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line
     * segment.
     */
    float LineSegment3::distance(const Vector3& point) const
    {
        return distance3::point_line_segment(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line segment.
     */
    float LineSegment3::distsqr(const Vector3& point) const
    {
        return distance3::point_line_segment(point, *this).distsqr;
    }



} // namespace ml7
