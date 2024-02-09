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
    RenderingContextImpl(RenderingDevice* rendering_device, unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context, wrl::ComPtr<ID3D11RenderTargetView> d3d_render_target_view, wrl::ComPtr<ID3D11DepthStencilView> d3d_depth_stencil_view);

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

    /**
     * The Direct3D 11 (standard) render target view interface.
     */
    wrl::ComPtr<ID3D11RenderTargetView> _d3d_render_target_view;

    /**
     * The Direct3D 11 (standard) depth/stencil view interface.
     */
    wrl::ComPtr<ID3D11DepthStencilView> _d3d_depth_stencil_view;

private:
    D3D11_PRIMITIVE_TOPOLOGY _d3d_primitive_topology;



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



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
