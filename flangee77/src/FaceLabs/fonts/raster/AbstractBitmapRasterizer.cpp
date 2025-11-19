#include "AbstractBitmapRasterizer.h"



namespace fl7::fonts::raster {



    /**
     * Constructs an abstract bitmap rasterizer with a pixel format of `A8_UNORM`.
     */
    AbstractBitmapRasterizer::AbstractBitmapRasterizer() noexcept
        : AbstractRasterizer(xl7::graphics::PixelFormat::A8_UNORM)
    {
    }



} // namespace fl7::fonts::raster
