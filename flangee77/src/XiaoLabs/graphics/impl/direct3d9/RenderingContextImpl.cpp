#include "RenderingContextImpl.h"

#include "./meshes/VertexBufferImpl.h"
#include "./meshes/IndexBufferImpl.h"

#include "./textures/Texture2DImpl.h"

#include "./shaders/VertexShaderImpl.h"
#include "./shaders/PixelShaderImpl.h"

#include "./states/SamplerStateImpl.h"
#include "./states/RasterizerStateImpl.h"
#include "./states/DepthStencilStateImpl.h"
#include "./states/BlendStateImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

#include <CoreLabs/logging.h>



namespace xl7::graphics::impl::direct3d9 {



    static DWORD _d3d_clear_flags_from(ClearFlags clear_flags)
    {
        static_assert(static_cast<unsigned>(ClearFlags::ColorBuffer) == static_cast<unsigned>(D3DCLEAR_TARGET));
        static_assert(static_cast<unsigned>(ClearFlags::DepthBuffer) == static_cast<unsigned>(D3DCLEAR_ZBUFFER));
        static_assert(static_cast<unsigned>(ClearFlags::StencilBuffer) == static_cast<unsigned>(D3DCLEAR_STENCIL));

        return static_cast<DWORD>(clear_flags);
    }

    static D3DPRIMITIVETYPE _d3d_primitive_type_from(xl7::graphics::meshes::Topology topology)
    {
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::Topology::PointList) == static_cast<unsigned>(D3DPT_POINTLIST));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::Topology::LineList) == static_cast<unsigned>(D3DPT_LINELIST));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::Topology::LineStrip) == static_cast<unsigned>(D3DPT_LINESTRIP));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::Topology::TriangleList) == static_cast<unsigned>(D3DPT_TRIANGLELIST));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::Topology::TriangleStrip) == static_cast<unsigned>(D3DPT_TRIANGLESTRIP));

        return static_cast<D3DPRIMITIVETYPE>(topology);
    }

    static BYTE _d3d_vertex_element_usage_from(xl7::graphics::meshes::VertexLayout::Semantic semantic)
    {
        switch (semantic)
        {
        case xl7::graphics::meshes::VertexLayout::Semantic::POSITION:       return D3DDECLUSAGE_POSITION;
        case xl7::graphics::meshes::VertexLayout::Semantic::POSITIONT:      return D3DDECLUSAGE_POSITIONT;
        case xl7::graphics::meshes::VertexLayout::Semantic::NORMAL:         return D3DDECLUSAGE_NORMAL;
        case xl7::graphics::meshes::VertexLayout::Semantic::TANGENT:        return D3DDECLUSAGE_TANGENT;
        case xl7::graphics::meshes::VertexLayout::Semantic::BINORMAL:       return D3DDECLUSAGE_BINORMAL;
        case xl7::graphics::meshes::VertexLayout::Semantic::COLOR:          return D3DDECLUSAGE_COLOR;
        case xl7::graphics::meshes::VertexLayout::Semantic::TEXCOORD:       return D3DDECLUSAGE_TEXCOORD;
        case xl7::graphics::meshes::VertexLayout::Semantic::BLENDINDICES:   return D3DDECLUSAGE_BLENDINDICES;
        case xl7::graphics::meshes::VertexLayout::Semantic::BLENDWEIGHT:    return D3DDECLUSAGE_BLENDWEIGHT;
        case xl7::graphics::meshes::VertexLayout::Semantic::PSIZE:          return D3DDECLUSAGE_PSIZE;
        default:
            assert(false);
        }

        return 0;
    }

    static BYTE _d3d_vertex_element_type_from(xl7::graphics::meshes::VertexLayout::DataType data_type)
    {
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::VertexLayout::DataType::FLOAT1) == static_cast<unsigned>(D3DDECLTYPE_FLOAT1));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::VertexLayout::DataType::FLOAT2) == static_cast<unsigned>(D3DDECLTYPE_FLOAT2));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::VertexLayout::DataType::FLOAT3) == static_cast<unsigned>(D3DDECLTYPE_FLOAT3));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::VertexLayout::DataType::FLOAT4) == static_cast<unsigned>(D3DDECLTYPE_FLOAT4));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::VertexLayout::DataType::COLOR) == static_cast<unsigned>(D3DDECLTYPE_D3DCOLOR));
        static_assert(static_cast<unsigned>(xl7::graphics::meshes::VertexLayout::DataType::UBYTE4) == static_cast<unsigned>(D3DDECLTYPE_UBYTE4));

        return static_cast<BYTE>(data_type);
    }

    static D3DVIEWPORT9 _d3d_viewport_from(const Viewport& viewport)
    {
        D3DVIEWPORT9 d3d_viewport;
        d3d_viewport.X = viewport.x;
        d3d_viewport.Y = viewport.y;
        d3d_viewport.Width = viewport.width;
        d3d_viewport.Height = viewport.height;
        d3d_viewport.MinZ = viewport.min_z;
        d3d_viewport.MaxZ = viewport.max_z;
        return d3d_viewport;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    RenderingContextImpl::RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device, wrl::ComPtr<IDirect3DSurface9> d3d_render_target_surface, wrl::ComPtr<IDirect3DSurface9> d3d_depth_stencil_surface)
        : RenderingContext(rendering_device, index)
        , _d3d_device(std::move(d3d_device))
        , _d3d_render_target_surface(std::move(d3d_render_target_surface))
        , _d3d_depth_stencil_surface(std::move(d3d_depth_stencil_surface))
    {
        assert(index == 0);
        assert(_d3d_device);
    }



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################

    /**
     * Performs a forced synchronization with the hardware state.
     */
    bool RenderingContextImpl::_synchronize_hardware_state_impl()
    {
        hardware_states = HardwareStates();
        for (unsigned i = 0; i < pipeline::AbstractShaderStage::MAX_TEXTURE_SAMPLER_SLOTS; ++i)
        {
            states::SamplerStateImpl::map_d3d_values({}, hardware_states.vs.sampler_state_type_values[i]);
            states::SamplerStateImpl::map_d3d_values({}, hardware_states.ps.sampler_state_type_values[i]);
        }
        hardware_states.viewport = _d3d_viewport_from(get_rendering_device()->get_default_viewport());
        states::RasterizerStateImpl::map_d3d_values({}, hardware_states.rasterizer_state_type_values);
        states::DepthStencilStateImpl::map_d3d_values({}, hardware_states.depth_stencil_state_type_values);
        states::BlendStateImpl::map_d3d_values({}, hardware_states.blend_state_type_values);

        auto _set_texture_and_sampler_states = [this](IDirect3DBaseTexture9* base_texture, std::span<const std::pair<D3DSAMPLERSTATETYPE, DWORD>> d3d_sampler_state_type_values, unsigned slot_index, unsigned stage_base)
        {
            _d3d_device->SetTexture(stage_base + slot_index, base_texture);
            for (const auto& [d3d_sampler_state_type, value] : d3d_sampler_state_type_values)
                _d3d_device->SetSamplerState(stage_base + slot_index, d3d_sampler_state_type, value);
        };

        auto _set_render_states = [this](std::span<const std::pair<D3DRENDERSTATETYPE, DWORD>> d3d_render_state_type_values)
        {
            for (const auto& [d3d_render_state_type, value] : d3d_render_state_type_values)
                _d3d_device->SetRenderState(d3d_render_state_type, value);
        };

        for (unsigned i = 0; i < get_rendering_device()->get_capabilities().max_concurrent_vertex_stream_count; ++i)
            _d3d_device->SetStreamSource(0, hardware_states.vertex_buffers[i], 0, 0);
        _d3d_device->SetIndices(hardware_states.index_buffer);
        _d3d_device->SetVertexDeclaration(hardware_states.vertex_declaration);

        _d3d_device->SetVertexShader(hardware_states.vs.shader);
        for (unsigned i = 0; i < 4; ++i)
            _set_texture_and_sampler_states(hardware_states.vs.base_textures[i], hardware_states.vs.sampler_state_type_values[i], i, D3DVERTEXTEXTURESAMPLER0);

        _d3d_device->SetPixelShader(hardware_states.ps.shader);
        for (unsigned i = 0; i < 8; ++i)
            _set_texture_and_sampler_states(hardware_states.ps.base_textures[i], hardware_states.ps.sampler_state_type_values[i], i, 0);

        _d3d_device->SetViewport(&hardware_states.viewport);

        _set_render_states(hardware_states.rasterizer_state_type_values);
        _set_render_states(hardware_states.depth_stencil_state_type_values);
        _set_render_states(hardware_states.blend_state_type_values);

        return true;
    }

    /**
     * Begins a scene.
     */
    bool RenderingContextImpl::_begin_scene_impl()
    {
        HRESULT hresult = _d3d_device->BeginScene();
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::BeginScene"));
            return false;
        }

        return true;
    }

    /**
     * Ends a scene that was begun by calling `begin_scene`.
     */
    bool RenderingContextImpl::_end_scene_impl()
    {
        HRESULT hresult = _d3d_device->EndScene();
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::EndScene"));
            return false;
        }

        return true;
    }

    /**
     * Clears the currently bound render target(s).
     */
    bool RenderingContextImpl::_clear_impl(const ResolvedTargetStates& resolved_target_states, ClearFlags clear_flags, const Color& color, float depth, unsigned stencil)
    {
        if (!_flush_target_states(resolved_target_states))
            return false;

        DWORD d3d_clear_flags = _d3d_clear_flags_from(clear_flags);
        if (!d3d_clear_flags)
            return true;

        HRESULT hresult = _d3d_device->Clear(0, nullptr, d3d_clear_flags, color.to_bgra32(), depth, stencil);
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::Clear"));
            return false;
        }

        return true;
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_vertex)
    {
        if (!_flush_draw_states(resolved_draw_states))
            return false;

        D3DPRIMITIVETYPE d3d_primitive_type = _d3d_primitive_type_from(resolved_draw_states.topology);

        HRESULT hresult = _d3d_device->DrawPrimitive(d3d_primitive_type, start_vertex, primitive_count);
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::DrawPrimitive"));
            return false;
        }

        return true;
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_indexed_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        if (!_flush_draw_states(resolved_draw_states))
            return false;

        D3DPRIMITIVETYPE d3d_primitive_type = _d3d_primitive_type_from(resolved_draw_states.topology);

        // Should we optimize for these two values?
        unsigned min_vertex_index = 0;
        assert(resolved_draw_states.vertex_buffers[0]);
        unsigned vertex_count = resolved_draw_states.vertex_buffers[0]->get_desc().count;

        HRESULT hresult = _d3d_device->DrawIndexedPrimitive(d3d_primitive_type, base_vertex, min_vertex_index, vertex_count, start_index, primitive_count);
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::DrawPrimitive"));
            return false;
        }

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Transfers the current render target states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_target_states(const ResolvedTargetStates& resolved_target_states)
    {
        HRESULT hresult;


        const unsigned max_render_target_count = get_rendering_device()->get_capabilities().max_simultaneous_render_target_count;
        for (unsigned target_index = 0; target_index < max_render_target_count; ++target_index)
        {
            IDirect3DSurface9* d3d_render_target;
            if (target_index == 0)
                d3d_render_target = _d3d_render_target_surface.Get();
            else
                d3d_render_target = nullptr;

            if (d3d_render_target != hardware_states.render_targets[target_index])
            {
                hresult = _d3d_device->SetRenderTarget(target_index, d3d_render_target);
                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetRenderTarget"));
                    return false;
                }
                hardware_states.render_targets[target_index] = d3d_render_target;
            }
        } // for each render target

        IDirect3DSurface9* d3d_depth_stencil_surface = _d3d_depth_stencil_surface.Get();

        if (d3d_depth_stencil_surface != hardware_states.depth_stencil_surface)
        {
            hresult = _d3d_device->SetDepthStencilSurface(d3d_depth_stencil_surface);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetDepthStencilSurface"));
                return false;
            }
            hardware_states.depth_stencil_surface = d3d_depth_stencil_surface;
        }


        return true;
    }

    /**
     * Transfers all current draw states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_draw_states(const ResolvedDrawStates& resolved_draw_states)
    {
        if (!_flush_target_states(resolved_draw_states))
            return false;

        shared::meshes::VertexBufferBinding vertex_buffer_binding;

        HRESULT hresult;


        const unsigned max_vertex_stream_count = get_rendering_device()->get_capabilities().max_concurrent_vertex_stream_count;
        for (unsigned stream_index = 0; stream_index < max_vertex_stream_count; ++stream_index)
        {
            const auto* vertex_buffer = static_cast<const meshes::VertexBufferImpl*>(resolved_draw_states.vertex_buffers[stream_index]); // NOLINT(*-pro-type-static-cast-downcast)
            IDirect3DVertexBuffer9* d3d_vertex_buffer;
            unsigned stride;
            if (vertex_buffer) {
                vertex_buffer_binding.stream_count = stream_index = stream_index + 1;
                vertex_buffer_binding.vertex_buffer_ids[stream_index] = vertex_buffer->get_id();
                d3d_vertex_buffer = vertex_buffer->get_raw_d3d_vertex_buffer();
                stride = vertex_buffer->get_stride();
            } else {
                vertex_buffer_binding.vertex_buffer_ids[stream_index].invalidate();
                d3d_vertex_buffer = nullptr;
                stride = 0;
            }

            if (d3d_vertex_buffer != hardware_states.vertex_buffers[stream_index])
            {
                hresult = _d3d_device->SetStreamSource(stream_index, d3d_vertex_buffer, 0, stride);
                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetStreamSource"));
                    return false;
                }
                hardware_states.vertex_buffers[stream_index] = d3d_vertex_buffer;
            }
        } // for each vertex stream

        const auto* index_buffer = static_cast<const meshes::IndexBufferImpl*>(resolved_draw_states.index_buffer); // NOLINT(*-pro-type-static-cast-downcast)
        IDirect3DIndexBuffer9* d3d_index_buffer;
        if (index_buffer)
            d3d_index_buffer = index_buffer->get_raw_d3d_index_buffer();
        else
            d3d_index_buffer = nullptr;

        if (d3d_index_buffer != hardware_states.index_buffer)
        {
            hresult = _d3d_device->SetIndices(d3d_index_buffer);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetIndices"));
                return false;
            }
            hardware_states.index_buffer = d3d_index_buffer;
        }


        IDirect3DVertexDeclaration9* d3d_vertex_declaration = _find_d3d_vertex_declaration(vertex_buffer_binding);
        if (!d3d_vertex_declaration)
            d3d_vertex_declaration = _find_or_create_d3d_vertex_declaration(vertex_buffer_binding);

        if (d3d_vertex_declaration != hardware_states.vertex_declaration)
        {
            hresult = _d3d_device->SetVertexDeclaration(d3d_vertex_declaration);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetVertexDeclaration"));
                return false;
            }
            hardware_states.vertex_declaration = d3d_vertex_declaration;
        }


        const auto* vertex_shader = static_cast<const shaders::VertexShaderImpl*>(resolved_draw_states.vs.shader); // NOLINT(*-pro-type-static-cast-downcast)
        IDirect3DVertexShader9* d3d_vertex_shader;
        if (vertex_shader)
            d3d_vertex_shader = vertex_shader->get_raw_d3d_vertex_shader();
        else
            d3d_vertex_shader = nullptr;

        if (d3d_vertex_shader != hardware_states.vs.shader)
        {
            hresult = _d3d_device->SetVertexShader(d3d_vertex_shader);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetVertexShader"));
                return false;
            }
            hardware_states.vs.shader = d3d_vertex_shader;
        }

        const auto* pixel_shader = static_cast<const shaders::PixelShaderImpl*>(resolved_draw_states.ps.shader); // NOLINT(*-pro-type-static-cast-downcast)
        IDirect3DPixelShader9* d3d_pixel_shader;
        if (pixel_shader)
            d3d_pixel_shader = pixel_shader->get_raw_d3d_pixel_shader();
        else
            d3d_pixel_shader = nullptr;

        if (d3d_pixel_shader != hardware_states.ps.shader)
        {
            hresult = _d3d_device->SetPixelShader(d3d_pixel_shader);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetPixelShader"));
                return false;
            }
            hardware_states.ps.shader = d3d_pixel_shader;
        }


        _flush_shader_constant_states(resolved_draw_states.vs, hardware_states.vs, &IDirect3DDevice9::SetVertexShaderConstantF, &IDirect3DDevice9::SetVertexShaderConstantI, &IDirect3DDevice9::SetVertexShaderConstantB);
        _flush_shader_constant_states(resolved_draw_states.ps, hardware_states.ps, &IDirect3DDevice9::SetPixelShaderConstantF, &IDirect3DDevice9::SetPixelShaderConstantI, &IDirect3DDevice9::SetPixelShaderConstantB);

        _flush_texture_sampler_states(resolved_draw_states.vs, hardware_states.vs, 4, D3DVERTEXTEXTURESAMPLER0);
        _flush_texture_sampler_states(resolved_draw_states.ps, hardware_states.ps, 8, 0);


        D3DVIEWPORT9 d3d_viewport = _d3d_viewport_from(resolved_draw_states.viewport);
        if (std::memcmp(&d3d_viewport, &hardware_states.viewport, sizeof(D3DVIEWPORT9)) != 0) // NOLINT(bugprone-suspicious-memory-comparison)
        {
            hresult = _d3d_device->SetViewport(&d3d_viewport);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetViewport"));
                return false;
            }
            hardware_states.viewport = d3d_viewport;
        }


        const auto* rasterizer_state = static_cast<const states::RasterizerStateImpl*>(resolved_draw_states.rasterizer_state); // NOLINT(*-pro-type-static-cast-downcast)
        const states::D3DRasterizerStateTypeValues* d3d_rasterizer_state_type_values;
        if (rasterizer_state)
            d3d_rasterizer_state_type_values = &rasterizer_state->get_d3d_rasterizer_state_type_values();
        else
        {
            static states::D3DRasterizerStateTypeValues default_rasterizer_state_type_values;
            states::RasterizerStateImpl::map_d3d_values({}, default_rasterizer_state_type_values);
            d3d_rasterizer_state_type_values = &default_rasterizer_state_type_values;
        }

        for (size_t k = 0; k < states::D3D_RASTERIZER_STATE_TYPE_COUNT; ++k)
        {
            if ((*d3d_rasterizer_state_type_values)[k].second != hardware_states.rasterizer_state_type_values[k].second)
            {
                hresult = _d3d_device->SetRenderState((*d3d_rasterizer_state_type_values)[k].first, (*d3d_rasterizer_state_type_values)[k].second);
                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetRenderState"));
                    return false;
                }
                hardware_states.rasterizer_state_type_values[k] = (*d3d_rasterizer_state_type_values)[k];
            }
        } // for each rasterizer state type/value


        const auto* depth_stencil_state = static_cast<const states::DepthStencilStateImpl*>(resolved_draw_states.depth_stencil_state); // NOLINT(*-pro-type-static-cast-downcast)
        const states::D3DDepthStencilStateTypeValues* d3d_depth_stencil_state_type_values;
        if (depth_stencil_state)
            d3d_depth_stencil_state_type_values = &depth_stencil_state->get_d3d_depth_stencil_state_type_values();
        else
        {
            static states::D3DDepthStencilStateTypeValues default_depth_stencil_state_type_values;
            states::DepthStencilStateImpl::map_d3d_values({}, default_depth_stencil_state_type_values);
            d3d_depth_stencil_state_type_values = &default_depth_stencil_state_type_values;
        }

        bool swapped_winding_order = resolved_draw_states.rasterizer_state != nullptr && resolved_draw_states.rasterizer_state->get_desc().winding_order == xl7::graphics::states::RasterizerState::WindingOrder::CounterClockwise;
        for (size_t k = 0; k < states::D3D_DEPTH_STENCIL_STATE_TYPE_COUNT; ++k)
        {
            size_t ks = k;
            size_t kh = k;
            if (swapped_winding_order && k >= 6)
                k >= 10 ? ks -= 4 : ks += 4;

            if ((*d3d_depth_stencil_state_type_values)[ks].second != hardware_states.depth_stencil_state_type_values[kh].second)
            {
                hresult = _d3d_device->SetRenderState((*d3d_depth_stencil_state_type_values)[ks].first, (*d3d_depth_stencil_state_type_values)[ks].second);
                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetRenderState"));
                    return false;
                }
                hardware_states.depth_stencil_state_type_values[kh] = (*d3d_depth_stencil_state_type_values)[ks];
            }
        } // for each depth/stencil state type/value

        if (resolved_draw_states.stencil_reference_value != hardware_states.stencil_reference_value)
        {
            hresult = _d3d_device->SetRenderState(D3DRS_STENCILREF, resolved_draw_states.stencil_reference_value);
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetRenderState"));
                return false;
            }
            hardware_states.stencil_reference_value = resolved_draw_states.stencil_reference_value;
        }


        const auto* blend_state = static_cast<const states::BlendStateImpl*>(resolved_draw_states.blend_state); // NOLINT(*-pro-type-static-cast-downcast)
        const states::D3DBlendStateTypeValues* d3d_blend_state_type_values;
        if (blend_state)
            d3d_blend_state_type_values = &blend_state->get_d3d_blend_state_type_values();
        else
        {
            static states::D3DBlendStateTypeValues default_blend_state_type_values;
            states::BlendStateImpl::map_d3d_values({}, default_blend_state_type_values);
            d3d_blend_state_type_values = &default_blend_state_type_values;
        }

        for (size_t k = 0; k < states::D3D_BLEND_STATE_TYPE_COUNT; ++k)
        {
            if ((*d3d_blend_state_type_values)[k].second != hardware_states.blend_state_type_values[k].second)
            {
                hresult = _d3d_device->SetRenderState((*d3d_blend_state_type_values)[k].first, (*d3d_blend_state_type_values)[k].second);
                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetRenderState"));
                    return false;
                }
                hardware_states.blend_state_type_values[k] = (*d3d_blend_state_type_values)[k];
            }
        } // for each blend state type/value

        if (resolved_draw_states.blend_factor != hardware_states.blend_factor)
        {
            hresult = _d3d_device->SetRenderState(D3DRS_BLENDFACTOR, resolved_draw_states.blend_factor.to_bgra32());
            if (FAILED(hresult))
            {
                LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetRenderState"));
                return false;
            }
            hardware_states.blend_factor = resolved_draw_states.blend_factor;
        }


        return true;
    }

    /**
     * Transfers the current shader constant states for the indirectly/implicitly
     * specified shader to the device if necessary.
     */
    bool RenderingContextImpl::_flush_shader_constant_states(const ResolvedAbstractShaderStates& resolved_shader_states, HardwareStates::AbstractShaderStates& hardware_shader_states, HRESULT (IDirect3DDevice9::*SetShaderConstantF)(UINT, const float*, UINT), HRESULT (IDirect3DDevice9::*SetShaderConstantI)(UINT, const int*, UINT), HRESULT (IDirect3DDevice9::*SetShaderConstantB)(UINT, const BOOL*, UINT))
    {
        HRESULT hresult;


        for (unsigned mapping_index = 0; mapping_index < resolved_shader_states.constant_buffer_count; ++mapping_index)
        {
            const auto* constant_buffer = resolved_shader_states.constant_buffers[mapping_index];
            if (!constant_buffer)
                continue;
            const auto* constant_buffer_mapping = resolved_shader_states.constant_buffer_mappings[mapping_index];
            assert(constant_buffer_mapping);

            for (const auto& constant_mapping : constant_buffer_mapping->constant_mappings)
            {
                assert(constant_mapping.slot_index == 0);

                assert(constant_buffer->get_data().size() >= static_cast<size_t>(constant_mapping.source_offset + constant_mapping.padded_size));
                const void* data_ptr = constant_buffer->get_data().data() + constant_mapping.source_offset;

                assert(constant_mapping.shader_offset % 16 == 0);
                assert(constant_mapping.padded_size % 4 == 0);

                unsigned start_register = constant_mapping.shader_offset / 16;
                unsigned register_count = (constant_mapping.padded_size + 15) / 16;
                if (constant_mapping.padded_size % 16 != 0)
                {
                    size_t min_size = static_cast<size_t>(register_count) * 16;
                    if (_temp_constant_data.size() < min_size)
                        _temp_constant_data.resize(min_size);
                    std::memcpy(_temp_constant_data.data(), data_ptr, constant_mapping.size); // Yes, size, not padded_size. padded_size wouldn't be wrong either, but copying size bytes is completely sufficient.
                    data_ptr = _temp_constant_data.data();
                }

                switch (constant_mapping.constant_type)
                {
                case xl7::graphics::shaders::ConstantType::Float:
                    static_assert(sizeof(float) == 4);
                    hresult = (_d3d_device.Get()->*SetShaderConstantF)(start_register, static_cast<const float*>(data_ptr), register_count);
                    break;
                case xl7::graphics::shaders::ConstantType::Int:
                    static_assert(sizeof(int) == 4);
                    assert(constant_mapping.shader_offset % 16 == 0);
                    assert(constant_mapping.padded_size % 4 == 0);
                    hresult = (_d3d_device.Get()->*SetShaderConstantI)(start_register, static_cast<const int*>(data_ptr), register_count);
                    break;
                case xl7::graphics::shaders::ConstantType::Bool:
                    static_assert(sizeof(BOOL) == 4);
                    assert(constant_mapping.shader_offset % 16 == 0);
                    assert(constant_mapping.padded_size % 4 == 0);
                    hresult = (_d3d_device.Get()->*SetShaderConstantB)(start_register, static_cast<const BOOL*>(data_ptr), register_count);
                    break;
                default:
                    assert(false);
                } // switch constant type

                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetShaderConstant"));
                    return false;
                }
            } // for each constant mapping
        } // for each constant buffer (mapping)


        return true;
    }

    /**
     * Transfers the current texture/sampler states for the indirectly/implicitly
     * specified shader to the device if necessary.
     */
    bool RenderingContextImpl::_flush_texture_sampler_states(const ResolvedTextureSamplerStates& resolved_texture_sampler_states, HardwareStates::TextureSamplerStates& hardware_texture_sampler_states, unsigned max_stage_count, unsigned stage_base)
    {
        HRESULT hresult;


        const unsigned max_texture_sampler_slot_count = get_rendering_device()->get_capabilities().max_texture_sampler_slot_count;
        for (unsigned slot_index = 0; slot_index < max_texture_sampler_slot_count; ++slot_index)
        {
            const auto* texture = resolved_texture_sampler_states.textures[slot_index];
            IDirect3DBaseTexture9* d3d_base_texture;
            if (texture) {
                d3d_base_texture = texture->get_raw_resource<IDirect3DBaseTexture9>();
            } else {
                d3d_base_texture = nullptr;
            }

            if (slot_index >= max_stage_count)
            {
                if (d3d_base_texture)
                    LOG_WARNING(u8"Texture/sampler slot exceeds allowed number of stages of " + cl7::to_string(max_stage_count) + u8".");
                continue;
            }

            if (d3d_base_texture != hardware_texture_sampler_states.base_textures[slot_index])
            {
                hresult = _d3d_device->SetTexture(stage_base + slot_index, d3d_base_texture);
                if (FAILED(hresult))
                {
                    LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetTexture"));
                    return false;
                }
                hardware_texture_sampler_states.base_textures[slot_index] = d3d_base_texture;
            }

            const auto* sampler_state = static_cast<const states::SamplerStateImpl*>(resolved_texture_sampler_states.sampler_states[slot_index]); // NOLINT(*-pro-type-static-cast-downcast)
            const states::D3DSamplerStateTypeValues* d3d_sampler_state_type_values;
            if (sampler_state)
                d3d_sampler_state_type_values = &sampler_state->get_d3d_sampler_state_type_values();
            else
            {
                static states::D3DSamplerStateTypeValues default_sampler_state_type_values;
                states::SamplerStateImpl::map_d3d_values({}, default_sampler_state_type_values);
                d3d_sampler_state_type_values = &default_sampler_state_type_values;
            }

            for (size_t k = 0; k < states::D3D_SAMPLER_STATE_TYPE_COUNT; ++k)
            {
                if ((*d3d_sampler_state_type_values)[k].second != hardware_texture_sampler_states.sampler_state_type_values[slot_index][k].second)
                {
                    hresult = _d3d_device->SetSamplerState(stage_base + slot_index, (*d3d_sampler_state_type_values)[k].first, (*d3d_sampler_state_type_values)[k].second);
                    if (FAILED(hresult))
                    {
                        LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::SetSamplerState"));
                        return false;
                    }
                    hardware_texture_sampler_states.sampler_state_type_values[slot_index][k] = (*d3d_sampler_state_type_values)[k];
                }
            } // for each sampler state type/value
        } // for each texture/sampler slot


        return true;
    }

    /**
     * Tries to find a suitable Direct3D 9 vertex declaration based on the currently
     * set vertex buffer(s).
     */
    IDirect3DVertexDeclaration9* RenderingContextImpl::_find_d3d_vertex_declaration(const shared::meshes::VertexBufferBinding& vertex_buffer_binding)
    {
        auto it = _d3d_vertex_declarations_by_binding.find(vertex_buffer_binding);
        if (it != _d3d_vertex_declarations_by_binding.end())
            return it->second.Get();

        return nullptr;
    }

    /**
     * Tries to find a suitable Direct3D 9 vertex declaration, and otherwise creates
     * a new one, both based on the vertex layout(s) of the currently set vertex
     * buffer(s).
     */
    IDirect3DVertexDeclaration9* RenderingContextImpl::_find_or_create_d3d_vertex_declaration(const shared::meshes::VertexBufferBinding& vertex_buffer_binding)
    {
        shared::meshes::ComposedVertexLayout composed_vertex_layout{vertex_buffer_binding};

        auto it = _d3d_vertex_declarations_by_layout.find(composed_vertex_layout);
        if (it != _d3d_vertex_declarations_by_layout.end())
            return it->second.Get();

        std::vector<D3DVERTEXELEMENT9> d3d_vertex_elements;
        for (unsigned stream_index = 0; stream_index < pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS; ++stream_index)
        {
            if (!vertex_buffer_binding.vertex_buffer_ids[stream_index].is_valid())
                continue;

            WORD offset = 0;

            for (const xl7::graphics::meshes::VertexLayout::Element& element : composed_vertex_layout.vertex_layouts[stream_index].elements)
            {
                D3DVERTEXELEMENT9 d3d_vertex_element;
                d3d_vertex_element.Stream = static_cast<WORD>(stream_index);
                d3d_vertex_element.Offset = offset;
                d3d_vertex_element.Type = _d3d_vertex_element_type_from(element.data_type);
                d3d_vertex_element.Method = D3DDECLMETHOD_DEFAULT;
                d3d_vertex_element.Usage = _d3d_vertex_element_usage_from(element.semantic);
                d3d_vertex_element.UsageIndex = static_cast<BYTE>(element.semantic_index);

                d3d_vertex_elements.push_back(d3d_vertex_element);

                offset += static_cast<WORD>(element.get_size());
            } // for each vertex element
        } // for each vertex stream

        d3d_vertex_elements.push_back(D3DDECL_END());

        wrl::ComPtr<IDirect3DVertexDeclaration9> d3d_vertex_declaration;
        HRESULT hresult = _d3d_device->CreateVertexDeclaration(d3d_vertex_elements.data(), &d3d_vertex_declaration);
        if (FAILED(hresult))
        {
            LOG_ERROR(errors::d3d9_result(hresult, u8"IDirect3DDevice9::CreateVertexDeclaration"));
            return nullptr;
        }

        _d3d_vertex_declarations_by_layout.emplace(std::move(composed_vertex_layout), d3d_vertex_declaration);
        _d3d_vertex_declarations_by_binding.insert(std::make_pair(vertex_buffer_binding, d3d_vertex_declaration));

        return d3d_vertex_declaration.Get();
    }



} // namespace xl7::graphics::impl::direct3d9
