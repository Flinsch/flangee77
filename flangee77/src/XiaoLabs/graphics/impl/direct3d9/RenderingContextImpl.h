#ifndef XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
#include "../../RenderingContext.h"

#include "./prerequisites.h"

#include "./states/D3DSamplerStateTypeValues.h"
#include "./states/D3DRasterizerStateTypeValues.h"
#include "./states/D3DDepthStencilStateTypeValues.h"
#include "./states/D3DBlendStateTypeValues.h"

#include "../shared/meshes/VertexBufferBinding.h"
#include "../shared/meshes/ComposedVertexLayout.h"

#include <unordered_map>



namespace xl7::graphics::impl::direct3d9 {



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
     * Returns the Direct3D 9 device interface.
     */
    IDirect3DDevice9* get_raw_d3d_device() const { return _d3d_device.Get(); }



protected:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device, wrl::ComPtr<IDirect3DSurface9> d3d_render_target_surface, wrl::ComPtr<IDirect3DSurface9> d3d_depth_stencil_surface);
    ~RenderingContextImpl() override = default;



private:

    struct HardwareStates
    {
        IDirect3DSurface9* render_targets[pipeline::OutputMergerStage::MAX_RENDER_TARGETS] = {};
        IDirect3DSurface9* depth_stencil_surface = nullptr;

        IDirect3DVertexBuffer9* vertex_buffers[pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS] = {};
        IDirect3DIndexBuffer9* index_buffer = nullptr;

        IDirect3DVertexDeclaration9* vertex_declaration = nullptr;

        struct TextureSamplerStates
        {
            IDirect3DBaseTexture9* base_textures[pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS] = {};
            states::D3DSamplerStateTypeValues sampler_state_type_values[pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS] = {};
        };

        struct AbstractShaderStates
            : public TextureSamplerStates
        {
            
        };

        template <class TDirect3DShader9>
        struct ShaderStates
            : public AbstractShaderStates
        {
            TDirect3DShader9* shader = nullptr;
        };

        ShaderStates<IDirect3DVertexShader9> vs;
        ShaderStates<IDirect3DPixelShader9> ps;

        D3DVIEWPORT9 viewport = {};

        states::D3DRasterizerStateTypeValues rasterizer_state_type_values = {};
        states::D3DDepthStencilStateTypeValues depth_stencil_state_type_values = {};
        states::D3DBlendStateTypeValues blend_state_type_values = {};

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
     * Transfers the current shader constant states for the indirectly/implicitly
     * specified shader to the device if necessary.
     */
    bool _flush_shader_constant_states(const ResolvedAbstractShaderStates& resolved_shader_states, HardwareStates::AbstractShaderStates& hardware_shader_states, HRESULT (IDirect3DDevice9::*SetShaderConstantF)(UINT, const float*, UINT), HRESULT (IDirect3DDevice9::*SetShaderConstantI)(UINT, const int*, UINT), HRESULT (IDirect3DDevice9::*SetShaderConstantB)(UINT, const BOOL*, UINT));

    /**
     * Transfers the current texture/sampler states for the indirectly/implicitly
     * specified shader to the device if necessary.
     */
    bool _flush_texture_sampler_states(const ResolvedTextureSamplerStates& resolved_texture_sampler_states, HardwareStates::TextureSamplerStates& hardware_texture_sampler_states, unsigned max_stage_count, unsigned stage_base);

    /**
     * Tries to find a suitable Direct3D 9 vertex declaration based on the currently
     * set vertex buffer(s).
     */
    IDirect3DVertexDeclaration9* _find_d3d_vertex_declaration(const shared::meshes::VertexBufferBinding& vertex_buffer_binding);

    /**
     * Tries to find a suitable Direct3D 9 vertex declaration, and otherwise creates
     * a new one, both based on the vertex layout(s) of the currently set vertex
     * buffer(s).
     */
    IDirect3DVertexDeclaration9* _find_or_create_d3d_vertex_declaration(const shared::meshes::VertexBufferBinding& vertex_buffer_binding);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

    /**
     * The Direct3D 9 (standard) render target surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_render_target_surface;

    /**
     * The Direct3D 9 (standard) depth/stencil surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_depth_stencil_surface;


    HardwareStates hardware_states;

    std::unordered_map<shared::meshes::VertexBufferBinding, wrl::ComPtr<IDirect3DVertexDeclaration9>> _d3d_vertex_declarations_by_binding;
    std::unordered_map<shared::meshes::ComposedVertexLayout, wrl::ComPtr<IDirect3DVertexDeclaration9>> _d3d_vertex_declarations_by_layout;

    cl7::byte_vector _temp_constant_data;

}; // class RenderingContextImpl



} // namespace xl7::graphics::impl::direct3d9

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
