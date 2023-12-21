#include "VertexBufferImpl.h"

#include "../GraphicsSystemImpl.h"
#include "../RenderingDeviceImpl.h"
#include "../errors.h"

#include <CoreLabs/logging.h>



namespace xl7 {
namespace graphics {
namespace impl {
namespace direct3d9 {
namespace meshes {



    static DWORD _d3d_fvf(const xl7::graphics::meshes::VertexLayout& vertex_layout)
    {
        

        return D3DFMT_UNKNOWN;
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    VertexBufferImpl::VertexBufferImpl(xl7::graphics::meshes::MeshManager* manager, const cl7::string& identifier, const Desc& desc)
        : VertexBuffer( manager, identifier, desc )
        , d3d_fvf( _d3d_fvf( desc.vertex_layout ) )
        , _d3d_device( dynamic_cast<RenderingDeviceImpl*>( GraphicsSystem::instance().get_rendering_device() )->get_raw_d3d_device() )
        , _d3d_vertex_buffer()
    {
    }



    // #############################################################################
    // Resource Implementations
    // #############################################################################

    /**
     * Requests/acquires the resource, bringing it into a usable state.
     */
    bool VertexBufferImpl::_request_impl()
    {
        return _create_vertex_buffer();
    }

    /**
     * Releases the resource.
     */
    bool VertexBufferImpl::_release_impl()
    {
        _d3d_vertex_buffer.Reset();

        return true;
    }

    /**
     * Temporarily resigns some stuff to free up some (hardware) memory etc.
     */
    bool VertexBufferImpl::_resign_impl()
    {
        _d3d_vertex_buffer.Reset();

        return true;
    }

    /**
     * Restores the resource after it has been (temporarily) resigned, returning it
     * to a usable state.
     */
    bool VertexBufferImpl::_restore_impl()
    {
        if ( !_create_vertex_buffer() )
            return false;

        // 

        return true;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Creates the Direct3D 9 index buffer interface.
     */
    bool VertexBufferImpl::_create_vertex_buffer()
    {
        assert( _d3d_device );
        assert( !_d3d_vertex_buffer );

        HRESULT hresult = _d3d_device->CreateVertexBuffer(
            this->size,
            D3DUSAGE_WRITEONLY,
            d3d_fvf,
            D3DPOOL_MANAGED,
            &_d3d_vertex_buffer,
            NULL );

        if ( FAILED(hresult) )
        {
            LOG_ERROR( errors::d3d9_result( hresult, TEXT("IDirect3DDevice9::CreateVertexBuffer") ) );
            return false;
        }

        return true;
    }



} // namespace meshes
} // namespace direct3d9
} // namespace impl
} // namespace graphics
} // namespace xl7
