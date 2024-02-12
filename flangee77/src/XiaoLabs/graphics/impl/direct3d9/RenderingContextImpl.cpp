#include "RenderingContextImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

#include <XiaoLabs/graphics.h>

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {



    RenderingContextImpl::HardwareStates::HardwareStates()
    {
        memset( this, 0, sizeof(HardwareStates) );
    }



    static DWORD _d3d_clear_flags_from(ClearFlags clear_flags)
    {
        static_assert( static_cast<unsigned>( ClearFlags::ColorBuffer ) == static_cast<unsigned>( D3DCLEAR_TARGET ) );
        static_assert( static_cast<unsigned>( ClearFlags::DepthBuffer ) == static_cast<unsigned>( D3DCLEAR_ZBUFFER ) );
        static_assert( static_cast<unsigned>( ClearFlags::StencilBuffer ) == static_cast<unsigned>( D3DCLEAR_STENCIL ) );

        return static_cast<DWORD>( clear_flags );
    }

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
    RenderingContextImpl::RenderingContextImpl(RenderingDeviceImpl* rendering_device, unsigned index, wrl::ComPtr<IDirect3DDevice9> d3d_device, wrl::ComPtr<IDirect3DSurface9> d3d_render_target_surface, wrl::ComPtr<IDirect3DSurface9> d3d_depth_stencil_surface)
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
     * Clears the currently bound render target(s).
     */
    bool RenderingContextImpl::_clear_impl(const ResolvedTargetStates& resolved_target_states, ClearFlags clear_flags, const Color& color, float depth, unsigned stencil)
    {
        if ( !_flush_target_states( resolved_target_states ) )
            return false;

        DWORD d3d_clear_flags = _d3d_clear_flags_from( clear_flags );
        if ( !d3d_clear_flags )
            return true;

        HRESULT hresult = _d3d_device->Clear( 0, nullptr, d3d_clear_flags, color.to_argb32(), depth, stencil );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::Clear") ) );
            return false;
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

        D3DPRIMITIVETYPE d3d_primitive_type = _d3d_primitive_type_from( resolved_draw_states.topology );

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
    bool RenderingContextImpl::_draw_indexed_impl(const ResolvedDrawStates& resolved_draw_states, unsigned primitive_count, unsigned start_index, signed base_vertex)
    {
        if ( !_flush_draw_states( resolved_draw_states ) )
            return false;

        D3DPRIMITIVETYPE d3d_primitive_type = _d3d_primitive_type_from( resolved_draw_states.topology );

        // Should we optimize for these two values?
        unsigned min_vertex_index = 0;
        assert( resolved_draw_states.vertex_buffers[0] );
        unsigned vertex_count = resolved_draw_states.vertex_buffers[0]->get_desc().count;

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
    bool RenderingContextImpl::_flush_target_states(const ResolvedTargetStates& resolved_target_states)
    {
        HRESULT hresult;

        const unsigned max_render_target_count = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->get_capabilities().max_simultaneous_render_target_count;
        for ( unsigned target_index = 0; target_index < max_render_target_count; ++target_index )
        {
            IDirect3DSurface9* d3d_render_target;
            if ( false )
                ;
            else if ( target_index == 0 )
                d3d_render_target = _d3d_render_target_surface.Get();
            else
                d3d_render_target = nullptr;

            if ( d3d_render_target != hardware_states.render_targets[ target_index ] )
            {
                hresult = _d3d_device->SetRenderTarget( target_index, d3d_render_target );
                if ( FAILED(hresult) )
                {
                    LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetRenderTarget") ) );
                    return false;
                }
                hardware_states.render_targets[ target_index ] = d3d_render_target;
            }
        } // for each render target

        IDirect3DSurface9* d3d_depth_stencil_surface;
        if ( false )
            ;
        else
            d3d_depth_stencil_surface = _d3d_depth_stencil_surface.Get();

        if ( d3d_depth_stencil_surface != hardware_states.depth_stencil_surface )
        {
            hresult = _d3d_device->SetDepthStencilSurface( d3d_depth_stencil_surface );
            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetDepthStencilSurface") ) );
                return false;
            }
            hardware_states.depth_stencil_surface = d3d_depth_stencil_surface;
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

        

        return true;
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
