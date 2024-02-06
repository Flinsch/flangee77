#include "ShaderCode.h"

#include <CoreLabs/strings.h>



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

    /**
     * Explicit constructor.
     */
    ShaderCode::ShaderCode(const cl7::astring_view& high_level_code)
        : _language( Language::HighLevel )
        , _code_data( cl7::strings::to_bytes( high_level_code ) )
    {
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
