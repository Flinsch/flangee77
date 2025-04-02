#ifndef XL7_GRAPHICS_SHADERS_CONSTANTTYPE_H
#define XL7_GRAPHICS_SHADERS_CONSTANTTYPE_H

#include <CoreLabs/root.h>



namespace xl7::graphics::shaders {



enum struct ConstantType
{
    /** The constant is a Boolean. */
    Bool = 1, // Increase immediate compatibility with Direct3D.
    /** The constant is an integer. */
    Int,
    /** The constant is a floating-point number. */
    Float,
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTTYPE_H
