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
    * Recompiles the shader code. This tends to result in the resource having to be
    * completely recreated in the background.
    */
    bool Shader::recompile(const MacroDefinitions& macro_definitions)
    {
        if ( !_check_managed() )
            return false;

        if ( !is_recompilable() )
        {
            assert( is_precompiled() );
            LOG_ERROR( TEXT("The (precompiled) shader \"") + get_identifier() + TEXT("\" is not recompilable.") );
            return false;
        }

        return _recompile_impl( macro_definitions, _parameter_table );
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
