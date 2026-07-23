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

        /**
         * The neighboring edge's tangent at this edge's start/end corner (i.e., the
         * previous/next edge's tangent_at(1.0)/tangent_at(0.0) in the original,
         * uncolored contour, not necessarily a same-colored neighbor). Used to
         * resolve the sign correctly when a query point's nearest point on this
         * edge clamps to that corner (see _query_edge): a single edge's own tangent
         * alone can't tell whether such a point is inside or outside, since that
         * depends on the angle the two edges meeting there actually form.
         */

        /** The previous edge's tangent at this edge's start corner. */
        ml7::Vector2f tangent_in_at_start;
        /** The next edge's tangent at this edge's end corner. */
        ml7::Vector2f tangent_out_at_end;
    };

    struct EdgeQuery
    {
        /** The (unsigned) distance to the nearest point on the edge. */
        float unsigned_distance;
        /** Positive if the query point lies to the "inside" side of the edge's local tangent. */
        float signed_orthogonality;

        /**
         * True if the nearest point genuinely lies within the edge's interior (an
         * actual perpendicular foot), false if it had to be clamped to one of the
         * edge's endpoints (a corner "pseudo-distance"). A clamped match's sign
         * comes from that endpoint's tangent, which doesn't represent the true
         * boundary orientation for points off to the side of the corner rather than
         * in front of the edge, so such matches are only trustworthy as a fallback.
         */
        bool is_true_distance;
    };



    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

    size_t _cache_key_params_hash() const override;

    static std::vector<std::vector<ColoredSegment>> _color_contours(const std::vector<std::vector<ml7::QuadraticBezier2f>>& contours);
    static EdgeQuery _query_edge(const ColoredSegment& segment, ml7::Vector2f point);
    static float _channel_signed_distance(const std::vector<std::vector<ColoredSegment>>& contours, uint8_t channel, ml7::Vector2f point);

    float _pixel_range;

}; // class MsdfRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_MSDFRASTERIZER_H
