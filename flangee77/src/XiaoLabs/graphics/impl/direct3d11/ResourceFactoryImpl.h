#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_RESOURCEFACTORYIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RESOURCEFACTORYIMPL_H
#include "../../IResourceFactory.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



class ResourceFactoryImpl final
    : public IResourceFactory
{

public:
    class Attorney
    {
        static ResourceFactoryImpl* create() { return new ResourceFactoryImpl(); }
        friend class RenderingDeviceImpl;
    };

    virtual xl7::graphics::shaders::VertexShader* create_vertex_shader(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier);
    virtual xl7::graphics::shaders::PixelShader* create_pixel_shader(xl7::graphics::shaders::ShaderManager* manager, const cl7::string& identifier);

}; // class ResourceFactoryImpl



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_RESOURCEFACTORYIMPL_H
