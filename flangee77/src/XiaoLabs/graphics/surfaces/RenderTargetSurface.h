#ifndef XL7_GRAPHICS_SURFACES_RENDERTARGETSURFACE_H
#define XL7_GRAPHICS_SURFACES_RENDERTARGETSURFACE_H
#include "./FramebufferSurface.h"



namespace xl7::graphics::surfaces {



class RenderTargetSurface
    : public FramebufferSurface
{

public:
    XL7_DERIVE_RESOURCE_ID(FramebufferSurface);

    RenderTargetSurface() = delete;

    RenderTargetSurface(const RenderTargetSurface&) = delete;
    RenderTargetSurface& operator=(const RenderTargetSurface&) = delete;
    RenderTargetSurface(RenderTargetSurface&&) = delete;
    RenderTargetSurface& operator=(RenderTargetSurface&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"render target surface"; }



protected:
    RenderTargetSurface(const CreateParams<Desc>& params)
        : FramebufferSurface(Type::RenderTargetSurface, params)
    {
    }

    ~RenderTargetSurface() override = default;

}; // class RenderTargetSurface



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_RENDERTARGETSURFACE_H
