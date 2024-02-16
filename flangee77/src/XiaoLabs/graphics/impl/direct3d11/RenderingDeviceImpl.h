#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
#include "../../RenderingDevice.h"

#include "./prerequisites.h"

#include "../shared/meshes/VertexBufferBinding.h"
#include "../shared/meshes/ComposedVertexLayout.h"

#include <unordered_map>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



class RenderingDeviceImpl final
    : public RenderingDevice
{
    friend class GraphicsSystemImpl;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Default constructor.
     */
    RenderingDeviceImpl();

    /**
     * Destructor.
     */
    virtual ~RenderingDeviceImpl() = default;

private:
    /** Copy constructor. */
    RenderingDeviceImpl(const RenderingDeviceImpl&) = delete;
    /** Copy assignment operator. */
    RenderingDeviceImpl& operator = (const RenderingDeviceImpl&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The Direct3D 11 device interface.
     */
    wrl::ComPtr<ID3D11DeviceN> _d3d_device;

    /**
     * The Direct3D 11 (immediate) device context interface.
     */
    wrl::ComPtr<ID3D11DeviceContextN> _d3d_immediate_context;

    /**
     * The Direct3D 11 (standard) render target view interface.
     */
    wrl::ComPtr<ID3D11RenderTargetView> _d3d_render_target_view;

    /**
     * The Direct3D 11 (standard) depth/stencil view interface.
     */
    wrl::ComPtr<ID3D11DepthStencilView> _d3d_depth_stencil_view;

    /**
     * The DXGI swap chain interface.
     */
    wrl::ComPtr<IDXGISwapChainN> _dxgi_swap_chain;

    /**
     * The set of features targeted by the Direct3D 11 device.
     */
    D3D_FEATURE_LEVEL _d3d_feature_level;

private:
    std::unordered_map<shared::meshes::VertexBufferBinding, wrl::ComPtr<ID3D11InputLayout>> _d3d_input_layouts_by_binding;
    std::unordered_map<shared::meshes::ComposedVertexLayout, wrl::ComPtr<ID3D11InputLayout>> _d3d_input_layouts_by_layout;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 11 device interface.
     */
    ID3D11DeviceN* get_raw_d3d_device() const { return _d3d_device.Get(); }



    // #############################################################################
    // RenderingDevice Implementations
    // #############################################################################
private:
    /**
     * Initializes the rendering device and determines the capabilities (as far as
     * they can be determined).
     */
    virtual bool _init_impl(Capabilities& capabilities) override;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown_impl() override;

private:
    /**
     * Creates a new rendering context with the specified index (0: primary context).
     * Returns NULL if the rendering context could not be created.
     */
    virtual RenderingContext* _create_rendering_context_impl(unsigned index) override;

private:
    /**
     * Checks whether the device is lost. If so, true is returned.
     */
    virtual bool _check_device_lost_impl() override;

    /**
     * Tries to reset/reinitialize the device after it has been lost. If the device
     * has been restored to an operational state, true is returned.
     */
    virtual bool _handle_device_lost_impl() override;

private:
    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    virtual bool _present_impl() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:

public:
    /**
     * Tries to find a suitable Direct3D 11 input layout based on the currently set
     * vertex buffer(s).
     */
    ID3D11InputLayout* _find_d3d_input_layout(const shared::meshes::VertexBufferBinding& vertex_buffer_binding);

    /**
     * Tries to find a suitable Direct3D 11 input layout, and otherwise creates a
     * new one, both based on the vertex layout(s) of the currently set vertex
     * buffer(s).
     */
    ID3D11InputLayout* _find_or_create_d3d_input_layout(const shared::meshes::VertexBufferBinding& vertex_buffer_binding, const void* shader_bytecode_with_input_signature, size_t bytecode_length);

}; // class RenderingDeviceImpl



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
