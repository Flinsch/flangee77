#include "TestRenderer.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/platform/filesystem.h>
#include <CoreLabs/io/File.h>
#include <CoreLabs/io/Utf8Reader.h>



namespace fl7::fonts::render {



    TestRenderer::~TestRenderer() noexcept
    {
        _release_gpu_resources();
    }



    void TestRenderer::_before_begin()
    {
        _ensure_gpu_resources();
    }

    void TestRenderer::_after_end()
    {
        _vertices.clear();
    }

    void TestRenderer::_emit_glyph(const Glyph& glyph, const State& state)
    {
        const ml7::Vector2f scaled_font_size = state.text_metrics.scaled_font_size;
        const ml7::Vector2f origin = state.cursor;
        const xl7::graphics::Color color = {1.0f, 1.0f, 1.0f, 1.0f};

        auto to_vertex = [&](ml7::Vector2f p)
        {
            const ml7::Vector2f position = origin + ml7::Vector2f{p.x * scaled_font_size.x, -p.y * scaled_font_size.y};
            return Vertex{.position = position, .texcoord = {}, .color = color, .weight = 0.0f, .outline_color = {}, .outline_width = 0.0f};
        };

        for (const Contour& contour : glyph.contours)
        {
            for (size_t i = 0; i < contour.segment_count(); ++i)
            {
                const auto& segment = contour.segment_at(i);
                _vertices.push_back(to_vertex(segment.p0));
                _vertices.push_back(to_vertex(segment.p1));
                _vertices.push_back(to_vertex(segment.p1));
                _vertices.push_back(to_vertex(segment.p2));
            }
        }
    }

    void TestRenderer::_do_flush()
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
        // creating/destroying one per glyph per frame.
        if (!_vertex_buffer_id || _vertex_buffer_capacity < _vertices.size())
        {
            if (_vertex_buffer_id)
                xl7::graphics::mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);

            _vertex_buffer_capacity = static_cast<unsigned>(_vertices.size());

            xl7::graphics::meshes::VertexBufferDesc vb_desc{
                .usage = xl7::graphics::meshes::MeshBufferUsage::Transient,
                .topology = xl7::graphics::meshes::Topology::LineList,
                .vertex_count = _vertex_buffer_capacity,
                .vertex_stride = sizeof(Vertex),
                .vertex_layout = _vertex_layout,
            };
            _vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(u8"Test Renderer VB", vb_desc);
        }

        auto* vb = xl7::graphics::mesh_manager()->find_resource<xl7::graphics::meshes::VertexBuffer>(_vertex_buffer_id);
        assert(vb);
        vb->edit().write(xl7::graphics::meshes::VertexBufferWrite::from_vertices<Vertex>(std::span<const Vertex>(_vertices)));

        rendering_context->pipeline.ia.set_vertex_buffer_id(_vertex_buffer_id);
        rendering_context->pipeline.ia.set_index_buffer_id({});

        rendering_context->pipeline.vs.set_vertex_shader_id(_vertex_shader_id);
        rendering_context->pipeline.vs.set_constant_buffer_id(0, _constant_buffer_id);

        rendering_context->pipeline.ps.set_pixel_shader_id(_pixel_shader_id);

        rendering_context->pipeline.om.set_blend_state_id(_blend_state_id);
        rendering_context->pipeline.om.set_depth_stencil_state_id(_depth_stencil_state_id);

        rendering_context->draw(xl7::graphics::meshes::Topology::LineList, static_cast<unsigned>(_vertices.size() / 2), 0);

        _vertices.clear();
    }

    void TestRenderer::_ensure_gpu_resources()
    {
        if (_vertex_shader_id)
            return;

        _vertex_layout.elements = {
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::POSITION, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::COLOR,    .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT4},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 1, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT1},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::COLOR,    .semantic_index = 1, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT4},
            {.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 2, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT1},
        };
        assert(_vertex_layout.calculate_size() == sizeof(Vertex));

        // Reuses atlas-renderer-common.hlsli's vertex transform and vertex
        // layout (position/texcoord/color) as-is; texcoord just goes unused.
        // Also shared with AbstractTextureAtlasBasedRenderer's background quads.
        cl7::io::File shader_file(cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/fonts/solid-color-renderer.hlsl");
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

        _vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(u8"Test Renderer VS", shader_desc, shader_write);
        _pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(u8"Test Renderer PS", shader_desc, shader_write);

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
        _constant_buffer_id = xl7::graphics::shader_manager()->create_constant_buffer(u8"Test Renderer CB", cb_desc);

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

        // Wireframe overlay, drawn in submission order, must never occlude or
        // be occluded via depth (see AbstractTextureAtlasBasedRenderer for why).
        xl7::graphics::states::DepthStencilStateDesc depth_stencil_desc{
            .is_depth_testing_enabled = false,
            .is_depth_writing_enabled = false,
        };
        _depth_stencil_state_id = xl7::graphics::state_manager()->ensure_depth_stencil_state(depth_stencil_desc);
    }

    void TestRenderer::_release_gpu_resources()
    {
        if (_vertex_buffer_id)
            xl7::graphics::mesh_manager()->release_resource_and_invalidate(_vertex_buffer_id);
        _vertex_buffer_capacity = 0;

        if (_blend_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_blend_state_id);
        if (_depth_stencil_state_id)
            xl7::graphics::state_manager()->release_resource_and_invalidate(_depth_stencil_state_id);
        if (_constant_buffer_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_constant_buffer_id);
        if (_pixel_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_pixel_shader_id);
        if (_vertex_shader_id)
            xl7::graphics::shader_manager()->release_resource_and_invalidate(_vertex_shader_id);
    }



} // namespace fl7::fonts::render
