#ifndef ML7_DETAIL_DISTANCE_H
#define ML7_DETAIL_DISTANCE_H

#include <CoreLabs/functional.h>



namespace ml7::detail::distance {



    template <auto clamp, class TPointResult, class TVector>
        requires(cl7::unary_function<decltype(clamp), float> &&
            requires (TPointResult& result) {
            { result.distsqr } -> std::convertible_to<float>;
            { result.point } -> std::convertible_to<TVector>;
            { result.t } -> std::convertible_to<float>;
        })
    TPointResult point_line(const TVector& point, const TVector& origin, const TVector& direction)
    {
        TPointResult result;

        const auto& P = point;
        const auto& A = origin;

        const auto& AB = direction;
        const auto AP = P - A;

        const float len2 = AB.lensqr();
        if (len2 == 0.0f)
        {
            result.distsqr = AP.lensqr();
            result.point = A;
            result.t = 0.0f;
            return result;
        }

        float t = AP.dot(AB) / len2;
        t = clamp(t);

        const auto Q = A + t * AB;
        const auto QP = P - Q;

        result.distsqr = QP.lensqr();
        result.point = Q;
        result.t = t;
        return result;
    }



} // namespace ml7::distance2

#endif // ML7_DETAIL_DISTANCE_H
