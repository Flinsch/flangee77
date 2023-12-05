#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
#include "../../RenderingDevice.h"

#include "./prerequisites.h"



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
    /** The Direct3D 11 device interface. */
    wrl::ComPtr<ID3D11Device> _d3d_device;
    /** The Direct3D 11 device interface (1st extension). */
    wrl::ComPtr<ID3D11Device1> _d3d_device1;

    /** The (immediate) device context interface. */
    wrl::ComPtr<ID3D11DeviceContext> _d3d_immediate_context;
    /** The (immediate) device context interface (1st extension). */
    wrl::ComPtr<ID3D11DeviceContext1> _d3d_immediate_context1;

    /** The DXGI swap chain interface. */
    wrl::ComPtr<IDXGISwapChain> _dxgi_swap_chain;
    /** The DXGI swap chain interface (1st extension). */
    //wrl::ComPtr<IDXGISwapChain1> _dxgi_swap_chain1;

    /**
     * The set of features targeted by the Direct3D 11 device.
     */
    D3D_FEATURE_LEVEL _d3d_feature_level;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /** Returns the Direct3D 11 device interface. */
    ID3D11Device* get_raw_d3d_device() const { return _d3d_device.Get(); }
    /** Returns the Direct3D 11 device interface (1st extension). */
    ID3D11Device1* get_raw_d3d_device1() const { return _d3d_device1.Get(); }



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

}; // class RenderingDeviceImpl



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
