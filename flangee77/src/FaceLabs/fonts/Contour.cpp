#include "Contour.h"



namespace fl7::fonts {



    /** Returns the effective number of Bézier curves. */
    size_t Contour::segment_count() const
    {
        assert(points.empty() || (points.size() >= 3 && points.size() % 2 == 1));
        if (points.empty())
            return 0;
        return (points.size() - 1) / 2;
    }

    /** Returns the quadratic Bézier curve identified by the given index. */
    const ml7::QuadraticBezier2f& Contour::segment_at(size_t index) const
    {
        assert(index < segment_count());
        size_t start_point_index = index * 2;
        assert(start_point_index + 2 < points.size());
        return *reinterpret_cast<const ml7::QuadraticBezier2f*>(&points[start_point_index]);
    }



} // namespace fl7::fonts
