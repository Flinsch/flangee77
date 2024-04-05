#pragma once
#ifndef XL7_GRAPHICS_SURFACES_RENDERTARGET_H
#define XL7_GRAPHICS_SURFACES_RENDERTARGET_H
#include "./Surface.h"

#include "../Viewport.h"



namespace xl7 {
namespace graphics {
namespace surfaces {



class RenderTarget
    : public Surface
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderTarget(Type type, const CreateParams<Desc>& params);

    /**
     * Destructor.
     */
    virtual ~RenderTarget() = default;

private:
    /** Default constructor. */
    RenderTarget() = delete;
    /** Copy constructor. */
    RenderTarget(const RenderTarget&) = delete;
    /** Copy assignment operator. */
    RenderTarget& operator = (const RenderTarget&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
protected:
    /**
     * The default viewport based on the size of the render target.
     */
    const Viewport _default_viewport;

private:



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the default viewport based on the size of the render target.
     */
    const Viewport& get_default_viewport() const { return _default_viewport; }

public:



    // #############################################################################
    // Resource Implementations
    // #############################################################################
public:
    /**
     * Returns the specific type of the resource, as a "human-friendly" string.
     */
    virtual cl7::string_view get_type_string() const override { return TEXT("render target"); }

}; // class RenderTarget



} // namespace surfaces
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_SURFACES_RENDERTARGET_H
