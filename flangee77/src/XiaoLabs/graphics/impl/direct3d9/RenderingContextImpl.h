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
    RenderingContextImpl(RenderingDevice* rendering_device, unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device, wrl::ComPtr<IDirect3DSurface9> d3d_render_target_surface, wrl::ComPtr<IDirect3DSurface9> d3d_depth_stencil_surface);

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

    /**
     * The (standard) Direct3D 9 render target surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_render_target_surface;

    /**
     * The (standard) Direct3D 9 depth/stencil surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_depth_stencil_surface;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 device interface.
     */
    IDirect3DDevice9* get_raw_d3d_device() const { return _d3d_device.Get(); }



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

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    virtual bool _draw_impl(const DrawStates& draw_states, unsigned primitive_count, unsigned start_vertex) override;

    /**
     * Draws indexed, non-instanced primitives.
     */
    virtual bool _draw_indexed_impl(const DrawStates& draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Transfers the current states to the device if necessary.
     */
    bool _flush_states(const DrawStates& draw_states);

}; // class RenderingContextImpl



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
