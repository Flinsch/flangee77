#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDECLARATION_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDECLARATION_H

#include "./ConstantTable.h"

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {
namespace shaders {



struct ConstantBufferDeclaration
{

    /** The name of the constant buffer. */
    cl7::astring name;

    /** The 0-based index of the constant buffer. */
    unsigned index;

    /** The table of constant declarations. */
    ConstantTable constant_table;



    /** Calculates the total size of the constant buffer, in bytes. */
    unsigned calculate_size() const;

}; // struct ConstantBufferDeclaration



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDECLARATION_H
