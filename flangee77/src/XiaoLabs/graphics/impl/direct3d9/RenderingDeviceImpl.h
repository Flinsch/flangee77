#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
#include "../../RenderingDevice.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



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

private:
    /**
     * The Direct3D 9 (standard) render target surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_render_target_surface;

    /**
     * The Direct3D 9 (standard) depth/stencil surface interface.
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

private:
    /**
     * Checks whether the device (generally) supports the specified combination of
     * pixel format and channel order for the specified texture type.
     */
    virtual bool _check_texture_format_impl(xl7::graphics::textures::Texture::Type texture_type, PixelFormat pixel_format, ChannelOrder channel_order) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Tries to determine the available video memory composition.
     */
    bool _determine_video_memory(Capabilities::Memory& memory_capabilities);

}; // class RenderingDeviceImpl



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGDEVICEIMPL_H
