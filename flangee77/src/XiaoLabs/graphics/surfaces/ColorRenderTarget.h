#pragma once
#ifndef XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
#define XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
#include "./RenderTarget.h"



namespace xl7::graphics::surfaces {



class ColorRenderTarget // NOLINT(*-virtual-class-destructor)
    : public RenderTarget
{

public:
    ColorRenderTarget() = delete;

    ColorRenderTarget(const ColorRenderTarget&) = delete;
    ColorRenderTarget& operator = (const ColorRenderTarget&) = delete;
    ColorRenderTarget(ColorRenderTarget&&) = delete;
    ColorRenderTarget& operator = (ColorRenderTarget&&) = delete;



    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    cl7::string_view get_type_string() const override { return TEXT("color render target"); }



protected:
    ColorRenderTarget(const CreateParams<Desc>& params)
        : RenderTarget(Type::ColorRenderTarget, params)
    {
    }

    ~ColorRenderTarget() override = default;

}; // class ColorRenderTarget



} // namespace xl7::graphics::surfaces

#endif // XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
