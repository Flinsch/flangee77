#ifndef XL7_GRAPHICS_SHADERS_CONSTANTMAPPING_H
#define XL7_GRAPHICS_SHADERS_CONSTANTMAPPING_H

#include "./ConstantType.h"



namespace xl7::graphics::shaders {



struct ConstantMapping
{

    /** The 0-based index of the constant buffer, as expected by the shader. */
    unsigned slot_index;

    /** The constant type (bool, int, float). */
    ConstantType constant_type;

    /** The to-be-mapped offset within the "source" constant buffer, in bytes. */
    unsigned source_offset;
    /** The mapped offset as anticipated by the shader, in bytes. */
    unsigned shader_offset;

    /** The actual total size of the constant, in bytes. */
    unsigned size;
    /** The padded total size of the constant, in bytes. */
    unsigned padded_size;

}; // struct ConstantMapping



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTMAPPING_H
