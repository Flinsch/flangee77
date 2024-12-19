#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDECLARATION_H
#define XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDECLARATION_H

#include "./ConstantBufferLayout.h"

#include <CoreLabs/string.h>



namespace xl7::graphics::shaders {



struct ConstantBufferDeclaration
{

    /** The name of the constant buffer. */
    cl7::astring name;

    /** The 0-based index of the constant buffer. */
    unsigned index;

    /** The layout specification of the constant buffer. */
    ConstantBufferLayout layout;

}; // struct ConstantBufferDeclaration



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTBUFFERDECLARATION_H
