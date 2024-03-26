#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
#define XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
#include "../../RenderingContext.h"

#include "./prerequisites.h"



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



class RenderingDeviceImpl;



class RenderingContextImpl final
    : public RenderingContext
{
    friend class RenderingDeviceImpl;

private:
    struct HardwareStates
    {
        ID3D11RenderTargetView* render_target_views[ pipeline::OutputMergerStage::MAX_RENDER_TARGETS ];
        ID3D11DepthStencilView* depth_stencil_view;

        ID3D11Buffer* vertex_buffers[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];
        ID3D11Buffer* index_buffer;

        D3D11_PRIMITIVE_TOPOLOGY primitive_topology;

        ID3D11InputLayout* input_layout;

        struct TextureSamplerStates
        {
            ID3D11ShaderResourceView* shader_resource_views[ pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS ];
        };

        template <class TD3D11Shader>
        struct ShaderStates
            : public TextureSamplerStates
        {
            TD3D11Shader* shader;
        };

        ShaderStates<ID3D11VertexShader> vs;
        ShaderStates<ID3D11PixelShader> ps;

        HardwareStates();
    } hardware_states;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context, wrl::ComPtr<ID3D11RenderTargetView> d3d_render_target_view, wrl::ComPtr<ID3D11DepthStencilView> d3d_depth_stencil_view);

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

private:
    /**
     * The Direct3D 11 (standard) render target view interface.
     */
    wrl::ComPtr<ID3D11RenderTargetView> _d3d_render_target_view;

    /**
     * The Direct3D 11 (standard) depth/stencil view interface.
     */
    wrl::ComPtr<ID3D11DepthStencilView> _d3d_depth_stencil_view;



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
     * Clears the currently bound render target(s).
     */
    virtual bool _clear_impl(const ResolvedTargetStates& resolved_target_states, ClearFlags clear_flags, const Color& color, float depth, unsigned stencil) override;

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    virtual bool _draw_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_vertex) override;

    /**
     * Draws indexed, non-instanced primitives.
     */
    virtual bool _draw_indexed_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Transfers the current states to the device if necessary.
     */
    bool _flush_target_states(const ResolvedTargetStates& resolved_draw_states);

    /**
     * Transfers the current states to the device if necessary.
     */
    bool _flush_draw_states(const ResolvedDrawStates& resolved_draw_states);

    /**
     * Transfers the current states to the device if necessary.
     */
    bool _flush_texture_sampler_states(const ResolvedTextureSamplerStates& resolved_texture_sampler_states, HardwareStates::TextureSamplerStates& hardware_texture_sampler_states, void (ID3D11DeviceContextN::*SetShaderResources)(unsigned, unsigned, ID3D11ShaderResourceView*const*));

}; // class RenderingContextImpl



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D11_RENDERINGCONTEXTIMPL_H
