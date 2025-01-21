#ifndef XL7_GRAPHICS_PIPELINE_VERTEXSHADERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_VERTEXSHADERSTAGE_H
#include "./AbstractShaderStage.h"

#include "../shaders/VertexShader.h"



namespace xl7::graphics::pipeline {



class VertexShaderStage
    : public AbstractShaderStage
{

    XL7_GRAPHICS_PIPELINE_SPECIFIC_STATE_PROXY(vertex_shader_id, shaders::VertexShader::ID, shader_id, xl7::resources::id_cast<shaders::VertexShader::ID>);

}; // class VertexShaderStage



} // namespace xl7::graphics::pipeline

#endif // XL7_GRAPHICS_PIPELINE_VERTEXSHADERSTAGE_H
