#pragma once
#ifndef XL7_GRAPHICS_SHADERS_PARAMETERTABLE_H
#define XL7_GRAPHICS_SHADERS_PARAMETERTABLE_H

#include <CoreLabs/string.h>

#include <map>



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

struct ParameterDesc
{
    /** The parameter type. */
    ParameterType type;

    /** The register index. */
    unsigned register_index;
    /** The number of registers occupied. */
    unsigned register_count;

    /** The number of rows. */
    unsigned row_count;
    /** The number of columns. */
    unsigned column_count;
    /** The number of array elements. */
    unsigned element_count;
};

typedef std::map<cl7::astring, ParameterDesc, std::less<>> ParameterTable;



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_PARAMETERTABLE_H
