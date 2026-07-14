#ifndef FL7_FONTS_RASTER_MSDFRASTERIZER_H
#define FL7_FONTS_RASTER_MSDFRASTERIZER_H
#include "./AbstractRasterizer.h"

#include <MathLabs/QuadraticBezier2.h>

#include <vector>



namespace fl7::fonts::raster {



/**
 * A rasterizer that generates a multi-channel signed distance field (MSDF) for a
 * glyph's outline. Compared to a plain (single-channel) SDF, corners are preserved
 * by distributing the contour's edges across the color channels ("edge coloring")
 * and reconstructing sharp corners via a per-pixel median of the three channels in
 * the pixel shader, instead of the single distance value degrading corners into
 * rounded blobs at large scale-up factors.
 */
class MsdfRasterizer
    : public AbstractRasterizer
{

public:
    /**
     * Constructs an MSDF rasterizer. `pixel_range` is the distance, in pixels, that
     * maps to the full encoded [0, 1] output range; distances beyond it saturate to
     * fully inside/outside.
     */
    explicit MsdfRasterizer(float pixel_range = 4.0f) noexcept;

    ~MsdfRasterizer() noexcept override = default;



private:
    struct ColoredSegment
    {
        /** The (pixel-space) edge itself. */
        ml7::QuadraticBezier2f bezier;
        /** Bitmask of channels this edge contributes to (1 = R, 2 = G, 4 = B). */
        uint8_t channels;
    };

    struct EdgeQuery
    {
        /** The (unsigned) distance to the nearest point on the edge. */
        float unsigned_distance;
        /** Positive if the query point lies to the "inside" side of the edge's local tangent. */
        float signed_orthogonality;
    };



    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

    size_t _cache_key_params_hash() const override;

    static std::vector<std::vector<ColoredSegment>> _color_contours(const std::vector<std::vector<ml7::QuadraticBezier2f>>& contours);
    static EdgeQuery _query_edge(const ml7::QuadraticBezier2f& bezier, ml7::Vector2f point);
    static float _channel_signed_distance(const std::vector<std::vector<ColoredSegment>>& contours, uint8_t channel, ml7::Vector2f point);

    float _pixel_range;

}; // class MsdfRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_MSDFRASTERIZER_H
