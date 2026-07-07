#include "AbstractTextureAtlasBasedRenderer.h"

#include <AlgoLabs/packing/SkylinePacker.h>

#include <XiaoLabs/graphics.h>
#include <XiaoLabs/graphics/textures/Texture2DDesc.h>
#include <XiaoLabs/graphics/textures/Texture2DWrite.h>
#include <XiaoLabs/graphics/meshes/VertexBufferDesc.h>
#include <XiaoLabs/graphics/meshes/VertexBufferWrite.h>
#include <XiaoLabs/graphics/shaders/ConstantBufferDesc.h>
#include <XiaoLabs/graphics/shaders/ConstantBufferWrite.h>
#include <XiaoLabs/graphics/shaders/ShaderDesc.h>
#include <XiaoLabs/graphics/shaders/ShaderWrite.h>
#include <XiaoLabs/graphics/shaders/ShaderCode.h>
#include <XiaoLabs/graphics/shaders/CompileOptions.h>
#include <XiaoLabs/graphics/shaders/ConstantType.h>
#include <XiaoLabs/graphics/shaders/ConstantClass.h>
#include <XiaoLabs/graphics/states/BlendFactor.h>
#include <XiaoLabs/graphics/states/BlendOperation.h>
#include <XiaoLabs/graphics/states/BlendStateDesc.h>
#include <XiaoLabs/graphics/states/SamplerStateDesc.h>

#include <CoreLabs/platform/filesystem.h>
#include <CoreLabs/io/File.h>
#include <CoreLabs/io/Utf8Reader.h>

#include <atomic>
#include <cassert>



namespace fl7::fonts::render {



    AbstractTextureAtlasBasedRenderer::AbstractTextureAtlasBasedRenderer(raster::AbstractRasterizer* rasterizer, Config config)
        : _rasterizer(rasterizer)
        , _config(config)
    {
        assert(rasterizer);

        static std::atomic<unsigned> s_instance_counter{0};
        _resource_prefix = u8"Font Atlas #" + cl7::to_string(s_instance_counter.fetch_add(1)) + u8" ";
    }

    AbstractTextureAtlasBasedRenderer::~AbstractTextureAtlasBasedRenderer() noexcept
    {
        _release_gpu_resources();
    }



    void AbstractTextureAtlasBasedRenderer::_before_begin()
    {
        _ensure_gpu_resources();
    }

    void AbstractTextureAtlasBasedRenderer::_after_end()
    {
        _vertices.clear();
        _batches.clear();
        _current_batch_font_size = 0.0f;
    }

    void AbstractTextureAtlasBasedRenderer::_do_flush()
    {
        if (_vertices.empty())
            return;

        auto* rendering_context = xl7::graphics::primary_context();

        struct CbData
        {
            ml7::Vector2f inv_target_size;
        } cb_data;

        auto viewport = rendering_context->resolve_effective_viewport();
        cb_data.inv_target_size = {
            1.0f / static_cast<float>(viewport.width),
            1.0f / static_cast<float>(viewport.height),
        };

        auto* cb = xl7::graphics::shader_manager()->find_resource<xl7::graphics::shaders::ConstantBuffer>(_constant_buffer_id);
        assert(cb);
        cb->edit().write(xl7::graphics::shaders::ConstantBufferWrite::from_data_ptr(&cb_data));

        // Grow (never shrink) a single persistent vertex buffer instead of
        // creating/destroying one per batch per frame.
        if (!_vertex_buffer_id || _vertex_buffer_capacity < _vertices.size())
        {
            if (_vertex_buffer_id)
                xl7::graphics::mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);

            _vertex_buffer_capacity = static_cast<unsigned>(_vertices.size());

            xl7::graphics::meshes::VertexBufferDesc vb_desc{
                .usage = xl7::graphics::meshes::MeshBufferUsage::Transient,
                .topology = xl7::graphics::meshes::Topology::TriangleList,
                .vertex_count = _vertex_buffer_capacity,
                .vertex_stride = sizeof(Vertex),
                .vertex_layout = _vertex_layout,
            };
            _vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(_resource_prefix + u8"VB", vb_desc);
        }

        auto* vb = xl7::graphics::mesh_manager()->find_resource<xl7::graphics::meshes::VertexBuffer>(_vertex_buffer_id);
        assert(vb);
        vb->edit().write(xl7::graphics::meshes::VertexBufferWrite::from_vertices<Vertex>(std::span<const Vertex>(_vertices)));

        rendering_context->pipeline.vs.set_vertex_shader_id(_vertex_shader_id);
        rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);
        rendering_context->pipeline.ps.set_pixel_shader_id(_pixel_shader_id);
        rendering_context->pipeline.ps.set_sampler_state_id(0, _sampler_state_id);
        rendering_context->pipeline.om.set_blend_state_id(_blend_state_id);

        rendering_context->pipeline.ia.set_vertex_buffer_id(_vertex_buffer_id);
        rendering_context->pipeline.ia.set_index_buffer_id({});

        for (const DrawBatch& batch : _batches)
        {
            if (batch.vertex_count == 0)
                continue;

            auto it = _atlas_layers.find(batch.font_size);
            if (it == _atlas_layers.end())
                continue;

            AtlasLayer& layer = it->second;
            if (!layer.texture_id)
                continue;

            rendering_context->pipeline.ps.set_texture_id(0, layer.texture_id);

            rendering_context->draw(xl7::graphics::meshes::Topology::TriangleList, batch.vertex_count / 3, batch.first_vertex);
        }

        // Batches drawn above must not be resubmitted by a later flush
        // (e.g. a manual flush() call or a nested ScopedBatch closing early).
        _vertices.clear();
        _batches.clear();
        _current_batch_font_size = 0.0f;
    }

    void AbstractTextureAtlasBasedRenderer::_emit_glyph(const Glyph& glyph, const State& state)
    {
        const float font_size = state.text_style.font_size;

        const GlyphCacheEntry* entry = _get_or_rasterize_glyph(glyph, font_size);
        if (!entry)
            return;

        // Start a new batch when the atlas texture changes.
        if (_batches.empty() || font_size != _current_batch_font_size)
        {
            _batches.push_back({.font_size = font_size, .first_vertex = static_cast<unsigned>(_vertices.size()), .vertex_count = 0});
            _current_batch_font_size = font_size;
        }

        const ml7::Vector2f& scaling = state.text_style.scaling;

        const float left   = state.cursor.x + static_cast<float>(entry->pixel_offset.left) * scaling.x;
        const float top    = state.cursor.y + static_cast<float>(entry->pixel_offset.top) * scaling.y;
        const float right  = left + static_cast<float>(entry->image_width) * scaling.x;
        const float bottom = top  + static_cast<float>(entry->image_height) * scaling.y;

        const AtlasLayer& layer = *entry->layer;
        const float iw = 1.0f / static_cast<float>(layer.width);
        const float ih = 1.0f / static_cast<float>(layer.height);
        const float u0 = static_cast<float>(entry->rect.position.x) * iw;
        const float v0 = static_cast<float>(entry->rect.position.y) * ih;
        const float u1 = static_cast<float>(entry->rect.position.x + entry->image_width) * iw;
        const float v1 = static_cast<float>(entry->rect.position.y + entry->image_height) * ih;

        const xl7::graphics::Color color = state.text_style.text_color;

        const Vertex tl = {.position = {left,  top   }, .texcoord = {u0, v0}, .color = color};
        const Vertex tr = {.position = {right, top   }, .texcoord = {u1, v0}, .color = color};
        const Vertex bl = {.position = {left,  bottom}, .texcoord = {u0, v1}, .color = color};
        const Vertex br = {.position = {right, bottom}, .texcoord = {u1, v1}, .color = color};

        _vertices.push_back(tl);
        _vertices.push_back(tr);
        _vertices.push_back(bl);
        _vertices.push_back(tr);
        _vertices.push_back(br);
        _vertices.push_back(bl);

        _batches.back().vertex_count += 6;
    }



    AbstractTextureAtlasBasedRenderer::AtlasLayer& AbstractTextureAtlasBasedRenderer::_get_or_create_atlas_layer(float font_size)
    {
        auto [it, inserted] = _atlas_layers.try_emplace(font_size);
        AtlasLayer& layer = it->second;
        if (!inserted)
            return layer;

        layer.font_size = font_size;
        layer.width  = _config.atlas_size;
        layer.height = _config.atlas_size;
        layer.packer = std::make_unique<al7::packing::SkylinePacker>(al7::packing::Size{_config.atlas_size, _config.atlas_size});

        const xl7::graphics::textures::Texture2DDesc desc{
            .usage = xl7::graphics::textures::TextureUsage::Default,
            .pixel_format = _rasterizer->get_pixel_format(),
            .preferred_channel_order = _rasterizer->get_channel_order(),
            .mip_levels = 1,
            .extent = {.width = layer.width, .height = layer.height},
        };
        layer.texture_id = xl7::graphics::texture_manager()->create_texture_2d(_resource_prefix + u8"Texture " + cl7::to_string(font_size), desc);

        return layer;
    }

    const AbstractTextureAtlasBasedRenderer::GlyphCacheEntry* AbstractTextureAtlasBasedRenderer::_get_or_rasterize_glyph(const Glyph& glyph, float font_size)
    {
        GlyphCacheKey key{.codepoint = glyph.codepoint.value, .font_size = font_size};

        auto [it, inserted] = _glyph_cache.try_emplace(key);
        if (!inserted)
            return &it->second;

        raster::RasterResult raster_result = _rasterizer->rasterize_glyph(glyph, raster::RasterSizeConfig{.font_size = font_size, .padding = _config.raster_padding});

        const xl7::graphics::images::Image& image = raster_result.glyph_image;
        if (image.get_width() == 0 || image.get_height() == 0)
        {
            _glyph_cache.erase(it);
            return nullptr;
        }

        AtlasLayer& layer = _get_or_create_atlas_layer(font_size);

        auto rect_result = layer.packer->insert(al7::packing::Size{image.get_width(), image.get_height()});
        if (!rect_result)
        {
            _glyph_cache.erase(it);
            return nullptr; // atlas full; glyph skipped
        }

        _blit_into_atlas(layer, *rect_result, image);

        GlyphCacheEntry& entry = it->second;
        entry.rect = *rect_result;
        entry.pixel_offset = raster_result.pixel_offset;
        entry.image_width = image.get_width();
        entry.image_height = image.get_height();
        entry.layer = &layer;

        return &entry;
    }

    void AbstractTextureAtlasBasedRenderer::_blit_into_atlas(AtlasLayer& layer, const al7::packing::Rect& rect, const xl7::graphics::images::Image& image)
    {
        auto* texture = xl7::graphics::texture_manager()->find_resource<xl7::graphics::textures::Texture2D>(layer.texture_id);
        assert(texture);
        if (!texture)
            return;

        const unsigned bytes_per_pixel = image.get_desc().determine_bytes_per_pixel();

        texture->edit().write({
            .data = image.get_data(),
            .region = {
                .x = static_cast<unsigned>(rect.position.x),
                .y = static_cast<unsigned>(rect.position.y),
                .width = image.get_width(),
                .height = image.get_height(),
            },
            .row_pitch = image.get_width() * bytes_per_pixel,
        });
    }

    void AbstractTextureAtlasBasedRenderer::_ensure_gpu_resources()
    {
        if (_vertex_shader_id)
            return;

        _vertex_layout.elements = {
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::POSITION, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::COLOR,    .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT4},
        };
        assert(_vertex_layout.calculate_size() == sizeof(Vertex));

        cl7::io::File shader_file(cl7::platform::filesystem::get_working_directory() + _get_shader_path());
        cl7::io::Utf8Reader shader_reader(&shader_file);
        cl7::u8string shader_code_str = shader_reader.read_all();

        xl7::graphics::shaders::ShaderCode shader_code{shader_code_str};
        xl7::graphics::shaders::CompileOptions compile_options;
        compile_options.include_directories.push_back(shader_file.get_path());
        xl7::graphics::shaders::ShaderWrite shader_write{
            .shader_code = &shader_code,
            .compile_options = &compile_options,
        };

        const xl7::graphics::shaders::ShaderDesc shader_desc{
            .language = xl7::graphics::shaders::ShaderCode::Language::HighLevel,
            .entry_point = "",
        };

        _vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(_resource_prefix + u8"VS", shader_desc, shader_write);
        _pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(_resource_prefix + u8"PS", shader_desc, shader_write);

        xl7::graphics::shaders::ConstantBufferDesc cb_desc;
        cb_desc.layout.constant_declarations = {
            {
                .name = "InvTargetSize",
                .constant_type  = xl7::graphics::shaders::ConstantType::Float,
                .constant_class = xl7::graphics::shaders::ConstantClass::Vector,
                .row_count = 1,
                .column_count = 2,
                .element_count = 1,
                .offset = 0,
                .size = 8,
                .padded_size = 8,
            },
        };
        _constant_buffer_id = xl7::graphics::shader_manager()->create_constant_buffer(_resource_prefix + u8"CB", cb_desc);

        xl7::graphics::states::SamplerStateDesc sampler_desc{};
        _sampler_state_id = xl7::graphics::state_manager()->ensure_sampler_state(sampler_desc);

        xl7::graphics::states::BlendStateDesc blend_desc{
            .is_blending_enabled = true,
            .src_color_factor = xl7::graphics::states::BlendFactor::SrcAlpha,
            .dest_color_factor = xl7::graphics::states::BlendFactor::InvSrcAlpha,
            .color_operation = xl7::graphics::states::BlendOperation::Add,
            .src_alpha_factor = xl7::graphics::states::BlendFactor::One,
            .dest_alpha_factor = xl7::graphics::states::BlendFactor::InvSrcAlpha,
            .alpha_operation = xl7::graphics::states::BlendOperation::Add,
        };
        _blend_state_id = xl7::graphics::state_manager()->ensure_blend_state(blend_desc);
    }

    void AbstractTextureAtlasBasedRenderer::_release_gpu_resources()
    {
        for (auto& [font_size, layer] : _atlas_layers)
        {
            if (layer.texture_id)
                xl7::graphics::texture_manager()->release_resource_and_invalidate(layer.texture_id);
        }

        if (_vertex_buffer_id)
            xl7::graphics::mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);
        _vertex_buffer_capacity = 0;

        if (_blend_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_blend_state_id);
        if (_sampler_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_sampler_state_id);
        if (_constant_buffer_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_constant_buffer_id);
        if (_pixel_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_pixel_shader_id);
        if (_vertex_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_vertex_shader_id);
    }



} // namespace fl7::fonts::render
