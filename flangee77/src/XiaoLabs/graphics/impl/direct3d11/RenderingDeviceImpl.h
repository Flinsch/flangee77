#ifndef XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
#include "../../RenderingDevice.h"

#include "./prerequisites.h"

#include "./shaders/D3DConstantBufferWrapper.h"

#include "../shared/meshes/VertexBufferBinding.h"
#include "../shared/meshes/ComposedVertexLayout.h"

#include <unordered_map>



namespace xl7::graphics::impl::direct3d11 {



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
     * Returns the Direct3D 11 device interface.
     */
    ID3D11DeviceN* get_raw_d3d_device() const { return _d3d_device.Get(); }

    /**
     * Returns the set of features targeted by the Direct3D 11 device.
     */
    const D3D_FEATURE_LEVEL& get_d3d_feature_level() const { return _d3d_feature_level; }



    // #############################################################################
    // "Public" (Internal) Helpers
    // #############################################################################

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

    /**
     * Tries to find actual Direct3D 11 constant buffers for the specified shader,
     * and otherwise creates some.
     */
    std::span<shaders::D3DConstantBufferWrapper*> _find_or_create_d3d_constant_buffers(resources::ResourceID shader_id);

    /**
     * Releases the actual Direct3D 11 constant buffers associated with the
     * specified shader.
     */
    void _release_d3d_constant_buffers(resources::ResourceID shader_id);



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
     * Tries to find an actual Direct3D 11 constant buffer that matches the
     * specified layout.
     */
    shaders::D3DConstantBufferWrapper* _find_d3d_constant_buffer(const xl7::graphics::shaders::ConstantBufferLayout& constant_buffer_layout);

    /**
     * Tries to find an actual Direct3D 11 constant buffer that matches the
     * specified layout, otherwise creates a new one based on said layout.
     */
    shaders::D3DConstantBufferWrapper* _find_or_create_d3d_constant_buffer(const xl7::graphics::shaders::ConstantBufferLayout& constant_buffer_layout);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 11 device interface.
     */
    wrl::ComPtr<ID3D11DeviceN> _d3d_device;

    /**
     * The set of features targeted by the Direct3D 11 device.
     */
    D3D_FEATURE_LEVEL _d3d_feature_level;

    /**
     * The flag specifying whether the display supports tearing, a requirement of
     * variable refresh rate displays.
     */
    bool _allow_tearing;

    /**
     * The DXGI swap chain interface.
     */
    wrl::ComPtr<IDXGISwapChainN> _dxgi_swap_chain;

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


    std::unordered_map<shared::meshes::VertexBufferBinding, wrl::ComPtr<ID3D11InputLayout>> _d3d_input_layouts_by_binding;
    std::unordered_map<shared::meshes::ComposedVertexLayout, wrl::ComPtr<ID3D11InputLayout>> _d3d_input_layouts_by_layout;

    std::vector<std::unique_ptr<shaders::D3DConstantBufferWrapper>> _d3d_constant_buffer_registry;
    std::unordered_map<resources::ResourceID, std::vector<shaders::D3DConstantBufferWrapper*>> _d3d_constant_buffers_by_shader_id;

}; // class RenderingDeviceImpl



} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGDEVICEIMPL_H
