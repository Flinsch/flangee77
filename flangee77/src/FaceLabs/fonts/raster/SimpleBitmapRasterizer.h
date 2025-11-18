#ifndef FL7_FONTS_RASTER_SIMPLEBITMAPRASTERIZER_H
#define FL7_FONTS_RASTER_SIMPLEBITMAPRASTERIZER_H
#include "./AbstractBitmapRasterizer.h"



namespace fl7::fonts::raster {



/**
 * A (binary) bitmap rasterizer that tends to either fully or not at all illuminate
 * the pixels. Anti-aliasing can be optionally enabled, but in a very rudimentary
 * form. If edge smoothing is desired, a different, more sophisticated rasterizer
 * should be used.
 */
class SimpleBitmapRasterizer
    : public AbstractBitmapRasterizer
{

public:
    /**
     * Constructs a (binary) bitmap rasterizer with optional anti-aliasing. An
     * anti-aliasing quality of 0 disables edge smoothing, and rasterization results
     * include only pixels that are either fully lit or not lit at all. (To speak of
     * "quality" here can actually be considered an outrage.) The final pixel output
     * can be forced to binary values despite any anti-aliasing (to use edge
     * smoothing only internally for intermediate results).
     */
    SimpleBitmapRasterizer(unsigned aa_quality = 0, bool force_binary = false) noexcept;

    ~SimpleBitmapRasterizer() noexcept override = default;



private:
    void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) override;

    unsigned _aa_quality;
    bool _force_binary;

}; // class SimpleBitmapRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_SIMPLEBITMAPRASTERIZER_H
