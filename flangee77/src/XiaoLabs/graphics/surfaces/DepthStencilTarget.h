#pragma once
#ifndef XL7_GRAPHICS_SURFACES_DEPTHSTENCILTARGET_H
#define XL7_GRAPHICS_SURFACES_DEPTHSTENCILTARGET_H
#include "./RenderTarget.h"



namespace xl7::graphics::surfaces {



class DepthStencilTarget // NOLINT(*-virtual-class-destructor)
    : public RenderTarget
{

public:
    DepthStencilTarget() = delete;

    DepthStencilTarget(const DepthStencilTarget&) = delete;
    DepthStencilTarget& operator = (const DepthStencilTarget&) = delete;
    DepthStencilTarget(DepthStencilTarget&&) = delete;
    DepthStencilTarget& operator = (DepthStencilTarget&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("depth/stencil buffer"); }



protected:
    DepthStencilTarget(const CreateParams<Desc>& params)
        : RenderTarget(Type::DepthStencilTarget, params)
    {
    }

    ~DepthStencilTarget() override = default;

}; // class DepthStencilTarget



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_DEPTHSTENCILTARGET_H
