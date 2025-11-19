#ifndef FL7_FONTS_RASTER_ABSTRACTBITMAPRASTERIZER_H
#define FL7_FONTS_RASTER_ABSTRACTBITMAPRASTERIZER_H
#include "./AbstractRasterizer.h"



namespace fl7::fonts::raster {



/**
 * Abstract class for rasterizing glyphs into "classic" (grayscale) bitmap images.
 * The pixel format is set to `A8_UNORM` for this.
 */
class AbstractBitmapRasterizer
    : public AbstractRasterizer
{

public:
    /**
     * Constructs an abstract bitmap rasterizer with a pixel format of `A8_UNORM`.
     */
    AbstractBitmapRasterizer() noexcept;

    ~AbstractBitmapRasterizer() noexcept override = default;

}; // class AbstractBitmapRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ABSTRACTBITMAPRASTERIZER_H
