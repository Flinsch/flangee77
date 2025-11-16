#include "SimpleBitmapRasterizer.h"

#include <MathLabs/algorithm/find_quadratic_roots.h>



namespace fl7::fonts::raster {



    /**
     * Constructs a (binary) bitmap rasterizer with optional anti-aliasing. An
     * anti-aliasing quality of 0 disables edge smoothing, and rasterization results
     * include only pixels that are either fully lit or not lit at all. (To speak of
     * "quality" here can actually be considered an outrage.) The final pixel output
     * can be forced to binary values despite any anti-aliasing (to use edge
     * smoothing only internally for intermediate results).
     */
    SimpleBitmapRasterizer::SimpleBitmapRasterizer(unsigned aa_quality, bool force_binary) noexcept
        : _aa_quality(aa_quality)
        , _force_binary(force_binary)
    {
    }



    void SimpleBitmapRasterizer::_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        size_t number_of_segments = 0;
        for (const auto& contour : glyph.contours)
            number_of_segments += contour.segment_count();

        std::vector<ml7::QuadraticBezier2f> segments;
        segments.reserve(number_of_segments);
        for (const auto& contour : glyph.contours)
            for (size_t i = 0; i < contour.segment_count(); ++i)
                segments.push_back(contour.segment_at(i));

        const ml7::Vector2f transform{size_config.font_size, -size_config.font_size};

        for (auto& bezier : segments)
        {
            bezier.p0 *= transform;
            bezier.p1 *= transform;
            bezier.p2 *= transform;
        }

        const ml7::Vector2f origin_center{
            0.5f + static_cast<float>(pixel_offset.left),
            0.5f + static_cast<float>(pixel_offset.top),
        };

        // Always at least one sample,
        // even in the binary case, of course.
        const bool binary_output = _aa_quality == 0 || _force_binary;
        const size_t sample_count = _aa_quality ? _aa_quality : 1;
        // Distribute the samples evenly around
        // the pixel's (vertical) center.
        const float sample_step = 1.0f / static_cast<float>(sample_count);
        const float sample_offset = sample_step * 0.5f - 0.5f;

        for (size_t row = 0; row < canvas.height(); ++row)
        {
            for (size_t col = 0; col < canvas.width(); ++col)
            {
                float coverage = 0.0f;

                for (size_t k = 0; k < sample_count; ++k)
                {
                    const ml7::Vector2f pixel_center = origin_center + ml7::Vector2f{
                        static_cast<float>(col),
                        static_cast<float>(row) + static_cast<float>(k) * sample_step + sample_offset,
                    };

                    for (const auto& bezier : segments)
                    {
                        // Have Bézier points relative to the current pixel.
                        ml7::Vector2f p0 = bezier.p0 - pixel_center;
                        ml7::Vector2f p1 = bezier.p1 - pixel_center;
                        ml7::Vector2f p2 = bezier.p2 - pixel_center;

                        // Apply winding rule to check if the contour is "filled", so
                        // not a "hole" that needs to be subtracted from the coverage.
                        const bool is_filled = p0.y < 0.0f || p2.y > 0.0f;

                        // Curves that lie entirely above or below the ray are skipped.
                        // If two segments run in the same direction (upwards or downwards),
                        // only one of them is counted at their common start/end point
                        // to avoid double counting. However, if they run in opposite
                        // directions, the ray does not intersect the contour, but only
                        // touches it, and both such curves are either skipped or counted.
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

                        const auto roots = ml7::algorithm::find_quadratic_roots(a.y, b.y, c.y);
                        if (roots.empty())
                            continue;

                        const float sign = is_filled ? +1.0f : -1.0f;
                        constexpr float t_epsilon = 1e-4f;
                        constexpr float t_min = 0.0f - t_epsilon;
                        constexpr float t_max = 1.0f + t_epsilon;

                        float sample_coverage = 0.0f;
                        float signed_count = 0.0f;

                        for (auto t : roots)
                        {
                            if (t < t_min || t > t_max)
                                continue;
                            t = ml7::clamp01(t); // Remove any epsilon.

                            // How much of the pixel is covered?
                            // Far left: not at all (0), far right: fully covered (1),
                            // exactly in the middle: half covered (0.5).
                            // The intersection point should lie between -0.5 and +0.5;
                            // therefore, the value to be saturated must be shifted accordingly.
                            const float intersect = a.x * t*t + b.x * t + c.x;
                            sample_coverage += ml7::clamp01(intersect + 0.5f);
                            signed_count += sign;
                        }

                        if (signed_count != 0.0f)
                            coverage += sample_coverage / signed_count;
                    } // for each segment
                } // for each pixel sample

                coverage /= static_cast<float>(sample_count);

                if (binary_output)
                    canvas.map_element<uint8_t>(row, col) = coverage >= 0.5f ? 0xff : 0x00;
                else
                    canvas.map_element<uint8_t>(row, col) = static_cast<uint8_t>(ml7::clamp01(coverage) * 255.0f);
            } // for each pixel column
        } // for each pixel row
    }



} // namespace fl7::fonts::raster
