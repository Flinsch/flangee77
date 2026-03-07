#ifndef XL7_GRAPHICS_STATES_COMPARISONFUNCTION_H
#define XL7_GRAPHICS_STATES_COMPARISONFUNCTION_H

#include <CoreLabs/root.h>



namespace xl7::graphics::states {



/** Defines the comparison function used for depth or stencil testing. */
enum struct ComparisonFunction
{
    /** The comparison always fails. */
    Never = 1,
    /** Pass if the source value is less than the destination value. */
    Less,
    /** Pass if the source value is equal to the destination value. */
    Equal,
    /** Pass if the source value is less than or equal to the destination value. */
    LessEqual,
    /** Pass if the source value is greater than the destination value. */
    Greater,
    /** Pass if the source value is not equal to the destination value. */
    NotEqual,
    /** Pass if the source value is greater than or equal to the destination value. */
    GreaterEqual,
    /** The comparison always passes. */
    Always,
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_COMPARISONFUNCTION_H
