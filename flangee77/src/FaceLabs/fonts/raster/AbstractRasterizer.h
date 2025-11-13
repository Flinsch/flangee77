#ifndef FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
#define FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H

#include "./RasterResult.h"
#include "./RasterSizeConfig.h"

#include "../Glyph.h"

#include <DataLabs/Buffer2d.h>



namespace fl7::fonts::raster {



class AbstractRasterizer
{

public:
    virtual ~AbstractRasterizer() noexcept = default;



    /**
     * Rasterizes a glyph into an image and calculates its positioning offset.
     */
    RasterResult rasterize_glyph(const Glyph& glyph, const RasterSizeConfig& size_config);

    /**
     * Rasterizes a glyph directly into a pre-allocated canvas (buffer) at a
     * specified offset.
     */
    void rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas);



    /**
     * Returns the pixel format of this rasterizer's output images.
     */
    virtual xl7::graphics::PixelFormat get_pixel_format() const = 0;

    /**
     * Returns the channel order of this rasterizer's output images (defaults to RGBA).
     */
    virtual xl7::graphics::ChannelOrder get_channel_order() const { return xl7::graphics::ChannelOrder::RGBA; }



private:
    virtual void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) = 0;

}; // class AbstractRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
