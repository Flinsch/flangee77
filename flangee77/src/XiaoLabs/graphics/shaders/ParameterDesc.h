#pragma once
#ifndef XL7_GRAPHICS_SHADERS_PARAMETERDESC_H
#define XL7_GRAPHICS_SHADERS_PARAMETERDESC_H

#include "./ParameterType.h"



namespace xl7 {
namespace graphics {
namespace shaders {



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



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_PARAMETERDESC_H
