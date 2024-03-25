#include "RenderingContextImpl.h"

#include "./meshes/VertexBufferImpl.h"
#include "./meshes/IndexBufferImpl.h"

#include "./shaders/VertexShaderImpl.h"
#include "./shaders/PixelShaderImpl.h"

#include "./RenderingDeviceImpl.h"
#include "./errors.h"

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

    static D3DPRIMITIVETYPE _d3d_primitive_type_from(xl7::graphics::meshes::Topology topology)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::PointList ) == static_cast<unsigned>( D3DPT_POINTLIST ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::LineList ) == static_cast<unsigned>( D3DPT_LINELIST ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::LineStrip ) == static_cast<unsigned>( D3DPT_LINESTRIP ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::TriangleList ) == static_cast<unsigned>( D3DPT_TRIANGLELIST ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::Topology::TriangleStrip ) == static_cast<unsigned>( D3DPT_TRIANGLESTRIP ) );

        return static_cast<D3DPRIMITIVETYPE>( topology );
    }

    static BYTE _d3d_vertex_element_usage_from(xl7::graphics::meshes::VertexLayout::Semantic semantic)
    {
        switch ( semantic )
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
            assert( false );
        }

        return 0;
    }

    static BYTE _d3d_vertex_element_type_from(xl7::graphics::meshes::VertexLayout::DataType data_type)
    {
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::VertexLayout::DataType::FLOAT1 ) == static_cast<unsigned>( D3DDECLTYPE_FLOAT1 ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::VertexLayout::DataType::FLOAT2 ) == static_cast<unsigned>( D3DDECLTYPE_FLOAT2 ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::VertexLayout::DataType::FLOAT3 ) == static_cast<unsigned>( D3DDECLTYPE_FLOAT3 ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::VertexLayout::DataType::FLOAT4 ) == static_cast<unsigned>( D3DDECLTYPE_FLOAT4 ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::VertexLayout::DataType::COLOR ) == static_cast<unsigned>( D3DDECLTYPE_D3DCOLOR ) );
        static_assert( static_cast<unsigned>( xl7::graphics::meshes::VertexLayout::DataType::UBYTE4 ) == static_cast<unsigned>( D3DDECLTYPE_UBYTE4 ) );

        return static_cast<BYTE>( data_type );
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

        shared::meshes::VertexBufferBinding vertex_buffer_binding;

        HRESULT hresult;


        const unsigned max_vertex_stream_count = static_cast<RenderingDeviceImpl*>( get_rendering_device() )->get_capabilities().max_concurrent_vertex_stream_count;
        for ( unsigned stream_index = 0; stream_index < max_vertex_stream_count; ++stream_index )
        {
            auto* vertex_buffer = static_cast<const meshes::VertexBufferImpl*>( resolved_draw_states.vertex_buffers[ stream_index ] );
            IDirect3DVertexBuffer9* d3d_vertex_buffer;
            unsigned stride;
            if ( vertex_buffer ) {
                vertex_buffer_binding.stream_count = stream_index = stream_index + 1;
                vertex_buffer_binding.vertex_buffer_ids[ stream_index ] = vertex_buffer->get_id();
                d3d_vertex_buffer = vertex_buffer->get_raw_d3d_vertex_buffer();
                stride = vertex_buffer->get_stride();
            } else {
                vertex_buffer_binding.vertex_buffer_ids[ stream_index ] = resources::ResourceID::INVALID_ID;
                d3d_vertex_buffer = nullptr;
                stride = 0;
            }

            if ( d3d_vertex_buffer != hardware_states.vertex_buffers[ stream_index ] )
            {
                hresult = _d3d_device->SetStreamSource( stream_index, d3d_vertex_buffer, 0, stride );
                if ( FAILED(hresult) )
                {
                    LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetStreamSource") ) );
                    return false;
                }
                hardware_states.vertex_buffers[ stream_index ] = d3d_vertex_buffer;
            }
        } // for each vertex stream

        auto* index_buffer = static_cast<const meshes::IndexBufferImpl*>( resolved_draw_states.index_buffer );
        IDirect3DIndexBuffer9* d3d_index_buffer;
        if ( index_buffer )
            d3d_index_buffer = index_buffer->get_raw_d3d_index_buffer();
        else
            d3d_index_buffer = nullptr;

        if ( d3d_index_buffer != hardware_states.index_buffer )
        {
            hresult = _d3d_device->SetIndices( d3d_index_buffer );
            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetIndices") ) );
                return false;
            }
            hardware_states.index_buffer = d3d_index_buffer;
        }


        IDirect3DVertexDeclaration9* d3d_vertex_declaration = _find_d3d_vertex_declaration( vertex_buffer_binding );
        if ( !d3d_vertex_declaration )
            d3d_vertex_declaration = _find_or_create_d3d_vertex_declaration( vertex_buffer_binding );

        if ( d3d_vertex_declaration != hardware_states.vertex_declaration )
        {
            hresult = _d3d_device->SetVertexDeclaration( d3d_vertex_declaration );
            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetVertexDeclaration") ) );
                return false;
            }
            hardware_states.vertex_declaration = d3d_vertex_declaration;
        }


        auto* vertex_shader = static_cast<const shaders::VertexShaderImpl*>( resolved_draw_states.vertex_shader.shader );
        IDirect3DVertexShader9* d3d_vertex_shader;
        if ( vertex_shader )
            d3d_vertex_shader = vertex_shader->get_raw_d3d_vertex_shader();
        else
            d3d_vertex_shader = nullptr;

        if ( d3d_vertex_shader != hardware_states.vertex_shader )
        {
            hresult = _d3d_device->SetVertexShader( d3d_vertex_shader );
            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetVertexShader") ) );
                return false;
            }
            hardware_states.vertex_shader = d3d_vertex_shader;
        }

        auto* pixel_shader = static_cast<const shaders::PixelShaderImpl*>( resolved_draw_states.pixel_shader.shader );
        IDirect3DPixelShader9* d3d_pixel_shader;
        if ( pixel_shader )
            d3d_pixel_shader = pixel_shader->get_raw_d3d_pixel_shader();
        else
            d3d_pixel_shader = nullptr;

        if ( d3d_pixel_shader != hardware_states.pixel_shader )
        {
            hresult = _d3d_device->SetPixelShader( d3d_pixel_shader );
            if ( FAILED(hresult) )
            {
                LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::SetPixelShader") ) );
                return false;
            }
            hardware_states.pixel_shader = d3d_pixel_shader;
        }


        return true;
    }

    /**
     * Tries to find a suitable Direct3D 9 vertex declaration based on the currently
     * set vertex buffer(s).
     */
    IDirect3DVertexDeclaration9* RenderingContextImpl::_find_d3d_vertex_declaration(const shared::meshes::VertexBufferBinding& vertex_buffer_binding)
    {
        auto it = _d3d_vertex_declarations_by_binding.find( vertex_buffer_binding );
        if ( it != _d3d_vertex_declarations_by_binding.end() )
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
        shared::meshes::ComposedVertexLayout composed_vertex_layout{ vertex_buffer_binding };

        auto it = _d3d_vertex_declarations_by_layout.find( composed_vertex_layout );
        if ( it != _d3d_vertex_declarations_by_layout.end() )
            return it->second.Get();

        std::vector<D3DVERTEXELEMENT9> d3d_vertex_elements;
        for ( unsigned stream_index = 0; stream_index < pipeline::InputAssemblerStage::MAX_VERTEX_STREAMS; ++stream_index )
        {
            if ( !vertex_buffer_binding.vertex_buffer_ids[ stream_index ].is_valid() )
                continue;

            WORD offset = 0;

            for ( const xl7::graphics::meshes::VertexLayout::Element& element : composed_vertex_layout.vertex_layouts[ stream_index ].elements )
            {
                D3DVERTEXELEMENT9 d3d_vertex_element;
                d3d_vertex_element.Stream = static_cast<WORD>( stream_index );
                d3d_vertex_element.Offset = offset;
                d3d_vertex_element.Type = _d3d_vertex_element_type_from( element.data_type );
                d3d_vertex_element.Method = D3DDECLMETHOD_DEFAULT;
                d3d_vertex_element.Usage = _d3d_vertex_element_usage_from( element.semantic );
                d3d_vertex_element.UsageIndex = static_cast<BYTE>( element.semantic_index );

                d3d_vertex_elements.push_back( d3d_vertex_element );

                offset += static_cast<WORD>( element.get_size() );
            } // for each vertex element
        } // for each vertex stream

        d3d_vertex_elements.push_back( D3DDECL_END() );

        wrl::ComPtr<IDirect3DVertexDeclaration9> d3d_vertex_declaration;
        HRESULT hresult = _d3d_device->CreateVertexDeclaration( &d3d_vertex_elements[0], &d3d_vertex_declaration );
        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateVertexDeclaration") ) );
            return nullptr;
        }

        _d3d_vertex_declarations_by_layout.emplace( std::make_pair( std::move(composed_vertex_layout), d3d_vertex_declaration ) );
        _d3d_vertex_declarations_by_binding.insert( std::make_pair( vertex_buffer_binding, d3d_vertex_declaration ) );

        return d3d_vertex_declaration.Get();
    }



} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
