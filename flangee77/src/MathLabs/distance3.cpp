#include "distance3.h"

#include "./detail/distance.h"

#include "./functions.h"



namespace ml7::distance3 {



    /**
     * Calculates the distance result of a point and a ray in 3D.
     */
    PointResult point_ray(const ml7::Vector3& point, const ml7::Ray3& ray)
    {
        return ml7::detail::distance::point_line<ml7::ramp<0.0f, 1.0f, float>, PointResult>(point, ray.origin, ray.direction);
    }



} // namespace ml7::distance3
