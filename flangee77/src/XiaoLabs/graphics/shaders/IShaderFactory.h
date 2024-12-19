#pragma once
#ifndef XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H
#define XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H

#include "./ConstantBuffer.h"

#include "./VertexShader.h"
#include "./PixelShader.h"



namespace xl7::graphics::shaders {



class ShaderManager;



class IShaderFactory
{
public:
    virtual ~IShaderFactory() = default;

    virtual xl7::graphics::shaders::ConstantBuffer* create_constant_buffer(const resources::Resource::CreateParams<xl7::graphics::shaders::ConstantBuffer::Desc>& params) = 0;

    virtual xl7::graphics::shaders::VertexShader* create_vertex_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::VertexShader::Desc>& params) = 0;
    virtual xl7::graphics::shaders::PixelShader* create_pixel_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::PixelShader::Desc>& params) = 0;

}; // class IShaderFactory



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H
