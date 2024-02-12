#pragma once
#ifndef XL7_GRAPHICS_SURFACES_DEPTHSTENCILTARGET_H
#define XL7_GRAPHICS_SURFACES_DEPTHSTENCILTARGET_H
#include "./Surface.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



class DepthStencilTarget
    : public Surface
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    DepthStencilTarget(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~DepthStencilTarget() = default;

private:
    /** Default constructor. */
    DepthStencilTarget() = delete;
    /** Copy constructor. */
    DepthStencilTarget(const DepthStencilTarget&) = delete;
    /** Copy assignment operator. */
    DepthStencilTarget& operator = (const DepthStencilTarget&) = delete;



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("depth/stencil buffer"); }

}; // class DepthStencilTarget



} // namespace surfaces
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_DEPTHSTENCILTARGET_H
