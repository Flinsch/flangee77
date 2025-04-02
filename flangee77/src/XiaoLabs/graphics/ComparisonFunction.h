#ifndef XL7_GRAPHICS_COMPARISONFUNCTION_H
#define XL7_GRAPHICS_COMPARISONFUNCTION_H

#include <CoreLabs/root.h>



namespace xl7::graphics {



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



} // namespace xl7::graphics

#endif // XL7_GRAPHICS_COMPARISONFUNCTION_H
