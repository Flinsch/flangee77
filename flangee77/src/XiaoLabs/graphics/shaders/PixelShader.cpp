#include "PixelShader.h"

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
    PixelShader::PixelShader(ShaderManager* manager)
        : Shader( manager )
    {
    }

    /**
     * Explicit constructor.
     */
    PixelShader::PixelShader(ShaderManager* manager, const cl7::string& identifier)
        : Shader( manager, identifier )
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7