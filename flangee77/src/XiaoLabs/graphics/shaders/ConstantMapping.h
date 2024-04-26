#pragma once
#ifndef XL7_GRAPHICS_SHADERS_CONSTANTMAPPING_H
#define XL7_GRAPHICS_SHADERS_CONSTANTMAPPING_H

#include <CoreLabs/root.h>



namespace xl7 {
namespace graphics {
namespace shaders {



struct ConstantMapping
{

    /** The 0-based index of the constant buffer, as expected by the shader. */
    unsigned index;

    /** The to-be-mapped offset within the "source" constant buffer, in bytes. */
    unsigned source_offset;
    /** The mapped offset as anticipated by the shader, in bytes. */
    unsigned shader_offset;

    /** The actual total size of the constant, in bytes. */
    unsigned size;
    /** The padded total size of the constant, in bytes. */
    unsigned padded_size;

}; // struct ConstantMapping



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_CONSTANTMAPPING_H
