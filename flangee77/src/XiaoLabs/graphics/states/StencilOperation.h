#ifndef XL7_GRAPHICS_STATES_STENCILOPERATION_H
#define XL7_GRAPHICS_STATES_STENCILOPERATION_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Specifies the stencil buffer operation to perform depending on the result of the stencil and depth tests. */
enum struct StencilOperation
{
    /** Keep the existing stencil value. */
    Keep = 1,
    /** Set the stencil value to 0. */
    Zero,
    /** Replace the stencil value with the reference value. */
    Replace,
    /** Increment the existing stencil value by 1 and clamp the result to the maximum value. */
    IncrClamp,
    /** Decrement the existing stencil value by 1 and clamp the result to 0. */
    DecrClamp,
    /** Invert the bits of the existing stencil value. */
    Invert,
    /** Increment the existing stencil value by 1 and wrap to 0 if exceeding the maximum value. */
    IncrWrap,
    /** Decrement the existing stencil value by 1 and wrap to the maximum value if falling below 0. */
    DecrWrap,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_STENCILOPERATION_H
