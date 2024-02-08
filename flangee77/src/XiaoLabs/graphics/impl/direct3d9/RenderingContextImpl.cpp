#include "RenderingContextImpl.h"

#include "./errors.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



    static D3DPRIMITIVETYPE _d3d_primitive_type_from(meshes::Topology topology)
    {
        static_assert( static_cast<unsigned>( meshes::Topology::PointList ) == static_cast<unsigned>( D3DPT_POINTLIST ) );
        static_assert( static_cast<unsigned>( meshes::Topology::LineList ) == static_cast<unsigned>( D3DPT_LINELIST ) );
        static_assert( static_cast<unsigned>( meshes::Topology::LineStrip ) == static_cast<unsigned>( D3DPT_LINESTRIP ) );
        static_assert( static_cast<unsigned>( meshes::Topology::TriangleList ) == static_cast<unsigned>( D3DPT_TRIANGLELIST ) );
        static_assert( static_cast<unsigned>( meshes::Topology::TriangleStrip ) == static_cast<unsigned>( D3DPT_TRIANGLESTRIP ) );

        return static_cast<D3DPRIMITIVETYPE>( topology );
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    RenderingContextImpl::RenderingContextImpl(RenderingDevice* rendering_device, unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device, wrl::ComPtr<IDirect3DSurface9> d3d_render_target_surface, wrl::ComPtr<IDirect3DSurface9> d3d_depth_stencil_surface)
        : RenderingContext( rendering_device, index )
        , _d3d_device( d3d_device )
        , _d3d_render_target_surface( d3d_render_target_surface )
        , _d3d_depth_stencil_surface( d3d_depth_stencil_surface )
    {
        assert( index == 0 );
        assert( d3d_device );
    }



    // #############################################################################
    // RenderingContext Implementations
    // #############################################################################

    /**
     * Begins a scene.
     */
    bool RenderingContextImpl::_begin_scene_impl()
    {
        HRESULT hresult = _d3d_device->BeginScene();
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::BeginScene") ) );
            return false;
        }

        return true;
    }

    /**
     * Ends a scene that was begun by calling begin_scene.
     */
    bool RenderingContextImpl::_end_scene_impl()
    {
        HRESULT hresult = _d3d_device->EndScene();
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::EndScene") ) );
            return false;
        }

        return true;
    }

    /**
     * Draws non-indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_impl(const DrawStates& draw_states, unsigned primitive_count, unsigned start_vertex)
    {
        if ( !_flush_states( draw_states ) )
            return false;

        D3DPRIMITIVETYPE d3d_primitive_type = _d3d_primitive_type_from( draw_states.topology );

        HRESULT hresult = _d3d_device->DrawPrimitive( d3d_primitive_type, start_vertex, primitive_count );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::DrawPrimitive") ) );
            return false;
        }

        return true;
    }

    /**
     * Draws indexed, non-instanced primitives.
     */
    bool RenderingContextImpl::_draw_indexed_impl(const DrawStates& draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        if ( !_flush_states( draw_states ) )
            return false;

        D3DPRIMITIVETYPE d3d_primitive_type = _d3d_primitive_type_from( draw_states.topology );

        // Should we optimize for these two values?
        unsigned min_vertex_index = 0;
        assert( draw_states.vertex_buffers[0] );
        unsigned vertex_count = draw_states.vertex_buffers[0]->get_desc().count;

        HRESULT hresult = _d3d_device->DrawIndexedPrimitive( d3d_primitive_type, base_vertex, min_vertex_index, vertex_count, start_index, primitive_count );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::DrawPrimitive") ) );
            return false;
        }

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
        

        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
