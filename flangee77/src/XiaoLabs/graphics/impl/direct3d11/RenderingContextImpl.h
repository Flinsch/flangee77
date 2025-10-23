#ifndef XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
#include "../../RenderingContext.h"

#include "./prerequisites.h"

#include "./shaders/D3DConstantBufferWrapper.h"



namespace xl7::graphics::impl::direct3d11 {



class RenderingDeviceImpl;



class RenderingContextImpl final
    : public RenderingContext
{
    friend class RenderingDeviceImpl;

public:
    RenderingContextImpl() = delete;

    RenderingContextImpl(const RenderingContextImpl&) = delete;
    RenderingContextImpl& operator=(const RenderingContextImpl&) = delete;
    RenderingContextImpl(RenderingContextImpl&&) = delete;
    RenderingContextImpl& operator=(RenderingContextImpl&&) = delete;



    /**
     * Returns the Direct3D 11 device context interface.
     */
    ID3D11DeviceContextN* get_raw_d3d_device_context() const { return _d3d_device_context.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context, wrl::ComPtr<ID3D11RenderTargetView> d3d_render_target_view, wrl::ComPtr<ID3D11DepthStencilView> d3d_depth_stencil_view);
    ~RenderingContextImpl() override = default;



private:

    struct HardwareStates
    {
        ID3D11RenderTargetView* render_target_views[pipeline::OutputMergerStage::MAX_RENDER_TARGETS] = {};
        ID3D11DepthStencilView* depth_stencil_view = nullptr;

        ID3D11Buffer* vertex_buffers[pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS] = {};
        ID3D11Buffer* index_buffer = nullptr;

        D3D11_PRIMITIVE_TOPOLOGY primitive_topology = static_cast<D3D11_PRIMITIVE_TOPOLOGY>(0);

        ID3D11InputLayout* input_layout = nullptr;

        struct TextureSamplerStates
        {
            ID3D11ShaderResourceView* shader_resource_views[pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS] = {};
            ID3D11SamplerState* sampler_states[pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS] = {};
        };

        struct AbstractShaderStates
            : public TextureSamplerStates
        {
            shaders::D3DConstantBufferWrapper* constant_buffer_wrappers[pipeline::AbstractShaderStage::MAX_CONSTANT_BUFFER_SLOTS] = {};
            ID3D11Buffer* constant_buffers[pipeline::AbstractShaderStage::MAX_CONSTANT_BUFFER_SLOTS] = {};
        };

        template <class TD3D11Shader>
        struct ShaderStates
            : public AbstractShaderStates
        {
            TD3D11Shader* shader = nullptr;
        };

        ShaderStates<ID3D11VertexShader> vs;
        ShaderStates<ID3D11PixelShader> ps;

        D3D11_VIEWPORT viewport = {};

        ID3D11RasterizerState* rasterizer_state = nullptr;
        ID3D11DepthStencilState* depth_stencil_state = nullptr;
        ID3D11BlendState* blend_state = nullptr;

        unsigned stencil_reference_value = 0;
        Color blend_factor;

        HardwareStates() = default;
    };



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################

    /**
     * Performs a forced synchronization with the hardware state.
     */
    bool _synchronize_hardware_state_impl() override;

    /**
     * Begins a scene.
     */
    bool _begin_scene_impl() override;

    /**
     * Ends a scene that was begun by calling `begin_scene`.
     */
    bool _end_scene_impl() override;

    /**
     * Clears the currently bound render target(s).
     */
    bool _clear_impl(const ResolvedTargetStates& resolved_target_states, ClearFlags clear_flags, const Color& color, float depth, unsigned stencil) override;

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool _draw_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_vertex) override;

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool _draw_indexed_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Transfers the current render target states to the device if necessary.
     */
    bool _flush_target_states(const ResolvedTargetStates& resolved_target_states);

    /**
     * Transfers all current draw states to the device if necessary.
     */
    bool _flush_draw_states(const ResolvedDrawStates& resolved_draw_states);

    /**
     * Resolves temporary constant buffer references and deletes those that are not
     * valid anymore.
     */
    bool _resolve_temp_constant_buffer_references();

    /**
     * Gathers/prepares the current states for the indirectly/implicitly
     * specified shader.
     */
    bool _prepare_shader_constant_states(const ResolvedAbstractShaderStates& resolved_shader_states, HardwareStates::AbstractShaderStates& hardware_shader_states);

    /**
     * Transfers the prepared constant buffer states to the device if necessary.
     */
    bool _flush_temp_constant_buffer_data();

    /**
     * Transfers the prepared constant buffer states for the indirectly/implicitly
     * specified shader to the device if necessary.
     */
    bool _flush_constant_buffer_states(HardwareStates::AbstractShaderStates& hardware_shader_states, void (ID3D11DeviceContextN::*SetConstantBuffers)(unsigned, unsigned, ID3D11Buffer*const*));

    /**
     * Transfers the current texture/samper states for the indirectly/implicitly
     * specified shader to the device if necessary.
     */
    bool _flush_texture_sampler_states(const ResolvedTextureSamplerStates& resolved_texture_sampler_states, HardwareStates::TextureSamplerStates& hardware_texture_sampler_states, void (ID3D11DeviceContextN::*SetShaderResources)(unsigned, unsigned, ID3D11ShaderResourceView*const*), void (ID3D11DeviceContextN::*SetSamplers)(unsigned, unsigned, ID3D11SamplerState*const*));



    // #############################################################################
    // Attributes
    // #############################################################################

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


    HardwareStates hardware_states;

    std::vector<shaders::D3DConstantBufferWrapper*> _temp_d3d_constant_buffer_wrappers;

}; // class RenderingContextImpl



} // namespace xl7::graphics::impl::direct3d11

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
