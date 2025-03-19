#ifndef XL7_GRAPHICS_SURFACES_FRAMEBUFFERSURFACE_H
#define XL7_GRAPHICS_SURFACES_FRAMEBUFFERSURFACE_H
#include "./Surface.h"

#include "../Viewport.h"



namespace xl7::graphics::surfaces {



class FramebufferSurface
    : public Surface
{

public:
    XL7_DERIVE_RESOURCE_ID(Surface);

    FramebufferSurface() = delete;

    FramebufferSurface(const FramebufferSurface&) = delete;
    FramebufferSurface& operator=(const FramebufferSurface&) = delete;
    FramebufferSurface(FramebufferSurface&&) = delete;
    FramebufferSurface& operator=(FramebufferSurface&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"framebuffer surface"; }

    /**
     * Returns the default viewport based on the size of the framebuffer.
     */
    const Viewport& get_default_viewport() const { return _default_viewport; }



protected:
    FramebufferSurface(Type type, const CreateParams<Desc>& params)
        : Surface(Type::RenderTargetSurface, params)
        , _default_viewport{.x=0, .y=0, .width=params.desc.width, .height=params.desc.height, .min_z=0.0f, .max_z=1.0f}
    {
    }

    ~FramebufferSurface() override = default;



private:
    /**
     * The default viewport based on the size of the framebuffer.
     */
    const Viewport _default_viewport;

}; // class FramebufferSurface



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_FRAMEBUFFERSURFACE_H
