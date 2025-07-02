#include "distance3.h"



namespace ml7::distance3 {



    /**
     * Calculates the distance result of a point and a ray in 3D.
     */
    PointResult point_ray(const ml7::Vector3& point, const ml7::Ray3& ray)
    {
        PointResult result;

        const auto& P = point;
        const auto& A = ray.origin;

        const auto& AB = ray.direction;
        const auto AP = P - A;

        const float len3 = AB.lensqr();
        if (len3 == 0.0f)
        {
            result.distsqr = AP.lensqr();
            result.point = A;
            result.t = 0.0f;
            return result;
        }

        float t = AP.dot(AB) / len3;
        t = std::max(t, 0.0f);

        const auto Q = A + t * AB;
        const auto QP = P - Q;

        result.distsqr = QP.lensqr();
        result.point = Q;
        result.t = t;
        return result;
    }



} // namespace ml7::distance3
