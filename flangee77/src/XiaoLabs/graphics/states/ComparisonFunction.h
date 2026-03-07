#ifndef XL7_GRAPHICS_STATES_COMPARISONFUNCTION_H
#define XL7_GRAPHICS_STATES_COMPARISONFUNCTION_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



enum struct ComparisonFunction
{
    Never = 1,
    Less,
    Equal,
    LessEqual,
    Greater,
    NotEqual,
    GreaterEqual,
    Always,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_COMPARISONFUNCTION_H
