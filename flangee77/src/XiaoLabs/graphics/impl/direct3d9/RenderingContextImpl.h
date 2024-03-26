#pragma once
#ifndef XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
#define XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
#include "../../RenderingContext.h"

#include "./prerequisites.h"

#include "../shared/meshes/VertexBufferBinding.h"
#include "../shared/meshes/ComposedVertexLayout.h"

#include <unordered_map>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



class RenderingDeviceImpl;



class RenderingContextImpl final
    : public RenderingContext
{
    friend class RenderingDeviceImpl;

private:
    struct HardwareStates
    {
        IDirect3DSurface9* render_targets[ pipeline::OutputMergerStage::MAX_RENDER_TARGETS ];
        IDirect3DSurface9* depth_stencil_surface;

        IDirect3DVertexBuffer9* vertex_buffers[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];
        IDirect3DIndexBuffer9* index_buffer;

        IDirect3DVertexDeclaration9* vertex_declaration;

        struct TextureSamplerStates
        {
            IDirect3DBaseTexture9* base_textures[ pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS ];
        };

        template <class TDirect3DShader9>
        struct ShaderStates
            : public TextureSamplerStates
        {
            TDirect3DShader9* shader;
        };

        ShaderStates<IDirect3DVertexShader9> vs;
        ShaderStates<IDirect3DPixelShader9> ps;

        HardwareStates();
    };



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
protected:
    /**
     * Explicit constructor.
     */
    RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device, wrl::ComPtr<IDirect3DSurface9> d3d_render_target_surface, wrl::ComPtr<IDirect3DSurface9> d3d_depth_stencil_surface);

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
     * The Direct3D 9 device interface.
     */
    wrl::ComPtr<IDirect3DDevice9> _d3d_device;

private:
    /**
     * The Direct3D 9 (standard) render target surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_render_target_surface;

    /**
     * The Direct3D 9 (standard) depth/stencil surface interface.
     */
    wrl::ComPtr<IDirect3DSurface9> _d3d_depth_stencil_surface;

private:
    HardwareStates hardware_states;

    std::unordered_map<shared::meshes::VertexBufferBinding, wrl::ComPtr<IDirect3DVertexDeclaration9>> _d3d_vertex_declarations_by_binding;
    std::unordered_map<shared::meshes::ComposedVertexLayout, wrl::ComPtr<IDirect3DVertexDeclaration9>> _d3d_vertex_declarations_by_layout;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the Direct3D 9 device interface.
     */
    IDirect3DDevice9* get_raw_d3d_device() const { return _d3d_device.Get(); }



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
    bool _flush_texture_sampler_states(const ResolvedTextureSamplerStates& resolved_texture_sampler_states, HardwareStates::TextureSamplerStates& hardware_texture_sampler_states, unsigned max_stage_count, unsigned stage_base);

private:
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

}; // class RenderingContextImpl



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7

#endif // XL7_GRAPHICS_IMPL_D3D9_RENDERINGCONTEXTIMPL_H
