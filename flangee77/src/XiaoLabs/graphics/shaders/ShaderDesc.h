#ifndef XL7_GRAPHICS_SHADERS_SHADERDESC_H
#define XL7_GRAPHICS_SHADERS_SHADERDESC_H

#include "./ShaderCode.h"



namespace xl7::graphics::shaders {



struct ShaderDesc
{
    /** The language of the (source) code on which the shader is based. */
    ShaderCode::Language language;
    /** The name of the shader entry point (can be empty, especially for precompiled shaders). */
    cl7::astring entry_point;
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_SHADERDESC_H
