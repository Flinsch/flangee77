#ifndef ML7_DETAIL_SUBDIVIDE_H
#define ML7_DETAIL_SUBDIVIDE_H

#include <CoreLabs/root.h>



namespace ml7::detail::subdivide {



    template <class TQuadraticBezier, std::floating_point T = typename TQuadraticBezier::scalar_type>
        requires(std::same_as<T, typename TQuadraticBezier::scalar_type>)
    std::pair<TQuadraticBezier, TQuadraticBezier> subdivide_quadratic_bezier(const TQuadraticBezier& bezier, T t)
    {
        using TVector = TQuadraticBezier::vector_type;

        const TVector& p0 = bezier.p0;
        const TVector& p1 = bezier.p1;
        const TVector& p2 = bezier.p2;

        const T u = T{1} - t;

        const TVector p01 = u * p0 + t * p1;
        const TVector p12 = u * p1 + t * p2;
        const TVector p012 = u * p01 + t * p12;

        return {{p0, p01, p012}, {p012, p12, p2}};
    }



    template <class TCubicBezier, std::floating_point T = typename TCubicBezier::scalar_type>
        requires(std::same_as<T, typename TCubicBezier::scalar_type>)
    std::pair<TCubicBezier, TCubicBezier> subdivide_cubic_bezier(const TCubicBezier& bezier, T t)
    {
        using TVector = TCubicBezier::vector_type;

        const TVector& p0 = bezier.p0;
        const TVector& p1 = bezier.p1;
        const TVector& p2 = bezier.p2;
        const TVector& p3 = bezier.p3;

        const T u = T{1} - t;

        const TVector p01 = u * p0 + t * p1;
        const TVector p12 = u * p1 + t * p2;
        const TVector p23 = u * p2 + t * p3;
        const TVector p012 = u * p01 + t * p12;
        const TVector p123 = u * p12 + t * p23;
        const TVector p0123 = u * p012 + t * p123;

        return {{p0, p01, p012, p0123}, {p0123, p123, p23, p3}};
    }



} // namespace ml7::detail::subdivide

#endif // ML7_DETAIL_SUBDIVIDE_H
