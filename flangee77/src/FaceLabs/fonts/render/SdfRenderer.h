#ifndef FL7_FONTS_RENDER_SDFRENDERER_H
#define FL7_FONTS_RENDER_SDFRENDERER_H
#include "./AbstractTextureAtlasBasedRenderer.h"



namespace fl7::fonts::render {



/**
 * Texture-atlas-based renderer using signed distance field (SDF) rasterization.
 */
class SdfRenderer
    : public AbstractTextureAtlasBasedRenderer
{

public:
    explicit SdfRenderer(raster::AbstractRasterizer* rasterizer, Config config = {});
    ~SdfRenderer() noexcept override = default;



protected:
    cl7::u8string _get_shader_path() const override;

}; // class SdfRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_SDFRENDERER_H
