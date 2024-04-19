#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTDECLARATION_H
#define XL7_GRAPHICS_SHADERS_CONSTANTDECLARATION_H

#include "./ConstantType.h"
#include "./ConstantClass.h"

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {
namespace shaders {



struct ConstantDeclaration
{

    /** The constant type (bool, int, float). */
    ConstantType constant_type;
    /** The constant class (scalar, vector, matrix). */
    ConstantClass constant_class;

    /** The name of the constant. */
    cl7::astring name;

    /** The offset within the constant buffer, in bytes. */
    unsigned offset;
    /** The (total) size of the constant, in bytes. */
    unsigned size;

    /** The number of rows. */
    unsigned row_count;
    /** The number of columns. */
    unsigned column_count;
    /** The number of (array) elements. */
    unsigned element_count;

}; // struct ConstantDeclaration



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTDECLARATION_H
