#include "CodeDataProvider.h"



namespace xl7::graphics::shaders {



    /**
     * Explicit constructor.
     */
    CodeDataProvider::CodeDataProvider(const ShaderCode* shader_code, const CompileOptions* compile_options)
        : resources::DefaultDataProvider(shader_code ? shader_code->get_code_data() : cl7::byte_view())
        , _shader_code(shader_code)
        , _compile_options(compile_options)
    {
        assert(_shader_code);
        assert(_compile_options);
    }



} // namespace xl7::graphics::shaders
