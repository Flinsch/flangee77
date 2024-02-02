#include "PixelShader.h"



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    PixelShader::PixelShader(const CreateParams<Desc>& params)
        : Shader( Type::PixelShader, params )
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
