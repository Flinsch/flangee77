#include "SdfRenderer.h"



namespace fl7::fonts::render {



    SdfRenderer::SdfRenderer(raster::AbstractRasterizer* rasterizer, Config config)
        : AbstractTextureAtlasBasedRenderer(rasterizer, config)
    {
    }



    cl7::u8string SdfRenderer::_get_shader_path() const
    {
        return u8"assets/shaders/fonts/sdf-renderer.hlsl";
    }



} // namespace fl7::fonts::render
