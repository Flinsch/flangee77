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



namespace xl7::graphics::impl::direct3d9 {



    xl7::graphics::textures::Texture2D* ResourceFactoryImpl::create_texture_2d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2DDesc>& params)
    {
        return textures::Texture2DImpl::Attorney::create(params);
    }

    xl7::graphics::textures::Texture3D* ResourceFactoryImpl::create_texture_3d(const resources::Resource::CreateParams<xl7::graphics::textures::Texture3DDesc>& params)
    {
        return nullptr;
    }

    xl7::graphics::textures::Texture2DArray* ResourceFactoryImpl::create_texture_2d_array(const resources::Resource::CreateParams<xl7::graphics::textures::Texture2DArrayDesc>& params)
    {
        return nullptr;
    }

    xl7::graphics::textures::Cubemap* ResourceFactoryImpl::create_cubemap(const resources::Resource::CreateParams<xl7::graphics::textures::CubemapDesc>& params)
    {
        return nullptr;
    }



    xl7::graphics::meshes::VertexBuffer* ResourceFactoryImpl::create_vertex_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::VertexBufferDesc>& params)
    {
        return meshes::VertexBufferImpl::Attorney::create(params);
    }

    xl7::graphics::meshes::IndexBuffer* ResourceFactoryImpl::create_index_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::IndexBufferDesc>& params)
    {
        return meshes::IndexBufferImpl::Attorney::create(params);
    }



    xl7::graphics::shaders::ConstantBuffer* ResourceFactoryImpl::create_constant_buffer(const resources::Resource::CreateParams<xl7::graphics::shaders::ConstantBufferDesc>& params)
    {
        return shaders::ConstantBufferImpl::Attorney::create(params);
    }



    xl7::graphics::shaders::VertexShader* ResourceFactoryImpl::create_vertex_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::ShaderDesc>& params)
    {
        return shaders::VertexShaderImpl::Attorney::create(params);
    }

    xl7::graphics::shaders::PixelShader* ResourceFactoryImpl::create_pixel_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::ShaderDesc>& params)
    {
        return shaders::PixelShaderImpl::Attorney::create(params);
    }



    xl7::graphics::states::SamplerState* ResourceFactoryImpl::create_sampler_state(const resources::Resource::CreateParams<xl7::graphics::states::SamplerStateDesc>& params)
    {
        return states::SamplerStateImpl::Attorney::create(params);
    }

    xl7::graphics::states::RasterizerState* ResourceFactoryImpl::create_rasterizer_state(const resources::Resource::CreateParams<xl7::graphics::states::RasterizerStateDesc>& params)
    {
        return states::RasterizerStateImpl::Attorney::create(params);
    }

    xl7::graphics::states::DepthStencilState* ResourceFactoryImpl::create_depth_stencil_state(const resources::Resource::CreateParams<xl7::graphics::states::DepthStencilStateDesc>& params)
    {
        return states::DepthStencilStateImpl::Attorney::create(params);
    }

    xl7::graphics::states::BlendState* ResourceFactoryImpl::create_blend_state(const resources::Resource::CreateParams<xl7::graphics::states::BlendStateDesc>& params)
    {
        return states::BlendStateImpl::Attorney::create(params);
    }



} // namespace xl7::graphics::impl::direct3d9
