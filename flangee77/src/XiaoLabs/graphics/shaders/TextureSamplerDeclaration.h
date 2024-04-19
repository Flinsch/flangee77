#pragma once
#ifndef XL7_GRAPHICS_SHADERS_TEXTURESAMPLERDECLARATION_H
#define XL7_GRAPHICS_SHADERS_TEXTURESAMPLERDECLARATION_H

#include <CoreLabs/string.h>



namespace xl7 {
namespace graphics {
namespace shaders {



struct TextureSamplerDeclaration
{

    /** The name of the texture/sampler. */
    cl7::astring name;

    /** The 0-based texture/sampler slot index. */
    unsigned index;

    /** The number of (array) elements. */
    unsigned element_count;

}; // struct TextureSamplerDeclaration



} // namespace shaders
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SHADERS_TEXTURESAMPLERDECLARATION_H
