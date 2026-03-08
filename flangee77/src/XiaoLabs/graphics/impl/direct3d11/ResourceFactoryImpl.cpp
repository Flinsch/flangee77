#include "ResourceFactoryImpl.h"

#include "./textures/Texture2DImpl.h"

#include "./meshes/VertexBufferImpl.h"
#include "./meshes/IndexBufferImpl.h"

#include "./shaders/ConstantBufferImpl.h"

#include "./shaders/VertexShaderImpl.h"
#include "./shaders/PixelShaderImpl.h"

#include "./states/SamplerStateImpl.h"
#include "./states/RasterizerStateImpl.h"
#include "./states/DepthStencilStateImpl.h"
#include "./states/BlendStateImpl.h"



namespace xl7::graphics::impl::direct3d11 {



    graphics::textures::Texture2D* ResourceFactoryImpl::create_texture_2d(const resources::Resource::CreateContext& ctx, const graphics::textures::Texture2DDesc& desc)
    {
        return textures::Texture2DImpl::Attorney::create(ctx, desc);
    }

    graphics::textures::Texture3D* ResourceFactoryImpl::create_texture_3d(const resources::Resource::CreateContext& ctx, const graphics::textures::Texture3DDesc& desc)
    {
        return nullptr;
    }

    graphics::textures::Texture2DArray* ResourceFactoryImpl::create_texture_2d_array(const resources::Resource::CreateContext& ctx, const graphics::textures::Texture2DArrayDesc& desc)
    {
        return nullptr;
    }

    graphics::textures::Cubemap* ResourceFactoryImpl::create_cubemap(const resources::Resource::CreateContext& ctx, const graphics::textures::CubemapDesc& desc)
    {
        return nullptr;
    }



    graphics::meshes::VertexBuffer* ResourceFactoryImpl::create_vertex_buffer(const resources::Resource::CreateContext& ctx, const graphics::meshes::VertexBufferDesc& desc)
    {
        return meshes::VertexBufferImpl::Attorney::create(ctx, desc);
    }

    graphics::meshes::IndexBuffer* ResourceFactoryImpl::create_index_buffer(const resources::Resource::CreateContext& ctx, const graphics::meshes::IndexBufferDesc& desc)
    {
        return meshes::IndexBufferImpl::Attorney::create(ctx, desc);
    }



    graphics::shaders::ConstantBuffer* ResourceFactoryImpl::create_constant_buffer(const resources::Resource::CreateContext& ctx, const graphics::shaders::ConstantBufferDesc& desc)
    {
        return shaders::ConstantBufferImpl::Attorney::create(ctx, desc);
    }



    graphics::shaders::VertexShader* ResourceFactoryImpl::create_vertex_shader(const resources::Resource::CreateContext& ctx, const graphics::shaders::ShaderDesc& desc)
    {
        return shaders::VertexShaderImpl::Attorney::create(ctx, desc);
    }

    graphics::shaders::PixelShader* ResourceFactoryImpl::create_pixel_shader(const resources::Resource::CreateContext& ctx, const graphics::shaders::ShaderDesc& desc)
    {
        return shaders::PixelShaderImpl::Attorney::create(ctx, desc);
    }



    graphics::states::SamplerState* ResourceFactoryImpl::create_sampler_state(const resources::Resource::CreateContext& ctx, const graphics::states::SamplerStateDesc& desc)
    {
        return states::SamplerStateImpl::Attorney::create(ctx, desc);
    }

    graphics::states::RasterizerState* ResourceFactoryImpl::create_rasterizer_state(const resources::Resource::CreateContext& ctx, const graphics::states::RasterizerStateDesc& desc)
    {
        return states::RasterizerStateImpl::Attorney::create(ctx, desc);
    }

    graphics::states::DepthStencilState* ResourceFactoryImpl::create_depth_stencil_state(const resources::Resource::CreateContext& ctx, const graphics::states::DepthStencilStateDesc& desc)
    {
        return states::DepthStencilStateImpl::Attorney::create(ctx, desc);
    }

    graphics::states::BlendState* ResourceFactoryImpl::create_blend_state(const resources::Resource::CreateContext& ctx, const graphics::states::BlendStateDesc& desc)
    {
        return states::BlendStateImpl::Attorney::create(ctx, desc);
    }



} // namespace xl7::graphics::impl::direct3d11
