#ifndef XL7_GRAPHICS_STATES_BLENDOPERATION_H
#define XL7_GRAPHICS_STATES_BLENDOPERATION_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Specifies the arithmetic operation used to combine source and destination colors during blending. */
enum struct BlendOperation
{
    /** Add the source and destination values. */
    Add = 1,
    /** Subtract the destination value from the source value. */
    Subtract,
    /** Subtract the source value from the destination value. */
    RevSubtract,
    /** Select the minimum of the source and destination values. */
    Min,
    /** Select the maximum of the source and destination values. */
    Max,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDOPERATION_H
