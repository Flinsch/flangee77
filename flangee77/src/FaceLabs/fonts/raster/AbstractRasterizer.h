#ifndef FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
#define FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H

#include "../Glyph.h"

#include <XiaoLabs/graphics/images/Image.h>

#include <DataLabs/Buffer2d.h>



namespace fl7::fonts::raster {



class AbstractRasterizer
{

public:
    /**
     * Represents the positioning offset of a glyph's pixel grid relative to its
     * typographic origin, in pixel space.
     */
    struct Offset
    {
        /**
         * The horizontal offset of the glyph's pixel grid relative to the left edge
         * of its advance width (typographic origin), in pixel space. This can be
         * negative, depending on the padding used for rasterization or overshoots
         * of the glyph's outline.
         */
        int left;

        /**
         * The vertical offset of the glyph's pixel grid relative to the typographic
         * baseline, in pixel space. This is typically negative, as most glyphs
         * extend above the baseline (e.g., ascenders). The value may also be
         * influenced by rasterization padding or overshoots of the glyph's outline.
         */
        int top;
    };

    /**
     * Encapsulates the output of a glyph rasterization operation.
     */
    struct Result
    {
        /** The rasterized image of the glyph. */
        xl7::graphics::images::Image image;
        /** The positioning offset of the glyph's pixel grid relative to its typographic origin, in pixel space. */
        Offset offset;
    };



    virtual ~AbstractRasterizer() noexcept = default;



    /**
     * Rasterizes a glyph into an image and calculates its positioning offset. The
     * font size to be used for rasterization is specified in pixels. Additional
     * padding (in pixels) can be applied around the glyph during rasterization.
     */
    Result rasterize_glyph(const Glyph& glyph, float font_size, unsigned padding);

    /**
     * Rasterizes a glyph directly into a pre-allocated canvas (buffer) at a
     * specified offset. The font size to be used for rasterization is specified in
     * pixels. Additional padding (in pixels) can be applied around the glyph during
     * rasterization.
     */
    void rasterize_glyph_into(const Glyph& glyph, float font_size, unsigned padding, const Offset& offset, dl7::Buffer2dSpan canvas);



    /**
     * Returns the pixel format of this rasterizer's output images.
     */
    virtual xl7::graphics::PixelFormat get_pixel_format() const = 0;

    /**
     * Returns the channel order of this rasterizer's output images (defaults to RGBA).
     */
    virtual xl7::graphics::ChannelOrder get_channel_order() const { return xl7::graphics::ChannelOrder::RGBA; }



private:
    virtual void _rasterize_glyph_into(const Glyph& glyph, float font_size, unsigned padding, const Offset& offset, dl7::Buffer2dSpan canvas) = 0;

}; // class AbstractRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ABSTRACTRASTERIZER_H
