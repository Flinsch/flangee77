#include "SdfRasterizer.h"

#include "./detail/glyph_geometry.h"

#include <XiaoLabs/graphics/PixelFormat.h>

#include <algorithm>
#include <limits>



namespace fl7::fonts::raster {



    /**
     * Constructs an SDF rasterizer. `pixel_range` is the distance, in pixels, that
     * maps to the full encoded [0, 1] output range; distances beyond it saturate to
     * fully inside/outside.
     */
    SdfRasterizer::SdfRasterizer(float pixel_range) noexcept
        : AbstractRasterizer(xl7::graphics::PixelFormat::R8_UNORM)
        , _pixel_range(pixel_range)
    {
    }



    void SdfRasterizer::_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas)
    {
        const auto segments = detail::transform_glyph_segments(glyph, size_config.font_size);

        for (size_t row = 0; row < canvas.height(); ++row)
        {
            for (size_t col = 0; col < canvas.width(); ++col)
            {
                const ml7::Vector2f point{
                    0.5f + static_cast<float>(col) + static_cast<float>(pixel_offset.left),
                    0.5f + static_cast<float>(row) + static_cast<float>(pixel_offset.top),
                };

                float unsigned_distance = std::numeric_limits<float>::max();
                for (const auto& segment : segments)
                    unsigned_distance = std::min(unsigned_distance, segment.distance(point));

                const bool inside = detail::is_point_inside(segments, point);
                const float signed_distance = inside ? unsigned_distance : -unsigned_distance;

                const float normalized = ml7::clamp01(0.5f + signed_distance / _pixel_range);
                canvas.map_element<uint8_t>(row, col) = static_cast<uint8_t>(normalized * 255.0f);
            } // for each pixel column
        } // for each pixel row
    }



} // namespace fl7::fonts::raster
