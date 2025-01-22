#ifndef XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
#define XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
#include "./RenderTarget.h"



namespace xl7::graphics::surfaces {



class ColorRenderTarget
    : public RenderTarget
{

public:
    XL7_DERIVE_RESOURCE_ID(RenderTarget);

    ColorRenderTarget() = delete;

    ColorRenderTarget(const ColorRenderTarget&) = delete;
    ColorRenderTarget& operator=(const ColorRenderTarget&) = delete;
    ColorRenderTarget(ColorRenderTarget&&) = delete;
    ColorRenderTarget& operator=(ColorRenderTarget&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::u8string_view get_type_string() const override { return u8"color render target"; }



protected:
    ColorRenderTarget(const CreateParams<Desc>& params)
        : RenderTarget(Type::ColorRenderTarget, params)
    {
    }

    ~ColorRenderTarget() override = default;

}; // class ColorRenderTarget



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
