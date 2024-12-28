#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTDECLARATION_H
#define XL7_GRAPHICS_SHADERS_CONSTANTDECLARATION_H

#include "./ConstantType.h"
#include "./ConstantClass.h"

#include <CoreLabs/string.h>



namespace xl7::graphics::shaders {



struct ConstantDeclaration
{

    /** The name of the constant. */
    cl7::u8string name;

    /** The constant type (bool, int, float). */
    ConstantType constant_type;
    /** The constant class (scalar, vector, matrix). */
    ConstantClass constant_class;

    /** The number of rows (1 if not matrix). */
    unsigned row_count;
    /** The number of columns (1 if scalar). */
    unsigned column_count;
    /** The number of (array) elements (1 if not array). */
    unsigned element_count;

    /** The offset within the constant buffer, in bytes. */
    unsigned offset;
    /** The actual total size of the constant, in bytes. */
    unsigned size;
    /** The padded total size of the constant, in bytes. It is wise not to set this value manually, but via ConstantBufferLayout::sort_and_adjust_padded_sizes. */
    unsigned padded_size;

}; // struct ConstantDeclaration



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTDECLARATION_H
