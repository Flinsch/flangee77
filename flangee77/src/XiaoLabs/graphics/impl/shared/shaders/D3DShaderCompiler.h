#pragma once
#ifndef XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
#define XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H

#include "../../../shaders/ShaderCode.h"
#include "../../../shaders/MacroDefinitions.h"

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace shared {
namespace shaders {



class D3DShaderCompiler
{

    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Compiles HLSL code from the specified file into bytecode for a given target.
     * The file path is also used to resolve any #include directives. If an error
     * occurs, an object with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode compile_hlsl_code(const cl7::string& file_path, const xl7::graphics::shaders::MacroDefinitions& macro_definitions, const cl7::astring& entry_point, const cl7::astring& target);

    /**
     * Compiles the given HLSL code into bytecode for a given target. The include
     * path is used to resolve any #include directives. If an error occurs, an
     * object with an "unknown" language and empty data is returned.
     */
    xl7::graphics::shaders::ShaderCode compile_hlsl_code(const xl7::graphics::shaders::ShaderCode& hlsl_code, const cl7::string& include_path, const xl7::graphics::shaders::MacroDefinitions& macro_definitions, const cl7::astring& entry_point, const cl7::astring& target);

}; // class D3DShaderCompiler



} // namespace shaders
} // namespace shared
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_SHARED_SHADERS_D3DSHADERCOMPILER_H
