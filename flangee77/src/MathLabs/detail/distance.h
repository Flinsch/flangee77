#ifndef ML7_DETAIL_DISTANCE_H
#define ML7_DETAIL_DISTANCE_H

#include <MathLabs/ops.h>

#include <CoreLabs/functional.h>



namespace ml7::detail::distance {



template <class TVector>
struct PointResult
{
    using T = TVector::scalar_type;

    T distance_squared;
    TVector point;
    T t;

    PointResult(T distance_squared, const TVector& point, T t) noexcept : distance_squared(distance_squared), point(point), t(t) {}
    PointResult() noexcept = default;
};

template <std::floating_point T>
struct PointResult_distance_squared
{
    T distance_squared;

    template <class TVector>
        requires(std::same_as<T, typename TVector::scalar_type>)
    PointResult_distance_squared(T distance_squared, const TVector& point, T t) noexcept : distance_squared(distance_squared) {}
    PointResult_distance_squared() noexcept = default;
};

template <class TVector>
struct PointResult_point
{
    TVector point;

    template <std::floating_point T>
        requires(std::same_as<T, typename TVector::scalar_type>)
    PointResult_point(T distance_squared, const TVector& point, T t) noexcept : point(point) {}
    PointResult_point() noexcept = default;
};

template <std::floating_point T>
struct PointResult_t
{
    T t;

    template <class TVector>
        requires(std::same_as<T, typename TVector::scalar_type>)
    PointResult_t(T distance_squared, const TVector& point, T t) noexcept : t(t) {}
    PointResult_t() noexcept = default;
};



    template <typename clamp, class TPointResult, class TVector, std::floating_point T = typename TVector::scalar_type>
        requires(requires (T distance_squared, const TVector& point, T t) {
            TPointResult{distance_squared, point, t};
        } && requires {
            { clamp{}(T{}) } -> std::convertible_to<T>;
        } && std::same_as<T, typename TVector::scalar_type>)
    TPointResult point_line(const TVector& point, const TVector& origin, const TVector& direction)
    {
        TPointResult result;

        const auto& P = point;
        const auto& A = origin;

        const auto& AB = direction;
        const auto AP = P - A;

        const T len2 = AB.length_squared();
        if (len2 == T{0})
            return {AP.length_squared(), A, T{0}};

        T t = AP.dot(AB) / len2;
        t = clamp{}(t);

        const auto Q = A + t * AB;
        const auto QP = P - Q;

        return {QP.length_squared(), Q, t};
    }



    template <class TPointResult, class TVector, class TBezier, std::floating_point T = typename TVector::scalar_type>
        requires(requires (T distance_squared, const TVector& point, T t) {
            TPointResult{distance_squared, point, t};
        } && std::same_as<T, typename TVector::scalar_type> && std::same_as<T, typename TBezier::scalar_type>)
    TPointResult point_bezier(const TVector& point, const TBezier& bezier)
    {
        // TODO:
        // For better performance, precision, and/or robustness, switch to numeric root finding
        // (e.g., subdivision and Newton-Raphson refinement) instead of symbolic polynomials?

        struct StoppingCriteria
        {
            T flatness_tolerance = T{1e-5};
            unsigned max_iteration_depth = 10;
        };

        struct Helper
        {
            static T compute_flatness(const TBezier& bezier)
            {
                T max_dd = T{0};

                for (size_t i = 1; i + 1 < bezier.control_points().size(); ++i)
                {
                    const T dd = point_line<ops::clamp01, PointResult_distance_squared<T>>(bezier.control_points()[i], bezier.control_points().front(), bezier.control_points().back() - bezier.control_points().front()).distance_squared;
                    max_dd = std::max(max_dd, dd);
                }

                return std::sqrt(max_dd);
            }

            static PointResult<TVector> recurse(const TVector& point, const TBezier& bezier, const StoppingCriteria& criteria, T t0 = T{0}, T t1 = T{1}, unsigned depth = 0)
            {
                const T flatness = compute_flatness(bezier);
                if (flatness < criteria.flatness_tolerance || depth >= criteria.max_iteration_depth)
                {
                    auto r = point_line<ops::clamp01, PointResult<TVector>>(point, bezier.control_points().front(), bezier.control_points().back() - bezier.control_points().front());
                    r.t = t0 + (t1 - t0) * r.t;
                    return r;
                }

                const T t_ = (t0 + t1) * T{0.5};
                const auto [bezier1, bezier2] = bezier.subdivide(T{0.5});
                auto r1 = recurse(point, bezier1, criteria, t0, t_, depth + 1);
                auto r2 = recurse(point, bezier2, criteria, t_, t1, depth + 1);
                return r1.distance_squared <= r2.distance_squared ? r1 : r2;
            }
        };

        auto r = Helper::recurse(point, bezier, {});
        assert(r.t >= T{0} && r.t <= T{1});

        T tmp_len2;

        tmp_len2 = (point - bezier.control_points().front()).length_squared();
        if (r.distance_squared > tmp_len2)
        {
            r.distance_squared = tmp_len2;
            r.point = bezier.control_points().front();
            r.t = T{0};
            return {r.distance_squared, r.point, r.t};
        }

        tmp_len2 = (point - bezier.control_points().back()).length_squared();
        if (r.distance_squared > tmp_len2)
        {
            r.distance_squared = tmp_len2;
            r.point = bezier.control_points().back();
            r.t = T{1};
            return {r.distance_squared, r.point, r.t};
        }

        return {r.distance_squared, r.point, r.t};
    }



} // namespace ml7::detail::distance

#endif // ML7_DETAIL_DISTANCE_H
