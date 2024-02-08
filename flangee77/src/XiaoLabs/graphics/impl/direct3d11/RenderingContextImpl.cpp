#include "RenderingContextImpl.h"

#include "./errors.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d11 {



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
    RenderingContextImpl::RenderingContextImpl(RenderingDevice* rendering_device, unsigned index, wrl::ComPtr<ID3D11DeviceContextN> d3d_device_context)
        : RenderingContext( rendering_device, index )
        , _d3d_device_context( d3d_device_context )
        , _d3d_primitive_topology( D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED )
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
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_impl(const DrawStates& draw_states, unsigned primitive_count, unsigned start_vertex)
    {
        if ( !_flush_states( draw_states ) )
            return false;

        unsigned vertex_count = meshes::MeshUtil::calculate_vertex_index_count( draw_states.topology, primitive_count );

        _d3d_device_context->Draw( vertex_count, start_vertex );

        return true;
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_indexed_impl(const DrawStates& draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        if ( !_flush_states( draw_states ) )
            return false;

        unsigned index_count = meshes::MeshUtil::calculate_vertex_index_count( draw_states.topology, primitive_count );

        _d3d_device_context->DrawIndexed( index_count, start_index, base_vertex );

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Transfers the current states to the device if necessary.
     */
    bool RenderingContextImpl::_flush_states(const DrawStates& draw_states)
    {
        

        D3D11_PRIMITIVE_TOPOLOGY d3d_primitive_topology = _d3d_primitive_topology_from( draw_states.topology );
        if ( d3d_primitive_topology != _d3d_primitive_topology )
        {
            _d3d_device_context->IASetPrimitiveTopology( d3d_primitive_topology );
            _d3d_primitive_topology = d3d_primitive_topology;
        }

        return true;
    }



} // namespace direct3d11
} // namespace impl
} // namespace graphics
} // namespace xl7
