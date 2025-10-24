#include "RawGlyph.h"

#include <span>



namespace fl7::fonts::detail {



    Glyph RawGlyph::normalize(float em_per_unit) const
    {
        assert(em_per_unit > 0.0f);

        Glyph glyph;

        glyph.contours.resize(end_point_indices.size());

        size_t start_point_index = 0;

        for (size_t contour_index = 0; contour_index < end_point_indices.size(); ++contour_index)
        {
            const size_t end_point_index = end_point_indices[contour_index];
            assert(end_point_index >= start_point_index);

            const size_t number_of_points = end_point_index - start_point_index + 1;
            const auto points = std::span(contour_points.begin() + static_cast<ptrdiff_t>(start_point_index), number_of_points);
            assert(points.size() >= 2); // Two points are actually nonsense semantically, but from a technical point of view this precondition is enough for us.

            // Count the number of quadratic Bézier curves (segments):
            // Each on-curve point marks the start of a new Bézier curve,
            // where each two consecutive off-curve points have an implicit
            // on-curve point between them, also marking a new Bézier curve.
            assert(points[0].on_curve);
            size_t number_of_segments = 1;
            for (size_t i = 1; i < number_of_points; ++i)
            {
                if (points[i].on_curve || !points[i - 1].on_curve)
                    ++number_of_segments;
            }

            auto& contour = glyph.contours[contour_index];
            contour.points.reserve(number_of_segments * 2 + 1);

            for (size_t i = 0; i < number_of_points; ++i)
            {
                const auto& p0 = points[i];
                const ml7::Vector2f v0{static_cast<float>(p0.x), static_cast<float>(p0.y)};

                assert(p0.on_curve);

                const auto& p1 = points[(i + 1) % number_of_points];
                const ml7::Vector2f v1{static_cast<float>(p1.x), static_cast<float>(p1.y)};

                if (p1.on_curve)
                {
                    // Two consecutive on-curve points define a straight line segment.
                    const auto v_ = (v0 + v1) * 0.5f;
                    //contour.bezier_curves.emplace_back(v0, v_, v1);
                    contour.points.emplace_back(v0);
                    contour.points.emplace_back(v_);
                    continue;
                }

                const auto& p2 = points[(i + 2) % number_of_points];
                const ml7::Vector2f v2{static_cast<float>(p2.x), static_cast<float>(p2.y)};

                if (p2.on_curve)
                {
                    // An on-curve point, an off-curve point, and another
                    // on-curve point define a single quadratic Bézier curve.
                    //contour.bezier_curves.emplace_back(v0, v1, v2);
                    contour.points.emplace_back(v0);
                    contour.points.emplace_back(v1);
                    ++i; // Skip one additional point (the one off-curve point of the Bézier curve).
                    continue;
                }

                const auto& p3 = points[(i + 3) % number_of_points];
                //const ml7::Vector2f v3{static_cast<float>(p3.x), static_cast<float>(p3.y)};

                if (p3.on_curve)
                {
                    // An on-curve point, two consecutive off-curve points, and another
                    // on-curve point define two quadratic Bézier curves with an implied
                    // on-curve point exactly halfway between the two off-curve points.
                    const auto v_ = (v1 + v2) * 0.5f;
                    //contour.bezier_curves.emplace_back(v0, v1, v_);
                    //contour.bezier_curves.emplace_back(v_, v2, v3);
                    contour.points.emplace_back(v0);
                    contour.points.emplace_back(v1);
                    contour.points.emplace_back(v_);
                    contour.points.emplace_back(v2);
                    i += 2; // Skip two additional points (the off-curve points, one per Bézier curve).
                    continue;
                }

                // It's possible, though not common, for a font file to contain sequences
                // of more than two consecutive off-curve points in the raw data.

                // Find next on-curve point and calculate
                // number of resulting Bézier curves.
                size_t n = 4;
                while (!points[(i + n) % number_of_points].on_curve)
                    ++n;
                const size_t bezier_count = n - 1;
                assert(bezier_count >= 3);

                const auto& pn = points[(i + n) % number_of_points];
                //const ml7::Vector2f vn{static_cast<float>(pn.x), static_cast<float>(pn.y)};

                assert(pn.on_curve);

                // Insert multiple quadratic Bézier curves, following
                // the same pattern as before, in a generalized form.
                auto v_ = (v1 + v2) * 0.5f;
                auto vL = v2;
                //contour.bezier_curves.emplace_back(v0, v1, v_);
                contour.points.emplace_back(v0);
                contour.points.emplace_back(v1);
                for (size_t k = 0; k < bezier_count - 2; ++k)
                {
                    const auto& pC = points[(i + k + 3) % number_of_points];
                    const ml7::Vector2f vC = {static_cast<float>(pC.x), static_cast<float>(pC.y)};

                    const ml7::Vector2f vt_ = (vL + vC) * 0.5f;
                    //contour.bezier_curves.emplace_back(v_, vL, vt_);
                    contour.points.emplace_back(v_);
                    contour.points.emplace_back(vL);
                    v_ = vt_;
                    vL = vC;
                }
                //contour.bezier_curves.emplace_back(v_, vL, vn);
                contour.points.emplace_back(v_);
                contour.points.emplace_back(vL);
                i += bezier_count; // Skip those additional points (the off-curve points, one per Bézier curve).
            } // for each point

            // Duplicate the first point and
            // append it to close the contour.
            assert(contour.points.size() % 2 == 0);
            contour.points.push_back(contour.points.front());
            assert(contour.points.size() % 2 == 1);

            assert(contour.points.size() == number_of_segments * 2 + 1);

            start_point_index = end_point_index + 1;
        } // for each contour

        for (auto& contour : glyph.contours)
            for (auto& point : contour.points)
                point *= em_per_unit;

        return glyph;
    }



} // namespace fl7::fonts::detail
