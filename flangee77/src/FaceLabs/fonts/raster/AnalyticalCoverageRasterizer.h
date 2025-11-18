#ifndef FL7_FONTS_RASTER_ANALYTICALCOVERAGERASTERIZER_H
#define FL7_FONTS_RASTER_ANALYTICALCOVERAGERASTERIZER_H
#include "./AbstractBitmapRasterizer.h"



namespace fl7::fonts::raster {



/**
 * A bitmap rasterizer that computes the "exact" pixel coverage analytically by
 * integrating the intersection area of a glyph's geometry and the pixel cells.
 */
class AnalyticalCoverageRasterizer
    : public AbstractBitmapRasterizer
{

public:
    ~AnalyticalCoverageRasterizer() noexcept override = default;



private:
    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

}; // class AnalyticalCoverageRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ANALYTICALCOVERAGERASTERIZER_H
