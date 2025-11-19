#ifndef FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
#define FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H

#include "./RasterResult.h"
#include "./RasterSizeConfig.h"

#include "../Glyph.h"

#include <DataLabs/Buffer2d.h>



namespace fl7::fonts::raster {



/**
 * Abstract class for rasterizing glyphs into images, for example as a "classic"
 * bitmap, a signed distance field (SDF), or a multi-channel signed distance field
 * (MSDF), and (optionally) calculating the positional offset of a rasterized image
 * relative to the typographic origin of the glyph.
 */
class AbstractRasterizer
{

public:
    /**
     * Constructs an abstract rasterizer with the specified pixel format for the
     * output images and an (optional) channel order (which defaults to RGBA).
     */
    AbstractRasterizer(xl7::graphics::PixelFormat pixel_format, xl7::graphics::ChannelOrder channel_order = xl7::graphics::ChannelOrder::RGBA) noexcept;

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
    xl7::graphics::PixelFormat get_pixel_format() const { return _pixel_format; };

    /**
     * Returns the channel order of this rasterizer's output images (defaults to RGBA).
     */
    xl7::graphics::ChannelOrder get_channel_order() const { return _channel_order; }



private:
    virtual void _rasterize_glyph_into(const Glyph& glyph, const RasterSizeConfig& size_config, const PixelOffset& pixel_offset, dl7::Buffer2dSpan canvas) = 0;

    /** The pixel format of this rasterizer's output images. */
    xl7::graphics::PixelFormat _pixel_format;
    /** The channel order of this rasterizer's output images (defaults to RGBA). */
    xl7::graphics::ChannelOrder _channel_order = xl7::graphics::ChannelOrder::RGBA;

}; // class AbstractRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
