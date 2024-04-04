#pragma once
#ifndef XL7_GRAPHICS_STATES_DEPTHSTENCILSTATE_H
#define XL7_GRAPHICS_STATES_DEPTHSTENCILSTATE_H
#include "./AbstractState.h"

#include "../ComparisonFunction.h"



namespace xl7 {
namespace graphics {
namespace states {



class DepthStencilState
    : public AbstractState
{

public:
    

public:
    enum class StencilOperation
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

    struct Desc
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



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    DepthStencilState(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~DepthStencilState() = default;

private:
    /** Default constructor. */
    DepthStencilState() = delete;
    /** Copy constructor. */
    DepthStencilState(const DepthStencilState&) = delete;
    /** Copy assignment operator. */
    DepthStencilState& operator = (const DepthStencilState&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The descriptor of the sampler state.
     */
    const Desc _desc;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the descriptor of the sampler state.
     */
    const Desc& get_desc() const { return _desc; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("depth/stencil state"); }

}; // class DepthStencilState



} // namespace states
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_STATES_DEPTHSTENCILSTATE_H
