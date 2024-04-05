#pragma once
#ifndef XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
#define XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
#include "./RenderTarget.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



class ColorRenderTarget
    : public RenderTarget
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    ColorRenderTarget(const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~ColorRenderTarget() = default;

private:
    /** Default constructor. */
    ColorRenderTarget() = delete;
    /** Copy constructor. */
    ColorRenderTarget(const ColorRenderTarget&) = delete;
    /** Copy assignment operator. */
    ColorRenderTarget& operator = (const ColorRenderTarget&) = delete;



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("color render target"); }

}; // class ColorRenderTarget



} // namespace surfaces
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_COLORRENDERTARGET_H
