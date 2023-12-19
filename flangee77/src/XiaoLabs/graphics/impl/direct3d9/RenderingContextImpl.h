#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
#include "../../RenderingContext.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



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
    RenderingContextImpl(unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device);

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
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;



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



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
