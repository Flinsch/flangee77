#include "CodeDataProvider.h"



namespace xl7 {
namespace graphics {
namespace shaders {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    CodeDataProvider::CodeDataProvider(const ShaderCode* shader_code, const MacroDefinitions* macro_definitions)
        : resources::DefaultDataProvider( shader_code ? shader_code->get_code_data() : cl7::byte_view() )
        , _shader_code( *shader_code )
        , _macro_definitions( *macro_definitions )
    {
        assert( shader_code );
        assert( macro_definitions );
    }



} // namespace shaders
} // namespace graphics
} // namespace xl7
