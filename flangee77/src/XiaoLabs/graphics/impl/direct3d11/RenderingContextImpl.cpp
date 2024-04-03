#include "RenderingContextImpl.h"

#include "./meshes/VertexBufferImpl.h"
#include "./meshes/IndexBufferImpl.h"

#include "./shaders/VertexShaderImpl.h"
#include "./shaders/PixelShaderImpl.h"

#include "./states/SamplerStateImpl.h"
#include "./states/RasterizerStateImpl.h"
#include "./states/BlendStateImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



    RenderingContextImpl::HardwareStates::HardwareStates()
    {
        memset( this, 0, sizeof(HardwareStates) );
    }



    static UINT _d3d_clear_flags_from(ClearFlags clear_flags)
    {
        static_assert( static_cast<unsigned>( ClearFlags::DepthBuffer ) / 2 == static_cast<unsigned>( D3D11_CLEAR_DEPTH ) );
        static_assert( static_cast<unsigned>( ClearFlags::StencilBuffer ) / 2 == static_cast<unsigned>( D3D11_CLEAR_STENCIL ) );

        return static_cast<UINT>( clear_flags ) >> 1;
    }

    static D3D11_PRIMITIVE_TOPOLOGY _d3d_primitive_topology_from(xl7::graphics::meshes::Topology topology)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::PointList ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::LineList ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_LINELIST ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::LineStrip ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::TriangleList ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::TriangleStrip ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ) );

        return static_cast<D3D11_PRIMITIVE_TOPOLOGY>( topology );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingContextImpl::RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context, wrl::ComPtr<ID3D11RenderTargetView> d3d_render_target_view, wrl::ComPtr<ID3D11DepthStencilView> d3d_depth_stencil_view)
        : RenderingContext( rendering_device, index )
        , _d3d_device_context( d3d_device_context )
        , _d3d_render_target_view( d3d_render_target_view )
        , _d3d_depth_stencil_view( d3d_depth_stencil_view )
    {
        assert( d3d_device_context );
    }



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################

    /**
     * Begins a scene.
     */
    bool RenderingContextImpl::_begin_scene_impl()
    {
        // Nothing to do here?

        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingContextImpl::_end_scene_impl()
    {
        // Nothing to do here?

        return true;
    }

    /**
     * Clears the currently bound render target(s).
     */
    bool RenderingContextImpl::_clear_impl(const ResolvedTargetStates& resolved_target_states, ClearFlags clear_flags, const Color& color, float depth, unsigned stencil)
    {
        if ( !_flush_target_states( resolved_target_states ) )
            return false;

        if ( (clear_flags & ClearFlags::ColorBuffer) == ClearFlags::ColorBuffer )
        {
            for ( auto& d3d_render_target_view : hardware_states.render_target_views )
            {
                if ( d3d_render_target_view )
                    _d3d_device_context->ClearRenderTargetView( d3d_render_target_view, color.get_rgba_ptr() );
            }
        }

        UINT d3d_clear_flags = _d3d_clear_flags_from( clear_flags );
        if ( d3d_clear_flags )
        {
            if ( hardware_states.depth_stencil_view )
                _d3d_device_context->ClearDepthStencilView( hardware_states.depth_stencil_view, d3d_clear_flags, depth, stencil );
        }

        return true;
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_vertex)
    {
        if ( !_flush_draw_states( resolved_draw_states ) )
            return false;

        unsigned vertex_count = xl7::graphics::meshes::MeshUtil::calculate_vertex_index_count( resolved_draw_states.topology, primitive_count );

        _d3d_device_context->Draw( vertex_count, start_vertex );

        return true;
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_indexed_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        if ( !_flush_draw_states( resolved_draw_states ) )
            return false;

        unsigned index_count = xl7::graphics::meshes::MeshUtil::calculate_vertex_index_count( resolved_draw_states.topology, primitive_count );

        _d3d_device_context->DrawIndexed( index_count, start_index, base_vertex );

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Transfers the current states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_target_states(const ResolvedTargetStates& resolved_target_states)
    {
        unsigned target_count = 0;

        const unsigned max_render_target_count = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->get_capabilities().max_simultaneous_render_target_count;
        for ( unsigned target_index = 0; target_index < max_render_target_count; ++target_index )
        {
            ID3D11RenderTargetView* d3d_render_target_view;
            if ( false )
                ;
            else if ( target_index == 0 )
                d3d_render_target_view = _d3d_render_target_view.Get();
            else
                d3d_render_target_view = nullptr;

            if ( d3d_render_target_view != hardware_states.render_target_views[ target_index ] )
            {
                target_count = target_index + 1;
                hardware_states.render_target_views[ target_index ] = d3d_render_target_view;
            }
        } // for each render target

        ID3D11DepthStencilView* d3d_depth_stencil_view;
        if ( false )
            ;
        else
            d3d_depth_stencil_view = _d3d_depth_stencil_view.Get();

        if ( d3d_depth_stencil_view != hardware_states.depth_stencil_view || target_count > 0 )
        {
            hardware_states.depth_stencil_view = d3d_depth_stencil_view;

            _d3d_device_context->OMSetRenderTargets( target_count, &hardware_states.render_target_views[0], hardware_states.depth_stencil_view );
        }


        return true;
    }

    /**
     * Transfers the current states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_draw_states(const ResolvedDrawStates& resolved_draw_states)
    {
        if ( !_flush_target_states( resolved_draw_states ) )
            return false;

        shared::meshes::VertexBufferBinding vertex_buffer_binding;


        unsigned stream_count = 0;
        unsigned stream_strides[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];
        unsigned stream_offsets[ pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS ];

        const unsigned max_vertex_stream_count = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->get_capabilities().max_concurrent_vertex_stream_count;
        for ( unsigned stream_index = 0; stream_index < max_vertex_stream_count; ++stream_index )
        {
            auto* vertex_buffer = static_cast<const meshes::VertexBufferImpl*>( resolved_draw_states.vertex_buffers[ stream_index ] );
            ID3D11Buffer* d3d_vertex_buffer;
            if ( vertex_buffer ) {
                vertex_buffer_binding.stream_count = stream_index = stream_index + 1;
                vertex_buffer_binding.vertex_buffer_ids[ stream_index ] = vertex_buffer->get_id();
                d3d_vertex_buffer = vertex_buffer->get_raw_d3d_vertex_buffer();
                stream_strides[ stream_index ] = vertex_buffer->get_stride();
                stream_offsets[ stream_index ] = 0;
            } else {
                vertex_buffer_binding.vertex_buffer_ids[ stream_index ] = resources::ResourceID::INVALID_ID;
                d3d_vertex_buffer = nullptr;
                stream_strides[ stream_index ] = 0;
                stream_offsets[ stream_index ] = 0;
            }

            if ( d3d_vertex_buffer != hardware_states.vertex_buffers[ stream_index ] )
            {
                stream_count = stream_index + 1;
                hardware_states.vertex_buffers[ stream_index ] = d3d_vertex_buffer;
            }
        } // for each vertex stream

        if ( stream_count > 0 )
        {
            _d3d_device_context->IASetVertexBuffers( 0, stream_count, &hardware_states.vertex_buffers[0], stream_strides, stream_offsets );
        }

        auto* index_buffer = static_cast<const meshes::IndexBufferImpl*>( resolved_draw_states.index_buffer );
        ID3D11Buffer* d3d_index_buffer;
        DXGI_FORMAT dxgi_format;
        if ( index_buffer ) {
            d3d_index_buffer = index_buffer->get_raw_d3d_index_buffer();
            dxgi_format = index_buffer->get_dxgi_format();
        } else {
            d3d_index_buffer = nullptr;
            dxgi_format = DXGI_FORMAT_UNKNOWN;
        }

        if ( d3d_index_buffer != hardware_states.index_buffer )
        {
            _d3d_device_context->IASetIndexBuffer( d3d_index_buffer, dxgi_format, 0 );
            hardware_states.index_buffer = d3d_index_buffer;
        }


        D3D11_PRIMITIVE_TOPOLOGY d3d_primitive_topology = _d3d_primitive_topology_from( resolved_draw_states.topology );
        if ( d3d_primitive_topology != hardware_states.primitive_topology )
        {
            _d3d_device_context->IASetPrimitiveTopology( d3d_primitive_topology );
            hardware_states.primitive_topology = d3d_primitive_topology;
        }


        auto* vertex_shader = static_cast<const shaders::VertexShaderImpl*>( resolved_draw_states.vs.shader );
        ID3D11VertexShader* d3d_vertex_shader;
        if ( vertex_shader )
            d3d_vertex_shader = vertex_shader->get_raw_d3d_vertex_shader();
        else
            d3d_vertex_shader = nullptr;

        if ( d3d_vertex_shader != hardware_states.vs.shader )
        {
            _d3d_device_context->VSSetShader( d3d_vertex_shader, nullptr, 0 );
            hardware_states.vs.shader = d3d_vertex_shader;
        }

        auto* pixel_shader = static_cast<const shaders::PixelShaderImpl*>( resolved_draw_states.ps.shader );
        ID3D11PixelShader* d3d_pixel_shader;
        if ( pixel_shader )
            d3d_pixel_shader = pixel_shader->get_raw_d3d_pixel_shader();
        else
            d3d_pixel_shader = nullptr;

        if ( d3d_pixel_shader != hardware_states.ps.shader )
        {
            _d3d_device_context->PSSetShader( d3d_pixel_shader, nullptr, 0 );
            hardware_states.ps.shader = d3d_pixel_shader;
        }


        _flush_texture_sampler_states( resolved_draw_states.vs, hardware_states.vs, &ID3D11DeviceContextN::VSSetShaderResources, &ID3D11DeviceContextN::VSSetSamplers );
        _flush_texture_sampler_states( resolved_draw_states.ps, hardware_states.ps, &ID3D11DeviceContextN::PSSetShaderResources, &ID3D11DeviceContextN::PSSetSamplers );


        ID3D11InputLayout* d3d_input_layout = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->_find_d3d_input_layout( vertex_buffer_binding );
        if ( !d3d_input_layout )
        {
            if ( !vertex_shader || vertex_shader->get_bytecode().get_code_data().empty() )
            {
                LOG_ERROR( TEXT("No compiled vertex shader bytecode provided to validate against the input layout being created.") );
                return false;
            }

            const cl7::byte_vector& code_data = vertex_shader->get_bytecode().get_code_data();
            d3d_input_layout = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->_find_or_create_d3d_input_layout( vertex_buffer_binding, code_data.data(), code_data.size() );
        }

        if ( d3d_input_layout != hardware_states.input_layout )
        {
            _d3d_device_context->IASetInputLayout( d3d_input_layout );
            hardware_states.input_layout = d3d_input_layout;
        }


        auto* rasterizer_state = static_cast<const states::RasterizerStateImpl*>( resolved_draw_states.rasterizer_state );
        ID3D11RasterizerState* d3d_rasterizer_state;
        if ( rasterizer_state )
            d3d_rasterizer_state = rasterizer_state->get_raw_d3d_rasterizer_state();
        else
            d3d_rasterizer_state = nullptr;

        if ( d3d_rasterizer_state != hardware_states.rasterizer_state )
        {
            _d3d_device_context->RSSetState( d3d_rasterizer_state );
            hardware_states.rasterizer_state = d3d_rasterizer_state;
        }


        auto* blend_state = static_cast<const states::BlendStateImpl*>( resolved_draw_states.blend_state );
        ID3D11BlendState* d3d_blend_state;
        if ( blend_state )
            d3d_blend_state = blend_state->get_raw_d3d_blend_state();
        else
            d3d_blend_state = nullptr;

        if ( d3d_blend_state != hardware_states.blend_state || resolved_draw_states.blend_factor != hardware_states.blend_factor )
        {
            _d3d_device_context->OMSetBlendState( d3d_blend_state, resolved_draw_states.blend_factor.get_rgba_ptr(), 0xffffffff );
            hardware_states.blend_state = d3d_blend_state;
            hardware_states.blend_factor = resolved_draw_states.blend_factor;
        }


        return true;
    }

    /**
     * Transfers the current states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_texture_sampler_states(const ResolvedTextureSamplerStates& resolved_texture_sampler_states, HardwareStates::TextureSamplerStates& hardware_texture_sampler_states, void (ID3D11DeviceContextN::*SetShaderResources)(unsigned, unsigned, ID3D11ShaderResourceView*const*), void (ID3D11DeviceContextN::*SetSamplers)(unsigned, unsigned, ID3D11SamplerState*const*))
    {
        unsigned slot_count = 0;

        const unsigned max_texture_sampler_slot_count = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->get_capabilities().max_texture_sampler_slot_count;
        for ( unsigned slot_index = 0; slot_index < max_texture_sampler_slot_count; ++slot_index )
        {
            auto* texture = resolved_texture_sampler_states.textures[ slot_index ];
            ID3D11ShaderResourceView* d3d_shader_resource_view;
            if ( texture ) {
                d3d_shader_resource_view = texture->get_raw_resource<ID3D11ShaderResourceView>();
            } else {
                d3d_shader_resource_view = nullptr;
            }

            auto* sampler_state = static_cast<const states::SamplerStateImpl*>( resolved_texture_sampler_states.sampler_states[ slot_index ] );
            ID3D11SamplerState* d3d_sampler_state;
            if ( sampler_state )
                d3d_sampler_state = sampler_state->get_raw_d3d_sampler_state();
            else
                d3d_sampler_state = nullptr;

            if ( d3d_shader_resource_view != hardware_texture_sampler_states.shader_resource_views[ slot_index ] )
            {
                slot_count = slot_index + 1;
                hardware_texture_sampler_states.shader_resource_views[ slot_index ] = d3d_shader_resource_view;
            }

            if ( d3d_sampler_state != hardware_texture_sampler_states.sampler_states[ slot_index ] )
            {
                slot_count = slot_index + 1;
                hardware_texture_sampler_states.sampler_states[ slot_index ] = d3d_sampler_state;
            }
        } // for each texture/sampler slot

        if ( slot_count > 0 )
        {
            (_d3d_device_context.Get()->*SetShaderResources)( 0, slot_count, &hardware_texture_sampler_states.shader_resource_views[0] );
            (_d3d_device_context.Get()->*SetSamplers)( 0, slot_count, &hardware_texture_sampler_states.sampler_states[0] );
        }


        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
