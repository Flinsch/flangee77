#ifndef XL7_GRAPHICS_STATES_DEPTHSTENCILSTATEDESC_H
#define XL7_GRAPHICS_STATES_DEPTHSTENCILSTATEDESC_H

#include "./StencilOperationDesc.h"

#include "./ComparisonFunction.h"



namespace xl7::graphics::states {



struct DepthStencilStateDesc
{
    /** Enables (true) or disables (false) depth testing. */
    bool is_depth_testing_enabled               = true;
    /** Enables (true) or disables (false) depth writing. */
    bool is_depth_writing_enabled               = true;
    /** The function used for depth testing, comparing newly rendered depth data against exiting depth data. */
    ComparisonFunction depth_test_function      = ComparisonFunction::Less;

    /** Enables (true) or disables (false) stenciling. */
    bool is_stenciling_enabled                  = false;
    /** The bitmask applied to the reference value and each stencil value to determine the significant bits for the stencil test. */
    unsigned stencil_test_mask                  = 0xffffffff;
    /** The bitmask applied to values written into the stencil buffer. */
    unsigned stencil_write_mask                 = 0xffffffff;

    /** Identifies how to use the results of the depth/stencil tests for pixels whose surface normal is facing towards the camera. */
    StencilOperationDesc front_face_stenciling;
    /** Identifies how to use the results of the depth/stencil tests for pixels whose surface normal is facing away from the camera. */
    StencilOperationDesc back_face_stenciling;
};



} // namespace xl7::graphics::states

#endif // XL7_GRAPHICS_STATES_DEPTHSTENCILSTATEDESC_H
