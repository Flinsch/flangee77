#ifndef FL7_FONTS_RENDER_MSDFRENDERER_H
#define FL7_FONTS_RENDER_MSDFRENDERER_H
#include "./AbstractTextureAtlasBasedRenderer.h"



namespace fl7::fonts::render {



/**
 * Texture-atlas-based renderer using multi-channel signed distance field (MSDF)
 * rasterization.
 */
class MsdfRenderer
    : public AbstractTextureAtlasBasedRenderer
{

public:
    explicit MsdfRenderer(raster::AbstractRasterizer* rasterizer, Config config = {});
    ~MsdfRenderer() noexcept override = default;



protected:
    cl7::u8string _get_shader_path() const override;

}; // class MsdfRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_MSDFRENDERER_H
