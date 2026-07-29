#include "QuadRenderer.h"

#include "./meshes/ClippedQuad.h"

#include <XiaoLabs/graphics.h>
#include <XiaoLabs/resources/ResourceId.h>

#include <CoreLabs/io/File.h>
#include <CoreLabs/io/Utf8Reader.h>
#include <CoreLabs/platform/filesystem.h>

#include <algorithm>
#include <atomic>
#include <limits>



namespace xl7::graphics {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    QuadRenderer::QuadRenderer()
    {
        // Prefix every GPU resource identifier this instance creates with a
        // unique instance number, so multiple QuadRenderer instances don't
        // collide (ResourceManager::_add_resource asserts on duplicate
        // identifiers); same pattern as AbstractTextureAtlasBasedRenderer.
        static std::atomic<unsigned> s_instance_counter{0};
        _resource_prefix = u8"QuadRenderer #" + cl7::to_string(s_instance_counter.fetch_add(1)) + u8" ";
    }

    QuadRenderer::~QuadRenderer()
    {
        _release_gpu_resources();
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Begins a new quad rendering batch. If you only call draw_rect()/
     * draw_textured_rect() occasionally, it is not necessary to call begin()/
     * end() manually: the renderer will manage batching internally.
     *
     * For greater convenience, the RAII helper ScopedBatch should be used via
     * begin_batch().
     */
    void QuadRenderer::begin()
    {
        if (_batch_depth == 0)
            _ensure_gpu_resources();

        ++_batch_depth;
    }

    /**
     * Submits any queued geometry and finalizes the current batch. This implicitly
     * calls flush().
     *
     * For greater convenience, the RAII helper ScopedBatch should be used via
     * begin_batch(), which ultimately calls end() automatically.
     */
    void QuadRenderer::end()
    {
        assert(_batch_depth > 0);
        if (_batch_depth == 0)
            return;

        if (--_batch_depth == 0)
            flush();
    }

    /**
     * Forces the submission of all currently batched geometry. This can be called
     * manually if external render state changes, or when mixing with other
     * rendering systems that may modify render state.
     */
    void QuadRenderer::flush()
    {
        if (_batches.empty())
            return;

        RenderingContext* rendering_context = primary_context();

        // Update the constant buffer (screen-pixel-to-NDC transform).
        struct CbData
        {
            ml7::Vector2f inv_target_size;
        } cb_data;

        const Viewport viewport = rendering_context->resolve_effective_viewport();
        cb_data.inv_target_size = {
            1.0f / static_cast<float>(viewport.width),
            1.0f / static_cast<float>(viewport.height),
        };

        auto* cb = shader_manager()->find_resource<shaders::ConstantBuffer>(_constant_buffer_id);
        assert(cb);
        cb->edit().write(shaders::ConstantBufferWrite::from_data_ptr(&cb_data));

        // Grow (never shrink) a single persistent vertex buffer instead of
        // creating/destroying one per batch per frame.
        if (!_vertex_buffer_id || _vertex_buffer_capacity < _vertices.size())
        {
            if (_vertex_buffer_id)
                mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);

            _vertex_buffer_capacity = static_cast<unsigned>(_vertices.size());

            const meshes::VertexBufferDesc vb_desc{
                .usage = meshes::MeshBufferUsage::Transient,
                .topology = meshes::Topology::TriangleList,
                .vertex_count = _vertex_buffer_capacity,
                .vertex_stride = sizeof(Vertex),
                .vertex_layout = _vertex_layout,
            };
            _vertex_buffer_id = mesh_manager()->create_vertex_buffer(_resource_prefix + u8"VB", vb_desc);
        }

        auto* vb = mesh_manager()->find_resource<meshes::VertexBuffer>(_vertex_buffer_id);
        assert(vb);
        vb->edit().write(meshes::VertexBufferWrite::from_vertices<Vertex>(std::span<const Vertex>(_vertices)));

        // Set once-per-flush pipeline state.
        rendering_context->pipeline.om.set_blend_state_id(_blend_state_id);
        rendering_context->pipeline.om.set_depth_stencil_state_id(_depth_stencil_state_id);

        rendering_context->pipeline.vs.set_vertex_shader_id(_vertex_shader_id);
        rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);

        rendering_context->pipeline.ps.set_sampler_state_id(0, _sampler_state_id);

        rendering_context->pipeline.ia.set_vertex_buffer_id(_vertex_buffer_id);
        rendering_context->pipeline.ia.set_index_buffer_id({});

        // Draw each batch, in submission order, switching the pixel shader
        // and/or bound texture only when the batch kind/texture changes.
        for (const DrawBatch& batch : _batches)
        {
            if (batch.vertex_count == 0)
                continue;

            if (batch.kind == BatchKind::Textured)
            {
                rendering_context->pipeline.ps.set_pixel_shader_id(_textured_pixel_shader_id);
                rendering_context->pipeline.ps.set_texture_id(0, resources::id_cast<textures::Texture::Id>(batch.texture_id));
            }
            else
            {
                rendering_context->pipeline.ps.set_pixel_shader_id(_solid_pixel_shader_id);
            }

            rendering_context->draw(meshes::Topology::TriangleList, batch.vertex_count / 3, batch.first_vertex);
        }

        _vertices.clear();
        _batches.clear();
    }



    /**
     * Draws a flat-colored, axis-aligned rect (screen pixel space, y-down), clipped
     * against the current clip rect (see push_clip_rect()).
     *
     * If no active batch is open, begin()/end() are called automatically.
     */
    void QuadRenderer::draw_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, const Color& color)
    {
        ScopedBatch implicit_batch(_batch_depth == 0 ? this : nullptr);
        _emit_quad(position_min, position_max, {}, {}, color, BatchKind::Solid, {});
    }

    /**
     * Draws a simple-textured, axis-aligned rect (screen pixel space, y-down),
     * clipped against the current clip rect (see push_clip_rect()). The sampled
     * texture color is modulated by `tint`.
     *
     * If no active batch is open, begin()/end() are called automatically.
     */
    void QuadRenderer::draw_textured_rect(ml7::Vector2f position_min, ml7::Vector2f position_max, textures::Texture2D::Id texture_id, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const Color& tint)
    {
        ScopedBatch implicit_batch(_batch_depth == 0 ? this : nullptr);
        _emit_quad(position_min, position_max, uv_min, uv_max, tint, BatchKind::Textured, texture_id);
    }



    /**
     * Pushes a new clip rect, intersected with the current one (there is no clip
     * rect initially, i.e., unbounded). Quads drawn while this clip rect is active
     * are clipped against it (see ClippedQuad::clip()): a quad clipped away
     * entirely is silently skipped.
     */
    void QuadRenderer::push_clip_rect(ml7::Vector2f clip_min, ml7::Vector2f clip_max)
    {
        if (!_clip_rect_stack.empty())
        {
            const ClipRect& current = _clip_rect_stack.back();
            clip_min = {std::max(clip_min.x, current.min.x), std::max(clip_min.y, current.min.y)};
            clip_max = {std::min(clip_max.x, current.max.x), std::min(clip_max.y, current.max.y)};
        }

        _clip_rect_stack.push_back({.min = clip_min, .max = clip_max});
    }

    /**
     * Pops the most recently pushed clip rect, restoring the previous one (or the
     * unbounded default, if the stack is empty afterward).
     */
    void QuadRenderer::pop_clip_rect()
    {
        assert(!_clip_rect_stack.empty());
        if (!_clip_rect_stack.empty())
            _clip_rect_stack.pop_back();
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Clips the specified quad against the current clip rect (if any) and, if
     * anything remains visible, appends it to the current batch.
     */
    void QuadRenderer::_emit_quad(ml7::Vector2f position_min, ml7::Vector2f position_max, ml7::Vector2f uv_min, ml7::Vector2f uv_max, const Color& color, BatchKind kind, textures::Texture2D::Id texture_id)
    {
        ml7::Vector2f clip_min{-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()};
        ml7::Vector2f clip_max{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()};
        if (!_clip_rect_stack.empty())
        {
            clip_min = _clip_rect_stack.back().min;
            clip_max = _clip_rect_stack.back().max;
        }

        const auto clipped = meshes::ClippedQuad::clip(position_min, position_max, uv_min, uv_max, clip_min, clip_max);
        if (!clipped)
            return;

        if (_batches.empty() || _batches.back().kind != kind || (kind == BatchKind::Textured && _batches.back().texture_id != texture_id))
            _batches.push_back({.kind = kind, .texture_id = texture_id, .first_vertex = static_cast<unsigned>(_vertices.size()), .vertex_count = 0});

        // Same winding order as AbstractTextureAtlasBasedRenderer::_emit_background:
        // tl, tr, bl, tr, br, bl.
        const Vertex tl{.position = {clipped->position_min.x, clipped->position_min.y}, .texcoord = {clipped->uv_min.x, clipped->uv_min.y}, .color = color};
        const Vertex tr{.position = {clipped->position_max.x, clipped->position_min.y}, .texcoord = {clipped->uv_max.x, clipped->uv_min.y}, .color = color};
        const Vertex bl{.position = {clipped->position_min.x, clipped->position_max.y}, .texcoord = {clipped->uv_min.x, clipped->uv_max.y}, .color = color};
        const Vertex br{.position = {clipped->position_max.x, clipped->position_max.y}, .texcoord = {clipped->uv_max.x, clipped->uv_max.y}, .color = color};

        _vertices.push_back(tl);
        _vertices.push_back(tr);
        _vertices.push_back(bl);
        _vertices.push_back(tr);
        _vertices.push_back(br);
        _vertices.push_back(bl);

        _batches.back().vertex_count += 6;
    }

    /**
     * Lazily creates the GPU resources (shaders, constant buffer, states) this
     * renderer needs, the first time they are needed.
     */
    void QuadRenderer::_ensure_gpu_resources()
    {
        if (_vertex_shader_id)
            return;

        _vertex_layout.elements = {
            {.semantic = meshes::VertexLayout::Semantic::POSITION, .semantic_index = 0, .data_type = meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 0, .data_type = meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = meshes::VertexLayout::Semantic::COLOR, .semantic_index = 0, .data_type = meshes::VertexLayout::DataType::FLOAT4},
        };
        assert(_vertex_layout.calculate_size() == sizeof(Vertex));

        // Load the shader source once; compile it twice (solid/textured
        // pixel shader variants share the same vertex shader).
        cl7::io::File shader_file(cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/graphics/quad-renderer.hlsl");
        cl7::io::Utf8Reader shader_reader(&shader_file);
        const cl7::u8string shader_code_str = shader_reader.read_all();

        const shaders::ShaderCode shader_code{shader_code_str};
        const shaders::ShaderDesc shader_desc{
            .language = shaders::ShaderCode::Language::HighLevel,
            .entry_point = "",
        };

        shaders::CompileOptions solid_compile_options;
        solid_compile_options.include_directories.push_back(shader_file.get_path());
        const shaders::ShaderWrite solid_shader_write{
            .shader_code = &shader_code,
            .compile_options = &solid_compile_options,
        };

        shaders::CompileOptions textured_compile_options;
        textured_compile_options.include_directories.push_back(shader_file.get_path());
        textured_compile_options.macro_definitions["TEXTURED"] = "1";
        const shaders::ShaderWrite textured_shader_write{
            .shader_code = &shader_code,
            .compile_options = &textured_compile_options,
        };

        _vertex_shader_id = shader_manager()->create_vertex_shader(_resource_prefix + u8"VS", shader_desc, solid_shader_write);
        _solid_pixel_shader_id = shader_manager()->create_pixel_shader(_resource_prefix + u8"Solid PS", shader_desc, solid_shader_write);
        _textured_pixel_shader_id = shader_manager()->create_pixel_shader(_resource_prefix + u8"Textured PS", shader_desc, textured_shader_write);

        shaders::ConstantBufferDesc cb_desc;
        cb_desc.layout.constant_declarations = {
            {
                .name = "InvTargetSize",
                .constant_type = shaders::ConstantType::Float,
                .constant_class = shaders::ConstantClass::Vector,
                .row_count = 1,
                .column_count = 2,
                .element_count = 1,
                .offset = 0,
                .size = 8,
                .padded_size = 8,
            },
        };
        _constant_buffer_id = shader_manager()->create_constant_buffer(_resource_prefix + u8"CB", cb_desc);

        const states::SamplerStateDesc sampler_desc{};
        _sampler_state_id = state_manager()->ensure_sampler_state(sampler_desc);

        constexpr states::BlendStateDesc blend_desc{
            .is_blending_enabled = true,
            .src_color_factor = states::BlendFactor::SrcAlpha,
            .dest_color_factor = states::BlendFactor::InvSrcAlpha,
            .color_operation = states::BlendOperation::Add,
            .src_alpha_factor = states::BlendFactor::One,
            .dest_alpha_factor = states::BlendFactor::InvSrcAlpha,
            .alpha_operation = states::BlendOperation::Add,
        };
        _blend_state_id = state_manager()->ensure_blend_state(blend_desc);

        constexpr states::DepthStencilStateDesc depth_stencil_desc{
            .is_depth_testing_enabled = false,
            .is_depth_writing_enabled = false,
        };
        _depth_stencil_state_id = state_manager()->ensure_depth_stencil_state(depth_stencil_desc);
    }

    /**
     * Releases all GPU resources held by this renderer.
     */
    void QuadRenderer::_release_gpu_resources()
    {
        if (_vertex_buffer_id)
            mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);
        _vertex_buffer_capacity = 0;

        if (_depth_stencil_state_id)
            state_manager()->release_resource_and_invalidate(_depth_stencil_state_id);
        if (_blend_state_id)
            state_manager()->release_resource_and_invalidate(_blend_state_id);
        if (_sampler_state_id)
            state_manager()->release_resource_and_invalidate(_sampler_state_id);

        if (_constant_buffer_id)
            shader_manager()->release_resource_and_invalidate(_constant_buffer_id);
        if (_textured_pixel_shader_id)
            shader_manager()->release_resource_and_invalidate(_textured_pixel_shader_id);
        if (_solid_pixel_shader_id)
            shader_manager()->release_resource_and_invalidate(_solid_pixel_shader_id);
        if (_vertex_shader_id)
            shader_manager()->release_resource_and_invalidate(_vertex_shader_id);
    }



} // namespace xl7::graphics
