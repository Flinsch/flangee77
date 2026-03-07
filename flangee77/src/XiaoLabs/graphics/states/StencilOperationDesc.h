#ifndef XL7_GRAPHICS_STATES_STENCILOPERATIONDESC_H
#define XL7_GRAPHICS_STATES_STENCILOPERATIONDESC_H

#include "./StencilOperation.h"

#include "./ComparisonFunction.h"



namespace xl7::graphics::states {



struct StencilOperationDesc
{
    /** The function used for stencil testing, comparing the (masked) reference value against the (masked) existing stencil value. */
    ComparisonFunction stencil_test_function    = ComparisonFunction::Always;
    /** The stencil operation to perform when the stencil test fails. */
    StencilOperation stencil_fail_operation     = StencilOperation::Keep;
    /** The stencil operation to perform when the stencil test passes but the depth test fails. */
    StencilOperation depth_fail_operation       = StencilOperation::Keep;
    /** The stencil operation to perform when stencil test and depth test both pass. */
    StencilOperation pass_operation             = StencilOperation::Keep;
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_STENCILOPERATIONDESC_H
