#include "VertexShader.h"



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexShader::VertexShader(const CreateParams<Desc>& params)
        : Shader( Type::VertexShader, params )
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
