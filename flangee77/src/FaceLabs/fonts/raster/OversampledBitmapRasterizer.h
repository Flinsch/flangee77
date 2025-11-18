#ifndef FL7_FONTS_RASTER_OVERSAMPLEDBITMAPRASTERIZER_H
#define FL7_FONTS_RASTER_OVERSAMPLEDBITMAPRASTERIZER_H
#include "./AbstractBitmapRasterizer.h"



namespace fl7::fonts::raster {



/**
 * A bitmap rasterizer with optional oversampling to enable anti-aliasing via box
 * coverage averaging. Depending on the intensity used, oversampling can be very
 * computationally expensive. To improve the quality without excessive computation,
 * it might be better to use an analytical coverage rasterizer instead.
 */
class OversampledBitmapRasterizer
    : public AbstractBitmapRasterizer
{

public:
    /**
     * Constructs a bitmap rasterizer with the specified oversampling factor (e.g.,
     * (a factor of 4 effectively means 16 samples per pixel). A factor less than 2
     * disables oversampling and therefore also the actual anti-aliasing effect of
     * this rasterizer. Either way, a basic anti-aliasing quality can optionally be
     * set, which is then passed to the internally used simple binary rasterizer.
     * However, this only helps with the final result to a limited extent; one
     * cannot really speak of "quality" here.
     */
    OversampledBitmapRasterizer(unsigned oversampling, unsigned basic_aa_quality = 0) noexcept;

  /**
     * Constructs a bitmap rasterizer with the specified oversampling factor (e.g.,
     * (a factor of 4 effectively means 16 samples per pixel). A factor less than 2
     * disables oversampling and therefore also the actual anti-aliasing effect of
     * this rasterizer. Either way, the specified bitmap rasterizer is used
     * internally to generate the potentially oversampled intermediate result.
     */
    OversampledBitmapRasterizer(unsigned oversampling, AbstractBitmapRasterizer* bitmap_rasterizer) noexcept;

    ~OversampledBitmapRasterizer() noexcept override = default;



private:
    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

    void _do_rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, const dl7::Buffer2dSpan& canvas);

    unsigned _oversampling;
    unsigned _basic_aa_quality = 0;

    AbstractBitmapRasterizer* _bitmap_rasterizer = nullptr;

}; // class OversampledBitmapRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_OVERSAMPLEDBITMAPRASTERIZER_H
