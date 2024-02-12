#include "PixelShaderImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include "../../shared/shaders/D3DShaderCompiler.h"

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
    PixelShaderImpl::PixelShaderImpl(const CreateParams<Desc>& params)
        : PixelShader( params )
        , _d3d_device( static_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device() )
        , _d3d_pixel_shader()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Releases/"unacquires" the resource.
     * The resource may be in an incompletely acquired state when this function is
     * called. Any cleanup work that is necessary should still be carried out.
     */
    bool PixelShaderImpl::_release_impl()
    {
        _d3d_pixel_shader.Reset();

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
    bool PixelShaderImpl::_acquire_precompiled_impl(const xl7::graphics::shaders::CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out)
    {
        const xl7::graphics::shaders::ShaderCode& bytecode = code_provider.get_shader_code();
        assert( bytecode.get_language() == xl7::graphics::shaders::ShaderCode::Language::Bytecode );

        HRESULT hresult = _d3d_device->CreatePixelShader(
            bytecode.get_code_data().data(),
            bytecode.get_code_data().size(),
            nullptr,
            &_d3d_pixel_shader );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d11_result( hresult, TEXT("ID3D11Device::CreatePixelShader") ) );
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
    bool PixelShaderImpl::_acquire_recompilable_impl(const xl7::graphics::shaders::CodeProvider& code_provider, xl7::graphics::shaders::ParameterTable& parameter_table_out)
    {
        const cl7::Version& version = GraphicsSystem::instance().get_rendering_device()->get_capabilities().shaders.pixel_shader_version;
        const cl7::astring target = "ps_" + cl7::to_astring(version.major) + "_" + cl7::to_astring(version.minor);
        const cl7::astring entry_point = _desc.entry_point.empty() ? "mainPixel" : _desc.entry_point;

        const xl7::graphics::shaders::ShaderCode& hlsl_code = code_provider.get_shader_code();
        assert( hlsl_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::HighLevel );

        const xl7::graphics::shaders::ShaderCode bytecode = shared::shaders::D3DShaderCompiler().compile_hlsl_code( hlsl_code, TEXT(""), code_provider.get_macro_definitions(), entry_point, target );
        if ( bytecode.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be compiled.") );
            return false;
        }

        return _acquire_precompiled_impl( xl7::graphics::shaders::CodeProvider( &bytecode, &code_provider.get_macro_definitions() ), parameter_table_out );
    }

    /**
     * Recompiles the shader code. This tends to result in the resource having to be
     * completely recreated in the background.
     */
    bool PixelShaderImpl::_recompile_impl(const xl7::graphics::shaders::MacroDefinitions& macro_definitions, xl7::graphics::shaders::ParameterTable& parameter_table_out)
    {
        xl7::graphics::shaders::ShaderCode hlsl_code( _desc.language, _data );
        assert( hlsl_code.get_language() == xl7::graphics::shaders::ShaderCode::Language::HighLevel );

        return _acquire_recompilable_impl( xl7::graphics::shaders::CodeProvider( &hlsl_code, &macro_definitions ), parameter_table_out );
    }



} // namespace shaders
} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
