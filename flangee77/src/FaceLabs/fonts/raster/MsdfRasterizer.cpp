#include "MsdfRasterizer.h"

#include "./detail/glyph_geometry.h"

#include <XiaoLabs/graphics/PixelFormat.h>

#include <CoreLabs/bits.h>

#include <algorithm>
#include <bit>
#include <limits>



namespace fl7::fonts::raster {



    namespace {

        constexpr uint8_t CHANNEL_R = 1;
        constexpr uint8_t CHANNEL_G = 2;
        constexpr uint8_t CHANNEL_B = 4;

        constexpr uint8_t COLOR_YELLOW  = CHANNEL_R | CHANNEL_G;
        constexpr uint8_t COLOR_CYAN    = CHANNEL_G | CHANNEL_B;
        constexpr uint8_t COLOR_MAGENTA = CHANNEL_B | CHANNEL_R;
        constexpr uint8_t COLOR_WHITE   = CHANNEL_R | CHANNEL_G | CHANNEL_B;

        struct RGBA8
        {
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
        };

        uint8_t _encode_channel(float signed_distance, float pixel_range)
        {
            const float normalized = ml7::clamp01(0.5f + signed_distance / pixel_range);
            return static_cast<uint8_t>(normalized * 255.0f);
        }

    } // namespace



    /**
     * Constructs an MSDF rasterizer. `pixel_range` is the distance, in pixels, that
     * maps to the full encoded [0, 1] output range; distances beyond it saturate to
     * fully inside/outside.
     */
    MsdfRasterizer::MsdfRasterizer(float pixel_range) noexcept
        : AbstractRasterizer(xl7::graphics::PixelFormat::R8G8B8A8_UNORM)
        , _pixel_range(pixel_range)
    {
    }



    std::vector<std::vector<MsdfRasterizer::ColoredSegment>> MsdfRasterizer::_color_contours(const std::vector<std::vector<ml7::QuadraticBezier2f>>& contours)
    {
        // Angle threshold (as a tangent dot product) beyond which two consecutive
        // segments are considered to meet at a corner rather than a smooth join.
        constexpr float corner_dot_threshold = 0.9986f; // ~3 degrees

        constexpr uint8_t colors[3] = {COLOR_YELLOW, COLOR_CYAN, COLOR_MAGENTA};

        std::vector<std::vector<ColoredSegment>> result;
        result.reserve(contours.size());

        for (const auto& segments : contours)
        {
            const size_t n = segments.size();

            std::vector<bool> is_corner(n, false);
            size_t corner_count = 0;
            for (size_t i = 0; i < n; ++i)
            {
                const ml7::Vector2f tangent_out = segments[i].tangent_at(1.0f).normalized();
                const ml7::Vector2f tangent_in = segments[(i + 1) % n].tangent_at(0.0f).normalized();
                if (tangent_out.dot(tangent_in) < corner_dot_threshold)
                {
                    is_corner[i] = true;
                    ++corner_count;
                }
            }

            std::vector<ColoredSegment> colored;
            colored.reserve(n);

            if (corner_count == 0)
            {
                // A fully smooth contour has no corners to preserve; one shared color
                // (all channels agree) reconstructs identically to a plain SDF.
                for (const auto& bezier : segments)
                    colored.push_back({.bezier = bezier, .channels = COLOR_WHITE});
            }
            else
            {
                size_t color_index = 0;
                for (size_t i = 0; i < n; ++i)
                {
                    colored.push_back({.bezier = segments[i], .channels = colors[color_index]});
                    if (is_corner[i])
                        color_index = (color_index + 1) % 3;
                } // for each segment
            }

            result.push_back(std::move(colored));
        } // for each contour

        return result;
    }

    MsdfRasterizer::EdgeQuery MsdfRasterizer::_query_edge(const ml7::QuadraticBezier2f& bezier, ml7::Vector2f point)
    {
        constexpr float t_epsilon = 1e-4f;

        // The true (clamped) nearest point on this segment; always geometrically
        // bounded, unlike projecting onto an unbounded extension of the tangent line,
        // which can produce a misleadingly small distance for points far from this
        // segment entirely (if they happen to lie near where that infinite line
        // passes, regardless of proximity to the segment itself).
        const float t = bezier.parameter(point);
        const ml7::Vector2f closest = bezier.point_at(t);

        // Near a corner (t clamped to an endpoint), use that endpoint's tangent
        // rather than the (numerically ill-defined) tangent of the point itself, so
        // the sign stays consistent with the neighboring segment sharing that corner.
        ml7::Vector2f tangent;
        if (t <= t_epsilon)
            tangent = bezier.tangent_at(0.0f);
        else if (t >= 1.0f - t_epsilon)
            tangent = bezier.tangent_at(1.0f);
        else
            tangent = bezier.tangent_at(t);

        const ml7::Vector2f to_point = point - closest;

        return {
            .unsigned_distance = to_point.length(),
            .signed_orthogonality = tangent.cross(to_point),
        };
    }

    float MsdfRasterizer::_channel_signed_distance(const std::vector<std::vector<ColoredSegment>>& contours, uint8_t channel, ml7::Vector2f point)
    {
        float best_unsigned_distance = std::numeric_limits<float>::max();
        float best_signed_orthogonality = 0.0f;

        for (const auto& contour : contours)
        {
            for (const auto& segment : contour)
            {
                if ((segment.channels & channel) == 0)
                    continue;

                const EdgeQuery query = _query_edge(segment.bezier, point);
                if (query.unsigned_distance < best_unsigned_distance)
                {
                    best_unsigned_distance = query.unsigned_distance;
                    best_signed_orthogonality = query.signed_orthogonality;
                }
            } // for each colored segment
        } // for each contour

        return best_signed_orthogonality >= 0.0f ? best_unsigned_distance : -best_unsigned_distance;
    }

    void MsdfRasterizer::_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        const auto contours = detail::transform_glyph_contours(glyph, size_config.font_size);
        const auto colored_contours = _color_contours(contours);

        for (size_t row = 0; row < canvas.height(); ++row)
        {
            for (size_t col = 0; col < canvas.width(); ++col)
            {
                const ml7::Vector2f point{
                    0.5f + static_cast<float>(col) + static_cast<float>(pixel_offset.left),
                    0.5f + static_cast<float>(row) + static_cast<float>(pixel_offset.top),
                };

                const float r = _channel_signed_distance(colored_contours, CHANNEL_R, point);
                const float g = _channel_signed_distance(colored_contours, CHANNEL_G, point);
                const float b = _channel_signed_distance(colored_contours, CHANNEL_B, point);

                canvas.map_element<RGBA8>(row, col) = {
                    .r = _encode_channel(r, _pixel_range),
                    .g = _encode_channel(g, _pixel_range),
                    .b = _encode_channel(b, _pixel_range),
                    .a = 255,
                };
            } // for each pixel column
        } // for each pixel row
    }



    size_t MsdfRasterizer::_cache_key_params_hash() const
    {
        // Bump this if `_rasterize_glyph_into` (or edge-coloring) above
        // changes algorithmically (i.e., inputs unchanged, but the output
        // would now differ), to force-invalidate previously cached results.
        constexpr uint32_t ALGORITHM_VERSION = 1;

        size_t hash = 0;
        cl7::bits::hash_combine(hash, static_cast<size_t>(ALGORITHM_VERSION));
        cl7::bits::hash_combine(hash, static_cast<size_t>(std::bit_cast<uint32_t>(_pixel_range)));
        return hash;
    }



} // namespace fl7::fonts::raster
