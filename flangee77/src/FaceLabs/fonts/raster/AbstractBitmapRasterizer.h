#ifndef FL7_FONTS_RASTER_ABSTRACTBITMAPRASTERIZER_H
#define FL7_FONTS_RASTER_ABSTRACTBITMAPRASTERIZER_H
#include "./AbstractRasterizer.h"



namespace fl7::fonts::raster {



class AbstractBitmapRasterizer
    : public AbstractRasterizer
{

public:
    ~AbstractBitmapRasterizer() noexcept override = default;



    /**
     * Returns the pixel format of this rasterizer's output images.
     * A bitmap rasterizer always has a pixel format of `A8_UNORM`.
     */
    xl7::graphics::PixelFormat get_pixel_format() const final { return xl7::graphics::PixelFormat::A8_UNORM; }

}; // class AbstractBitmapRasterizer



} // namespace fl7::fonts::raster

#endif // FL7_FONTS_RASTER_ABSTRACTBITMAPRASTERIZER_H
