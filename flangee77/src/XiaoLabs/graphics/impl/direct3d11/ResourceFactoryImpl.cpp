#include "ResourceFactoryImpl.h"

#include "./shaders/VertexShaderImpl.h"
#include "./shaders/PixelShaderImpl.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    xl7::graphics::shaders::VertexShader* ResourceFactoryImpl::create_vertex_shader(xl7::graphics::shaders::ShaderManager* manager)
    {
        return shaders::VertexShaderImpl::Attorney::create( manager );
    }

    xl7::graphics::shaders::PixelShader* ResourceFactoryImpl::create_pixel_shader(xl7::graphics::shaders::ShaderManager* manager)
    {
        return shaders::PixelShaderImpl::Attorney::create( manager );
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
