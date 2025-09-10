#ifndef ML7_DETAIL_DISTANCE_H
#define ML7_DETAIL_DISTANCE_H

#include <MathLabs/ops.h>

#include <CoreLabs/functional.h>



namespace ml7::detail::distance {



    template <typename clamp, class TPointResult, class TVector>
        requires(requires (TPointResult& result) {
            { result.distsqr } -> std::convertible_to<float>;
            { result.point } -> std::convertible_to<TVector>;
            { result.t } -> std::convertible_to<float>;
        } && requires {
            { clamp{}(float{}) } -> std::convertible_to<float>;
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
        t = clamp{}(t);

        const auto Q = A + t * AB;
        const auto QP = P - Q;

        result.distsqr = QP.lensqr();
        result.point = Q;
        result.t = t;
        return result;
    }



    template <class TPointResult, class TVector, class TBezier>
        requires(requires (TPointResult& result) {
            { result.distsqr } -> std::convertible_to<float>;
            { result.point } -> std::convertible_to<TVector>;
            { result.t } -> std::convertible_to<float>;
        })
    TPointResult point_bezier(const TVector& point, const TBezier& bezier)
    {
        // TODO:
        // For better performance, precision, and/or robustness, switch to numeric root finding
        // (e.g., subdivision and Newton-Raphson refinement) instead of symbolic polynomials?

        struct StoppingCriteria
        {
            float flatness_tolerance = 1e-5f;
            unsigned max_iteration_depth = 10;
        };

        struct Helper
        {
            static float compute_flatness(const TBezier& bezier)
            {
                float max_dd = 0.0f;

                for (size_t i = 1; i + 1 < bezier.control_points().size(); ++i)
                {
                    const float dd = point_line<ml7::ops::clamp01, TPointResult>(bezier.control_points()[i], bezier.control_points().front(), bezier.control_points().back() - bezier.control_points().front()).distsqr;
                    max_dd = std::max(max_dd, dd);
                }

                return std::sqrt(max_dd);
            }

            static TPointResult recurse(const TVector& point, const TBezier& bezier, const StoppingCriteria& criteria, float t0 = 0.0f, float t1 = 1.0f, unsigned depth = 0)
            {
                const float flatness = compute_flatness(bezier);
                if (flatness < criteria.flatness_tolerance || depth >= criteria.max_iteration_depth)
                {
                    auto r = point_line<ml7::ops::clamp01, TPointResult>(point, bezier.control_points().front(), bezier.control_points().back() - bezier.control_points().front());
                    r.t = t0 + (t1 - t0) * r.t;
                    return r;
                }

                const float t_ = (t0 + t1) * 0.5f;
                const auto [bezier1, bezier2] = bezier.subdivide(0.5f);
                auto r1 = recurse(point, bezier1, criteria, t0, t_, depth + 1);
                auto r2 = recurse(point, bezier2, criteria, t_, t1, depth + 1);
                return r1.distsqr <= r2.distsqr ? r1 : r2;
            }
        };

        auto result = Helper::recurse(point, bezier, {});
        assert(result.t >= 0.0f && result.t <= 1.0f);

        float tmp_len2;

        tmp_len2 = (point - bezier.control_points().front()).lensqr();
        if (result.distsqr > tmp_len2)
        {
            result.distsqr = tmp_len2;
            result.point = bezier.control_points().front();
            result.t = 0.0f;
            return result;
        }

        tmp_len2 = (point - bezier.control_points().back()).lensqr();
        if (result.distsqr > tmp_len2)
        {
            result.distsqr = tmp_len2;
            result.point = bezier.control_points().back();
            result.t = 1.0f;
            return result;
        }

        return result;
    }



} // namespace ml7::detail::distance

#endif // ML7_DETAIL_DISTANCE_H
