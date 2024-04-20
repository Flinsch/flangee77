#include "Shader.h"

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

        if ( !_recompile_impl( macro_definitions, _bytecode ) )
        {
            LOG_ERROR( TEXT("The ") + get_typed_identifier_string() + TEXT(" could not be recompiled.") );
            return false;
        }

        return _reflect_impl( _bytecode, _reflection_result ) && _validate_reflection_result( _reflection_result );
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Checks whether the given data provider complies with the specific properties
     * of the resource to (re)populate it, taking into account the current state of
     * the resource if necessary.
     */
    bool Shader::_check_data_impl(const resources::DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const CodeDataProvider&) );
        auto code_data_provider = static_cast<const CodeDataProvider&>( data_provider );

        if ( code_data_provider.get_language() == ShaderCode::Language::Unknown )
        {
            LOG_ERROR( TEXT("The language of the provided code for ") + get_typed_identifier_string() + TEXT(" is unknown.") );
            return false;
        }

        if ( is_precompiled() && code_data_provider.get_language() != ShaderCode::Language::Bytecode )
        {
            LOG_ERROR( TEXT("The provided code for the precompiled ") + get_typed_identifier_string() + TEXT(" has to be in bytecode.") );
            return false;
        }

        if ( is_recompilable() && code_data_provider.get_language() != ShaderCode::Language::HighLevel )
        {
            LOG_ERROR( TEXT("The provided code for the recompilable ") + get_typed_identifier_string() + TEXT(" has to be in high-level language.") );
            return false;
        }

        if ( code_data_provider.get_code_data().empty() )
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
    bool Shader::_acquire_impl(const resources::DataProvider& data_provider)
    {
        assert( typeid(data_provider) == typeid(const CodeDataProvider&) );
        auto code_data_provider = static_cast<const CodeDataProvider&>( data_provider );

        if ( is_precompiled() )
        {
            assert( code_data_provider.get_shader_code().get_language() == ShaderCode::Language::Bytecode );
            _bytecode = code_data_provider.get_shader_code();

            return _acquire_precompiled_impl( code_data_provider ) &&
                _reflect_impl( _bytecode, _reflection_result ) && _validate_reflection_result( _reflection_result );
        }
        if ( is_recompilable() )
        {
            return _acquire_recompilable_impl( code_data_provider, _bytecode ) &&
                _reflect_impl( _bytecode, _reflection_result ) && _validate_reflection_result( _reflection_result );
        }

        assert( false );
        return false;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * 
     */
    bool Shader::_validate_reflection_result(const ReflectionResult& reflection_result) const
    {
        for ( const auto& constant_buffer_declaration : reflection_result.constant_buffer_declarations )
        {
            const auto& constant_declarations = constant_buffer_declaration.constant_declarations;

            for ( const auto& constant_declaration : constant_declarations )
            {
                const bool is_scalar = constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::Scalar;
                const bool is_vector = constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::Vector;
                const bool is_matrix = constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::MatrixRows || constant_declaration.constant_class == xl7::graphics::shaders::ConstantClass::MatrixColumns;

                assert( (constant_declaration.row_count == 1 && !is_matrix) || (constant_declaration.row_count > 1 && is_matrix) );
                assert( (constant_declaration.column_count == 1 && is_scalar) || (constant_declaration.column_count > 1 && !is_scalar) );
                assert( constant_declaration.element_count >= 1 );
            } // for each constant declaration
        } // for each constant buffer declaration

        for ( const auto& texture_sampler_declaration : reflection_result.texture_sampler_declarations )
        {
        } // for each texture/sampler declaration

        return true;
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
