#ifndef XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
#define XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H

#include "../../../shaders/ShaderCode.h"
#include "../../../shaders/CompileOptions.h"

#include <CoreLabs/string.h>



namespace xl7::graphics::impl::shared::shaders {



struct D3DShaderCompiler
{

    /**
     * Compiles HLSL code from the specified file into bytecode for a given target.
     * The file path is also used to resolve any #include directives. If an error
     * occurs, an object with an "unknown" language and empty data is returned.
     */
    static xl7::graphics::shaders::ShaderCode compile_hlsl_code(const cl7::u8string& file_path, const xl7::graphics::shaders::CompileOptions& compile_options, const cl7::astring& entry_point, const cl7::astring& target);

    /**
     * Compiles the given HLSL code into bytecode for a given target. The include
     * path is used to resolve any #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    static xl7::graphics::shaders::ShaderCode compile_hlsl_code(const xl7::graphics::shaders::ShaderCode& hlsl_code, const cl7::u8string& include_path, const xl7::graphics::shaders::CompileOptions& compile_options, const cl7::astring& entry_point, const cl7::astring& target);

}; // struct D3DShaderCompiler



} // namespace xl7::graphics::impl::shared::shaders

#endif // XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
