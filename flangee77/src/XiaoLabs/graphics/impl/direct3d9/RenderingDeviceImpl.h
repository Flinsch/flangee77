#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
#include "../../RenderingDevice.h"

#include "./prerequisites.h"



namespace xl7::graphics::impl::direct3d9 {



class RenderingDeviceImpl final
    : public RenderingDevice
{
    friend class GraphicsSystemImpl;

public:
    RenderingDeviceImpl(const RenderingDeviceImpl&) = delete;
    RenderingDeviceImpl& operator=(const RenderingDeviceImpl&) = delete;
    RenderingDeviceImpl(RenderingDeviceImpl&&) = delete;
    RenderingDeviceImpl& operator=(RenderingDeviceImpl&&) = delete;



    /**
     * Returns the Direct3D 9 device interface.
     */
    IDirect3DDevice9* get_raw_d3d_device() const { return _d3d_device.Get(); }

    /**
     * Returns the structure containing information identifying the adapter.
     */
    const D3DADAPTER_IDENTIFIER9& get_d3d_adapter_identifier() const { return _d3d_adapter_identifier; }

    /**
     * Returns the Direct3D 9 presentation parameters.
     */
    const D3DPRESENT_PARAMETERS& get_d3d_present_parameters() const { return _d3d_present_parameters; }

    /**
     * Returns the capabilities of the underlying hardware.
     */
    const D3DCAPS9& get_d3d_device_caps() const { return _d3d_device_caps; }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RenderingDeviceImpl();
    ~RenderingDeviceImpl() override = default;



private:

    // #############################################################################
    // RenderingDevice Implementations
    // #############################################################################

    /**
     * Initializes the rendering device and determines the capabilities (as far as
     * they can be determined).
     */
    bool _init_impl(Capabilities& capabilities) override;

    /**
     * De-initializes the component.
     */
    bool _shutdown_impl() override;

    /**
     * Creates a new rendering context with the specified index (0: primary context).
     * Returns NULL if the rendering context could not be created.
     */
    RenderingContext* _create_rendering_context_impl(unsigned index) override;

    /**
     * Checks whether the device is lost. If so, true is returned.
     */
    bool _check_device_lost_impl() override;

    /**
     * Tries to reset/reinitialize the device after it has been lost. If the device
     * has been restored to an operational state, true is returned.
     */
    bool _handle_device_lost_impl() override;

    /**
     * Presents the contents of the next buffer in the device's swap chain.
     */
    bool _present_impl() override;

    /**
     * Checks whether the device (generally) supports the specified combination of
     * pixel format and channel order for the specified texture type.
     */
    bool _check_texture_format_impl(xl7::graphics::textures::Texture::Type texture_type, PixelFormat pixel_format, ChannelOrder channel_order) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Tries to determine the available video memory composition.
     */
    bool _determine_video_memory(Capabilities::Memory& memory_capabilities);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

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

    /**
     * The Direct3D 9 (standard) render target surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_render_target_surface;

    /**
     * The Direct3D 9 (standard) depth/stencil surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_depth_stencil_surface;

}; // class RenderingDeviceImpl



} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
