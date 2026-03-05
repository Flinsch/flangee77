#ifndef XL7_GRAPHICS_STATES_BLENDOPERATION_H
#define XL7_GRAPHICS_STATES_BLENDOPERATION_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



enum struct BlendOperation
{
    Add = 1,
    Subtract,
    RevSubtract,
    Min,
    Max,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_BLENDOPERATION_H
