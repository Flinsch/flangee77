#pragma once
#ifndef XL7_GRAPHICS_SHADERS_PARAMETERTYPE_H
#define XL7_GRAPHICS_SHADERS_PARAMETERTYPE_H

#include <CoreLabs/root.h>



namespace xl7 {
namespace graphics {
namespace shaders {



enum class ParameterType
{
    Invalid,
    Bool,
    Int,
    Float,
    Sampler,
};



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_PARAMETERTYPE_H
