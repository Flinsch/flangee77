#include "VertexShaderImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include "../../shared/shaders/D3DShaderCompiler.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexShaderImpl::VertexShaderImpl(const CreateParams<Desc>& params)
        : VertexShader( params )
        , _d3d_device( dynamic_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device() )
        , _d3d_vertex_shader()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Releases/"unacquires" the resource.
     */
    bool VertexShaderImpl::_release_impl()
    {
        _d3d_vertex_shader.Reset();

        return true;
    }



    // #############################################################################
    // Shader Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The actual code of the given code provider can possibly be ignored because the
     * local data buffer has already been filled based on it. It is still included as
     * it contains additional implementation-specific information.
     */
    bool VertexShaderImpl::_acquire_impl(const CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out)
    {
        assert( _d3d_device );
        assert( !_d3d_vertex_shader );

        const xl7::graphics::shaders::ShaderCode& shader_code = code_provider.shader_code;
        if ( shader_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::Unknown )
            return false;

        if ( shader_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::HighLevel )
            return _compile_impl( code_provider.macro_definitions, parameter_table_out );

        assert( shader_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::Bytecode );

        HRESULT hresult = _d3d_device->CreateVertexShader(
            reinterpret_cast<const DWORD*>( shader_code.get_code_data().data() ),
            &_d3d_vertex_shader );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateVertexShader") ) );
            return false;
        }

        return true;
    }

    /**
     * (Re)compiles the shader code. This tends to result in the resource having to
     * be completely recreated in the background.
     */
    bool VertexShaderImpl::_compile_impl(const xl7::graphics::shaders::MacroDefinitions& macro_definitions, xl7::graphics::shaders::ParameterTable& parameter_table_out)
    {
        const cl7::Version& version = GraphicsSystem::instance().get_rendering_device()->get_capabilities().shaders.vertex_shader_version;
        const cl7::astring target = "vs_" + cl7::to_astring(version.major) + "_" + cl7::to_astring(version.minor);
        const cl7::astring entry_point = _desc.entry_point.empty() ? "mainVertex" : _desc.entry_point;

        assert( is_recompilable() );
        const xl7::graphics::shaders::ShaderCode shader_code = shared::shaders::D3DShaderCompiler().compile_hlsl_code( xl7::graphics::shaders::ShaderCode( _desc.language, _data ), TEXT(""), macro_definitions, entry_point, target );
        if ( shader_code.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The vertex shader \"") + get_identifier() + TEXT("\" could not be compiled.") );
            return false;
        }

        _d3d_vertex_shader.Reset();

        CodeProvider code_provider( shader_code, macro_definitions );

        return _acquire_impl( code_provider, parameter_table_out );
    }



} // namespace shaders
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
