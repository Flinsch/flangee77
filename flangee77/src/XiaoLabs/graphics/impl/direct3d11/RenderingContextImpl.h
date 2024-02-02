#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
#include "../../RenderingContext.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



class RenderingContextImpl final
    : public RenderingContext
{
    friend class RenderingDeviceImpl;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingContextImpl(unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context);

    /**
     * Destructor.
     */
    virtual ~RenderingContextImpl() = default;

private:
    /** Default constructor. */
    RenderingContextImpl();
    /** Copy constructor. */
    RenderingContextImpl(const RenderingContextImpl&) = delete;
    /** Copy assignment operator. */
    RenderingContextImpl& operator = (const RenderingContextImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 device context interface.
     */
    wrl::ComPtr<ID3D11DeviceContextN> _d3d_device_context;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 11 device context interface.
     */
    ID3D11DeviceContextN* get_raw_d3d_device_context() const { return _d3d_device_context.Get(); }



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################
private:
    /**
     * Begins a scene.
     */
    virtual bool _begin_scene_impl() override;

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    virtual bool _end_scene_impl() override;

}; // class RenderingContextImpl



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
