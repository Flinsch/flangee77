#ifndef FL7_FONTS_RASTER_ANALYTICALCOVERAGERASTERIZER_H
#define FL7_FONTS_RASTER_ANALYTICALCOVERAGERASTERIZER_H
#include "./AbstractRasterizer.h"



namespace fl7::fonts::raster {



/**
 * This rasterizer computes the "exact" pixel coverage analytically by integrating
 * the intersection area of a glyph's curve geometry and the pixel cells.
 */
class AnalyticalCoverageRasterizer
    : public AbstractRasterizer
{

public:
    /**
     *
     */
    AnalyticalCoverageRasterizer() noexcept;

    ~AnalyticalCoverageRasterizer() noexcept override = default;




    /**
     * Returns the pixel format of this rasterizer's output images.
     */
    xl7::graphics::PixelFormat get_pixel_format() const override { return xl7::graphics::PixelFormat::A8_UNORM; }



private:
    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

}; // class AnalyticalCoverageRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ANALYTICALCOVERAGERASTERIZER_H
