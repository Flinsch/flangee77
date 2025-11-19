#ifndef FL7_FONTS_RENDER_ABSTRACTTEXTUREATLASBASEDRENDERER_H
#define FL7_FONTS_RENDER_ABSTRACTTEXTUREATLASBASEDRENDERER_H
#include "./AbstractRenderer.h"



namespace fl7::fonts::render {



/**
 * Abstract class for rendering text based on texture atlases containing rasterized
 * glyphs in the form of, for example, "classic" (grayscale) bitmaps, signed
 * distance fields (SDF), or multi-channel signed distance fields (MSDF).
 */
class AbstractTextureAtlasBasedRenderer
    : public AbstractRenderer
{

public:
    ~AbstractTextureAtlasBasedRenderer() noexcept override = default;



private:

}; // class AbstractTextureAtlasBasedRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_ABSTRACTTEXTUREATLASBASEDRENDERER_H
