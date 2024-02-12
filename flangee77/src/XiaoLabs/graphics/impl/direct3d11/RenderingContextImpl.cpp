#include "RenderingContextImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

#include <XiaoLabs/graphics.h>

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

    static D3D11_PRIMITIVE_TOPOLOGY _d3d_primitive_topology_from(meshes::Topology topology)
    {
        static_assert( static_cast<unsigned>( meshes::Topology::PointList ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_POINTLIST ) );
        static_assert( static_cast<unsigned>( meshes::Topology::LineList ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_LINELIST ) );
        static_assert( static_cast<unsigned>( meshes::Topology::LineStrip ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP ) );
        static_assert( static_cast<unsigned>( meshes::Topology::TriangleList ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ) );
        static_assert( static_cast<unsigned>( meshes::Topology::TriangleStrip ) == static_cast<unsigned>( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP ) );

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

        unsigned vertex_count = meshes::MeshUtil::calculate_vertex_index_count( resolved_draw_states.topology, primitive_count );

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

        unsigned index_count = meshes::MeshUtil::calculate_vertex_index_count( resolved_draw_states.topology, primitive_count );

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
        unsigned render_target_count = 0;

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
                render_target_count = target_index + 1;
                hardware_states.render_target_views[ target_index ] = d3d_render_target_view;
            }
        } // for each render target

        ID3D11DepthStencilView* d3d_depth_stencil_view;
        if ( false )
            ;
        else
            d3d_depth_stencil_view = _d3d_depth_stencil_view.Get();

        if ( d3d_depth_stencil_view != hardware_states.depth_stencil_view )
        {
            hardware_states.depth_stencil_view= d3d_depth_stencil_view;
        }

        _d3d_device_context->OMSetRenderTargets( render_target_count, &hardware_states.render_target_views[0], hardware_states.depth_stencil_view );

        return true;
    }

    /**
     * Transfers the current states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_draw_states(const ResolvedDrawStates& resolved_draw_states)
    {
        if ( !_flush_target_states( resolved_draw_states ) )
            return false;

        

        D3D11_PRIMITIVE_TOPOLOGY d3d_primitive_topology = _d3d_primitive_topology_from( resolved_draw_states.topology );
        if ( d3d_primitive_topology != hardware_states.primitive_topology )
        {
            _d3d_device_context->IASetPrimitiveTopology( d3d_primitive_topology );
            hardware_states.primitive_topology = d3d_primitive_topology;
        }

        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
