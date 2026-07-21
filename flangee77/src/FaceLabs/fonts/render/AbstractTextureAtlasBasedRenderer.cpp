#include "AbstractTextureAtlasBasedRenderer.h"

#include <AlgoLabs/packing/SkylinePacker.h>

#include <XiaoLabs/graphics.h>
#include <XiaoLabs/graphics/meshes/ClippedQuad.h>

#include <CoreLabs/platform/filesystem.h>
#include <CoreLabs/io/File.h>
#include <CoreLabs/io/Utf8Reader.h>

#include <atomic>
#include <cassert>
#include <limits>



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
        _bg_vertices.clear();

        _glyph_vertices.clear();
        _glyph_batches.clear();
        _current_glyph_batch_font_size = 0.0f;

        _icon_vertices.clear();
        _icon_batches.clear();
        _current_icon_batch_texture_id = {};
    }

    void AbstractTextureAtlasBasedRenderer::_do_flush()
    {
        if (_bg_vertices.empty() && _glyph_vertices.empty() && _icon_vertices.empty())
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

        rendering_context->pipeline.om.set_blend_state_id(_blend_state_id);
        rendering_context->pipeline.om.set_depth_stencil_state_id(_depth_stencil_state_id);

        // Background quads are drawn first (and always as one contiguous,
        // untextured batch), so glyphs end up on top of them.
        if (!_bg_vertices.empty())
        {
            // Grow (never shrink) a single persistent vertex buffer instead of
            // creating/destroying one per batch per frame.
            if (!_bg_vertex_buffer_id || _bg_vertex_buffer_capacity < _bg_vertices.size())
            {
                if (_bg_vertex_buffer_id)
                    xl7::graphics::mesh_manager()->release_resource_and_invalidate(_bg_vertex_buffer_id);

                _bg_vertex_buffer_capacity = static_cast<unsigned>(_bg_vertices.size());

                xl7::graphics::meshes::VertexBufferDesc bg_vb_desc{
                    .usage = xl7::graphics::meshes::MeshBufferUsage::Transient,
                    .topology = xl7::graphics::meshes::Topology::TriangleList,
                    .vertex_count = _bg_vertex_buffer_capacity,
                    .vertex_stride = sizeof(Vertex),
                    .vertex_layout = _vertex_layout,
                };
                _bg_vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(_resource_prefix + u8"BG VB", bg_vb_desc);
            }

            auto* bg_vb = xl7::graphics::mesh_manager()->find_resource<xl7::graphics::meshes::VertexBuffer>(_bg_vertex_buffer_id);
            assert(bg_vb);
            bg_vb->edit().write(xl7::graphics::meshes::VertexBufferWrite::from_vertices<Vertex>(std::span<const Vertex>(_bg_vertices)));

            rendering_context->pipeline.vs.set_vertex_shader_id(_bg_vertex_shader_id);
            rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);
            rendering_context->pipeline.ps.set_pixel_shader_id(_bg_pixel_shader_id);

            rendering_context->pipeline.ia.set_vertex_buffer_id(_bg_vertex_buffer_id);
            rendering_context->pipeline.ia.set_index_buffer_id({});

            rendering_context->draw(xl7::graphics::meshes::Topology::TriangleList, static_cast<unsigned>(_bg_vertices.size() / 3), 0);
        }

        if (!_glyph_vertices.empty())
        {
            // Grow (never shrink) a single persistent vertex buffer instead of
            // creating/destroying one per batch per frame.
            if (!_glyph_vertex_buffer_id || _glyph_vertex_buffer_capacity < _glyph_vertices.size())
            {
                if (_glyph_vertex_buffer_id)
                    xl7::graphics::mesh_manager()->release_resource_and_invalidate(_glyph_vertex_buffer_id);

                _glyph_vertex_buffer_capacity = static_cast<unsigned>(_glyph_vertices.size());

                xl7::graphics::meshes::VertexBufferDesc vb_desc{
                    .usage = xl7::graphics::meshes::MeshBufferUsage::Transient,
                    .topology = xl7::graphics::meshes::Topology::TriangleList,
                    .vertex_count = _glyph_vertex_buffer_capacity,
                    .vertex_stride = sizeof(Vertex),
                    .vertex_layout = _vertex_layout,
                };
                _glyph_vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(_resource_prefix + u8"VB", vb_desc);
            }

            auto* vb = xl7::graphics::mesh_manager()->find_resource<xl7::graphics::meshes::VertexBuffer>(_glyph_vertex_buffer_id);
            assert(vb);
            vb->edit().write(xl7::graphics::meshes::VertexBufferWrite::from_vertices<Vertex>(std::span<const Vertex>(_glyph_vertices)));

            rendering_context->pipeline.vs.set_vertex_shader_id(_glyph_vertex_shader_id);
            rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);
            rendering_context->pipeline.ps.set_pixel_shader_id(_glyph_pixel_shader_id);
            rendering_context->pipeline.ps.set_sampler_state_id(0, _sampler_state_id);

            rendering_context->pipeline.ia.set_vertex_buffer_id(_glyph_vertex_buffer_id);
            rendering_context->pipeline.ia.set_index_buffer_id({});

            for (const DrawBatch& batch : _glyph_batches)
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
        }

        // Icons are drawn last, on top of glyphs (they don't typically overlap
        // anyway), each batch binding its own caller-supplied texture directly
        // instead of a shared glyph atlas layer.
        if (!_icon_vertices.empty())
        {
            if (!_icon_vertex_buffer_id || _icon_vertex_buffer_capacity < _icon_vertices.size())
            {
                if (_icon_vertex_buffer_id)
                    xl7::graphics::mesh_manager()->release_resource_and_invalidate(_icon_vertex_buffer_id);

                _icon_vertex_buffer_capacity = static_cast<unsigned>(_icon_vertices.size());

                xl7::graphics::meshes::VertexBufferDesc icon_vb_desc{
                    .usage = xl7::graphics::meshes::MeshBufferUsage::Transient,
                    .topology = xl7::graphics::meshes::Topology::TriangleList,
                    .vertex_count = _icon_vertex_buffer_capacity,
                    .vertex_stride = sizeof(Vertex),
                    .vertex_layout = _vertex_layout,
                };
                _icon_vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(_resource_prefix + u8"Icon VB", icon_vb_desc);
            }

            auto* icon_vb = xl7::graphics::mesh_manager()->find_resource<xl7::graphics::meshes::VertexBuffer>(_icon_vertex_buffer_id);
            assert(icon_vb);
            icon_vb->edit().write(xl7::graphics::meshes::VertexBufferWrite::from_vertices<Vertex>(std::span<const Vertex>(_icon_vertices)));

            rendering_context->pipeline.vs.set_vertex_shader_id(_icon_vertex_shader_id);
            rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);
            rendering_context->pipeline.ps.set_pixel_shader_id(_icon_pixel_shader_id);
            rendering_context->pipeline.ps.set_sampler_state_id(0, _sampler_state_id);

            rendering_context->pipeline.ia.set_vertex_buffer_id(_icon_vertex_buffer_id);
            rendering_context->pipeline.ia.set_index_buffer_id({});

            for (const IconDrawBatch& batch : _icon_batches)
            {
                if (batch.vertex_count == 0 || !batch.texture_id)
                    continue;

                rendering_context->pipeline.ps.set_texture_id(0, batch.texture_id);

                rendering_context->draw(xl7::graphics::meshes::Topology::TriangleList, batch.vertex_count / 3, batch.first_vertex);
            }
        }

        // Content drawn above must not be resubmitted by a later flush (e.g., a
        // manual flush() call or a nested ScopedBatch closing early).
        _bg_vertices.clear();
        _glyph_vertices.clear();
        _glyph_batches.clear();
        _current_glyph_batch_font_size = 0.0f;
        _icon_vertices.clear();
        _icon_batches.clear();
        _current_icon_batch_texture_id = {};
    }

    void AbstractTextureAtlasBasedRenderer::_emit_background(ml7::Vector2f position, ml7::Vector2f size, const State& state)
    {
        // Same box-clipping as _emit_glyph, just without any UV to remap.
        constexpr float infinity = std::numeric_limits<float>::infinity();
        const ml7::Vector2f clip_min = {
            state.box_size.x > 0.0f ? state.box_position.x : -infinity,
            state.box_size.y > 0.0f ? state.box_position.y : -infinity,
        };
        const ml7::Vector2f clip_max = {
            state.box_size.x > 0.0f ? state.box_position.x + state.box_size.x : infinity,
            state.box_size.y > 0.0f ? state.box_position.y + state.box_size.y : infinity,
        };

        const auto clipped = xl7::graphics::meshes::ClippedQuad::clip(position, position + size, {}, {}, clip_min, clip_max);
        if (!clipped)
            return;

        const xl7::graphics::Color color = state.text_style.background_color;

        // weight is left at 0: unused by the solid-color shader (no distance field to bias).
        const Vertex tl = {.position = {clipped->position_min.x, clipped->position_min.y}, .texcoord = {}, .color = color, .weight = 0.0f};
        const Vertex tr = {.position = {clipped->position_max.x, clipped->position_min.y}, .texcoord = {}, .color = color, .weight = 0.0f};
        const Vertex bl = {.position = {clipped->position_min.x, clipped->position_max.y}, .texcoord = {}, .color = color, .weight = 0.0f};
        const Vertex br = {.position = {clipped->position_max.x, clipped->position_max.y}, .texcoord = {}, .color = color, .weight = 0.0f};

        _bg_vertices.push_back(tl);
        _bg_vertices.push_back(tr);
        _bg_vertices.push_back(bl);
        _bg_vertices.push_back(tr);
        _bg_vertices.push_back(br);
        _bg_vertices.push_back(bl);
    }

    void AbstractTextureAtlasBasedRenderer::_emit_glyph(const Glyph& glyph, const State& state)
    {
        const float font_size = state.text_style.font_size;

        const GlyphCacheEntry* entry = _get_or_rasterize_glyph(glyph, font_size);
        if (!entry)
            return;

        // Start a new batch when the atlas texture changes.
        if (_glyph_batches.empty() || font_size != _current_glyph_batch_font_size)
        {
            _glyph_batches.push_back({.font_size = font_size, .first_vertex = static_cast<unsigned>(_glyph_vertices.size()), .vertex_count = 0});
            _current_glyph_batch_font_size = font_size;
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

        // A box_size component of 0 (or less) means "unconstrained" on that
        // axis (as with point-based draw_text, which always passes a
        // zero-size box and must therefore never be clipped).
        constexpr float infinity = std::numeric_limits<float>::infinity();
        const ml7::Vector2f clip_min = {
            state.box_size.x > 0.0f ? state.box_position.x : -infinity,
            state.box_size.y > 0.0f ? state.box_position.y : -infinity,
        };
        const ml7::Vector2f clip_max = {
            state.box_size.x > 0.0f ? state.box_position.x + state.box_size.x : infinity,
            state.box_size.y > 0.0f ? state.box_position.y + state.box_size.y : infinity,
        };

        const auto clipped = xl7::graphics::meshes::ClippedQuad::clip({left, top}, {right, bottom}, {u0, v0}, {u1, v1}, clip_min, clip_max);
        if (!clipped)
            return;

        const xl7::graphics::Color color = state.current_glyph_style.text_color;

        // Pseudo-italic: shear each row horizontally by an amount proportional
        // to its (screen-space, y-down) distance from the baseline, so the
        // glyph leans right above the baseline and left below it (descenders).
        // Applied after box-clipping, so a sheared glyph can slightly overshoot
        // the box's left/right edges. Only the top/bottom clip stays exact.
        constexpr float italic_shear_per_unit = 0.20f; // ~11 degrees of slant per unit of italic_intensity
        const float italic_intensity = state.current_glyph_style.italic_intensity;
        const float shear_top    = italic_intensity * italic_shear_per_unit * (state.cursor.y - clipped->position_min.y);
        const float shear_bottom = italic_intensity * italic_shear_per_unit * (state.cursor.y - clipped->position_max.y);

        const float weight = state.current_glyph_style.weight;

        const Vertex tl = {.position = {clipped->position_min.x + shear_top,    clipped->position_min.y}, .texcoord = {clipped->uv_min.x, clipped->uv_min.y}, .color = color, .weight = weight};
        const Vertex tr = {.position = {clipped->position_max.x + shear_top,    clipped->position_min.y}, .texcoord = {clipped->uv_max.x, clipped->uv_min.y}, .color = color, .weight = weight};
        const Vertex bl = {.position = {clipped->position_min.x + shear_bottom, clipped->position_max.y}, .texcoord = {clipped->uv_min.x, clipped->uv_max.y}, .color = color, .weight = weight};
        const Vertex br = {.position = {clipped->position_max.x + shear_bottom, clipped->position_max.y}, .texcoord = {clipped->uv_max.x, clipped->uv_max.y}, .color = color, .weight = weight};

        _glyph_vertices.push_back(tl);
        _glyph_vertices.push_back(tr);
        _glyph_vertices.push_back(bl);
        _glyph_vertices.push_back(tr);
        _glyph_vertices.push_back(br);
        _glyph_vertices.push_back(bl);

        _glyph_batches.back().vertex_count += 6;
    }

    void AbstractTextureAtlasBasedRenderer::_emit_icon(const Icon& icon, const State& state)
    {
        if (!icon.texture_id)
            return;

        // Start a new batch when the bound texture changes.
        if (_icon_batches.empty() || icon.texture_id != _current_icon_batch_texture_id)
        {
            _icon_batches.push_back({.texture_id = icon.texture_id, .first_vertex = static_cast<unsigned>(_icon_vertices.size()), .vertex_count = 0});
            _current_icon_batch_texture_id = icon.texture_id;
        }

        const ml7::Vector2f top_left = state.cursor + icon.offset;
        const ml7::Vector2f bottom_right = top_left + icon.size;

        // Same box-clipping as _emit_glyph.
        constexpr float infinity = std::numeric_limits<float>::infinity();
        const ml7::Vector2f clip_min = {
            state.box_size.x > 0.0f ? state.box_position.x : -infinity,
            state.box_size.y > 0.0f ? state.box_position.y : -infinity,
        };
        const ml7::Vector2f clip_max = {
            state.box_size.x > 0.0f ? state.box_position.x + state.box_size.x : infinity,
            state.box_size.y > 0.0f ? state.box_position.y + state.box_size.y : infinity,
        };

        const auto clipped = xl7::graphics::meshes::ClippedQuad::clip(top_left, bottom_right, icon.uv_min, icon.uv_max, clip_min, clip_max);
        if (!clipped)
            return;

        // Icons keep their own texture colors (unlike glyphs' coverage-tinted
        // text_color): only alpha is taken from the current style, so fading a
        // text block's alpha fades its icons along with it, without tinting them.
        const xl7::graphics::Color color = {1.0f, 1.0f, 1.0f, state.current_glyph_style.text_color.a};

        const Vertex tl = {.position = {clipped->position_min.x, clipped->position_min.y}, .texcoord = {clipped->uv_min.x, clipped->uv_min.y}, .color = color, .weight = 0.0f};
        const Vertex tr = {.position = {clipped->position_max.x, clipped->position_min.y}, .texcoord = {clipped->uv_max.x, clipped->uv_min.y}, .color = color, .weight = 0.0f};
        const Vertex bl = {.position = {clipped->position_min.x, clipped->position_max.y}, .texcoord = {clipped->uv_min.x, clipped->uv_max.y}, .color = color, .weight = 0.0f};
        const Vertex br = {.position = {clipped->position_max.x, clipped->position_max.y}, .texcoord = {clipped->uv_max.x, clipped->uv_max.y}, .color = color, .weight = 0.0f};

        _icon_vertices.push_back(tl);
        _icon_vertices.push_back(tr);
        _icon_vertices.push_back(bl);
        _icon_vertices.push_back(tr);
        _icon_vertices.push_back(br);
        _icon_vertices.push_back(bl);

        _icon_batches.back().vertex_count += 6;
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
        if (_glyph_vertex_shader_id)
            return;

        _vertex_layout.elements = {
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::POSITION, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::COLOR,    .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT4},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 1, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT1},
        };
        assert(_vertex_layout.calculate_size() == sizeof(Vertex));

        const xl7::graphics::shaders::ShaderDesc shader_desc{
            .language = xl7::graphics::shaders::ShaderCode::Language::HighLevel,
            .entry_point = "",
        };

        // Background quads need a solid-fill shader: the glyph shader above
        // always samples the atlas texture, so it can't produce a flat color.
        cl7::io::File bg_shader_file(cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/fonts/solid-color-renderer.hlsl");
        cl7::io::Utf8Reader bg_shader_reader(&bg_shader_file);
        cl7::u8string bg_shader_code_str = bg_shader_reader.read_all();

        xl7::graphics::shaders::ShaderCode bg_shader_code{bg_shader_code_str};
        xl7::graphics::shaders::CompileOptions bg_compile_options;
        bg_compile_options.include_directories.push_back(bg_shader_file.get_path());
        xl7::graphics::shaders::ShaderWrite bg_shader_write{
            .shader_code = &bg_shader_code,
            .compile_options = &bg_compile_options,
        };

        _bg_vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(_resource_prefix + u8"BG VS", shader_desc, bg_shader_write);
        _bg_pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(_resource_prefix + u8"BG PS", shader_desc, bg_shader_write);

        // Glyph quads use the renderer-specific shader.
        cl7::io::File glyph_shader_file(cl7::platform::filesystem::get_working_directory() + _get_shader_path());
        cl7::io::Utf8Reader glyph_shader_reader(&glyph_shader_file);
        cl7::u8string glyph_shader_code_str = glyph_shader_reader.read_all();

        xl7::graphics::shaders::ShaderCode glyph_shader_code{glyph_shader_code_str};
        xl7::graphics::shaders::CompileOptions glyph_compile_options;
        glyph_compile_options.include_directories.push_back(glyph_shader_file.get_path());
        xl7::graphics::shaders::ShaderWrite glyph_shader_write{
            .shader_code = &glyph_shader_code,
            .compile_options = &glyph_compile_options,
        };

        _glyph_vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(_resource_prefix + u8"VS", shader_desc, glyph_shader_write);
        _glyph_pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(_resource_prefix + u8"PS", shader_desc, glyph_shader_write);

        // Icons need their own shader too: unlike the glyph shader above (which
        // treats its texture as coverage/distance data, tinted by vertex color),
        // icons sample their texture directly and keep its own colors.
        cl7::io::File icon_shader_file(cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/fonts/icon-renderer.hlsl");
        cl7::io::Utf8Reader icon_shader_reader(&icon_shader_file);
        cl7::u8string icon_shader_code_str = icon_shader_reader.read_all();

        xl7::graphics::shaders::ShaderCode icon_shader_code{icon_shader_code_str};
        xl7::graphics::shaders::CompileOptions icon_compile_options;
        icon_compile_options.include_directories.push_back(icon_shader_file.get_path());
        xl7::graphics::shaders::ShaderWrite icon_shader_write{
            .shader_code = &icon_shader_code,
            .compile_options = &icon_compile_options,
        };

        _icon_vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(_resource_prefix + u8"Icon VS", shader_desc, icon_shader_write);
        _icon_pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(_resource_prefix + u8"Icon PS", shader_desc, icon_shader_write);

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

        xl7::graphics::states::DepthStencilStateDesc depth_stencil_desc{
            .is_depth_testing_enabled = false,
            .is_depth_writing_enabled = false,
        };
        _depth_stencil_state_id = xl7::graphics::state_manager()->ensure_depth_stencil_state(depth_stencil_desc);
    }

    void AbstractTextureAtlasBasedRenderer::_release_gpu_resources()
    {
        for (auto& [font_size, layer] : _atlas_layers)
        {
            if (layer.texture_id)
                xl7::graphics::texture_manager()->release_resource_and_invalidate(layer.texture_id);
        }

        if (_bg_vertex_buffer_id)
            xl7::graphics::mesh_manager()->release_resource_and_invalidate(_bg_vertex_buffer_id);
        _bg_vertex_buffer_capacity = 0;

        if (_glyph_vertex_buffer_id)
            xl7::graphics::mesh_manager()->release_resource_and_invalidate(_glyph_vertex_buffer_id);
        _glyph_vertex_buffer_capacity = 0;

        if (_icon_vertex_buffer_id)
            xl7::graphics::mesh_manager()->release_resource_and_invalidate(_icon_vertex_buffer_id);
        _icon_vertex_buffer_capacity = 0;

        if (_depth_stencil_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_depth_stencil_state_id);
        if (_blend_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_blend_state_id);
        if (_sampler_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_sampler_state_id);
        if (_constant_buffer_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_constant_buffer_id);
        if (_icon_pixel_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_icon_pixel_shader_id);
        if (_icon_vertex_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_icon_vertex_shader_id);
        if (_glyph_pixel_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_glyph_pixel_shader_id);
        if (_glyph_vertex_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_glyph_vertex_shader_id);
        if (_bg_pixel_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_bg_pixel_shader_id);
        if (_bg_vertex_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_bg_vertex_shader_id);
    }



} // namespace fl7::fonts::render
