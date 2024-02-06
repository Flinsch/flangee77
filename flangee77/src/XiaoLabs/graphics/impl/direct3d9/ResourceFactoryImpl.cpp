#include "ResourceFactoryImpl.h"

#include "./meshes/VertexBufferImpl.h"
#include "./meshes/IndexBufferImpl.h"

#include "./shaders/VertexShaderImpl.h"
#include "./shaders/PixelShaderImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



    xl7::graphics::meshes::VertexBuffer* ResourceFactoryImpl::create_vertex_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::VertexBuffer::Desc>& params)
    {
        return meshes::VertexBufferImpl::Attorney::create( params );
    }

    xl7::graphics::meshes::IndexBuffer* ResourceFactoryImpl::create_index_buffer(const resources::Resource::CreateParams<xl7::graphics::meshes::IndexBuffer::Desc>& params)
    {
        return meshes::IndexBufferImpl::Attorney::create( params );
    }



    xl7::graphics::shaders::VertexShader* ResourceFactoryImpl::create_vertex_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::VertexShader::Desc>& params)
    {
        return shaders::VertexShaderImpl::Attorney::create( params );
    }

    xl7::graphics::shaders::PixelShader* ResourceFactoryImpl::create_pixel_shader(const resources::Resource::CreateParams<xl7::graphics::shaders::VertexShader::Desc>& params)
    {
        return shaders::PixelShaderImpl::Attorney::create( params );
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
