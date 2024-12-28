#pragma once
#ifndef XL7_GRAPHICS_SHADERS_TEXTURESAMPLERDECLARATION_H
#define XL7_GRAPHICS_SHADERS_TEXTURESAMPLERDECLARATION_H

#include <CoreLabs/string.h>



namespace xl7::graphics::shaders {



struct TextureSamplerDeclaration
{

    /** The name of the texture/sampler. */
    cl7::u8string name;

    /** The 0-based texture/sampler slot index. */
    unsigned index;

    /** The number of (array) elements. */
    unsigned element_count;

}; // struct TextureSamplerDeclaration



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_TEXTURESAMPLERDECLARATION_H
