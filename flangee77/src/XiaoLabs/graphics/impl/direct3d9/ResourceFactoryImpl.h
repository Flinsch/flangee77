#ifndef XL7_GRAPHICS_IMPL_D3D9_RESOURCEFACTORYIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RESOURCEFACTORYIMPL_H
#include "../../IResourceFactory.h"



namespace xl7::graphics::impl::direct3d9 {



class ResourceFactoryImpl final
    : public IResourceFactory
{

public:
    class Attorney
    {
        static ResourceFactoryImpl* create() { return new ResourceFactoryImpl(); }
        friend class RenderingDeviceImpl;
    };

    //xl7::graphics::surfaces::

    xl7::graphics::textures::Texture2D* create_texture_2d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2DDesc>& params) override;
    xl7::graphics::textures::Texture3D* create_texture_3d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture3DDesc>& params) override;
    xl7::graphics::textures::Texture2DArray* create_texture_2d_array(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2DArrayDesc>& params) override;
    xl7::graphics::textures::Cubemap* create_cubemap(const resources::Resource::CreateParams<xl7::graphics::textures::CubemapDesc>& params) override;

    xl7::graphics::meshes::VertexBuffer* create_vertex_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::VertexBufferDesc>& params) override;
    xl7::graphics::meshes::IndexBuffer* create_index_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::IndexBufferDesc>& params) override;

    xl7::graphics::shaders::ConstantBuffer* create_constant_buffer(const resources::Resource::CreateParams<xl7::graphics::shaders::ConstantBufferDesc>& params) override;

    xl7::graphics::shaders::VertexShader* create_vertex_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::ShaderDesc>& params) override;
    xl7::graphics::shaders::PixelShader* create_pixel_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::ShaderDesc>& params) override;

    xl7::graphics::states::SamplerState* create_sampler_state(const resources::Resource::CreateParams<xl7::graphics::states::SamplerStateDesc>& params) override;
    xl7::graphics::states::RasterizerState* create_rasterizer_state(const resources::Resource::CreateParams<xl7::graphics::states::RasterizerStateDesc>& params) override;
    xl7::graphics::states::DepthStencilState* create_depth_stencil_state(const resources::Resource::CreateParams<xl7::graphics::states::DepthStencilStateDesc>& params) override;
    xl7::graphics::states::BlendState* create_blend_state(const resources::Resource::CreateParams<xl7::graphics::states::BlendStateDesc>& params) override;

}; // class ResourceFactoryImpl



} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_RESOURCEFACTORYIMPL_H
