#ifndef XL7_GRAPHICS_SHADERS_CONSTANTCLASS_H
#define XL7_GRAPHICS_SHADERS_CONSTANTCLASS_H

#include <CoreLabs/root.h>



namespace xl7::graphics::shaders {



enum struct ConstantClass
{
    /** The shader constant is a scalar. */
    Scalar = 0, // Have immediate compatibility with Direct3D.
    /** The shader constant is a vector. */
    Vector,
    /** The shader constant is a row-major matrix (consecutive elements of a row reside next to each other). */
    MatrixRows,
    /** The shader constant is a column-major matrix (consecutive elements of a column reside next to each other). */
    MatrixColumns,
};



} // namespace xl7::graphics::shaders

#endif // XL7_GRAPHICS_SHADERS_CONSTANTCLASS_H
