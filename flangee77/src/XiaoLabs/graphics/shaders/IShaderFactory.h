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

    virtual ConstantBuffer* create_constant_buffer(const resources::Resource::CreateContext& ctx, const ConstantBufferDesc& desc) = 0;

    virtual VertexShader* create_vertex_shader(const resources::Resource::CreateContext& ctx, const ShaderDesc& desc) = 0;
    virtual PixelShader* create_pixel_shader(const resources::Resource::CreateContext& ctx, const ShaderDesc& desc) = 0;

}; // class IShaderFactory



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_ISHADERFACTORY_H
