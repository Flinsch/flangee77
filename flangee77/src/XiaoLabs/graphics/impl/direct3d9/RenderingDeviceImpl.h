#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
#include "../../RenderingDevice.h"

#include <d3d9.h>

#include <wrl/client.h>
namespace wrl = Microsoft::WRL;



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



class GraphicsSystemImpl;



class RenderingDeviceImpl final
    : public RenderingDevice
{
    friend GraphicsSystemImpl;



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
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device_interface;

    /**
     * The structure containing information identifying the adapter.
     */
    D3DADAPTER_IDENTIFIER9 _d3d_adapter_identifier;

    /**
     * The Direct3D 9 presentation parameters.
     */
    D3DPRESENT_PARAMETERS _d3d_present_parameters;

    /**
     * The capabilities of the underlying hardware.
     */
    D3DCAPS9 _d3d_device_caps;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 device interface.
     */
    IDirect3DDevice9* get_raw_d3d_device_interface() const { return _d3d_device_interface.Get(); }



    // #############################################################################
    // RenderingDevice Implementations
    // #############################################################################
private:
    /**
     * Initializes the component.
     */
    virtual bool _init_impl(MemoryInfo& memory_info) override;

    /**
     * De-initializes the component.
     */
    virtual bool _shutdown_impl() override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Tries to determine the available video memory composition.
     */
    bool _determine_video_memory(MemoryInfo& memory_info);

}; // class RenderingDeviceImpl



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
