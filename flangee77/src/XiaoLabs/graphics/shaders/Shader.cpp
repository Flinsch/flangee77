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
    bool Shader::recompile()
    {
        if ( !_check_managed() )
            return false;

        if ( !is_recompilable() )
        {
            LOG_ERROR( TEXT("The shader \"") + get_identifier() + TEXT("\" is not managed anymore.") );
            return false;
        }

        return _recompile_impl();
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
