#ifndef XL7_GRAPHICS_SURFACES_DEPTHSTENCILSURFACE_H
#define XL7_GRAPHICS_SURFACES_DEPTHSTENCILSURFACE_H
#include "./FramebufferSurface.h"



namespace xl7::graphics::surfaces {



class DepthStencilSurface
    : public FramebufferSurface
{

public:
    XL7_DERIVE_RESOURCE_ID(FramebufferSurface);

    DepthStencilSurface() = delete;

    DepthStencilSurface(const DepthStencilSurface&) = delete;
    DepthStencilSurface& operator=(const DepthStencilSurface&) = delete;
    DepthStencilSurface(DepthStencilSurface&&) = delete;
    DepthStencilSurface& operator=(DepthStencilSurface&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"depth/stencil surface"; }



protected:
    DepthStencilSurface(const CreateParams<Desc>& params)
        : FramebufferSurface(Type::DepthStencilSurface, params)
    {
    }

    ~DepthStencilSurface() override = default;

}; // class DepthStencilSurface



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_DEPTHSTENCILSURFACE_H
