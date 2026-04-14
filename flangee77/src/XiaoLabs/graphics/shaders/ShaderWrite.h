#ifndef XL7_GRAPHICS_SHADERS_SHADERWRITE_H
#define XL7_GRAPHICS_SHADERS_SHADERWRITE_H

#include "./ShaderCode.h"
#include "./CompileOptions.h"



namespace xl7::graphics::shaders {



struct ShaderWrite
{
    /** A reference to the shader code. */
    const ShaderCode* shader_code;
    /** A reference to the compile options. */
    const CompileOptions* compile_options;
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADERWRITE_H
