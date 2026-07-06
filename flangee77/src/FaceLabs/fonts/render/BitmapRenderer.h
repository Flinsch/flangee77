#ifndef FL7_FONTS_RENDER_BITMAPRENDERER_H
#define FL7_FONTS_RENDER_BITMAPRENDERER_H
#include "./AbstractTextureAtlasBasedRenderer.h"

#include "../raster/AbstractBitmapRasterizer.h"



namespace fl7::fonts::render {



/**
 * Texture-atlas-based renderer using "classic" (grayscale) bitmap rasterization.
 * Accepts any AbstractBitmapRasterizer (Simple, Oversampled, Analytical, etc.).
 */
class BitmapRenderer
    : public AbstractTextureAtlasBasedRenderer
{

public:
    explicit BitmapRenderer(raster::AbstractBitmapRasterizer* rasterizer, Config config = {});
    ~BitmapRenderer() noexcept override = default;



protected:
    cl7::u8string _get_shader_path() const override;

}; // class BitmapRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_BITMAPRENDERER_H
