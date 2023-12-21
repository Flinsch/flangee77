#include "Shader.h"

#include "./ShaderManager.h"



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Shader::Shader(ShaderManager* manager, const cl7::string& identifier)
        : Resource( manager, identifier )
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
