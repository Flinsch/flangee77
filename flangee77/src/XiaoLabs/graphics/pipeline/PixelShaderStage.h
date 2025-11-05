#ifndef XL7_GRAPHICS_PIPELINE_PIXELSHADERSTAGE_H
#define XL7_GRAPHICS_PIPELINE_PIXELSHADERSTAGE_H
#include "./AbstractShaderStage.h"

#include "../shaders/PixelShader.h"



namespace xl7::graphics::pipeline {



class PixelShaderStage
    : public AbstractShaderStage
{

    XL7_GRAPHICS_PIPELINE_SPECIFIC_STATE_PROXY(pixel_shader_id, shaders::PixelShader::Id, shader_id, xl7::resources::id_cast<shaders::PixelShader::Id>);

}; // class PixelShaderStage



} // namespace xl7::graphics::pipeline

#endif // XL7_GRAPHICS_PIPELINE_PIXELSHADERSTAGE_H
