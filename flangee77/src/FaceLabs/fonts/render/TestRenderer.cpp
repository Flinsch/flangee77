#include "TestRenderer.h"

#include "../raster/SimpleBitmapRasterizer.h"
#include "../raster/OversampledBitmapRasterizer.h"
#include "../raster/AnalyticalCoverageRasterizer.h"

#include <XiaoLabs/graphics.h>

#include <MathLabs/Vector2.h>

#include <CoreLabs/platform/filesystem.h>
#include <CoreLabs/io/File.h>
#include <CoreLabs/io/Utf8Reader.h>

#include <filesystem>



namespace fl7::fonts::render {



    void TestRenderer::_emit_glyph(const Glyph& glyph, const State& state)
    {
        /*raster::SimpleBitmapRasterizer simple_bitmap_rasterizer{0, true};
        //raster::OversampledBitmapRasterizer oversampled_bitmap_rasterizer{2, 3};
        raster::OversampledBitmapRasterizer oversampled_bitmap_rasterizer{2, &simple_bitmap_rasterizer};
        raster::AnalyticalCoverageRasterizer analytical_coverage_rasterizer;
        //raster::AbstractBitmapRasterizer* bitmap_rasterizer = &simple_bitmap_rasterizer;
        raster::AbstractBitmapRasterizer* bitmap_rasterizer = &oversampled_bitmap_rasterizer;
        //raster::AbstractBitmapRasterizer* bitmap_rasterizer = &analytical_coverage_rasterizer;
        auto raster_result = bitmap_rasterizer->rasterize_glyph(glyph, {.font_size = state.text_style.font_size, .padding = 1});*/

        struct Vertex
        {
            ml7::Vector2f pos;
            ml7::Vector2f pad;
        };

        xl7::graphics::meshes::VertexLayout vertex_layout;
        vertex_layout.elements.push_back({.semantic = xl7::graphics::meshes::VertexLayout::Semantic::POSITION, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2});
        vertex_layout.elements.push_back({.semantic = xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD, .semantic_index = 0, .data_type = xl7::graphics::meshes::VertexLayout::DataType::FLOAT2});
        assert(vertex_layout.calculate_size() == sizeof(Vertex));

        std::vector<Vertex> vertices;
        for (const auto& contour : glyph.contours)
        {
            for (size_t i = 0; i < contour.segment_count(); ++i)
            {
                const auto& segment = contour.segment_at(i);
                vertices.push_back({.pos = segment.p0});
                vertices.push_back({.pos = segment.p1});
                vertices.push_back({.pos = segment.p1});
                vertices.push_back({.pos = segment.p2});
            }
        }

        if (vertices.empty())
            return;

        xl7::graphics::meshes::VertexBuffer::Desc vertex_buffer_desc{
            xl7::resources::ResourceUsage::Immutable,
            xl7::graphics::meshes::Topology::LineList,
            static_cast<unsigned>(vertices.size()),
            sizeof(Vertex),
            vertex_layout,
        };

        xl7::graphics::meshes::VertexDataProvider<Vertex> vertex_data_provider{vertices};

        auto vertex_buffer_id = xl7::graphics::mesh_manager()->create_vertex_buffer(u8"Test Renderer Vertex Buffer", vertex_buffer_desc, vertex_data_provider);

        cl7::io::File file(cl7::platform::filesystem::get_working_directory() + u8"assets/shaders/fonts/test-shader.hlsl");
        cl7::io::Utf8Reader utf8_reader(&file);
        cl7::u8string high_level_code = utf8_reader.read_all();
        xl7::graphics::shaders::ShaderCode shader_code{high_level_code};
        xl7::graphics::shaders::CompileOptions compile_options;
        xl7::graphics::shaders::CodeDataProvider code_data_provider{&shader_code, &compile_options};

        auto vertex_shader_id = xl7::graphics::shader_manager()->create_vertex_shader(u8"Test Renderer Vertex Shader", code_data_provider);
        auto pixel_shader_id = xl7::graphics::shader_manager()->create_pixel_shader(u8"Test Renderer Pixel Shader", code_data_provider);

        xl7::graphics::shaders::ConstantBuffer::Desc constant_buffer_desc;
        constant_buffer_desc.layout.constant_declarations = {
            {.name = "ScaledFontSize", .constant_type = xl7::graphics::shaders::ConstantType::Float, .constant_class = xl7::graphics::shaders::ConstantClass::Vector, .row_count = 1, .column_count = 2, .element_count = 1, .offset = 0, .size = 8, .padded_size = 8},
            {.name = "PixelPosition", .constant_type = xl7::graphics::shaders::ConstantType::Float, .constant_class = xl7::graphics::shaders::ConstantClass::Vector, .row_count = 1, .column_count = 2, .element_count = 1, .offset = 8, .size = 8, .padded_size = 8},
            {.name = "InvTargetSize", .constant_type = xl7::graphics::shaders::ConstantType::Float, .constant_class = xl7::graphics::shaders::ConstantClass::Vector, .row_count = 1, .column_count = 2, .element_count = 1, .offset = 16, .size = 8, .padded_size = 8},
            {.name = "Color", .constant_type = xl7::graphics::shaders::ConstantType::Float, .constant_class = xl7::graphics::shaders::ConstantClass::Vector, .row_count = 1, .column_count = 4, .element_count = 1, .offset = 24, .size = 16, .padded_size = 16},
        };

        auto constant_buffer_id = xl7::graphics::shader_manager()->create_constant_buffer(u8"Test Renderer Constant Buffer", constant_buffer_desc);

        auto* rendering_context = xl7::graphics::primary_context();

        struct ConstantBufferData
        {
            ml7::Vector2f scaled_font_size;
            ml7::Vector2f pixel_position;
            ml7::Vector2f inv_target_size;
            xl7::graphics::Color color;
        } constant_buffer_data;
        constant_buffer_data.scaled_font_size = state.text_metrics.scaled_font_size;
        constant_buffer_data.pixel_position = state.cursor + ml7::Vector2f{50.0f, 50.0f};
        constant_buffer_data.inv_target_size = {1.0f / static_cast<float>(rendering_context->resolve_effective_viewport().width), 1.0f / static_cast<float>(rendering_context->resolve_effective_viewport().height)};
        constant_buffer_data.color = {1.0f, 1.0f, 0.1f, 1.0f};

        xl7::graphics::shaders::ConstantDataProvider constant_data_provider(&constant_buffer_data);

        auto* constant_buffer = xl7::graphics::shader_manager()->find_resource<xl7::graphics::shaders::ConstantBuffer>(constant_buffer_id);
        assert(constant_buffer);
        constant_buffer->update(constant_data_provider);

        rendering_context->pipeline.ia.set_vertex_buffer_id(vertex_buffer_id);
        rendering_context->pipeline.ia.set_index_buffer_id({});

        rendering_context->pipeline.vs.set_vertex_shader_id(vertex_shader_id);
        rendering_context->pipeline.vs.set_constant_buffer_id(0, constant_buffer_id);

        rendering_context->pipeline.ps.set_pixel_shader_id(pixel_shader_id);
        rendering_context->pipeline.ps.set_constant_buffer_id(0, constant_buffer_id);

        rendering_context->draw();

        xl7::graphics::shader_manager()->release_resource_and_invalidate(constant_buffer_id);

        xl7::graphics::shader_manager()->release_resource_and_invalidate(vertex_shader_id);
        xl7::graphics::shader_manager()->release_resource_and_invalidate(pixel_shader_id);

        xl7::graphics::mesh_manager()->release_resource_and_invalidate(vertex_buffer_id);
    }



} // namespace fl7::fonts::render
