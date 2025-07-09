#include "Line3.h"

#include "./distance3.h"



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Line3::swap(Line3& other) noexcept
    {
        point.swap(other.point);
        direction.swap(other.direction);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line at the specified distance parameter.
     */
    Vector3 Line3::point_at(float t) const
    {
        return point + t * direction;
    }

    /**
     * Calculate the line parameter (distance from the anchor point to the closest
     * point) for the specified query point.
     */
    float Line3::parameter(const Vector3& point) const
    {
        return distance3::point_line(point, *this).t;
    }

    /**
     * Finds the closest point on this line to the specified query point.
     */
    Vector3 Line3::closest_point(const Vector3& point) const
    {
        return distance3::point_line(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line.
     */
    float Line3::distance(const Vector3& point) const
    {
        return distance3::point_line(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line.
     */
    float Line3::distsqr(const Vector3& point) const
    {
        return distance3::point_line(point, *this).distsqr;
    }



} // namespace ml7
