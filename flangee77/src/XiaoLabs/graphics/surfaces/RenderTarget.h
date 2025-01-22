#ifndef XL7_GRAPHICS_SURFACES_RENDERTARGET_H
#define XL7_GRAPHICS_SURFACES_RENDERTARGET_H
#include "./Surface.h"

#include "../Viewport.h"



namespace xl7::graphics::surfaces {



class RenderTarget
    : public Surface
{

public:
    XL7_DERIVE_RESOURCE_ID(Surface);

    RenderTarget() = delete;

    RenderTarget(const RenderTarget&) = delete;
    RenderTarget& operator=(const RenderTarget&) = delete;
    RenderTarget(RenderTarget&&) = delete;
    RenderTarget& operator=(RenderTarget&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"render target"; }

    /**
     * Returns the default viewport based on the size of the render target.
     */
    const Viewport& get_default_viewport() const { return _default_viewport; }



protected:
    RenderTarget(Type type, const CreateParams<Desc>& params)
        : Surface(Type::ColorRenderTarget, params)
        , _default_viewport{.x=0, .y=0, .width=params.desc.width, .height=params.desc.height, .min_z=0.0f, .max_z=1.0f}
    {
    }

    ~RenderTarget() override = default;



private:
    /**
     * The default viewport based on the size of the render target.
     */
    const Viewport _default_viewport;

}; // class RenderTarget



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_RENDERTARGET_H
