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

    //graphics::surfaces::

    graphics::textures::Texture2D* create_texture_2d(const resources::Resource::CreateContext& ctx, const graphics::textures::Texture2DDesc& desc) override;
    graphics::textures::Texture3D* create_texture_3d(const resources::Resource::CreateContext& ctx, const graphics::textures::Texture3DDesc& desc) override;
    graphics::textures::Texture2DArray* create_texture_2d_array(const resources::Resource::CreateContext& ctx, const graphics::textures::Texture2DArrayDesc& desc) override;
    graphics::textures::Cubemap* create_cubemap(const resources::Resource::CreateContext& ctx, const graphics::textures::CubemapDesc& desc) override;

    graphics::meshes::VertexBuffer* create_vertex_buffer(const resources::Resource::CreateContext& ctx, const graphics::meshes::VertexBufferDesc& desc) override;
    graphics::meshes::IndexBuffer* create_index_buffer(const resources::Resource::CreateContext& ctx, const graphics::meshes::IndexBufferDesc& desc) override;

    graphics::shaders::ConstantBuffer* create_constant_buffer(const resources::Resource::CreateContext& ctx, const graphics::shaders::ConstantBufferDesc& desc) override;

    graphics::shaders::VertexShader* create_vertex_shader(const resources::Resource::CreateContext& ctx, const graphics::shaders::ShaderDesc& desc) override;
    graphics::shaders::PixelShader* create_pixel_shader(const resources::Resource::CreateContext& ctx, const graphics::shaders::ShaderDesc& desc) override;

    graphics::states::SamplerState* create_sampler_state(const resources::Resource::CreateContext& ctx, const graphics::states::SamplerStateDesc& desc) override;
    graphics::states::RasterizerState* create_rasterizer_state(const resources::Resource::CreateContext& ctx, const graphics::states::RasterizerStateDesc& desc) override;
    graphics::states::DepthStencilState* create_depth_stencil_state(const resources::Resource::CreateContext& ctx, const graphics::states::DepthStencilStateDesc& desc) override;
    graphics::states::BlendState* create_blend_state(const resources::Resource::CreateContext& ctx, const graphics::states::BlendStateDesc& desc) override;

}; // class ResourceFactoryImpl



} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_RESOURCEFACTORYIMPL_H
