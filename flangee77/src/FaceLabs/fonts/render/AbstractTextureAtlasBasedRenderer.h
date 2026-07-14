#ifndef FL7_FONTS_RENDER_ABSTRACTTEXTUREATLASBASEDRENDERER_H
#define FL7_FONTS_RENDER_ABSTRACTTEXTUREATLASBASEDRENDERER_H
#include "./AbstractRenderer.h"

#include "../raster/AbstractRasterizer.h"
#include "../raster/PixelOffset.h"

#include <AlgoLabs/packing/AbstractRectPacker.h>

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/textures/Texture2D.h>
#include <XiaoLabs/graphics/meshes/VertexBuffer.h>
#include <XiaoLabs/graphics/meshes/VertexLayout.h>
#include <XiaoLabs/graphics/shaders/VertexShader.h>
#include <XiaoLabs/graphics/shaders/PixelShader.h>
#include <XiaoLabs/graphics/shaders/ConstantBuffer.h>
#include <XiaoLabs/graphics/states/SamplerState.h>
#include <XiaoLabs/graphics/states/BlendState.h>

#include <MathLabs/Vector2.h>

#include <CoreLabs/string.h>

#include <map>
#include <memory>
#include <vector>



namespace fl7::fonts::render {



/**
 * Abstract base class for texture-atlas-based text rendering.
 *
 * Glyphs are rasterized on demand and cached in per-font-size texture atlases.
 * During a batch, quads are accumulated and submitted together on flush.
 * Concrete subclasses provide the rendering shader via `_get_shader_path`.
 */
class AbstractTextureAtlasBasedRenderer
    : public AbstractRenderer
{

public:
    struct Config
    {
        /** Side length (width and height) of each atlas texture, in pixels. */
        unsigned atlas_size = 1024;
        /** Padding applied around each glyph during rasterization. */
        unsigned raster_padding = 1;
    };



    AbstractTextureAtlasBasedRenderer(raster::AbstractRasterizer* rasterizer, Config config);
    ~AbstractTextureAtlasBasedRenderer() noexcept override;



protected:
    /**
     * Returns the path to the HLSL shader file (relative to the working directory).
     * The file must contain both `mainVertex` and `mainPixel` entry points.
     */
    virtual cl7::u8string _get_shader_path() const = 0;



private:
    struct AtlasLayer
    {
        float font_size = 0.0f;
        unsigned width = 0;
        unsigned height = 0;
        std::unique_ptr<al7::packing::AbstractRectPacker> packer;
        xl7::graphics::textures::Texture2D::Id texture_id = {};
    };

    struct GlyphCacheKey
    {
        cl7::text::codec::codepoint::value_type codepoint;
        float font_size;
        auto operator<=>(const GlyphCacheKey&) const = default;
    };

    struct GlyphCacheEntry
    {
        al7::packing::Rect rect;
        raster::PixelOffset pixel_offset;
        unsigned image_width;
        unsigned image_height;
        const AtlasLayer* layer = nullptr;
    };

    struct Vertex
    {
        ml7::Vector2f position;
        ml7::Vector2f texcoord;
        xl7::graphics::Color color;
    };

    struct DrawBatch
    {
        float font_size;
        unsigned first_vertex;
        unsigned vertex_count;
    };



    void _before_begin() override;
    void _after_end() override;
    void _do_flush() override;
    void _emit_glyph(const Glyph& glyph, const State& state) override;

    AtlasLayer& _get_or_create_atlas_layer(float font_size);
    const GlyphCacheEntry* _get_or_rasterize_glyph(const Glyph& glyph, float font_size);
    void _blit_into_atlas(AtlasLayer& layer, const al7::packing::Rect& rect, const xl7::graphics::images::Image& image);
    void _ensure_gpu_resources();
    void _release_gpu_resources();



    raster::AbstractRasterizer* _rasterizer;
    Config _config;
    cl7::u8string _resource_prefix;

    std::map<float, AtlasLayer> _atlas_layers;
    std::map<GlyphCacheKey, GlyphCacheEntry> _glyph_cache;

    std::vector<Vertex> _vertices;
    std::vector<DrawBatch> _batches;
    float _current_batch_font_size = 0.0f;

    xl7::graphics::meshes::VertexLayout _vertex_layout;
    xl7::graphics::meshes::VertexBuffer::Id _vertex_buffer_id = {};
    unsigned _vertex_buffer_capacity = 0;

    xl7::graphics::shaders::VertexShader::Id _vertex_shader_id = {};
    xl7::graphics::shaders::PixelShader::Id _pixel_shader_id = {};
    xl7::graphics::shaders::ConstantBuffer::Id _constant_buffer_id = {};
    xl7::graphics::states::SamplerState::Id _sampler_state_id = {};
    xl7::graphics::states::BlendState::Id _blend_state_id = {};

}; // class AbstractTextureAtlasBasedRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_ABSTRACTTEXTUREATLASBASEDRENDERER_H
