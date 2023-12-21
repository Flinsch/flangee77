#pragma once
#ifndef XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H
#define XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H

#include "./VertexShader.h"
#include "./PixelShader.h"



namespace xl7 {
namespace graphics {
namespace shaders {



class ShaderManager;



class IShaderFactory
{
public:
    virtual ~IShaderFactory() = default;

    virtual xl7::graphics::shaders::VertexShader* create_vertex_shader(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier) = 0;
    virtual xl7::graphics::shaders::PixelShader* create_pixel_shader(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier) = 0;

}; // class IShaderFactory



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H
