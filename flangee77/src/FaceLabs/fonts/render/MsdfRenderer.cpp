#include "MsdfRenderer.h"



namespace fl7::fonts::render {



    MsdfRenderer::MsdfRenderer(raster::AbstractRasterizer* rasterizer, Config config)
        : AbstractTextureAtlasBasedRenderer(rasterizer, config)
    {
    }



    cl7::u8string MsdfRenderer::_get_shader_path() const
    {
        return u8"assets/shaders/fonts/msdf-renderer.hlsl";
    }



} // namespace fl7::fonts::render
