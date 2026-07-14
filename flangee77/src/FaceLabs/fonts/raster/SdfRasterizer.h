#ifndef FL7_FONTS_RASTER_SDFRASTERIZER_H
#define FL7_FONTS_RASTER_SDFRASTERIZER_H
#include "./AbstractRasterizer.h"



namespace fl7::fonts::raster {



/**
 * A rasterizer that generates a single-channel signed distance field (SDF) for a
 * glyph's outline, meant to be reconstructed via a threshold (e.g., smoothstep)
 * around the encoded midpoint in the pixel shader.
 */
class SdfRasterizer
    : public AbstractRasterizer
{

public:
    /**
     * Constructs an SDF rasterizer. `pixel_range` is the distance, in pixels, that
     * maps to the full encoded [0, 1] output range; distances beyond it saturate to
     * fully inside/outside.
     */
    explicit SdfRasterizer(float pixel_range = 4.0f) noexcept;

    ~SdfRasterizer() noexcept override = default;



private:
    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

    size_t _cache_key_params_hash() const override;

    float _pixel_range;

}; // class SdfRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_SDFRASTERIZER_H
