#include "Shader.h"

#include "./ShaderManager.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Shader::Shader(Type type, const CreateParams<Desc>& params)
        : Resource( params )
        , _type( type )
        , _desc( params.desc )
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * (Re)compiles the shader code. This tends to result in the resource having to
     * be completely recreated in the background.
     */
    bool Shader::recompile(const MacroDefinitions& macro_definitions)
    {
        if ( !_check_is_usable() )
            return false;

        if ( !is_recompilable() )
        {
            assert( is_precompiled() );
            LOG_ERROR( TEXT("The (precompiled) ") + get_typed_identifier_string() + TEXT(" is not recompilable.") );
            return false;
        }

        if ( !_recompile_impl( macro_definitions, _parameter_table ) )
        {
            LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be recompiled.") );
            return false;
        }

        return true;
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool Shader::_check_impl(const DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const CodeProvider&) );
        auto code_provider = static_cast<const CodeProvider&>( data_provider );

        if ( code_provider.shader_code.get_language() == ShaderCode::Language::Unknown )
        {
            LOG_ERROR( TEXT("The language of the provided code for ") + get_typed_identifier_string() + TEXT(" is unknown.") );
            return false;
        }

        if ( is_precompiled() && code_provider.shader_code.get_language() != ShaderCode::Language::Bytecode )
        {
            LOG_ERROR( TEXT("The provided code for the precompiled ") + get_typed_identifier_string() + TEXT(" has to be in bytecode.") );
            return false;
        }

        if ( is_recompilable() && code_provider.shader_code.get_language() != ShaderCode::Language::HighLevel )
        {
            LOG_ERROR( TEXT("The provided code for the recompilable ") + get_typed_identifier_string() + TEXT(" has to be in high-level language.") );
            return false;
        }

        if ( code_provider.shader_code.get_code_data().empty() )
        {
            LOG_ERROR( TEXT("The provided code for ") + get_typed_identifier_string() + TEXT(" is empty.") );
            return false;
        }

        return true;
    }

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     * The given data provider can possibly be ignored because the local data buffer
     * has already been filled based on it. It is still included in the event that
     * it contains additional implementation-specific information.
     */
    bool Shader::_acquire_impl(const DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const CodeProvider&) );
        auto code_provider = static_cast<const CodeProvider&>( data_provider );

        if ( is_precompiled() )
            return _acquire_precompiled_impl( code_provider, _parameter_table );
        if ( is_recompilable() )
            return _acquire_recompilable_impl( code_provider, _parameter_table );

        assert( false );
        return false;
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
