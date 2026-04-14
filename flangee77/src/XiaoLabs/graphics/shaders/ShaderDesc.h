#ifndef XL7_GRAPHICS_SHADERS_SHADERDESC_H
#define XL7_GRAPHICS_SHADERS_SHADERDESC_H

#include "./ShaderCode.h"



namespace xl7::graphics::shaders {



struct ShaderDesc
{
    /** The language of the (source) code of the shader program. */
    ShaderCode::Language language;
    /** The name of the entry point of the shader program (can be empty, especially for precompiled shaders). */
    cl7::astring entry_point;
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADERDESC_H
