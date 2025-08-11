#include "Line2.h"

#include "./distance2.h"



namespace ml7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Swap operation.
     */
    void Line2::swap(Line2& other) noexcept
    {
        point.swap(other.point);
        direction.swap(other.direction);
    }



    // #############################################################################
    // Properties / Queries
    // #############################################################################

    /**
     * Calculates the position on this line at the specified parameter t.
     */
    Vector2 Line2::point_at(float t) const
    {
        return point + t * direction;
    }

    /**
     * Calculates the parameter t such that the corresponding point on this line is
     * closest to the specified query point.
     */
    float Line2::parameter(const Vector2& point) const
    {
        return distance2::point_line(point, *this).t;
    }

    /**
     * Finds the closest point on this line to the specified query point.
     */
    Vector2 Line2::closest_point(const Vector2& point) const
    {
        return distance2::point_line(point, *this).point;
    }

    /**
     * Calculates the minimum distance from the specified query point to this line.
     */
    float Line2::distance(const Vector2& point) const
    {
        return distance2::point_line(point, *this).distance();
    }

    /**
     * Calculates the minimum squared distance from the specified query point to
     * this line.
     */
    float Line2::distsqr(const Vector2& point) const
    {
        return distance2::point_line(point, *this).distsqr;
    }



} // namespace ml7
