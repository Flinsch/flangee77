#include "BitmapRenderer.h"



namespace fl7::fonts::render {



    BitmapRenderer::BitmapRenderer(raster::AbstractBitmapRasterizer* rasterizer, Config config)
        : AbstractTextureAtlasBasedRenderer(rasterizer, config)
    {
    }



    cl7::u8string BitmapRenderer::_get_shader_path() const
    {
        return u8"assets/shaders/fonts/bitmap-renderer.hlsl";
    }



} // namespace fl7::fonts::render
