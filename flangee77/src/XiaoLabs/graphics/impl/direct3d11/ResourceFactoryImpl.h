#ifndef XL7_GRAPHICS_IMPL_D3D11_RESOURCEFACTORYIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RESOURCEFACTORYIMPL_H
#include "../../IResourceFactory.h"



namespace xl7::graphics::impl::direct3d11 {



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

    xl7::graphics::textures::Texture2D* create_texture_2d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2D::Desc>& params) override;
    xl7::graphics::textures::Texture3D* create_texture_3d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture3D::Desc>& params) override;
    xl7::graphics::textures::Texture2DArray* create_texture_2d_array(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2DArray::Desc>& params) override;
    xl7::graphics::textures::Cubemap* create_cubemap(const resources::Resource::CreateParams<xl7::graphics::textures::Cubemap::Desc>& params) override;

    xl7::graphics::meshes::VertexBuffer* create_vertex_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::VertexBuffer::Desc>& params) override;
    xl7::graphics::meshes::IndexBuffer* create_index_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::IndexBuffer::Desc>& params) override;

    xl7::graphics::shaders::ConstantBuffer* create_constant_buffer(const resources::Resource::CreateParams<xl7::graphics::shaders::ConstantBuffer::Desc>& params) override;

    xl7::graphics::shaders::VertexShader* create_vertex_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::VertexShader::Desc>& params) override;
    xl7::graphics::shaders::PixelShader* create_pixel_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::VertexShader::Desc>& params) override;

    xl7::graphics::states::SamplerState* create_sampler_state(const resources::Resource::CreateParams<xl7::graphics::states::SamplerState::Desc>& params) override;
    xl7::graphics::states::RasterizerState* create_rasterizer_state(const resources::Resource::CreateParams<xl7::graphics::states::RasterizerState::Desc>& params) override;
    xl7::graphics::states::DepthStencilState* create_depth_stencil_state(const resources::Resource::CreateParams<xl7::graphics::states::DepthStencilState::Desc>& params) override;
    xl7::graphics::states::BlendState* create_blend_state(const resources::Resource::CreateParams<xl7::graphics::states::BlendState::Desc>& params) override;

}; // class ResourceFactoryImpl



} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_RESOURCEFACTORYIMPL_H
