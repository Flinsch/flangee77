#include "./glyph_geometry.h"

#include <AlgoLabs/math/find_quadratic_roots.h>



namespace fl7::fonts::raster::detail {



    /**
     * Transforms all of the glyph's contour segments from font-normalized (em, y-up)
     * space into pixel space (scaled by font_size, y-down), flattening them into a
     * single list of quadratic Bezier curves. Contour boundaries are not preserved;
     * use `transform_glyph_contours` if per-contour grouping is required.
     */
    std::vector<ml7::QuadraticBezier2f> transform_glyph_segments(const Glyph& glyph, float font_size)
    {
        size_t number_of_segments = 0;
        for (const auto& contour : glyph.contours)
            number_of_segments += contour.segment_count();

        std::vector<ml7::QuadraticBezier2f> segments;
        segments.reserve(number_of_segments);

        const ml7::Vector2f transform{font_size, -font_size};

        for (const auto& contour : glyph.contours)
        {
            for (size_t i = 0; i < contour.segment_count(); ++i)
            {
                ml7::QuadraticBezier2f bezier = contour.segment_at(i);
                bezier.p0 *= transform;
                bezier.p1 *= transform;
                bezier.p2 *= transform;
                segments.push_back(bezier);
            }
        }

        return segments;
    }

    /**
     * Same as `transform_glyph_segments`, but keeps the segments grouped by contour.
     */
    std::vector<std::vector<ml7::QuadraticBezier2f>> transform_glyph_contours(const Glyph& glyph, float font_size)
    {
        std::vector<std::vector<ml7::QuadraticBezier2f>> contours;
        contours.reserve(glyph.contours.size());

        const ml7::Vector2f transform{font_size, -font_size};

        for (const auto& contour : glyph.contours)
        {
            std::vector<ml7::QuadraticBezier2f> segments;
            segments.reserve(contour.segment_count());

            for (size_t i = 0; i < contour.segment_count(); ++i)
            {
                ml7::QuadraticBezier2f bezier = contour.segment_at(i);
                bezier.p0 *= transform;
                bezier.p1 *= transform;
                bezier.p2 *= transform;
                segments.push_back(bezier);
            }

            if (!segments.empty())
                contours.push_back(std::move(segments));
        }

        return contours;
    }

    /**
     * Checks whether the given point lies inside the shape described by the given
     * (pixel-space) segments, using the nonzero winding rule via a horizontal ray cast.
     */
    bool is_point_inside(std::span<const ml7::QuadraticBezier2f> segments, ml7::Vector2f point)
    {
        int winding = 0;

        for (const auto& bezier : segments)
        {
            // Have Bezier points relative to the query point.
            const ml7::Vector2f p0 = bezier.p0 - point;
            const ml7::Vector2f p1 = bezier.p1 - point;
            const ml7::Vector2f p2 = bezier.p2 - point;

            // Apply winding rule to check if the contour is "filled", so not a "hole"
            // that needs to be subtracted. (Same convention as SimpleBitmapRasterizer.)
            const bool is_filled = p0.y < 0.0f || p2.y > 0.0f;

            // Curves that lie entirely above or below the ray are skipped.
            if (is_filled)
            {
                if (p0.y > 0.0f && p2.y >= 0.0f) continue;
                if (p0.y < 0.0f && p2.y <= 0.0f) continue;
            }
            else
            {
                if (p0.y >= 0.0f && p2.y > 0.0f) continue;
                if (p0.y <= 0.0f && p2.y < 0.0f) continue;
            }

            const ml7::Vector2f a = p0 - 2.0f * p1 + p2;
            const ml7::Vector2f b = 2.0f * (p1 - p0);
            const ml7::Vector2f c = p0;

            const auto roots = al7::math::find_quadratic_roots(a.y, b.y, c.y);
            if (roots.empty())
                continue;

            const int sign = is_filled ? +1 : -1;
            constexpr float t_epsilon = 1e-4f;
            constexpr float t_min = 0.0f - t_epsilon;
            constexpr float t_max = 1.0f + t_epsilon;

            for (auto t : roots)
            {
                if (t < t_min || t > t_max)
                    continue;
                t = ml7::clamp01(t); // Remove any epsilon.

                // Only crossings to the right of the query point (a ray cast towards +x) count.
                const float intersect_x = a.x * t*t + b.x * t + c.x;
                if (intersect_x > 0.0f)
                    winding += sign;
            } // for each root
        } // for each segment

        return winding != 0;
    }



} // namespace fl7::fonts::raster::detail
