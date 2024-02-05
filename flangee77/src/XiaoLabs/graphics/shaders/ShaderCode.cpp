#include "ShaderCode.h"



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    ShaderCode::ShaderCode(Language language, const cl7::byte_view& code_data)
        : _language( language )
        , _code_data( code_data.begin(), code_data.end() )
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
