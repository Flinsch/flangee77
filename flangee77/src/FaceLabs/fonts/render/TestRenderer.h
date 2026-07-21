#ifndef FL7_FONTS_RENDER_TESTRENDERER_H
#define FL7_FONTS_RENDER_TESTRENDERER_H
#include "./AbstractRenderer.h"

#include <XiaoLabs/graphics/Color.h>
#include <XiaoLabs/graphics/meshes/VertexBuffer.h>
#include <XiaoLabs/graphics/shaders/VertexShader.h>
#include <XiaoLabs/graphics/shaders/PixelShader.h>
#include <XiaoLabs/graphics/shaders/ConstantBuffer.h>
#include <XiaoLabs/graphics/states/BlendState.h>
#include <XiaoLabs/graphics/states/DepthStencilState.h>

#include <MathLabs/Vector2.h>

#include <vector>



namespace fl7::fonts::render {



/**
 * Debug renderer that draws a glyph's raw vector outline (its quadratic Bezier
 * contour segments) as a wireframe, for visually inspecting glyph geometry.
 * Not meant for production text rendering.
 */
class TestRenderer
    : public AbstractRenderer
{

public:
    ~TestRenderer() noexcept override;



private:
    struct Vertex
    {
        ml7::Vector2f position;
        ml7::Vector2f texcoord; // Unused by the shader; kept only to match the shared vertex layout.
        xl7::graphics::Color color;
        float weight; // Unused by the shader; kept only to match the shared vertex layout.
    };



    void _before_begin() override;
    void _after_end() override;
    void _do_flush() override;
    void _emit_glyph(const Glyph& glyph, const State& state) override;

    void _ensure_gpu_resources();
    void _release_gpu_resources();



    xl7::graphics::meshes::VertexLayout _vertex_layout;
    xl7::graphics::meshes::VertexBuffer::Id _vertex_buffer_id = {};
    unsigned _vertex_buffer_capacity = 0;

    xl7::graphics::shaders::VertexShader::Id _vertex_shader_id = {};
    xl7::graphics::shaders::PixelShader::Id _pixel_shader_id = {};
    xl7::graphics::shaders::ConstantBuffer::Id _constant_buffer_id = {};
    xl7::graphics::states::BlendState::Id _blend_state_id = {};
    xl7::graphics::states::DepthStencilState::Id _depth_stencil_state_id = {};

    std::vector<Vertex> _vertices;

}; // class TestRenderer



} // namespace fl7::fonts::render

#endif // FL7_FONTS_RENDER_TESTRENDERER_H
