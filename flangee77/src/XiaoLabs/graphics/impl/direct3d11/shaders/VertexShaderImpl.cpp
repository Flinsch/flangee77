#include "VertexShaderImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include "../../shared/shaders/D3DShaderCompiler.h"

#include "./D3DShaderReflection.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexShaderImpl::VertexShaderImpl(const CreateParams<Desc>& params)
        : VertexShader( params )
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Disposes/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool VertexShaderImpl::_dispose_impl()
    {
        static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->_release_d3d_constant_buffers( get_id() );

        _d3d_vertex_shader.Reset();

        return true;
    }



    // #############################################################################
    // Shader Implementations
    // #############################################################################

    /**
     * Requests/acquires a precompiled shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    bool VertexShaderImpl::_acquire_precompiled_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider)
    {
        auto d3d_device = static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device();
        assert( d3d_device );

        const xl7::graphics::shaders::ShaderCode& bytecode = code_data_provider.get_shader_code();
        assert( bytecode.get_language() == xl7::graphics::shaders::ShaderCode::Language::Bytecode );

        HRESULT hresult = d3d_device->CreateVertexShader(
            bytecode.get_code_data().data(),
            bytecode.get_code_data().size(),
            nullptr,
            &_d3d_vertex_shader );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreateVertexShader") ) );
            return false;
        }

        return true;
    }

    /**
     * Requests/acquires a recompilable shader resource.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    bool VertexShaderImpl::_acquire_recompilable_impl(const xl7::graphics::shaders::CodeDataProvider& code_data_provider, xl7::graphics::shaders::ShaderCode& bytecode_out)
    {
        const cl7::Version& version = GraphicsSystem::instance().get_rendering_device()->get_capabilities().shaders.vertex_shader_version;
        const cl7::astring target = "vs_" + cl7::to_astring(version.major) + "_" + cl7::to_astring(version.minor);
        const cl7::astring entry_point = _cascade_entry_point( code_data_provider.get_compile_options() );

        const xl7::graphics::shaders::ShaderCode& hlsl_code = code_data_provider.get_shader_code();
        assert( hlsl_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::HighLevel );

        bytecode_out = shared::shaders::D3DShaderCompiler().compile_hlsl_code( hlsl_code, TEXT(""), code_data_provider.get_compile_options(), entry_point, target );
        if ( bytecode_out.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be compiled.") );
            return false;
        }

        return _acquire_precompiled_impl( xl7::graphics::shaders::CodeDataProvider( &bytecode_out, &code_data_provider.get_compile_options() ) );
    }

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    bool VertexShaderImpl::_recompile_impl(const xl7::graphics::shaders::CompileOptions& compile_options, xl7::graphics::shaders::ShaderCode& bytecode_out)
    {
        static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->_release_d3d_constant_buffers( get_id() );

        xl7::graphics::shaders::ShaderCode hlsl_code( _desc.language, get_data() );
        assert( hlsl_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::HighLevel );

        return _acquire_recompilable_impl( xl7::graphics::shaders::CodeDataProvider( &hlsl_code, &compile_options ), bytecode_out );
    }

    /**
     * Performs a "reflection" on the (compiled) shader bytecode to determine
     * parameter declarations etc.
     */
    bool VertexShaderImpl::_reflect_impl(const xl7::graphics::shaders::ShaderCode& bytecode, xl7::graphics::shaders::ReflectionResult& reflection_result_out)
    {
        return D3DShaderReflection().reflect( bytecode, reflection_result_out );
    }



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
